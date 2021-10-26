// averysumner - traveller
// object_manager.cc
// contains object manager definitions
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

#include "object_manager.hh"

#include "logger.hh"

namespace traveller {

void ObjectManager::registerObject(GameObject_s* __game_object) {
  t_objid id = nextID();
  TRAVELLER_LOG_DEBUG("Registered new object with ID: %i!", id);
  _objects.insert({id, Object(__game_object)});
}

void ObjectManager::unregisterObject(GameObject_s* __game_object) {
  for (auto& object : _objects) {
    if (object.second.getGameObject() == __game_object) {
      TRAVELLER_LOG_DEBUG("Unregistered object with ID: %i!", object.first);
      _objects.erase(object.first);
    }
  }
}

t_objid ObjectManager::nextID() {
  t_objid id = _next_id;
  ++_next_id;

  return id;
}

t_objid ObjectManager::_next_id = 0;

std::unordered_map<t_objid, Object> ObjectManager::_objects;

} // namespace traveller