// averysumner - traveller
// source/traveller/dll_main.cc
// contains DllMain entry point
// Copyright 2021 averysumner
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// notice: this entire file is a huge mess and that probably won't change without some serious restructuring

#include <Windows.h>
#include <stdio.h>

#include "dynohook/core.h"
#include "dynohook/manager.h"
#include "dynohook/hook.h"
#include "dynohook/conventions/x86_ms_cdecl.h"

#include "logger.hh"
#include "addresses.hh"
#include "raw_api.hh"
#include "object_manager.hh"
#include "server.hh"
#include "client.hh"
#include "hook.hh"

using namespace traveller; // we can do this in the main file since DllMain can't be in a namespace

// =================================
// dinput8 jump functions
// =================================

HMODULE dinput8;
FARPROC dinput8_functions[5];

__declspec(naked) void __stdcall jumpDirectInput8Create() {
    _asm {
        jmp dinput8_functions[0];
    }
}

__declspec(naked) void __stdcall jumpDllCanUnloadNow() {
    _asm {
        jmp dinput8_functions[1];
    }
}

__declspec(naked) void __stdcall jumpDllGetClassObject() {
    _asm {
        jmp dinput8_functions[2];
    }
}

__declspec(naked) void __stdcall jumpDllRegisterServer() {
    _asm {
        jmp dinput8_functions[3];
    }
}

__declspec(naked) void __stdcall jumpDllUnregisterServer() {
    _asm {
        jmp dinput8_functions[4];
    }
}

// =================================
// thread functions
// =================================

DWORD WINAPI peerThread(Peer* __peer) {
    __peer->start();

    while (true) {
        __peer->update();
    }
}

// =================================
// engine event hooking
// =================================

typedef void(*t_event)();

t_event event_pre_initialize_trampoline;
t_event event_post_initialize_trampoline;
t_event event_update_trampoline;

void eventPreInitialize() {
    TRAVELLER_LOG("Running pre-initialization.");

    //*RawAPI::do_log = true;

    //*RawAPI::_netclient = true;

    event_pre_initialize_trampoline();
}

Peer* peer = nullptr;

void eventPostInitialize() {
    TRAVELLER_LOG("Running post-initialization.");

    //*RawAPI::isWindowed = true; // todo: make this configurable
    
    
    for (int i = 1; i < *RawAPI::argc; ++i) {
        std::string argument = (*RawAPI::argv)[i] + 1;

        if (argument == "server") {
            peer = new Server("localhost", 42069, 64);
        }
        else if (argument == "client") {
            *RawAPI::_netclient = true;
            peer = new Client("localhost", 42069);
        }
    }

    if (peer) {
        //HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)peerThread, peer, 0, 0);
        peer->start();
    }
}

void eventUpdate() {
    /*for (auto& object : ObjectManager::getObjects()) {
      t_vec3 position = object.second.getPosition();
      t_vec3 velocity = object.second.getVelocity();
      TRAVELLER_LOG_DEBUG("Entity ID: %i", object.first);
      TRAVELLER_LOG_DEBUG("Position: x: %f, y: %f, z: %f", position.x, position.y, position.z);
      TRAVELLER_LOG_DEBUG("Velocity: x: %f, y: %f, z: %f", velocity.x, velocity.y, velocity.z);
    }*/

    if (GetAsyncKeyState(VK_F3)) {
        std::cout << "Enter character ID: ";
        int characterID;
        std::cin >> characterID;

        //GameObject_s* game_object = RawAPI::AddGameObject();
        //RawAPI::InitCreature(game_object, characterID, 1);

        RawAPI::AddDynamicCreature(characterID, new nuvec_s(-27.0f, 0.0f, -50.0f), 32728, 0, nullptr, nullptr, 1, nullptr, nullptr, 0, 1);
    }

    if (peer) peer->update();
    
    /*
    for (auto& object : ObjectManager::getObjects()) {
      nuvec_s position = object.second.getPosition();
      nuvec_s velocity = object.second.getVelocity();
      TRAVELLER_LOG_DEBUG("Entity ID: %u", object.first);
      TRAVELLER_LOG_DEBUG("Character ID: %u", object.second.getGameObject()->character_id);
      TRAVELLER_LOG_DEBUG("Position: x: %f, y: %f, z: %f", position.x, position.y, position.z);
      TRAVELLER_LOG_DEBUG("Velocity: x: %f, y: %f, z: %f", velocity.x, velocity.y, velocity.z);
    }*/

    event_update_trampoline();
}

// =================================
// engine function hooking
// =================================

// todo: move these somewhere else

// copy pasted from ghidra
void* apiObjectCreate(size_t* __parameter) {
    size_t _Size;
    int iVar1;
    void* _Dst;

    if ((__parameter != (size_t*)0x0) && (_Size = *__parameter, _Size != 0)) {
        _Dst = (void*)__parameter[1];
        iVar1 = 0;
        do {
            if ((*(char*)((int)_Dst + 0x1fc) & 1) == 0) {
                memset(_Dst, 0, _Size);
                *(uint32_t*)((int)_Dst + 0x1fc) = *(uint32_t*)((int)_Dst + 0x1fc) | 1;
                *(char*)((int)_Dst + 0x28d) = (char)iVar1;
                ObjectManager::registerObject((GameObject_s*)_Dst); // notify object manager
                return _Dst;
            }
            iVar1 = iVar1 + 1;
            _Dst = (void*)((int)_Dst + _Size);
        } while (iVar1 < 0x40);
    }

    return (void*)0x0;
}

