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

#include <catch.hpp>
#include <sstream>

#include <PacketBuffer/PacketBuffer.h>

namespace {
	std::string string_to_hex(const std::string& input) {
		static const char* const lut = "0123456789ABCDEF";
		size_t len = input.length();

		std::string output;
		output.reserve(2 * len);
		for(size_t i = 0; i < len; ++i) {
			const unsigned char c = input[i];
			output.push_back(lut[c >> 4]);
			output.push_back(lut[c & 15]);
		}
		return output;
	}
}

TEST_CASE("Serializer/Std/Array", "[serializer][std][array]") {

	std::stringstream ss;
	PacketBuffer::Packer<std::ostream> packer(ss);
	PacketBuffer::Unpacker<std::istream> unpacker(ss);

	SECTION("empty array") {
		SECTION("should be correctly packed") {
			std::array<uint8_t, 0> array;
			packer.pack(array);

			CHECK(string_to_hex(ss.str()) == "");

			SECTION("and should unpack back") {
				std::array<uint8_t, 0> unpacked;
				unpacker.unpack(unpacked);

				CHECK(unpacked.size() == 0);
			}

		}
	}

	SECTION("should be correctly packed") {
		std::array<uint8_t, 2> array = {100, 200};
		packer.pack(array);

		CHECK(string_to_hex(ss.str()) == "64C8");

		SECTION("and should unpack back") {
			std::array<uint8_t, 2> unpacked;
			unpacker.unpack(unpacked);

			REQUIRE(unpacked.size() == 2);
			CHECK(unpacked == array);
		}
	}

}

TEST_CASE("Serializer/Std/CArray", "[serializer][std][c-array]") {
	std::stringstream ss;
	PacketBuffer::Packer<std::ostream> packer(ss);
	PacketBuffer::Unpacker<std::istream> unpacker(ss);

	SECTION("should be correctly packed") {
		uint8_t array[4] = {1, 2, 3, 4};
		packer.pack(array);

		CHECK(string_to_hex(ss.str()) == "01020304");

		SECTION("and should unpack back") {
			uint8_t unpacked[4];
			unpacker.unpack(unpacked);

			CHECK(unpacked[0] == array[0]);
			CHECK(unpacked[1] == array[1]);
			CHECK(unpacked[2] == array[2]);
			CHECK(unpacked[3] == array[3]);
		}
	}

}