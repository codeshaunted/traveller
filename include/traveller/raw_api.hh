// averysumner - traveller
// include/traveller/raw_api.hh
// contains raw api for nu2
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

#ifndef TRAVELLER_RAW_API_HH
#define TRAVELLER_RAW_API_HH

#include <d3d9types.h>

#include "raw_types.hh"

namespace traveller {

#define TRAVELLER_EXPAND(X) X
#define TRAVELLER_REGISTER_RAW_API_FUNCTION(Address, Return, Name, ...) typedef Return (*TRAVELLER_EXPAND(Name)Signature)(__VA_ARGS__);\
    static inline TRAVELLER_EXPAND(Name)Signature Name = (TRAVELLER_EXPAND(Name)Signature)Address
#define TRAVELLER_REGISTER_RAW_API_GLOBAL(Address, Type, Name) static inline Type* Name = (Type*)Address

class RawAPI {
    public:
        TRAVELLER_REGISTER_RAW_API_FUNCTION(0x0055cad0, void, RemoveGameObject, GameObject_s* __game_object);
        TRAVELLER_REGISTER_RAW_API_FUNCTION(0x0055c950, void, ClearGameObjects, APIOBJECTSYS_s* __api_object_system);
        TRAVELLER_REGISTER_RAW_API_FUNCTION(0x0041c7c0, uint32_t, InitCreature, GameObject_s* __game_object, uint32_t __creature_id, BOOL __can_respawn);
        TRAVELLER_REGISTER_RAW_API_FUNCTION(0x0041f8f0, GameObject_s*, AddCreature, uint32_t __creature_id, BOOL __can_respawn);
        TRAVELLER_REGISTER_RAW_API_FUNCTION(0x0043d890, GameObject_s*, AddDynamicCreature, uint32_t __creature_id, nuvec_s* __position, uint32_t __param_2, uint32_t __param_3, AIPATHINFO_s* __ai_path_info, AIGROUP_s* __ai_group, uint32_t __param_6, nugspline_s* __param_7, nuvec_s* __param_8, uint32_t __param_9, uint32_t __param_10);
        TRAVELLER_REGISTER_RAW_API_FUNCTION(0x0057a580, void, GoToNewLevel, uint32_t __level_id);
        TRAVELLER_REGISTER_RAW_API_FUNCTION(0x0062a8c0, void, FUN_0062a8c0, int __parameter_0, int __parameter_1, int __parameter_2); // todo: figure out what this does and give it a name

        TRAVELLER_REGISTER_RAW_API_GLOBAL(0x0091ef34, int32_t, argc);
        TRAVELLER_REGISTER_RAW_API_GLOBAL(0x0091ef38, char**, argv);
        TRAVELLER_REGISTER_RAW_API_GLOBAL(0x0093d878, WORLDINFO_s, WorldInfo);
        TRAVELLER_REGISTER_RAW_API_GLOBAL(0x0093d520, uint32_t, HIGHGAMEOBJECT);
        TRAVELLER_REGISTER_RAW_API_GLOBAL(0x00951bc0, uint32_t, Level);
        TRAVELLER_REGISTER_RAW_API_GLOBAL(0x00827610, BOOL, gBWindowHasFocus);
        TRAVELLER_REGISTER_RAW_API_GLOBAL(0x02976720, D3DPRESENT_PARAMETERS, presentParams); // this is actually part of the CD3DCore class
        TRAVELLER_REGISTER_RAW_API_GLOBAL(0x02976c44, bool, isWindowed); // this is actually part of the CD3DCore class
        TRAVELLER_REGISTER_RAW_API_GLOBAL(0x0093d830, GameObject_s*, player1);
        GameObject_s(**Obj)[64] = (GameObject_s(**)[64])0x0093d524;
};


#undef TRAVELLER_EXPAND
#undef TRAVELLER_REGISTER_RAW_API_FUNCTION
#undef TRAVELLER_REGISTER_RAW_API_GLOBAL

} // namespace traveller

#endif // TRAVELLER_RAW_API_HH