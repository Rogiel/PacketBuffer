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

#ifndef PACKETBUFFER_SERIALIZER_ENUM_H
#define PACKETBUFFER_SERIALIZER_ENUM_H

#include <type_traits>

namespace PacketBuffer {

	/**
	 * A ObjectSerializer for C++ enum and enum class values.
	 *
	 * @tparam Enum 	the enum type
	 */
	template<typename Enum>
	class ObjectSerializer<Enum, typename std::enable_if<std::is_enum<Enum>::value>::type> {
	public:
		/**
		 * The underlying storage type for the Enum
		 */
		using UnderlyingType = typename std::underlying_type<Enum>::type;

		template<typename Packer>
		static void pack(Packer& packer, Enum object) {
			packer((UnderlyingType) object);
		}

		template<typename Unpacker>
		static void unpack(Unpacker& unpacker, Enum& object) {
			unpacker((UnderlyingType&) object);
		}
	};

}

#endif //PACKETBUFFER_SERIALIZER_ENUM_H
