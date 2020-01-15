#pragma once

#include <windows.h>
#include <d3d9.h>
#include <dinput.h>

enum gameinputs : unsigned short {
	Up = 0x10,
	Right = 0x20,
	Down = 0x40,
	Left = 0x80,
	Punch = 0x4000,
	Kick = 0x2000,
	Slash = 0x8000,
	HSlash = 0x200,
	Dust = 0x100
};

typedef unsigned char   undefined;
typedef unsigned char    byte;
typedef long long    longlong;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef struct GameObjectData GameObjectData;

typedef struct GameMethods {
    void(WINAPI* GenerateAndShadePrimitives)();
    int(WINAPI* SetupD3D9)();
    bool(__cdecl* SteamAPI_Init)();
    LRESULT(WINAPI* WindowFunc)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    BOOL(WINAPI* IsDebuggerPresent)();
    GameObjectData*(*SpawnNewEffect)(GameObjectData*, unsigned int, uint*);
    GameObjectData*(*SpawnNewProjectile)(GameObjectData*, unsigned int, uint*);
    void(WINAPI* DrawStage)();
    void(__fastcall* DrawGameUI)(DWORD);
    void(WINAPI* D3DSceneSetting)(int);
} GameMethods;

typedef struct GameState {
    bool bIsBackgroundEnabled;
    bool bIsEffectsEnabled;
    LPDIRECT3DSURFACE9* gameRenderTarget;
    LPDIRECT3DSURFACE9* uiRenderTarget;
    LPDIRECT3DDEVICE9* d3dDevice;
    HWND* hWnd;
    GameObjectData** arrCharacters;
    DWORD* bHitboxDisplayEnabled;
    DWORD* nSceneClearColor;
} GameState;

HMODULE LocatePERoot();
HRESULT LocateGameMethods(HMODULE peRoot, GameMethods* methods);
HRESULT LocateGameState(HMODULE peRoot, GameState* state);

typedef enum XInputButtonEnum {
    XINPUTBTN_A = 16384,
    XINPUTBTN_B = 8192,
    XINPUTBTN_BACK = 1,
    XINPUTBTN_LB = 1024,
    XINPUTBTN_LS = 2,
    XINPUTBTN_LT = 256,
    XINPUTBTN_RB = 2048,
    XINPUTBTN_RS = 4,
    XINPUTBTN_RT = 512,
    XINPUTBTN_START = 8,
    XINPUTBTN_X = 32768,
    XINPUTBTN_Y = 4096
} XInputButtonEnum;

struct GameObjectSubStruct2 {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    undefined field_0x8;
    undefined field_0x9;
    undefined field_0xa;
    undefined field_0xb;
    undefined field_0xc;
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    undefined field_0x18;
    undefined field_0x19;
    undefined field_0x1a;
    undefined field_0x1b;
    undefined field_0x1c;
    undefined field_0x1d;
    undefined field_0x1e;
    undefined field_0x1f;
    undefined field_0x20;
    undefined field_0x21;
    undefined field_0x22;
    undefined field_0x23;
    undefined field_0x24;
    undefined field_0x25;
    undefined field_0x26;
    undefined field_0x27;
    void* field_0x28;
    undefined field_0x2c;
    undefined field_0x2d;
    undefined field_0x2e;
    undefined field_0x2f;
    undefined field_0x30;
    undefined field_0x31;
    undefined field_0x32;
    undefined field_0x33;
    undefined field_0x34;
    undefined field_0x35;
    undefined field_0x36;
    undefined field_0x37;
    undefined field_0x38;
    undefined field_0x39;
    undefined field_0x3a;
    undefined field_0x3b;
    undefined field_0x3c;
    undefined field_0x3d;
    undefined field_0x3e;
    undefined field_0x3f;
    undefined field_0x40;
    undefined field_0x41;
    undefined field_0x42;
    undefined field_0x43;
    undefined field_0x44;
    undefined field_0x45;
    undefined field_0x46;
    undefined field_0x47;
    undefined field_0x48;
    undefined field_0x49;
    short field_0x4a;
    short field_0x4c;
    short field_0x4e;
    short field_0x50;
    undefined field_0x52;
    undefined field_0x53;
    undefined field_0x54;
    undefined field_0x55;
    undefined field_0x56;
    undefined field_0x57;
    undefined field_0x58;
    undefined field_0x59;
    undefined field_0x5a;
    undefined field_0x5b;
};

