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

#ifndef PACKETBUFFER_SERIALIZER_STD_PAIR_H
#define PACKETBUFFER_SERIALIZER_STD_PAIR_H

#include "PacketBuffer/ObjectSerializer.h"

#include <utility>

namespace PacketBuffer {

	/**
	 * A ObjectSerializer for std::pair of types <tt>T1</tt> and <tt>T2</tt>.
	 *
	 * @tparam T1 the pair first type
	 * @tparam T2 the pair second type
	 */
	template<typename T1, typename T2>
	class ObjectSerializer<std::pair<T1, T2>> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::pair<T1, T2>& pair) {
			packer(pair.first, pair.second);
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::pair<T1, T2>& pair) {
			unpacker(pair.first, pair.second);
		}
	};


}

#endif //PACKETBUFFER_SERIALIZER_STD_PAIR_H
