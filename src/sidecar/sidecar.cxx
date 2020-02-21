#ifndef UNICODE
#define UNICODE
#endif

#define DIRECTINPUT_VERSION 0x0800

#include <stdio.h>
#include <windows.h>
#include <strsafe.h>
#include <detours.h>
#include <d3d9.h>
#include <dinput.h>
#include <tchar.h>
#include <ggponet.h>
#include <vdf_parser.hpp>

#include "../game/game.h"
#include "../overlay/overlay.h"
#include "sidecar.h"
#include "ID3D9Wrapper.h"

#define DEFAULT_ALPHA 0.87f

typedef IDirect3D9* (__stdcall* Direct3DCreate9_t)(UINT SDKVersion);
Direct3DCreate9_t orig_Direct3DCreate9;

static LPCWSTR DETOUR_FAILED_MESSAGE = TEXT("Could not detour targets!");

static HMODULE g_lpPEHeaderRoot;
static GameMethods g_gameMethods;
static GameState g_gameState;
static GGPOErrorCode g_result;
static char* szConfigFilePath;

HRESULT AttachInitialFunctionDetours(GameMethods* src);
HRESULT AttachInternalFunctionPointers(GameMethods* src);

IDirect3D9* __stdcall FakeDirect3DCreate9(UINT sdkVers)
{
	IDirect3D9* pD3d9 = orig_Direct3DCreate9(sdkVers); // real one
	Direct3D9Wrapper* ret = new Direct3D9Wrapper(&pD3d9);
	return pD3d9;
}

void hookDirect3DCreate9()
{
	HMODULE hM = GetModuleHandleA("d3d9.dll");
	orig_Direct3DCreate9 = (Direct3DCreate9_t)GetProcAddress(hM, "Direct3DCreate9");
	DetourAttach(&(PVOID&)orig_Direct3DCreate9, FakeDirect3DCreate9);
}

LRESULT WINAPI FakeWindowFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (OverlayWindowFunc(hWnd, msg, wParam, lParam)) {
		return true;
	}
	return g_gameMethods.WindowFunc(hWnd, msg, wParam, lParam);
}

int FakeSetupD3D9() {
	int out = g_gameMethods.SetupD3D9();
	ApplyConfiguration(&g_gameState);
	InitializeOverlay(&g_gameState);
	return out;
}

void FakeGenerateAndShadePrimitives() {
	// Guilty should've started the scene already.
	//
	// This method temporarily sets the render target to the UI render target
	// to draw the UI elements like the sidebars, then sets the render target
	// back to the game render target so that other methods can rely on the
	// invariant that the device's current render target is always the "game"
	// surface.
	g_gameMethods.GenerateAndShadePrimitives();

	// Since we want to draw Imgui as if it were a UI element (ex. with the
	// full, unscaled window as the viewport), set the render target while we
	// draw Imgui.
	(*g_gameState.d3dDevice)->SetRenderTarget(0, *g_gameState.uiRenderTarget);

	DrawOverlay(&g_gameMethods, &g_gameState);

	// Preserve the original method's invariant by setting the render target
	// back to the "game" surface.
	(*g_gameState.d3dDevice)->SetRenderTarget(0, *g_gameState.gameRenderTarget);

	// Guilty will end the scene after this call finishes.
}

void FakeDrawUIPrimitivesAndEndScene() {
	if (g_gameState.nFramesSkipped >= g_gameState.nFramesToSkipRender) {
		g_gameMethods.DrawUIPrimitivesAndEndScene();
	}
	else {
		g_gameState.nFramesSkipped++;
	}
}

void __cdecl FakeBeginSceneAndDrawGamePrimitives(int bShouldBeginScene) {
	if (g_gameState.nFramesSkipped >= g_gameState.nFramesToSkipRender) {
		g_gameMethods.BeginSceneAndDrawGamePrimitives(bShouldBeginScene);
	}
}

