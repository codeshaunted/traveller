// averysumner - traveller
// source/traveller/hook.cc
// contains hook class definitions
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

#include "hook.hh"

#include <Windows.h>

namespace traveller {

bool Hook::detour(uintptr_t __source, void* __destination, int __length) {
    if (__length < sizeof(s_JMPInstruction)) return false;

    DWORD old_protect;
    VirtualProtect((LPVOID)__source, __length, PAGE_EXECUTE_READWRITE, &old_protect);

    memset((void*)__source, 0x90, __length); // nop out the length

    s_JMPInstruction instruction;
    instruction.address = (uintptr_t)__destination - __source - sizeof(s_JMPInstruction);

    memcpy((void*)__source, &instruction, sizeof(s_JMPInstruction));

    DWORD dummy_protect;
    VirtualProtect((LPVOID)__source, __length, old_protect, &dummy_protect);

    return true;
}

void* Hook::trampoline(uintptr_t __source, void* __destination, int __length) {
    if (__length < sizeof(s_JMPInstruction)) return nullptr;

    void* bridge = VirtualAlloc(nullptr, __length + sizeof(s_JMPInstruction), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    memcpy(bridge, (void*)__source, __length);

    s_JMPInstruction bridge_instruction;
    bridge_instruction.address = __source - (uintptr_t)bridge - sizeof(s_JMPInstruction);

    memcpy((void*)((uintptr_t)bridge + __length), &bridge_instruction, sizeof(s_JMPInstruction));

    detour(__source, __destination, __length);

    return bridge;
}

void Hook::replaceCall(uintptr_t __source, void* __destination) {
    DWORD old_protect;
    VirtualProtect((LPVOID)(__source + 1), sizeof(__destination), PAGE_EXECUTE_READWRITE, &old_protect);

    uintptr_t address = (uintptr_t)__destination - __source - (sizeof(__destination) + 1);

    memcpy((void*)(__source + 1), &address, sizeof(__destination));

    DWORD dummy_protect;
    VirtualProtect((LPVOID)(__source + 1), sizeof(__destination), old_protect, &dummy_protect);
}

void Hook::insertNOP(uintptr_t __destination, int __length) {
    DWORD old_protect;
    VirtualProtect((LPVOID)(__destination), __length, PAGE_EXECUTE_READWRITE, &old_protect);

    memset((void*)__destination, 0x90, __length);

    DWORD dummy_protect;
    VirtualProtect((LPVOID)(__destination), __length, old_protect, &dummy_protect);
}

} // namespace traveller