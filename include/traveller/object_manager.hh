// averysumner - traveller
// object_manager.hh
// contains object manager declarations
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

#ifndef TRAVELLER_OBJECT_MANAGER_HH
#define TRAVELLER_OBJECT_MANAGER_HH

#include <unordered_map>

#include "object.hh"

namespace traveller {

class ObjectManager {
  public:
    static std::unordered_map<t_objid, Object>& getObjects();
    static void registerObject(GameObject_s* __game_object);
    static void unregisterObject(GameObject_s* __game_object);
  private:
    static t_objid nextID();
    static t_objid _next_id;
    static std::unordered_map<t_objid, Object> _objects;
};

} // namespace traveller

#endif // TRAVELLER_OBJECT_MANAGER_HH