void FakePollForInputs() {
	unsigned int inputs[2];
	char szMessageBuf[1024] = { 0 };

	g_gameMethods.PollForInputs();

	// Non-GGPO rewriting
	if (g_gameState.nFramesSkipped < g_gameState.nFramesToSkipRender) {
		*g_gameState.nP1CurrentFrameInputs = g_gameState.arrInputsDuringFrameSkip[g_gameState.nFramesSkipped][0];
		*g_gameState.nP2CurrentFrameInputs = g_gameState.arrInputsDuringFrameSkip[g_gameState.nFramesSkipped][1];
	}

	// GGPO Rewriting
	if (g_gameState.ggpoState.ggpo != NULL && g_gameState.ggpoState.bIsSynchronized != 0) {
		unsigned int* inputLocation = g_gameState.ggpoState.localPlayerIndex == 0 ?
			g_gameState.nP1CurrentFrameInputs :
			g_gameState.nP2CurrentFrameInputs;

		/* notify ggpo of the local player's inputs */
		g_gameState.ggpoState.lastResult = ggpo_add_local_input(
			g_gameState.ggpoState.ggpo,
			g_gameState.ggpoState.player_handles[g_gameState.ggpoState.localPlayerIndex],
			inputLocation,
			sizeof(int)
		);
		if (GGPO_SUCCEEDED(g_gameState.ggpoState.lastResult)) {
			g_gameState.ggpoState.lastResult = ggpo_synchronize_input(
				g_gameState.ggpoState.ggpo,
				inputs,
				sizeof(int) * 2,
				NULL
			);
			*g_gameState.nP1CurrentFrameInputs = inputs[0];
			*g_gameState.nP2CurrentFrameInputs = inputs[1];
			if (!GGPO_SUCCEEDED(g_gameState.ggpoState.lastResult)) {
				StringCchPrintfA(szMessageBuf, 1024, "FakePollForInputs: synchronize input failed with %d", g_gameState.ggpoState.lastResult);
				// MessageBoxA(NULL, szMessageBuf, NULL, MB_OK);
			}
		}
		else {
			StringCchPrintfA(szMessageBuf, 1024, "FakePollForInputs: add local input failed with %d", g_gameState.ggpoState.lastResult);
			// MessageBoxA(NULL, szMessageBuf, NULL, MB_OK);
		}
	}
}

bool FakeSteamAPI_Init() {
	DWORD error;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	AttachInternalFunctionPointers(&g_gameMethods);
	error = DetourTransactionCommit();
	if (error != NO_ERROR) {
		MessageBox(NULL, DETOUR_FAILED_MESSAGE, NULL, 0);
	}

	return g_gameMethods.SteamAPI_Init();
}

BOOL FakeIsDebuggerPresent() {
	return FALSE;
}

void FakeHandlePossibleSteamInvites() {
	HANDLE syncThread = g_gameState.sessionInitState.hSyncThread;
	g_gameMethods.HandlePossibleSteamInvites();

	if (syncThread != NULL && WaitForSingleObject(syncThread, 0) != WAIT_TIMEOUT) {
		// The thread is either signalled with an exit, or the wait failed.
		// Either way, we can assume the thread is probably dead.
		g_gameState.sessionInitState.hSyncThread = NULL;

		if (TryEnterCriticalSection(&g_gameState.sessionInitState.criticalSection)) {
			if (
				g_gameState.sessionInitState.bHasRequest &&
				g_gameState.sessionInitState.bHasResponse
			) {
				g_gameState.sessionInitState.nSessionState = Loading;
				EnterVersus2P(&g_gameState, g_gameState.ggpoState.characters, &STAGES[0]);
			}
			else {
				MessageBoxA(NULL, "Sync thread completed, without getting a request and response- cannot continue!", NULL, MB_OK);
			}
			LeaveCriticalSection(&g_gameState.sessionInitState.criticalSection);
		}
		else {
			MessageBoxA(NULL, "Main thread tried to get session critical session after sync thread completion, but couldn't!", NULL, MB_OK);
		}
	}
}