// also copy pasted from ghidra
void apiObjectDestroy(size_t* __parameter_0, void* __parameter_1) {
    byte bVar1;
    int iVar2;
    size_t sVar3;

    ObjectManager::unregisterObject((GameObject_s*)__parameter_1);

    if (((__parameter_0 != (size_t*)0x0) && (__parameter_1 != (void*)0x0)) && (*__parameter_0 != 0)) {
        if (__parameter_0 != (size_t*)0xfffffff8) {
            sVar3 = __parameter_0[1];
            iVar2 = 0x40;
            do {
                if ((*(byte*)(sVar3 + 0x1fc) & 1) != 0) {
                    RawAPI::FUN_0062a8c0((int)__parameter_0, sVar3, (int)__parameter_1);
                }
                sVar3 = sVar3 + *__parameter_0;
                iVar2 = iVar2 + -1;
            } while (iVar2 != 0);
            bVar1 = *(byte*)((int)__parameter_1 + 0x28d);
            __parameter_0[(uint32_t)bVar1 * 2 + 2] = 0;
            __parameter_0[(uint32_t)bVar1 * 2 + 3] = 0;
        }
        memset(__parameter_1, 0, *__parameter_0);
    }

    return;
}

// =================================
// utility functions
// =================================

void initializeConsole() {
    AllocConsole(); // spawns console on main process; todo: make this configurable

    // redirect stdin, stdout and stderr so we control them
    FILE* dummy_file;
    freopen_s(&dummy_file, "CONIN$", "r", stdin);
    freopen_s(&dummy_file, "CONOUT$", "w", stdout);
    freopen_s(&dummy_file, "CONOUT$", "w", stderr);

    TRAVELLER_LOG("Initialized console.");
}

void initializeDLLProxy() {
    dinput8 = LoadLibraryA("C:\\Windows\\System32\\dinput8.dll"); // todo: check if this actually loads

    if (dinput8) {
        TRAVELLER_LOG("Successfully loaded dinput8.dll for call redirects.");
    }
    else {
        TRAVELLER_LOG_ERROR("Failed to load dinput8.dll for call redirects!");
    }

    // assign addresses of functions
    dinput8_functions[0] = GetProcAddress(dinput8, "DirectInput8Create");
    dinput8_functions[1] = GetProcAddress(dinput8, "DllCanUnloadNow");
    dinput8_functions[2] = GetProcAddress(dinput8, "DllGetClassObject");
    dinput8_functions[3] = GetProcAddress(dinput8, "DllRegisterServer");
    dinput8_functions[4] = GetProcAddress(dinput8, "DllUnregisterServer");
}

dyno::ReturnAction hookPreInitialize(dyno::CallbackType type, dyno::IHook& hook) {
    int argc = hook.getArgument<int>(0);
    TRAVELLER_LOG("argc: %i", argc);
    //*RawAPI::_netclient = true;

    return dyno::ReturnAction::Ignored;
}

dyno::ReturnAction hookAddDynamicCreature(dyno::CallbackType type, dyno::IHook& hook) {
    TRAVELLER_LOG_DEBUG("AddDynamicCreature CALLED");
    int character_id = hook.getArgument<int>(0);
    TRAVELLER_LOG_DEBUG("character_id: %i", character_id);
    nuvec_s* position = hook.getArgument<nuvec_s*>(1);
    TRAVELLER_LOG_DEBUG("position: X: %f, Y: %f, Z: %f", position->x, position->y, position->z);
    int param_3 = hook.getArgument<int>(2);
    TRAVELLER_LOG_DEBUG("param_3: %i", param_3);
    char* param_4 = hook.getArgument<char*>(3);
    TRAVELLER_LOG_DEBUG("param_4: %s", param_4);
    int param_5 = hook.getArgument<int>(4);
    TRAVELLER_LOG_DEBUG("param_5: %i", param_5);
    int param_6 = hook.getArgument<int>(5);
    TRAVELLER_LOG_DEBUG("param_6: %i", param_6);
    int param_7 = hook.getArgument<int>(6);
    TRAVELLER_LOG_DEBUG("param_7: %i", param_7);
    int param_8 = hook.getArgument<int>(7);
    TRAVELLER_LOG_DEBUG("param_8: %i", param_8);
    int param_9 = hook.getArgument<int>(8);
    TRAVELLER_LOG_DEBUG("param_9: %i", param_9);
    int param_10 = hook.getArgument<int>(9);
    TRAVELLER_LOG_DEBUG("param_10: %i", param_10);
    int param_11 = hook.getArgument<int>(10);
    TRAVELLER_LOG_DEBUG("param_11: %i", param_11);

    return dyno::ReturnAction::Ignored;
}

