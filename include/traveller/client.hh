// averysumner - traveller
// include/traveller/client.hh
// contains client class declarations
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

#ifndef TRAVELLER_CLIENT_HH
#define TRAVELLER_CLIENT_HH

#include <string>

#include "peer.hh"

namespace traveller {

class Client : public Peer {
    public:
        Client(std::string __host, uint16_t __port) : Peer(false), _host(__host), _port(__port) {}
        void start();
        void update();
        void stop();
    private:
        std::string _host;
        uint16_t _port;
};

} // namespace traveller

#endif // TRAVELLER_CLIENT_HH