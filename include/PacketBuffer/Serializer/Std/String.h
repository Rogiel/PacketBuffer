/*
 * Copyright (c) 2017, Rogiel Sulzbach
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Rogiel Sulzbach nor the names of contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef PACKETBUFFER_SERIALIZER_STD_STRING_H
#define PACKETBUFFER_SERIALIZER_STD_STRING_H

#include "PacketBuffer/ObjectSerializer.h"

#include <string>

namespace PacketBuffer {

	/**
	 * A ObjectSerializer for std::basic_string with character of type
	 * <tt>T</tt>, traits of type <tt>Traits</tt> and using an allocator
	 * of type <tt>Allocator</tt>.
	 *
	 * @tparam T         the string character type
	 * @tparam Traits    the string traits
	 * @tparam Allocator the string character allocator
	 */
	template<typename T, typename Traits, typename Allocator>
	class ObjectSerializer<std::basic_string<T, Traits, Allocator>> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::basic_string<T, Traits, Allocator>& string) {
			auto length = static_cast<uint64_t>(string.size());
			packer(length);
			for(int i = 0; i < length; i++) {
				packer(string[i]);
			}
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::basic_string<T, Traits, Allocator>& string) {
			uint64_t length;
			unpacker(length);

			string.resize(length);
			for(int i = 0; i < length; i++) {
				unpacker(string[i]);
			}
		}
	};

	/**
	 * A ObjectSerializer for std::string.
	 */
	template<>
	class ObjectSerializer<std::string> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::string& string) {
			auto length = static_cast<uint64_t>(string.size());
			packer(length);
			packer.pack(string.data(), string.size());
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::string& string) {
			uint64_t length;
			unpacker(length);

			string.resize((size_t) length);
			unpacker.unpack(&string[0], string.size());
		}
	};

}

#endif //PACKETBUFFER_SERIALIZER_STD_STRING_H
