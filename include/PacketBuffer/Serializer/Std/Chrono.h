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

#ifndef PACKETBUFFER_SERIALIZER_STD_CHRONO_H
#define PACKETBUFFER_SERIALIZER_STD_CHRONO_H

#include "PacketBuffer/ObjectSerializer.h"

#include <chrono>

namespace PacketBuffer {

	/**
	 * A ObjectSerializer for std::chrono::duration values with representation
	 * of type <tt>R</tt> and period type of <tt>P</tt>.
	 *
	 * @tparam R the duration representation type
	 * @tparam P the duration period type
	 */
	template<typename R, typename P>
	class ObjectSerializer<std::chrono::duration<R, P>> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::chrono::duration<R, P>& duration) {
			packer((int64_t) duration.count());
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::chrono::duration<R, P>& duration) {
			int64_t r;
			unpacker(r);
			duration = std::chrono::duration<R, P>(r);
		}
	};

	/**
	 * A ObjectSerializer for std::chrono::time_point values with clock
	 * of type <tt>Clock</tt> and duration type of <tt>Duration</tt>.
	 *
	 * @tparam Clock    the time_point clock type
	 * @tparam Duration the time_point duration type
	 */
	template<typename Clock, typename Duration>
	class ObjectSerializer<std::chrono::time_point<Clock, Duration>> {
	public:
		template<typename Packer>
		static inline void pack(Packer& packer, const std::chrono::time_point<Clock, Duration>& point) {
			packer(point.time_since_epoch());
		}

		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, std::chrono::time_point<Clock, Duration>& point) {
			Duration d;
			unpacker(d);
			point = std::chrono::time_point<Clock, Duration>(d);
		}
	};

}

#endif //PACKETBUFFER_SERIALIZER_STD_CHRONO_H
