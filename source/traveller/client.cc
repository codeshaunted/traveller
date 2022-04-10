// averysumner - traveller
// source/traveller/client.cc
// contains client class definitions
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

#include "client.hh"

#include "MessageIdentifiers.h"

#include "logger.hh"
#include "raw_api.hh"
#include "object_manager.hh"

namespace traveller {

void Client::start() {
    _interface = RakNet::RakPeerInterface::GetInstance();

    RakNet::SocketDescriptor socket_descriptor;
    _interface->Startup(1, &socket_descriptor, 1);

    _interface->Connect(_host.c_str(), _port, 0, 0);

    TRAVELLER_LOG("Started client and attempted connection to %s|%u.", _host.c_str(), _port);
}

void Client::update() {
    for (RakNet::Packet* packet = _interface->Receive(); packet; _interface->DeallocatePacket(packet), packet = _interface->Receive()) {
        uint8_t message_id = packet->data[0];
        TRAVELLER_LOG("RECEIVED PACKET");

        switch (message_id) {
            case ID_CONNECTION_REQUEST_ACCEPTED:
                TRAVELLER_LOG("Sucessfully connected to %s.", packet->systemAddress.ToString());
                break;
            case ID_CONNECTION_LOST:
                TRAVELLER_LOG("Lost connection to server.", packet->systemAddress.ToString());
                break;
            default:
                RakNet::BitStream bitstream(packet->data, packet->length, false);
                _handleMessage(bitstream);
                break;
        }
    }

    _callCallbacks();
}

void Client::stop() {
    RakNet::RakPeerInterface::DestroyInstance(_interface);

    TRAVELLER_LOG("Stopping client.");
}

void Client::_handleMessage(RakNet::BitStream& __bitstream) {
    MessageID message_id;
    __bitstream.Read(message_id);

    TRAVELLER_LOG_DEBUG("GOT MESSAGE WITH ID: %u", message_id);

    switch (message_id) {
        case MessageID::SET_LEVEL: {
            MessageSetLevel message;
            message.deserialize(__bitstream);
            
            TRAVELLER_LOG("Changing level to level ID %u.", message.level_id);
            RawAPI::GoToNewLevel(message.level_id);

            _addUpdateCallback([this](uint32_t __callback_id) {
                if (*RawAPI::NewLData == 0) {
                    MessageLevelSet level_set_message(*RawAPI::next_level);
                    
                    send(level_set_message);
                    ObjectManager::clearObjects();
                    _removeUpdateCallback(__callback_id);
                }
            });

            break;
        }
        case MessageID::CONSTRUCT_OBJECT: {
            MessageConstructObject message;
            message.deserialize(__bitstream);

            TRAVELLER_LOG_DEBUG("Constructing new object with character ID of %u.", message.character_id);
            //GameObject_s* game_object = RawAPI::AddDynamicCreature(message.character_id, &message.position, 0, 0, nullptr, nullptr, 0, nullptr, nullptr, 0, 0);
            GameObject_s* game_object = RawAPI::AddCreature(message.character_id, false);

            //game_object->position = message.position;

            break;
        }
        default: {
            TRAVELLER_LOG_ERROR("Unhandled message ID: %u", message_id);
            break;
        }
    }
}

} // namespace traveller