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

TEST_CASE("Serializer/Std/String", "[serializer][std][string]") {

	std::stringstream ss;
	PacketBuffer::Packer<std::ostream> packer(ss);
	PacketBuffer::Unpacker<std::istream> unpacker(ss);

	SECTION("empty string") {
		SECTION("should be correctly packed") {
			std::string string = "";
			packer.pack(string);

			CHECK(string_to_hex(ss.str()) == "0000000000000000");

			SECTION("and should unpack back") {
				std::string unpacked;
				unpacker.unpack(unpacked);

				CHECK(unpacked.size() == 0);
				CHECK(unpacked == string);
			}

		}
	}

	SECTION("should be correctly packed") {
		std::string string = "Hello Testing World";
		packer.pack(string);

		CHECK(string_to_hex(ss.str()) == "130000000000000048656C6C6F2054657374696E6720576F726C64");

		SECTION("and should unpack back") {
			std::string unpacked;
			unpacker.unpack(unpacked);

			REQUIRE(unpacked.size() == 19);
			CHECK(unpacked == string);
		}
	}

	SECTION("with NULL character") {
		SECTION("should be correctly packed") {
			std::string string = "Hey";
			string.resize(4);
			string[3] = 0x00;
			packer.pack(string);

			CHECK(string_to_hex(ss.str()) == "040000000000000048657900");

			SECTION("and should unpack back") {
				std::string unpacked;
				unpacker.unpack(unpacked);

				REQUIRE(unpacked.size() == 4);
				CHECK(unpacked == string);
			}
		}
	}

}