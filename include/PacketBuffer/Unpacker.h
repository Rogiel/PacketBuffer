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

#ifndef PACKETBUFFER_UNPACKER_H
#define PACKETBUFFER_UNPACKER_H

#include <boost/endian/conversion.hpp>

#include "ObjectSerializer.h"

namespace PacketBuffer {

	template<typename Buffer, boost::endian::order Endianess = boost::endian::order::little>
	class Unpacker {
	private:
		Buffer& buffer;

	public:
		Unpacker(Buffer& buffer) : buffer(buffer) {};

		/**
		 * Deleted copy constructor.
		 */
		Unpacker(const Unpacker& other) = delete;

		/**
		 * Deleted copy assignment operator.
		 */
		Unpacker& operator=(const Unpacker& other) = delete;

		/**
		 * Deleted move constructor.
		 */
		Unpacker(Unpacker&& other) = delete;

		/**
		 * Deleted move assignment operator.
		 */
		Unpacker& operator=(Unpacker&& other) = delete;

		/**
		 * Default destructor.
		 */
		~Unpacker() = default;

	public: // Helper methods
		/**
		 * A helper <tt>&</tt> operator overload. Calls the unpack() method for the given type.
		 *
		 * @tparam T    the data type to unpack
		 * @param v     the value to unpack to
		 *
		 * @return this
		 */
		template<typename T>
		Unpacker& operator&(T& v) {
			return unpack(v);
		}

		/**
		 * A helper <tt>>></tt> operator overload that behaves similarly to ostreams operator. Calls the
		 * unpack() method for given type.
		 *
		 * @tparam T    the data type to unpack
		 * @param v     the value to unpack to
		 *
		 * @return this
		 */
		template<typename T>
		Unpacker& operator>>(T& v) {
			return unpack(v);
		}

		/**
		 * A helper call operator overload. Calls unpack() method for the given types.
		 *
		 * @tparam Ts   the types to be unpacked
		 * @param vs    the values to be unpacked. The values will be unpacked in the given order.
		 *
		 * @return this
		 */
		template<typename... Ts>
		Unpacker& operator()(Ts& ... vs) {
			return unpack(vs...);
		}

		/**
		 * Creates a default constructible object of type T and unpacks the data into it.
		 *
		 * @tparam T the type to be unpacked
		 *
		 * @return the unpacked object
		 */
		template<typename T>
		T unpack() {
			T v;
			unpack(v);
			return v;
		}

	public: // Object serialization
		/**
		 * Unpacks a sequence of objects of types <tt>T</tt> and <tt>Ts...</tt>.
		 *
		 * The objects will be unpacked in the given order.
		 *
		 * @tparam T    the type of the first object to be unpacked
		 * @tparam Ts   the type of the remaining objects to be unpacked
		 * @param v     the value of the first object to be unpacked
		 * @param vs    the value of the remaining objects to be unpacked
		 *
		 * @return this
		 */
		template<typename T, typename... Ts>
		Unpacker& unpack(T& v, Ts& ... vs) {
			unpack(v);
			unpack(vs...);
			return *this;
		}

		/**
		 * Unpacks a single object of type <tt>T</tt>.
		 *
		 * @tparam T        the type of the object to be unpacked
		 * @param object    the value of the object to be unpacked
		 *
		 * @return this
		 */
		template<typename T>
		Unpacker& unpack(T& object) {
			ObjectSerializer<T>::unpack(*this, object);
			return *this;
		}

	public: // Integer types
		/**
		 * Unpacks a uint8_t integer value.
		 *
		 * @param i the integer value to unpack
		 *
		 * @return this
		 */
		Unpacker& unpack(uint8_t& i) {
			static_assert(sizeof(i) == 1, "uint8_t size must be 1 byte");
			unpack(reinterpret_cast<char*>(&i), sizeof(i));
			boost::endian::conditional_reverse_inplace<Endianess, boost::endian::order::native>(i);
			return *this;
		}

		/**
		 * Unpacks a int8_t integer value.
		 *
		 * @param i the integer value to unpack
		 *
		 * @return this
		 */
		Unpacker& unpack(int8_t& i) {
			static_assert(sizeof(i) == 1, "int8_t size must be 1 byte");
			unpack(reinterpret_cast<char*>(&i), sizeof(i));
			boost::endian::conditional_reverse_inplace<Endianess, boost::endian::order::native>(i);
			return *this;
		}

