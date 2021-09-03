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

#include <Windows.h>
#include <stdio.h>

#include <filesystem>
#include <vector>

#include "subhook.h"

#include "logger.hh"
#include "addresses.hh"

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

struct s_Mod {
  t_event event_pre_initialize;
  t_event event_post_initialize;
  t_event event_update;
};

std::vector<s_Mod> mods;

void eventPreInitialize() {
  TRAVELLER_LOG("Running mod pre initializations.");
  ((t_event)event_pre_initialize_hook.GetTrampoline())();
}

void eventPostInitialize() {
  TRAVELLER_LOG("Running mod post initializations.");
  ((t_event)event_post_initialize_hook.GetTrampoline())();
}

void eventUpdate() {
  ((t_event)event_update_hook.GetTrampoline())();
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

void loadMods() {
  std::filesystem::create_directory("mods"); // creates the "mods" folder if it doesn't already exist.

  for (const auto& directory : std::filesystem::directory_iterator("mods")) {
    std::filesystem::path mod_path = std::filesystem::absolute(directory) / "mod.dll";
    HMODULE mod = LoadLibraryA((LPCSTR)mod_path.string().c_str());

    s_Mod mod_functions;
    mod_functions.event_pre_initialize = (t_event)GetProcAddress(mod, "eventPreInitialize");
    mod_functions.event_post_initialize = (t_event)GetProcAddress(mod, "eventPostInitialize");
    mod_functions.event_update = (t_event)GetProcAddress(mod, "eventUpdate");

    if (!mod) TRAVELLER_LOG_ERROR("Failed to load mod at \"%s\"!", mod_path);
    else mods.push_back(mod_functions);
  }
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
      loadMods();
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