void WINAPI FakeEnterBattleFiberEntry() {
	EnterCriticalSection(&g_gameState.sessionInitState.criticalSection);

	if (g_gameState.sessionInitState.nSessionState == Loading) {
		LeaveCriticalSection(&g_gameState.sessionInitState.criticalSection);

		// We're starting a GGPO session- enter a wait by transferring control back
		// to the main fiber. The main fiber will transfer control back to us, and
		// we can check again.
		PrepareGGPOSession(&g_gameState);
		while (true) {
			SwitchToFiber(*g_gameState.lpMainFiber);
			if (TryEnterCriticalSection(&g_gameState.sessionInitState.criticalSection)) {
				if (g_gameState.sessionInitState.nSessionState == Ready) {
					LeaveCriticalSection(&g_gameState.sessionInitState.criticalSection);
					break;
				}
				LeaveCriticalSection(&g_gameState.sessionInitState.criticalSection);
			}
		}
	}
	else {
		LeaveCriticalSection(&g_gameState.sessionInitState.criticalSection);
	}

	return g_gameMethods.EnterBattleFiberEntry();
}

HRESULT AttachInitialFunctionDetours(GameMethods* src) {
	DetourAttach(&(PVOID&)src->IsDebuggerPresent, FakeIsDebuggerPresent);
	DetourAttach(&(PVOID&)src->SteamAPI_Init, FakeSteamAPI_Init);
	hookDirect3DCreate9();

	return S_OK;
}

HRESULT AttachInternalFunctionPointers(GameMethods* src) {
	DetourAttach(&(PVOID&)src->GenerateAndShadePrimitives, FakeGenerateAndShadePrimitives);
	DetourAttach(&(PVOID&)src->SetupD3D9, FakeSetupD3D9);
	DetourAttach(&(PVOID&)src->WindowFunc, FakeWindowFunc);
	DetourAttach(&(PVOID&)src->BeginSceneAndDrawGamePrimitives, FakeBeginSceneAndDrawGamePrimitives);
	DetourAttach(&(PVOID&)src->DrawUIPrimitivesAndEndScene, FakeDrawUIPrimitivesAndEndScene);
	DetourAttach(&(PVOID&)src->PollForInputs, FakePollForInputs);
	DetourAttach(&(PVOID&)src->SimulateCurrentState, FakeSimulateCurrentState);
	DetourAttach(&(PVOID&)src->HandlePossibleSteamInvites, FakeHandlePossibleSteamInvites);
	DetourAttach(&(PVOID&)src->EnterBattleFiberEntry, FakeEnterBattleFiberEntry);

	return S_OK;
}

char* FindPayload() {
	HMODULE hMod;
	char* payload;
	DWORD payloadLength;

	for (hMod = DetourEnumerateModules(NULL); hMod != NULL; hMod = DetourEnumerateModules(hMod)) {
		payload = (char*)DetourFindPayload(hMod, s_guidSidecarPayload, &payloadLength);
		if (GetLastError() == 0 && payload != NULL) {
			return payload;
		}
	}
	MessageBoxA(NULL, "Could not find configuration file payload!", NULL, MB_OK);
	return NULL;
}

__declspec(dllexport) BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD dwReason,
	LPVOID reserved
) {
	LONG error;
	std::ifstream configFile;
	DWORD nConfigFilePathLength;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DetourRestoreAfterWith();
		g_gameState.szConfigPath = FindPayload();
		configFile.open(g_gameState.szConfigPath);
		g_gameState.config = tyti::vdf::read(configFile);
		g_lpPEHeaderRoot = LocatePERoot();
		if (LocateGameMethods(g_lpPEHeaderRoot, &g_gameMethods) != S_OK) {
			MessageBox(NULL, TEXT("Could not locate game methods!"), NULL, MB_OK);
		}

		if (LocateGameState(g_lpPEHeaderRoot, &g_gameState) != S_OK) {
			MessageBox(NULL, TEXT("Could not locate game state!"), NULL, MB_OK);
		}

		DetourTransactionBegin();
		AttachInitialFunctionDetours(&g_gameMethods);
		error = DetourTransactionCommit();
		if (error != NO_ERROR) {
			MessageBox(NULL, DETOUR_FAILED_MESSAGE, NULL, MB_OK);
		}
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		FreeOverlay();
		break;
	}
	return TRUE;
}
