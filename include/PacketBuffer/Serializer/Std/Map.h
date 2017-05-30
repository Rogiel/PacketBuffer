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

#ifndef PACKETBUFFER_SERIALIZER_STD_MAP_H
#define PACKETBUFFER_SERIALIZER_STD_MAP_H

#include "PacketBuffer/ObjectSerializer.h"
#include "PacketBuffer/Serializer/Std/Pair.h"

#include <map>
#include <unordered_map>

namespace PacketBuffer {

	/**
	 * A ObjectSerializer for std::map with keys of type <tt>K</tt>,
	 * values of type <tt>T</tt>, comparison functor of type
	 * <tt>Compare</tt> and using an allocator of type <tt>Allocator</tt>.
	 *
	 * @tparam K         the map element key
	 * @tparam V         the map element value
	 * @tparam Compare   the map comparison functor
	 * @tparam Allocator the map allocator type
	 */
	template<typename K, typename V, typename Compare, typename Allocator>
	class ObjectSerializer<std::map<K, V, Compare, Allocator>> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::map<K, V, Compare, Allocator>& map) {
			auto items = static_cast<uint64_t>(map.size());
			packer(items);
			for(const std::pair<K, V>& entry : map) {
				packer(entry);
			}
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::map<K, V, Compare, Allocator>& map) {
			uint64_t items;
			unpacker(items);

			for(int i = 0; i < items; i++) {
				std::pair<K, V> entry;
				unpacker(entry);
				map.insert(std::move(entry));
			}
		}
	};

	/**
	 * A ObjectSerializer for std::unordered_map with keys of type <tt>K</tt>,
	 * values of type <tt>T</tt>, comparison functor of type <tt>Compare</tt>
	 * and using an allocator of type <tt>Allocator</tt>.
	 *
	 * @tparam K         the map element key
	 * @tparam V         the map element value
	 * @tparam Compare   the map comparison functor
	 * @tparam Allocator the map allocator type
	 */
	template<typename K, typename V, class Hash, class Predicate, typename Allocator>
	class ObjectSerializer<std::unordered_map<K, V, Hash, Predicate, Allocator>> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::unordered_map<K, V, Hash, Predicate, Allocator>& map) {
			auto items = static_cast<uint64_t>(map.size());
			packer(items);
			for(const std::pair<K, V>& entry : map) {
				packer(entry);
			}
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::unordered_map<K, V, Hash, Predicate, Allocator>& map) {
			uint64_t items;
			unpacker(items);

			for(int i = 0; i < items; i++) {
				std::pair<K, V> entry;
				unpacker(entry);
				map.insert(std::move(entry));
			}
		}
	};

}

#endif //PACKETBUFFER_SERIALIZER_STD_MAP_H
