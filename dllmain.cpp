//
// NFSU2 EAXSound::Update Delta Time Fix
//

#include "framework.h"
#include "injector.hpp"

constexpr const float MinDeltaTime = (1.0f / 60.0f);
uintptr_t pEAXSound_Update;

#pragma runtime_checks("", off)

static void __stdcall EAXSound_Update_Hook(float t)
{
    void* that;
    _asm mov that, ecx

    // clamp min deltatime to 1/60 to avoid slowness...
    float newDT = t;
    if ((newDT < MinDeltaTime) && (newDT > 0.0f))
        newDT = MinDeltaTime;

    return reinterpret_cast<void(__thiscall*)(void*, float)>(pEAXSound_Update)(that, newDT);
}

#pragma runtime_checks("", restore)

void Init()
{
    uintptr_t loc_5814A8 = 0x5814A8;
    pEAXSound_Update = static_cast<uintptr_t>(injector::GetBranchDestination(loc_5814A8));
    injector::MakeCALL(loc_5814A8, EAXSound_Update_Hook);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Init();
        break;
    }
    return TRUE;
}
