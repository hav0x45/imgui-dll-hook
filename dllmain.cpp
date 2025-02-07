#define WIN32_LEAN_AND_MEAN
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Windows.h>
#include <thread>
#include <cstdint>
#include "hooks.h"

// Setup function
void Setup(const HMODULE instance) {

    try {
        MyGui::Setup();
        MyHooks::Setup();
    } catch (const std::exception& error) {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, error.what(), "Error!", 0);
        goto UNLOAD;
    }
    
    while (!GetAsyncKeyState(VK_END)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

UNLOAD:
    MyHooks::Destroy();
    MyGui::DestroyMenu();
    FreeLibraryAndExitThread(instance, 0);
}

// Entry point
BOOL WINAPI DllMain(const HMODULE instance, const uintptr_t reason, const void* reserved) {

    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);

        const auto MyThread = CreateThread(nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup),
            instance,
            0,
            nullptr);

        if (MyThread) {
            CloseHandle(MyThread);
        }
    }

    return true;
};