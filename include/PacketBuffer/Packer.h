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

#ifndef PACKETBUFFER_PACKER_H
#define PACKETBUFFER_PACKER_H

#include <boost/endian/conversion.hpp>

#include "ObjectSerializer.h"

namespace PacketBuffer {

	/**
	 * The Packer template class is responsible for converting C++ primitive types like integers and raw buffers
	 * into a platform independent raw buffer.
	 *
	 * By default, all integers are encoded as little endian, this, however can be changed by setting "Endianess"
	 * template parameter to something else.
	 *
	 * The <tt>Buffer</tt> class must implement a <tt>write</tt> with the following signature:
	 * @code
	 *  void write(const char* data, size_t length);
	 * @endcode
	 *
	 * @tparam Buffer       the buffer type to write data to
	 * @tparam Endianess    the endianess used to encode integer types
	 */
	template<typename Buffer, boost::endian::order Endianess = boost::endian::order::little>
	class Packer {
	private:
		/**
		 * A reference to the buffer in which packed data is written to
		 */
		Buffer& buffer;

	public:
		/**
		 * Creates a new Packer instance with the given buffer reference. Packed data will be written
		 * into the given buffer object.
		 *
		 * @param buffer the buffer object to write data to
		 */
		explicit Packer(Buffer& buffer) : buffer(buffer) {};

		/**
		 * Deleted copy constructor.
		 */
		Packer(const Packer& other) = delete;

		/**
		 * Deleted copy assignment operator.
		 */
		Packer& operator=(const Packer& other) = delete;

		/**
		 * Deleted move constructor.
		 */
		Packer(Packer&& other) = delete;

		/**
		 * Deleted move assignment operator.
		 */
		Packer& operator=(Packer&& other) = delete;

		/**
		 * Default destructor.
		 */
		~Packer() = default;

	public: // Helper methods
		/**
		 * A helper <tt>&</tt> operator overload. Calls the pack() method for the given type.
		 *
		 * @tparam T    the data type to pack
		 * @param v     the value to be packed
		 *
		 * @return this
		 */
		template<typename T>
		inline Packer& operator&(const T& v) {
			return pack(v);
		}

		/**
		 * A helper <tt><<</tt> operator overload that behaves similarly to ostreams operator. Calls the
		 * pack() method for given type.
		 *
		 * @tparam T    the data type to pack
		 * @param v     the value to be packed
		 *
		 * @return this
		 */
		template<typename T>
		inline Packer& operator<<(const T& v) {
			return pack(v);
		}

		/**
		 * A helper call operator overload. Calls pack() method for the given types.
		 *
		 * @tparam Ts   the types to be packed
		 * @param vs    the values to be packed. The values will be packed in the given order.
		 *
		 * @return this
		 */
		template<typename... Ts>
		inline Packer& operator()(const Ts& ... vs) {
			return pack(vs...);
		}

	public: // Object serialization
		/**
		 * Packs a sequence of objects of types <tt>T</tt> and <tt>Ts...</tt>.
		 *
		 * The objects will be packed in the given order.
		 *
		 * @tparam T    the type of the first object to be packed
		 * @tparam Ts   the type of the remaining objects to be packed
		 * @param v     the value of the first object to be packed
		 * @param vs    the value of the remaining objects to be packed
		 *
		 * @return this
		 */
		template<typename T, typename... Ts>
		inline Packer& pack(const T& v, const Ts& ... vs) {
			pack(v);
			pack(vs...);
			return *this;
		}

		/**
		 * Packs a single object of type <tt>T</tt>.
		 *
		 * @tparam T        the type of the object to be packed
		 * @param object    the value of the object to be packed
		 *
		 * @return this
		 */
		template<typename T>
		inline Packer& pack(const T& object) {
			ObjectSerializer<T>::pack(*this, object);
			return *this;
		}

	public: // Integer types
		/**
		 * Packs a uint8_t integer value.
		 *
		 * @param i the integer value to pack
		 *
		 * @return this
		 */
		inline Packer& pack(uint8_t i) {
			static_assert(sizeof(i) == 1, "uint8_t size must be 1 byte");
			boost::endian::conditional_reverse_inplace<boost::endian::order::native, Endianess>(i);
			return pack(reinterpret_cast<const char*>(&i), sizeof(i));
		}

