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

#include "logger.hh"

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
// utility functions
// =================================

void initializeConsole() {
  AllocConsole(); // spawns console on main process; todo: make this configurable
  
  // redirect stdin, stdout and stderr so we control them
  FILE* dummy_file;
  freopen_s(&dummy_file, "CONIN$", "r", stdin);
  freopen_s(&dummy_file, "CONOUT$", "w", stdout);
  freopen_s(&dummy_file, "CONOUT$", "w", stderr);
}

void initializeDLLProxy() {
  dinput8 = LoadLibraryA("C:\\Windows\\System32\\dinput8.dll"); // todo: check if this actually loads

  // assign addresses of functions
  dinput8_functions[0] = GetProcAddress(dinput8, "DirectInput8Create");
  dinput8_functions[1] = GetProcAddress(dinput8, "DllCanUnloadNow");
  dinput8_functions[2] = GetProcAddress(dinput8, "DllGetClassObject");
  dinput8_functions[3] = GetProcAddress(dinput8, "DllRegisterServer");
  dinput8_functions[4] = GetProcAddress(dinput8, "DllUnregisterServer");
}

// =================================
// DllMain
// =================================

BOOL WINAPI DllMain(HMODULE __module, DWORD __reason, LPVOID __load_type) {
  switch (__reason) {
    case DLL_PROCESS_ATTACH:
      initializeConsole();
      initializeDLLProxy();
      break;
    case DLL_PROCESS_DETACH:
      FreeLibrary(dinput8); // free loaded DLL
      break;
    default:
      break;
  }

  return TRUE;
}