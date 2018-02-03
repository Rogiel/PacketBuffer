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

#ifndef PACKETBUFFER_SERIALIZER_STD_ARRAY_H
#define PACKETBUFFER_SERIALIZER_STD_ARRAY_H

#include "PacketBuffer/ObjectSerializer.h"

#include <array>

namespace PacketBuffer {

	/**
	 * A ObjectSerializer for a statically sized array of type <tt>T</tt>
	 * with size of <tt>S</tt>.
	 *
	 * @note This serializer does not include the size of the original array
	 * when serialized. Further changing the size of the array will cause a
	 * change in the binary format of the packet.
	 *
	 * Serialized data takes the format as:
	 * @code
	 * (
	 * 	T: first,
	 * 	T: second,
	 * 	...
	 * 	T: last
	 * )
	 * @endcode
	 *
	 * @tparam T the array type
	 * @tparam S the array fixed size
	 */
	template<typename T, size_t S>
	class ObjectSerializer<T[S], typename std::enable_if<sizeof(T) != 1>::type> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, T const array[S]) {
			for(int i = 0; i < S; i++) {
				packer(array[i]);
			}
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, T array[S]) {
			for(int i = 0; i < S; i++) {
				unpacker(array[i]);
			}
		}
	};

	/**
	 * A ObjectSerializer for a statically sized array of type <tt>T</tt>
	 * with size of <tt>S</tt>.
	 *
	 * This implementation is specialized for object whose size is 1 byte.
	 *
	 * @note This serializer does not include the size of the original array
	 * when serialized. Further changing the size of the array will cause a
	 * change in the binary format of the packet.
	 *
	 * Serialized data takes the format as:
	 * @code
	 * (
	 * 	T: first,
	 * 	T: second,
	 * 	...
	 * 	T: last
	 * )
	 * @endcode
	 *
	 * @tparam T the array type
	 * @tparam S the array fixed size
	 */
	template<typename T, size_t S>
	class ObjectSerializer<T[S], typename std::enable_if<sizeof(T) == 1>::type> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, T const array[S]) {
			packer.pack(reinterpret_cast<const char*>(array), S);
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, T array[S]) {
			unpacker.unpack(reinterpret_cast<char*>(array), S);
		}
	};

	/**
	 * A ObjectSerializer for a statically sized array of type <tt>T</tt>
	 * with size of <tt>S</tt>.
	 *
	 * @note This serializer does not include the size of the original array
	 * when serialized. Further changing the size of the array will cause a
	 * change in the binary format of the packet.
	 *
	 * Serialized data takes the format as:
	 * @code
	 * (
	 * 	T: first,
	 * 	T: second,
	 * 	...
	 * 	T: last
	 * )
	 * @endcode
	 *
	 * @tparam T the array type
	 * @tparam S the array fixed size
	 */
	template<typename T, size_t S>
	class ObjectSerializer<std::array<T, S>> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::array<T, S>& array) {
			for(int i = 0; i < S; i++) {
				packer(array[i]);
			}
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::array<T, S>& array) {
			for(int i = 0; i < S; i++) {
				unpacker(array[i]);
			}
		}
	};


}

#endif //PACKETBUFFER_SERIALIZER_STD_ARRAY_H
