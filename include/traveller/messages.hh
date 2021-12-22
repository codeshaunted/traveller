// averysumner - traveller
// include/traveller/messages.hh
// contains messages declarations
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

#ifndef TRAVELLER_MESSAGES_HH
#define TRAVELLER_MESSAGES_HH

#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"

namespace traveller {

enum class MessageID : uint8_t {
    SET_LEVEL = ID_USER_PACKET_ENUM + 1,
    CONSTRUCT_OBJECT,
    DESTRUCT_OBJECT,
    UPDATE_OBJECT
};

class Message {
    public:
        Message(MessageID __message_id) : _message_id(__message_id) {}
        virtual void serialize(RakNet::BitStream& __bitstream) const {}
        virtual void deserialize(RakNet::BitStream& __bitstream) {}
    protected:
        MessageID _message_id;
};

class MessageSetLevel : public Message {
    public:
        MessageSetLevel(uint32_t __level_id = 0) : Message(MessageID::SET_LEVEL), level_id(__level_id) {}
        void serialize(RakNet::BitStream& __bitstream) const override {
            __bitstream.Write((RakNet::MessageID)_message_id);
            if (level_id != 0) {
                __bitstream.Write1();
                __bitstream.Write(level_id);
            }
            else {
                __bitstream.Write0();
            }
        }
        void deserialize(RakNet::BitStream& __bitstream) override {
            if (__bitstream.ReadBit()) {
                __bitstream.Read(level_id);
            }
        }
        uint32_t level_id;
};

class Messages {
    public:
        static void handle(RakNet::BitStream& __bitstream);
        static void handleSetLevel(const MessageSetLevel& __message);
};

} // namespace traveller

#endif // TRAVELLER_MESSAGES_HH