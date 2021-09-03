// averysumner - traveller
// api.hh
// contains high level modding API
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

#ifndef TRAVELLER_API_HH
#define TRAVELLER_API_HH

namespace traveller {

#define TRAVELLER_EVENT_PRE_INITIALIZE extern "C" __declspec(dllexport) void eventPreInitialize()
#define TRAVELLER_EVENT_POST_INITIALIZE extern "C" __declspec(dllexport) void eventPostInitialize()
#define TRAVELLER_EVENT_UPDATE extern "C" __declspec(dllexport) void eventUpdate()

} // namespace traveller

#endif // TRAVELLER_API_HH