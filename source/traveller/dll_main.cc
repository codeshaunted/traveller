// averysumner - traveller
// dll_main.cc
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

#include "subhook.h"

#include "logger.hh"
#include "addresses.hh"
#include "raw_api.hh"
#include "object_manager.hh"
#include "server.hh"

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
// engine event hooking
// =================================

subhook::Hook event_pre_initialize_hook;
subhook::Hook event_post_initialize_hook;
subhook::Hook event_update_hook;

typedef void(*t_event)();

void eventPreInitialize() {
  TRAVELLER_LOG("Running pre-initialization.");

  ((t_event)event_pre_initialize_hook.GetTrampoline())();
}

static Peer* peer;

static bool post_initialized = false;
void eventPostInitialize() {
  if (post_initialized) {
    ((t_event)event_post_initialize_hook.GetTrampoline())();
    return;
  }
  else {
    post_initialized = true;
  }

  TRAVELLER_LOG("Running post-initialization.");

  for (int i = 1; i < *raw_api::argc; ++i) {
    std::string argument = *raw_api::argv[i];
    argument.erase(0, 1);

    if (argument == "server") {
      peer = new Server("0.0.0.0", 42069, 64); // todo: add cli flags to configure this
    }
  }

  peer->start();

  ((t_event)event_post_initialize_hook.GetTrampoline())();
}

void eventUpdate() {
  //peer->update();

  ((t_event)event_update_hook.GetTrampoline())();
}

// =================================
// engine function hooking
// =================================

// todo: move these somewhere else

subhook::Hook function_api_object_create_hook;
subhook::Hook function_api_object_destroy_hook;

typedef void* (*t_apiObjectCreate)(size_t*);
typedef void (*t_apiObjectDestroy)(size_t*, void*);

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
          raw_api::FUN_0062a8c0((int)__parameter_0, sVar3, (int)__parameter_1);
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

void installEngineEventHooks() {
  event_pre_initialize_hook.Install((void*)TRAVELLER_EVENT_PRE_INITIALIZE_ADDRESS, (void*)eventPreInitialize);
  event_post_initialize_hook.Install((void*)TRAVELLER_EVENT_POST_INITIALIZE_ADDRESS, (void*)eventPostInitialize);
  event_update_hook.Install((void*)TRAVELLER_EVENT_UPDATE_ADDRESS, (void*)eventUpdate);
}

void installEngineFunctionHooks() {
  function_api_object_create_hook.Install((void*)TRAVELLER_FUNCTION_API_OBJECT_CREATE, (void*)apiObjectCreate);
  function_api_object_destroy_hook.Install((void*)TRAVELLER_FUNCTION_API_OBJECT_DESTROY, (void*)apiObjectDestroy);
}

// =================================
// DllMain
// =================================

BOOL WINAPI DllMain(HMODULE __module, DWORD __reason, LPVOID __load_type) {
  switch (__reason) {
    case DLL_PROCESS_ATTACH:
      initializeConsole();
      initializeDLLProxy();
      installEngineEventHooks();
      installEngineFunctionHooks();
      break;
    case DLL_PROCESS_DETACH:
      FreeLibrary(dinput8); // free loaded DLL
      TRAVELLER_LOG("Freed redirect dinput8.dll.");
      break;
    default:
      break;
  }

  return TRUE;
}