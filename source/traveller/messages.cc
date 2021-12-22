// averysumner - traveller
// source/traveller/messages.cc
// contains messages class definitions
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

#include "messages.hh"

#include "raw_api.hh"
#include "logger.hh"

namespace traveller {

void Messages::handle(RakNet::BitStream& __bitstream) {
    MessageID message_id;
    __bitstream.Read(message_id);

    TRAVELLER_LOG("GOT MESSAGE WITH ID: %u", message_id);

    switch (message_id) {
        case MessageID::SET_LEVEL: {
            MessageSetLevel message;
            message.deserialize(__bitstream);
            Messages::handleSetLevel(message);
            break;
        }
        default: {
            TRAVELLER_LOG_ERROR("Unhandled message ID: %u", message_id);
            break;
        }
    }
}

void Messages::handleSetLevel(const MessageSetLevel& __message) {
    TRAVELLER_LOG("GOT SETLEVEL");
    RawAPI::GoToNewLevel(__message.level_id);
}

} // namespace traveller