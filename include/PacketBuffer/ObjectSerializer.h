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

#ifndef PACKETBUFFER_OBJECTSERIALIZER_H
#define PACKETBUFFER_OBJECTSERIALIZER_H

namespace PacketBuffer {

	template<typename Packer, typename T>
	struct HasIntrusivePackMethod {
		template<typename U, void (U::*)(Packer&) const>
		struct SFINAE;

		template<typename U>
		static char test(SFINAE<U, &U::pack>*);

		template<typename U>
		static int test(...);

		static const bool value = sizeof(test<T>(0)) == sizeof(char);
	};

	template<typename Unpacker, typename T>
	struct HasIntrusiveUnpackMethod {
		template<typename U, void (U::*)(Unpacker&)>
		struct SFINAE;

		template<typename U>
		static char test(SFINAE<U, &U::unpack>*);

		template<typename U>
		static int test(...);

		static const bool value = sizeof(test<T>(0)) == sizeof(char);
	};

	/**
	 * The ObjectSerializer class template is responsible for implementing the serialization logic
	 * for non-primitive types.
	 *
	 * A user can provide a specialization for any type it wishes to add custom serialization code.
	 *
	 * By default, the ObjectSerializer will call the intrusive pack() and unpack() methods on the
	 * user given object. If these methods are not available, compilation will fail with a error
	 * message describing which of the required methods are missing.
	 *
	 * A custom ObjectSerialization specialization can be provided as follows:
	 *
	 * @code
	 *  struct UserDefinedType {
	 *      uint64_t    id;
	 *      std::string name;
	 *  }
	 *
	 *  namespace PacketBuffer {
	 *      template<>
	 *      class ObjectSerializer<UserDefinedType> {
	 *      public:
	 *          template<typename Packer>
	 *          static void pack(Packer& packer, const T& object) {
	 *              packer(object.id, object.name);
	 *          }
	 *
	 *          template<typename Unpacker>
	 *          static void unpack(Unpacker& unpacker, T& object) {
	 *              unpacker(object.id, object.name);
	 *          }
	 *      }
	 *  }
	 * @endcode
	 *
	 * If more convenient, a user can also provide a set of **intrusive** methods that allow
	 * packing and unpacking objects:
	 *
	 * @code
	 *  struct UserDefinedType {
	 *      uint64_t    id;
	 *      std::string name;
	 *
	 *      template<typename Packer>
	 *      void pack(Packer& packer) const {
	 *          packer(id, name);
	 *      }
	 *
	 *      template<typename Unpacker>
	 *      void unpack(Unpacker& unpacker) {
	 *          unpacker(id, name);
	 *      }
	 *  }
	 * @endcode
	 *
	 * @tparam T the type of the object to be packed and/or unpacked
	 */
	template<typename T, typename = void>
	class ObjectSerializer {
	public:
		/**
		 * Packs a object to its packed representation.
		 *
		 * This is a default implementation whose only job is to forward to the intrusive implementation,
		 * and if this is not available, fail at compile time.
		 *
		 * @tparam Packer   the packer type
		 * @param packer    the packer to write data from
		 * @param object    the object to be packed
		 */
		template<typename Packer>
		static inline void pack(Packer& packer, const T& object) {
			/*
			 * If you are getting an error here, this means that one of your objects being serialized does not implement
			 * a pack() method. A pack method can be implemented as follows:
			 *
			 * template<typename Packer>
			 * void pack(Packer& packer) const {
			 *  packer(
			 *      a, b, c
			 *  );
			 * }
			 */
			static_assert(HasIntrusivePackMethod<Packer, T>::value,
						  "The object of type T does not have a ObjectSerializer<T> "
								  "specialization and does not implement a intrusive pack(Packer&) method.");
			object.pack(packer);
		}

		/**
		 * Unpacks a object from its packet representation.
		 *
		 * This is a default implementation whose only job is to forward to the intrusive implementation,
		 * and if this is not available, fail at compile time.
		 *
		 * @tparam Unpacker the unpacker type
		 * @param unpacker  the unpacker to read data from
		 * @param object    the object to unpack to
		 */
		template<typename Unpacker>
		static inline void unpack(Unpacker& unpacker, T& object) {
			/*
			 * If you are getting an error here, this means that one of your objects being serialized does not implement
			 * a unpack() method. A unpack method can be implemented as follows:
			 *
			 * template<typename Unpacker>
			 * void unpack(Unpacker& unpacker) {
			 *  unpacker(
			 *      a, b, c
			 *  );
			 * }
			 */
			static_assert(HasIntrusiveUnpackMethod<Unpacker, T>::value,
						  "The object of type T does not have a ObjectSerializer<T> "
								  "specialization and does not implement a intrusive unpack(Unpacker&) method.");
			object.unpack(unpacker);
		}
	};

}


#endif //PACKETBUFFER_OBJECTSERIALIZER_H