		/**
		 * Unpacks a uint16_t integer value.
		 *
		 * @param i the integer value to unpack
		 *
		 * @return this
		 */
		Unpacker& unpack(uint16_t& i) {
			static_assert(sizeof(i) == 2, "uint16_t size must be 2 byte2");
			unpack(reinterpret_cast<char*>(&i), sizeof(i));
			boost::endian::conditional_reverse_inplace<Endianess, boost::endian::order::native>(i);
			return *this;
		}

		/**
		 * Unpacks a int16_t integer value.
		 *
		 * @param i the integer value to unpack
		 *
		 * @return this
		 */
		Unpacker& unpack(int16_t& i) {
			static_assert(sizeof(i) == 2, "int16_t size must be 2 byte2");
			unpack(reinterpret_cast<char*>(&i), sizeof(i));
			boost::endian::conditional_reverse_inplace<Endianess, boost::endian::order::native>(i);
			return *this;
		}

		/**
		 * Unpacks a uint32_t integer value.
		 *
		 * @param i the integer value to unpack
		 *
		 * @return this
		 */
		Unpacker& unpack(uint32_t& i) {
			static_assert(sizeof(i) == 4, "uint32_t size must be 4 bytes");
			unpack(reinterpret_cast<char*>(&i), sizeof(i));
			boost::endian::conditional_reverse_inplace<Endianess, boost::endian::order::native>(i);
			return *this;
		}

		/**
		 * Unpacks a int32_t integer value.
		 *
		 * @param i the integer value to unpack
		 *
		 * @return this
		 */
		Unpacker& unpack(int32_t& i) {
			static_assert(sizeof(i) == 4, "int32_t size must be 4 bytes");
			unpack(reinterpret_cast<char*>(&i), sizeof(i));
			boost::endian::conditional_reverse_inplace<Endianess, boost::endian::order::native>(i);
			return *this;
		}

		/**
		 * Unpacks a uint64_t integer value.
		 *
		 * @param i the integer value to unpack
		 *
		 * @return this
		 */
		Unpacker& unpack(uint64_t& i) {
			static_assert(sizeof(i) == 8, "uint64_t size must be 8 bytes");
			unpack(reinterpret_cast<char*>(&i), sizeof(i));
			boost::endian::conditional_reverse_inplace<Endianess, boost::endian::order::native>(i);
			return *this;
		}

		/**
		 * Unpacks a int64_t integer value.
		 *
		 * @param i the integer value to unpack
		 *
		 * @return this
		 */
		Unpacker& unpack(int64_t& i) {
			static_assert(sizeof(i) == 8, "int64_t size must be 8 bytes");
			unpack(reinterpret_cast<char*>(&i), sizeof(i));
			boost::endian::conditional_reverse_inplace<Endianess, boost::endian::order::native>(i);
			return *this;
		}

		/**
		 * Unpacks a boolean value.
		 *
		 * @param b the boolean value to unpack
		 *
		 * @return this
		 */
		Unpacker& unpack(bool& b) {
			static_assert(sizeof(b) == 1, "bool size must be 1 byte");
			unpack(reinterpret_cast<char*>(&b), sizeof(b));
			boost::endian::conditional_reverse_inplace<Endianess, boost::endian::order::native>(b);
			return *this;
		}

	public: // write operation
		/**
		 * Unpacks a char-pointer given by <tt>ptr</tt> with length given by <tt>size</tt>.
		 *
		 * @param ptr   the char pointer to be unpacked
		 * @param size  the char pointer length
		 *
		 * @return this
		 */
		Unpacker& unpack(char* ptr, size_t size) {
			buffer.read(ptr, size);
			return *this;
		}

		/**
		 * Unpacks a char-pointer given by <tt>ptr</tt> with length given by <tt>size</tt>.
		 *
		 * @param ptr   the char pointer to be unpacked
		 * @param size  the char pointer length
		 *
		 * @return this
		 */
		Unpacker& unpack(unsigned char* ptr, size_t size) {
			buffer.read(ptr, size);
			return *this;
		}

	};

}


#endif //PACKETBUFFER_UNPACKER_H
