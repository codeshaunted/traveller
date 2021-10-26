// averysumner - traveller
// server.hh
// contains server class declarations
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

#ifndef TRAVELLER_SERVER_HH
#define TRAVELLER_SERVER_HH

#include <string>

#include "enet.h"

#include "peer.hh"

#define PEER_BROADCAST (ENetPeer*)0xdeadbeef // magic number to tell SendPacket to broadcast

namespace traveller {

class Server : public Peer {
  public:
    Server(std::string __host, uint16_t __port, uint32_t __max_clients) : _host(__host), _port(__port), _max_clients(__max_clients) {}
    void start();
    void update();
    void stop();
    //void sendPacket(ENetPeer* __peer, packets::Base* __data);
    //void handlePacket(ENetPeer* __peer, packets::Base* __data);
  private:
    std::string _host;
    uint16_t _port;
    uint32_t _max_clients;
    ENetHost* _server;
};

} // namespace traveller

#endif // TRAVELLER_SERVER_HH