		/**
		 * Packs a int8_t integer value.
		 *
		 * @param i the integer value to pack
		 *
		 * @return this
		 */
		inline Packer& pack(int8_t i) {
			static_assert(sizeof(i) == 1, "int8_t size must be 1 byte");
			boost::endian::conditional_reverse_inplace<boost::endian::order::native, Endianess>(i);
			return pack(reinterpret_cast<const char*>(&i), sizeof(i));
		}

		/**
		 * Packs a uint16_t integer value.
		 *
		 * @param i the integer value to pack
		 *
		 * @return this
		 */
		inline Packer& pack(uint16_t i) {
			static_assert(sizeof(i) == 2, "uint16_t size must be 2 byte2");
			boost::endian::conditional_reverse_inplace<boost::endian::order::native, Endianess>(i);
			return pack(reinterpret_cast<const char*>(&i), sizeof(i));
		}

		/**
		 * Packs a int16_t integer value.
		 *
		 * @param i the integer value to pack
		 *
		 * @return this
		 */
		inline Packer& pack(int16_t i) {
			static_assert(sizeof(i) == 2, "int16_t size must be 2 byte2");
			boost::endian::conditional_reverse_inplace<boost::endian::order::native, Endianess>(i);
			return pack(reinterpret_cast<const char*>(&i), sizeof(i));
		}

		/**
		 * Packs a uint32_t integer value.
		 *
		 * @param i the integer value to pack
		 *
		 * @return this
		 */
		inline Packer& pack(uint32_t i) {
			static_assert(sizeof(i) == 4, "uint32_t size must be 4 bytes");
			boost::endian::conditional_reverse_inplace<boost::endian::order::native, Endianess>(i);
			return pack(reinterpret_cast<const char*>(&i), sizeof(i));
		}

		/**
		 * Packs a int32_t integer value.
		 *
		 * @param i the integer value to pack
		 *
		 * @return this
		 */
		inline Packer& pack(int32_t i) {
			static_assert(sizeof(i) == 4, "int32_t size must be 4 bytes");
			boost::endian::conditional_reverse_inplace<boost::endian::order::native, Endianess>(i);
			return pack(reinterpret_cast<const char*>(&i), sizeof(i));
		}

		/**
		 * Packs a uint64_t integer value.
		 *
		 * @param i the integer value to pack
		 *
		 * @return this
		 */
		inline Packer& pack(uint64_t i) {
			static_assert(sizeof(i) == 8, "uint64_t size must be 8 bytes");
			boost::endian::conditional_reverse_inplace<boost::endian::order::native, Endianess>(i);
			return pack(reinterpret_cast<const char*>(&i), sizeof(i));
		}

		/**
		 * Packs a int64_t integer value.
		 *
		 * @param i the integer value to pack
		 *
		 * @return this
		 */
		inline Packer& pack(int64_t i) {
			static_assert(sizeof(i) == 8, "int64_t size must be 8 bytes");
			boost::endian::conditional_reverse_inplace<boost::endian::order::native, Endianess>(i);
			return pack(reinterpret_cast<const char*>(&i), sizeof(i));
		}

		/**
		 * Packs a boolean value.
		 *
		 * @param b the boolean value to pack
		 *
		 * @return this
		 */
		inline Packer& pack(bool b) {
			static_assert(sizeof(b) == 1, "bool size must be 1 byte");
			boost::endian::conditional_reverse_inplace<boost::endian::order::native, Endianess>(b);
			return pack(reinterpret_cast<const char*>(&b), sizeof(b));
		}

	public: // write operation
		/**
		 * Packs a char-pointer gives by <tt>ptr</tt> with length given by <tt>size</tt>.
		 *
		 * @param ptr   the char pointer to be packed
		 * @param size  the char pointer length
		 *
		 * @return this
		 */
		inline Packer& pack(const char* ptr, size_t size) {
			buffer.write(ptr, size);
			return *this;
		}

		/**
		 * Packs a char-pointer givem by <tt>ptr</tt> with length given by <tt>size</tt>.
		 *
		 * @param ptr   the char pointer to be packed
		 * @param size  the char pointer length
		 *
		 * @return this
		 */
		inline Packer& pack(const unsigned char* ptr, size_t size) {
			buffer.write(ptr, size);
			return *this;
		}

	};

}


#endif //PACKETBUFFER_PACKER_H
