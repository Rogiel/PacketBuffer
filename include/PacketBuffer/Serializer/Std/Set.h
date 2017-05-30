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

#ifndef PACKETBUFFER_SERIALIZER_STD_SET_H
#define PACKETBUFFER_SERIALIZER_STD_SET_H

#include "PacketBuffer/ObjectSerializer.h"

#include <set>
#include <unordered_set>

namespace PacketBuffer {

	/**
	 * A ObjectSerializer for std::set with elements of type <tt>T</tt>,
	 * comparison functor of type <tt>Compare</tt> and using an allocator
	 * of type <tt>Allocator</tt>.
	 *
	 * @tparam T         the set element key
	 * @tparam Compare   the set comparison functor
	 * @tparam Allocator the set allocator type
	 */
	template<typename T, typename Compare, typename Allocator>
	class ObjectSerializer<std::set<T, Compare, Allocator>> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::set<T, Compare, Allocator>& set) {
			auto items = static_cast<uint64_t>(set.size());
			packer(items);
			for(auto& entry : set) {
				packer(entry);
			}
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::set<T, Compare, Allocator>& set) {
			uint64_t items;
			unpacker(items);

			for(int i = 0; i < items; i++) {
				T v;
				unpacker(v);
				set.insert(v);
			}
		}
	};

	/**
	 * A ObjectSerializer for std::unordered_set with elements of type
	 * <tt>T</tt>, hash functor of type <tt>Hash</tt>, predicate of type
	 * <tt>Predicate</tt> and using an allocator of type <tt>Allocator</tt>.
	 *
	 * @tparam T         the set element key
	 * @tparam Hash      the set hash functor
	 * @tparam Predicate the set predicate functor
	 * @tparam Allocator the set allocator type
	 */
	template<typename T, typename Hash, typename Predicate, typename Allocator>
	class ObjectSerializer<std::unordered_set<T, Hash, Predicate, Allocator>> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::unordered_set<T, Hash, Predicate, Allocator>& set) {
			auto items = static_cast<uint64_t>(set.size());
			packer(items);
			for(auto& entry : set) {
				packer(entry);
			}
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::unordered_set<T, Hash, Predicate, Allocator>& set) {
			uint64_t items;
			unpacker(items);

			set.reserve(items);
			for(int i = 0; i < items; i++) {
				T v;
				unpacker(v);
				set.insert(v);
			}
		}
	};

}

#endif //PACKETBUFFER_SERIALIZER_STD_SET_H