void installHooks() {
    auto& manager = dyno::HookManager::Get();
    
    auto pre_initialize_hook = manager.hookDetour((void*)TRAVELLER_EVENT_PRE_INITIALIZE_ADDRESS, [] { return new dyno::x86MsCdecl({ dyno::DataType::Int32, dyno::DataType::Pointer }, dyno::DataType::Int32); });
    pre_initialize_hook->addCallback(dyno::CallbackType::Pre, (dyno::CallbackHandler)hookPreInitialize);

    auto add_dynamic_creature_hook = manager.hookDetour((void*)0x0043d890, [] { return new dyno::x86MsCdecl({ dyno::DataType::Int32, dyno::DataType::Pointer, dyno::DataType::Int32, dyno::DataType::Pointer, dyno::DataType::Pointer, dyno::DataType::Pointer, dyno::DataType::Int32, dyno::DataType::Pointer, dyno::DataType::Pointer, dyno::DataType::Int32, dyno::DataType::Int32 }, dyno::DataType::Pointer); });
    add_dynamic_creature_hook->addCallback(dyno::CallbackType::Pre, (dyno::CallbackHandler)hookAddDynamicCreature);
    
    /*
    PVOID api_object_create = (PVOID)TRAVELLER_FUNCTION_API_OBJECT_CREATE_ADDRESS;
    PVOID api_object_destroy = (PVOID)TRAVELLER_FUNCTION_API_OBJECT_DESTROY_ADDRESS;
    PVOID event_pre_initialize = (PVOID)TRAVELLER_EVENT_PRE_INITIALIZE_ADDRESS;
    PVOID event_post_initialize = (PVOID)TRAVELLER_EVENT_POST_INITIALIZE_ADDRESS;
    PVOID event_update = (PVOID)TRAVELLER_EVENT_UPDATE_ADDRESS;

    DetourRestoreAfterWith();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&api_object_create, apiObjectCreate);
    DetourAttach(&api_object_destroy, apiObjectDestroy);
    DetourAttachEx(&event_pre_initialize, eventPreInitialize, &event_pre_initialize_trampoline, nullptr, nullptr);
    DetourAttachEx(&event_post_initialize, eventPostInitialize, &event_post_initialize_trampoline, nullptr, nullptr);
    DetourAttachEx(&event_update, eventUpdate, &event_update_trampoline, nullptr, nullptr);
    DetourTransactionCommit();*/

    // engine event hooks
    //event_pre_initialize_trampoline = (t_event)Hook::trampoline(TRAVELLER_EVENT_PRE_INITIALIZE_ADDRESS, eventPreInitialize);
    //Hook::replaceCall(TRAVELLER_EVENT_POST_INITIALIZE_ADDRESS, eventPostInitialize);
    event_update_trampoline = (t_event)Hook::trampoline(TRAVELLER_EVENT_UPDATE_ADDRESS, eventUpdate, 7);

    // engine function hooks
    Hook::detour(TRAVELLER_FUNCTION_API_OBJECT_CREATE_ADDRESS, apiObjectCreate);
    Hook::detour(TRAVELLER_FUNCTION_API_OBJECT_DESTROY_ADDRESS, apiObjectDestroy);

    // nop inserts
    Hook::insertNOP(TRAVELLER_SET_WINDOW_FOCUSED_ADDRESS, 6); // disable freeze on window unfocus
}

void removeHooks() {
    /*
    PVOID api_object_create = (PVOID)TRAVELLER_FUNCTION_API_OBJECT_CREATE_ADDRESS;
    PVOID api_object_destroy = (PVOID)TRAVELLER_FUNCTION_API_OBJECT_DESTROY_ADDRESS;
    PVOID event_pre_initialize = (PVOID)TRAVELLER_EVENT_PRE_INITIALIZE_ADDRESS;
    PVOID event_post_initialize = (PVOID)TRAVELLER_EVENT_POST_INITIALIZE_ADDRESS;
    PVOID event_update = (PVOID)TRAVELLER_EVENT_UPDATE_ADDRESS;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&api_object_create, apiObjectCreate);
    DetourDetach(&api_object_destroy, apiObjectDestroy);
    DetourDetach(&event_pre_initialize, eventPreInitialize);
    DetourDetach(&event_post_initialize, eventPostInitialize);
    DetourDetach(&event_update, eventUpdate);
    DetourTransactionCommit();*/
}

// =================================
// DllMain
// =================================

BOOL WINAPI DllMain(HMODULE __module, DWORD __reason, LPVOID __load_type) {
    //if (DetourIsHelperProcess()) return TRUE;

    switch (__reason) {
        case DLL_PROCESS_ATTACH:
            initializeConsole();
            initializeDLLProxy();
            installHooks();
            break;
        case DLL_PROCESS_DETACH:
            removeHooks();
            FreeLibrary(dinput8); // free loaded DLL
            TRAVELLER_LOG("Freed redirect dinput8.dll.");
            break;
        default:
            break;
    }

    return TRUE;
}