#include <windows.h>
#include <cstdint>
#include <stdio.h>

#define ArrayOffset 0x465D170

void  StartConsole() {
    if (AllocConsole()) {
        FILE *fpstdout = stdout;
        freopen_s(&fpstdout, "CONOUT$", "w", stdout);
    }

}

void *GetImageInfo() {

    HANDLE moduleHandle = GetModuleHandleA("DarkSoulsIII.exe");
    if (moduleHandle) {
        MEMORY_BASIC_INFORMATION memInfo;
        if (VirtualQuery((void *) moduleHandle, &memInfo, sizeof(memInfo)) != 0) {
            IMAGE_DOS_HEADER *hDos = (IMAGE_DOS_HEADER *) moduleHandle;
            IMAGE_NT_HEADERS *hPe = (IMAGE_NT_HEADERS *) ((ULONG64) memInfo.AllocationBase + (ULONG64) hDos->e_lfanew);

            if ((hDos->e_magic == IMAGE_DOS_SIGNATURE) && (hPe->Signature == IMAGE_NT_SIGNATURE)) {
                return memInfo.AllocationBase;
            }
        }
    }

    return nullptr;

}

void PatchArray() {
    //StartConsole();
    uintptr_t baseAddress = (uintptr_t) GetImageInfo();
    uint64_t *arrayAddress = (uint64_t *) (baseAddress + ArrayOffset);
    if (*arrayAddress > 0x60000000)
        return;

    for (int i = 0; i < 190; ++i) {
        arrayAddress[i] *= 2;
    }
}


BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            PatchArray();
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
            break;
    }
    return TRUE;
}