struct GameObjectData {
    WORD objectID;
    char field_0x2;
    char field_0x3;
    struct GameObjectData* field_0x4;
    struct GameObjectData* nextObject;
    DWORD stateFlags;
    int field_0x10;
    DWORD unc_upcomingAction;
    WORD actNo;
    WORD field_0x1a;
    short field_0x1c;
    short field_0x1e;
    void* field_0x20;
    undefined field_0x24;
    undefined field_0x25;
    undefined field_0x26;
    char playerIndex;
    unsigned short field_0x28;
    WORD field_0x2a;
    struct PlayerData* playerData;
    void (*UNC_EffectDrawCallback)(struct GameObjectData*);
    DWORD field_0x34;
    DWORD field_0x38;
    undefined field_0x3c;
    undefined field_0x3d;
    undefined field_0x3e;
    undefined field_0x3f;
    unsigned short nHitboxIndex;
    undefined field_0x42;
    undefined field_0x43;
    undefined field_0x44;
    undefined field_0x45;
    undefined field_0x46;
    undefined field_0x47;
    void* arrHitboxStorage;
    short field_0x4c;
    undefined field_0x4e;
    undefined field_0x4f;
    short preservedAspectScalingFactor;
    short verticalScalingFactor;
    void* hitboxPointer;
    undefined field_0x58;
    undefined field_0x59;
    undefined field_0x5a;
    undefined field_0x5b;
    byte field_0x5c;
    byte field_0x5d;
    byte field_0x5e;
    undefined field_0x5f;
    WORD field_0x60;
    undefined field_0x62;
    undefined field_0x63;
    struct GameObjectData* lpVictim;
    undefined field_0x68;
    undefined field_0x69;
    undefined field_0x6a;
    undefined field_0x6b;
    struct GameObjectData* lpAttacker;
    int unc_xPosMemory;
    int unc_yPosMemory;
    DWORD field_0x78;
    undefined field_0x7c;
    undefined field_0x7d;
    byte field_0x7e;
    undefined field_0x7f;
    undefined field_0x80;
    undefined field_0x81;
    undefined field_0x82;
    undefined field_0x83;
    byte numHitboxes;
    undefined field_0x85;
    short field_0x86;
    struct GameObjectSubStruct2* unk_SubStructure2;
    struct UNC_PlayerSubStructure* unk_SubStructure;
    undefined field_0x90;
    undefined field_0x91;
    undefined field_0x92;
    undefined field_0x93;
    void (*UNC_Callback0x94)(struct GameObjectData*);
    short d3dTextureIndex; /* Created by retype action */
    WORD field_0x9a;
    short textureMetadataOffsetStart;
    short field_0x9e;
    short textureMetadataLength;
    short imageNo; /* Created by retype action */
    int field_0xa4;
    undefined field_0xa8;
    undefined field_0xa9;
    undefined field_0xaa;
    undefined field_0xab;
    DWORD dwGraphicalEffects; /* 0x20000 means no shadow */
    int xPos;
    int ypos;
    int xvel;
    int yvel;
    undefined field_0xc0;
    undefined field_0xc1;
    undefined field_0xc2;
    undefined field_0xc3;
    undefined field_0xc4;
    undefined field_0xc5;
    undefined field_0xc6;
    undefined field_0xc7;
    DWORD field_0xc8;
    DWORD field_0xcc;
    int field_0xd0;
    DWORD gravity;
    undefined field_0xd8;
    undefined field_0xd9;
    undefined field_0xda;
    undefined field_0xdb;
    undefined field_0xdc;
    undefined field_0xdd;
    short field_0xde;
    DWORD** unk_DWORDARR;
    DWORD field_0xe4;
    DWORD field_0xe8;
    DWORD* field_0xec;
    WORD UNK_ANIM;
    WORD field_0xf2;
    unsigned short field_0xf4;
    undefined field_0xf6;
    undefined field_0xf7;
    WORD UNK_AnimationID;
    WORD UNC_AnimationClock;
    undefined field_0xfc;
    byte field_0xfd;
    undefined field_0xfe;
    undefined field_0xff;
    undefined field_0x100;
    undefined field_0x101;
    undefined field_0x102;
    undefined field_0x103;
    undefined field_0x104;
    undefined field_0x105;
    undefined field_0x106;
    undefined field_0x107;
    undefined field_0x108;
    undefined field_0x109;
    byte field_0x10a;
    undefined field_0x10b;
    undefined field_0x10c;
    undefined field_0x10d;
    undefined field_0x10e;
    undefined field_0x10f;
    undefined field_0x110;
    undefined field_0x111;
    undefined field_0x112;
    undefined field_0x113;
    undefined field_0x114;
    undefined field_0x115;
    undefined field_0x116;
    undefined field_0x117;
    undefined field_0x118;
    undefined field_0x119;
    undefined field_0x11a;
    undefined field_0x11b;
    undefined field_0x11c;
    undefined field_0x11d;
    undefined field_0x11e;
    undefined field_0x11f;
    undefined field_0x120;
    undefined field_0x121;
    undefined field_0x122;
    undefined field_0x123;
    undefined field_0x124;
    undefined field_0x125;
    undefined field_0x126;
    undefined field_0x127;
    undefined field_0x128;
    undefined field_0x129;
    undefined field_0x12a;
    undefined field_0x12b;
    undefined field_0x12c;
    undefined field_0x12d;
    undefined field_0x12e;
    undefined field_0x12f;
};

