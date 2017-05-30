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

#ifndef PACKETBUFFER_SERIALIZER_STD_EXPERIMENTAL_OPTIONAL_H
#define PACKETBUFFER_SERIALIZER_STD_EXPERIMENTAL_OPTIONAL_H

#include "PacketBuffer/ObjectSerializer.h"

#if __has_include(<experimental/optional>)

#include <experimental/optional>

namespace PacketBuffer {

	/**
	 * A ObjectSerializer for std::optional of type <tt>T</tt>.
	 *
	 * @tparam T the optional type
	 */
	template<typename T>
	class ObjectSerializer<std::experimental::optional<T>> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::experimental::optional<T>& optional) {
			if(optional) {
				packer(true);
				packer(*optional);
			} else {
				packer(false);
			}
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::experimental::optional<T>& optional) {
			bool hasValue;
			unpacker(hasValue);
			if(hasValue) {
				unpacker(*optional);
			}
		}
	};


}
#endif

#endif //PACKETBUFFER_SERIALIZER_STD_EXPERIMENTAL_OPTIONAL_H
