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

#ifndef PACKETBUFFER_SERIALIZER_STD_TUPLE_H
#define PACKETBUFFER_SERIALIZER_STD_TUPLE_H

#include "PacketBuffer/ObjectSerializer.h"

#include <tuple>

namespace PacketBuffer {

	/**
	 * A ObjectSerializer for std::tuple with elements of type <tt>Ts</tt>.
	 *
	 * @tparam Ts the tuple element types
	 */
	template<typename... Ts>
	class ObjectSerializer<std::tuple<Ts...>> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::tuple<Ts...>& tuple) {
			packImpl<Packer, 0, Ts...>(packer, tuple);
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::tuple<Ts...>& tuple) {
			unpackImpl<Unpacker, 0, Ts...>(unpacker, tuple);
		}

	private:
		template<typename Packer, size_t I, typename T, typename... OTs>
		static inline void packImpl(Packer& packer, const std::tuple<Ts...>& tuple) {
			packer(std::get<I>(tuple));
			packImpl<Packer, I + 1, OTs...>(packer, tuple);
		}

		template<typename Packer, size_t I>
		static inline void packImpl(Packer& packer, const std::tuple<Ts...>& tuple) {}

		template<typename Unpacker, size_t I, typename T, typename... OTs>
		static inline void unpackImpl(Unpacker& unpacker, std::tuple<Ts...>& tuple) {
			unpacker(std::get<I>(tuple));
			unpackImpl<Unpacker, I + 1, OTs...>(unpacker, tuple);
		}

		template<typename Unpacker, size_t I>
		static inline void unpackImpl(Unpacker& unpacker, std::tuple<Ts...>& tuple) {}

	};

}

#endif //PACKETBUFFER_SERIALIZER_STD_TUPLE_H