struct PlayerData {
    short currentTension;
    byte field_0x2;
    byte field_0x3;
    unsigned short field_0x4;
    unsigned short field_0x6;
    undefined field_0x8;
    undefined field_0x9;
    WORD field_0xa;
    undefined field_0xc;
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
    int unc_RunningXVel;
    int gameObjectXOffset;
    undefined field_0x18;
    undefined field_0x19;
    short guardBalance; /* SHORT, not unsigned short. Has negative values. */
    undefined field_0x1c;
    undefined field_0x1d;
    unsigned short field_0x1e;
    undefined field_0x20;
    undefined field_0x21;
    undefined field_0x22;
    undefined field_0x23;
    undefined field_0x24;
    undefined field_0x25;
    undefined field_0x26;
    undefined field_0x27;
    undefined field_0x28;
    undefined field_0x29;
    undefined field_0x2a;
    byte field_0x2b;
    undefined field_0x2c;
    undefined field_0x2d;
    undefined field_0x2e;
    unsigned short field_0x2f;
    undefined field_0x31;
    undefined field_0x32;
    undefined field_0x33;
    undefined field_0x34;
    char receiveRakushoBonus;
    undefined field_0x36;
    undefined field_0x37;
    DWORD field_0x38;
    undefined field_0x3c;
    undefined field_0x3d;
    undefined field_0x3e;
    undefined field_0x3f;
    void* field_0x40;
    struct GameObjectData* otherPlayerGameObject;
    enum XInputButtonEnum ctrlP;
    enum XInputButtonEnum unc_ctrls_K;
    enum XInputButtonEnum unc_ctrls_S;
    enum XInputButtonEnum unc_ctrls_H;
    enum XInputButtonEnum unc_ctrls_D;
    enum XInputButtonEnum unc_ctrls_Respect;
    enum XInputButtonEnum unc_ctrls_Reset;
    enum XInputButtonEnum unc_ctrls_Pause;
    enum XInputButtonEnum unc_ctrls_RecPlayer;
    enum XInputButtonEnum unc_ctrls_RecEnemy;
    enum XInputButtonEnum unc_ctrls_PlayMemory;
    enum XInputButtonEnum unc_ctrls_Switch;
    enum XInputButtonEnum unc_ctrls_EnemyWalk;
    enum XInputButtonEnum unc_ctrls_EnemyJump;
    enum XInputButtonEnum unc_ctrls_PKMacro;
    enum XInputButtonEnum unc_ctrls_PDMacro;
    enum XInputButtonEnum unc_ctrls_PKSMacro;
    enum XInputButtonEnum unc_ctrls_PKSHMacro;
    unsigned short unc_currentLife;
    unsigned short redHealth;
    byte nAirJumpsRemaining;
    byte nAirDashesRemaining;
    WORD field_0x72;
    undefined field_0x74;
    undefined field_0x75;
    undefined field_0x76;
    undefined field_0x77;
    undefined field_0x78;
    undefined field_0x79;
    undefined field_0x7a;
    undefined field_0x7b;
    undefined1 currentFaint;
    undefined field_0x7d;
    undefined field_0x7e;
    undefined field_0x7f;
    void* unc_funcPtr9;
    undefined field_0x84;
    undefined1 field_0x85;
    undefined field_0x86;
    byte maxFaint;
    unsigned short characterMeter1; /* Johnny: Mist finer level */
    short field_0x8a;
    WORD characterMeter2; /* Johnny: Number of coins left, Eddie: Eddie gauge */
    short field_0x8e;
    undefined field_0x90;
    undefined field_0x91;
    undefined field_0x92;
    undefined field_0x93;
    short field_0x94;
    undefined field_0x96;
    undefined field_0x97;
    undefined field_0x98;
    undefined field_0x99;
    undefined field_0x9a;
    undefined field_0x9b;
    undefined field_0x9c;
    undefined field_0x9d;
    undefined field_0x9e;
    undefined field_0x9f;
    undefined field_0xa0;
    undefined field_0xa1;
    undefined field_0xa2;
    undefined field_0xa3;
    undefined field_0xa4;
    undefined field_0xa5;
    undefined field_0xa6;
    undefined field_0xa7;
    undefined field_0xa8;
    undefined field_0xa9;
    undefined field_0xaa;
    undefined field_0xab;
    DWORD field_0xac;
    undefined field_0xb0;
    undefined field_0xb1;
    undefined field_0xb2;
    undefined field_0xb3;
    void* unc_funcPtrs1;
    void* unc_funcPtrs2;
    void* unc_funcPtrs3;
    void* unc_funcPtrs4;
    void* unc_funcPtrs5;
    void* unc_funcPtrs6;
    void* unc_funcPtrs7;
    void* unc_funcPtrs8;
    short* UNC_ThrowOffsetPointer; /* Created by retype action */
    byte isInstantKillMode; /* Created by retype action */
    undefined1 IsProjectileThrown; /* Created by retype action */
    undefined field_0xda;
    undefined field_0xdb;
    DWORD field_0xdc;
    undefined field_0xe0;
    undefined field_0xe1;
    undefined field_0xe2;
    undefined field_0xe3;
    undefined field_0xe4;
    undefined field_0xe5;
    undefined field_0xe6;
    undefined field_0xe7;
    undefined field_0xe8;
    undefined field_0xe9;
    undefined field_0xea;
    undefined field_0xeb;
    undefined field_0xec;
    undefined field_0xed;
    byte field_0xee;
    undefined field_0xef;
    byte field_0xf0;
    byte field_0xf1;
    byte field_0xf2;
    byte field_0xf3;
    byte field_0xf4;
    byte field_0xf5;
    undefined field_0xf6;
    undefined field_0xf7;
    unsigned short currentBurst;
    undefined field_0xfa;
    undefined field_0xfb;
    undefined field_0xfc;
    undefined field_0xfd;
    undefined field_0xfe;
    undefined field_0xff;
    undefined field_0x100;
    undefined field_0x101;
    byte field_0x102;
    byte field_0x103;
    byte field_0x104;
    byte field_0x105;
    byte field_0x106;
    byte field_0x107;
    byte field_0x108;
    byte field_0x109;
    undefined field_0x10a;
    undefined field_0x10b;
    undefined field_0x10c;
    undefined field_0x10d;
    undefined field_0x10e;
    undefined field_0x10f;
    undefined field_0x110;
    undefined field_0x111;
    byte field_0x112;
    undefined field_0x113;
    undefined field_0x114;
    byte field_0x115;
    undefined field_0x116;
    undefined field_0x117;
    int gameObjectYOffset;
    int field_0x11c;
    int field_0x120;
    undefined field_0x124;
    undefined field_0x125;
    undefined field_0x126;
    undefined field_0x127;
    undefined field_0x128;
    undefined field_0x129;
    undefined field_0x12a;
    undefined field_0x12b;
    undefined field_0x12c;
    undefined field_0x12d;
    undefined field_0x12e;
    undefined field_0x12f;
    undefined field_0x130;
    undefined field_0x131;
    undefined field_0x132;
    undefined field_0x133;
    undefined field_0x134;
    undefined field_0x135;
    undefined field_0x136;
    undefined field_0x137;
    undefined field_0x138;
    undefined field_0x139;
    undefined field_0x13a;
    undefined field_0x13b;
    undefined field_0x13c;
    undefined field_0x13d;
    undefined field_0x13e;
    undefined field_0x13f;
    undefined field_0x140;
    undefined field_0x141;
    undefined field_0x142;
    undefined field_0x143;
};

