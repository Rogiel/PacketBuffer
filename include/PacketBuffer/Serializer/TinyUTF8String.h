//
// Created by Rogiel on 8/28/2018.
//

#ifndef PACKETBUFFER_SERIALIZER_TINYUTF8STRING_H
#define PACKETBUFFER_SERIALIZER_TINYUTF8STRING_H

#include "PacketBuffer/ObjectSerializer.h"
#include <tinyutf8.h>

namespace PacketBuffer {

    /**
     * A ObjectSerializer for utf8_string from tinyutf8.
     */
    template<>
    class ObjectSerializer<utf8_string> {
    public:
        template<typename Packer>
        static inline void pack(Packer& packer, const utf8_string& string) {
            packer(string.cpp_str());
        }

        template<typename Unpacker>
        static inline void unpack(Unpacker& unpacker, utf8_string& string) {
            std::string rawString;
            unpacker(rawString);
            string = rawString;
        }
    };

}

#endif //PACKETBUFFER_SERIALIZER_TINYUTF8STRING_H
