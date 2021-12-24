// averysumner - traveller
// include/traveller/peer.hh
// contains base peer class declarations
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

#ifndef TRAVELLER_PEER_HH
#define TRAVELLER_PEER_HH

#include "RakPeerInterface.h"

#include "messages.hh"
#include "logger.hh"

namespace traveller {

class Peer {
    public:
        Peer(bool __is_server) : _is_server(__is_server) {}
        virtual void start();
        virtual void update();
        virtual void stop();
        bool getIsServer();
        void send(const RakNet::BitStream& __bitstream, const RakNet::SystemAddress& __system_address, bool __broadcast = false);
        void send(const RakNet::BitStream& __bitstream);
        void send(const Message& message, const RakNet::SystemAddress& __system_address, bool __broadcast = false);
        void send(const Message& message);
    protected:
        RakNet::RakPeerInterface* _interface;
        bool _is_server;
};

} // namespace traveller

#endif // TRAVELLER_PEER_HH