struct UNC_PlayerSubStructure {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    undefined field_0x8;
    undefined field_0x9;
    undefined field_0xa;
    undefined field_0xb;
    undefined field_0xc;
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    undefined field_0x18;
    undefined field_0x19;
    undefined field_0x1a;
    undefined field_0x1b;
    undefined field_0x1c;
    undefined field_0x1d;
    undefined field_0x1e;
    undefined field_0x1f;
    undefined field_0x20;
    undefined field_0x21;
    undefined field_0x22;
    undefined field_0x23;
    undefined field_0x24;
    undefined field_0x25;
    undefined field_0x26;
    undefined field_0x27;
    void* unc_FunctionPointer;
    undefined field_0x2c;
    undefined field_0x2d;
    undefined field_0x2e;
    undefined field_0x2f;
    undefined field_0x30;
    undefined field_0x31;
    undefined field_0x32;
    undefined field_0x33;
    undefined field_0x34;
    undefined field_0x35;
    undefined field_0x36;
    undefined field_0x37;
    undefined field_0x38;
    undefined field_0x39;
    undefined field_0x3a;
    undefined field_0x3b;
    undefined field_0x3c;
    undefined field_0x3d;
    undefined field_0x3e;
    undefined field_0x3f;
    undefined field_0x40;
    undefined field_0x41;
    undefined field_0x42;
    undefined field_0x43;
    undefined field_0x44;
    undefined field_0x45;
    undefined field_0x46;
    undefined field_0x47;
    undefined field_0x48;
    undefined field_0x49;
    undefined field_0x4a;
    undefined field_0x4b;
    undefined field_0x4c;
    undefined field_0x4d;
    undefined field_0x4e;
    undefined field_0x4f;
    undefined field_0x50;
    undefined field_0x51;
    undefined field_0x52;
    undefined field_0x53;
};