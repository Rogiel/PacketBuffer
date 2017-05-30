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
#include <algorithm>
#include <stdexcept>

#include <PacketBuffer/PacketBuffer.h>

std::string hex_to_string(const std::string& input) {
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();
	if(len & 1) throw std::invalid_argument("odd length");

	std::string output;
	output.reserve(len / 2);
	for(size_t i = 0; i < len; i += 2) {
		char a = input[i];
		const char* p = std::lower_bound(lut, lut + 16, a);
		if(*p != a) throw std::invalid_argument("not a hex digit");

		char b = input[i + 1];
		const char* q = std::lower_bound(lut, lut + 16, b);
		if(*q != b) throw std::invalid_argument("not a hex digit");

		output.push_back(((p - lut) << 4) | (q - lut));
	}
	return output;
}

TEST_CASE("Unpacker", "[unpacker]") {

	std::stringstream ss;

	SECTION("little endian") {
		PacketBuffer::Unpacker<std::istream, boost::endian::order::little> unpacker(ss);

		SECTION("uint8_t") {
			SECTION("min") {
				ss.str(hex_to_string("00"));
				CHECK(unpacker.unpack<uint8_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("01"));
				CHECK(unpacker.unpack<uint8_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("FF"));
				CHECK(unpacker.unpack<uint8_t>() == 255);
			}
		}

		SECTION("int8_t") {
			SECTION("min") {
				ss.str(hex_to_string("80"));
				CHECK(unpacker.unpack<int8_t>() == -128);
			}

			SECTION("zero") {
				ss.str(hex_to_string("00"));
				CHECK(unpacker.unpack<int8_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("01"));
				CHECK(unpacker.unpack<int8_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("7F"));
				CHECK(unpacker.unpack<int8_t>() == 127);
			}
		}

		SECTION("uint16_t") {
			SECTION("min") {
				ss.str(hex_to_string("0000"));
				CHECK(unpacker.unpack<uint16_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("0100"));
				CHECK(unpacker.unpack<uint16_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("FFFF"));
				CHECK(unpacker.unpack<uint16_t>() == 65535);
			}
		}

		SECTION("int16_t") {
			SECTION("min") {
				ss.str(hex_to_string("0080"));
				CHECK(unpacker.unpack<int16_t>() == -32768);
			}

			SECTION("zero") {
				ss.str(hex_to_string("0000"));
				CHECK(unpacker.unpack<int16_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("0100"));
				CHECK(unpacker.unpack<int16_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("FF7F"));
				CHECK(unpacker.unpack<int16_t>() == 32767);
			}
		}

		SECTION("uint32_t") {
			SECTION("min") {
				ss.str(hex_to_string("00000000"));
				CHECK(unpacker.unpack<uint32_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("01000000"));
				CHECK(unpacker.unpack<uint32_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("FFFFFFFF"));
				CHECK(unpacker.unpack<uint32_t>() == 4294967295);
			}
		}

		SECTION("int32_t") {
			SECTION("min") {
				ss.str(hex_to_string("00000080"));
				CHECK(unpacker.unpack<int32_t>() == -2147483648);
			}

			SECTION("zero") {
				ss.str(hex_to_string("00000000"));
				CHECK(unpacker.unpack<int32_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("01000000"));
				CHECK(unpacker.unpack<int32_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("FFFFFF7F"));
				CHECK(unpacker.unpack<int32_t>() == 2147483647);
			}
		}

		SECTION("uint64_t") {
			SECTION("min") {
				ss.str(hex_to_string("0000000000000000"));
				CHECK(unpacker.unpack<uint64_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("0100000000000000"));
				CHECK(unpacker.unpack<uint64_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("FFFFFFFFFFFFFFFF"));
				CHECK(unpacker.unpack<uint64_t>() == 18446744073709551615UL);
			}
		}

		SECTION("int64_t") {
			SECTION("min") {
				ss.str(hex_to_string("0000000000000080"));
				CHECK(unpacker.unpack<int64_t>() == -9223372036854775808L);
			}

			SECTION("zero") {
				ss.str(hex_to_string("0000000000000000"));
				CHECK(unpacker.unpack<int64_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("0100000000000000"));
				CHECK(unpacker.unpack<int64_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("FFFFFFFFFFFFFF7F"));
				CHECK(unpacker.unpack<int64_t>() == 9223372036854775807L);
			}
		}

		SECTION("bool") {
			SECTION("true") {
				ss.str(hex_to_string("01"));
				CHECK(unpacker.unpack<bool>() == true);
			}

			SECTION("false") {
				ss.str(hex_to_string("00"));
				CHECK(unpacker.unpack<bool>() == false);
			}
		}
	}

	SECTION("big endian") {
		PacketBuffer::Unpacker<std::istream, boost::endian::order::big> unpacker(ss);

		SECTION("uint8_t") {
			SECTION("min") {
				ss.str(hex_to_string("00"));
				CHECK(unpacker.unpack<uint8_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("01"));
				CHECK(unpacker.unpack<uint8_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("FF"));
				CHECK(unpacker.unpack<uint8_t>() == 255);
			}
		}

		SECTION("int8_t") {
			SECTION("min") {
				ss.str(hex_to_string("80"));
				CHECK(unpacker.unpack<int8_t>() == -128);
			}

			SECTION("zero") {
				ss.str(hex_to_string("00"));
				CHECK(unpacker.unpack<int8_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("01"));
				CHECK(unpacker.unpack<int8_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("7F"));
				CHECK(unpacker.unpack<int8_t>() == 127);
			}
		}

		SECTION("uint16_t") {
			SECTION("min") {
				ss.str(hex_to_string("0000"));
				CHECK(unpacker.unpack<uint16_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("0001"));
				CHECK(unpacker.unpack<uint16_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("FFFF"));
				CHECK(unpacker.unpack<uint16_t>() == 65535);
			}
		}

		SECTION("int16_t") {
			SECTION("min") {
				ss.str(hex_to_string("8000"));
				CHECK(unpacker.unpack<int16_t>() == -32768);
			}

			SECTION("zero") {
				ss.str(hex_to_string("0000"));
				CHECK(unpacker.unpack<int16_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("0001"));
				CHECK(unpacker.unpack<int16_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("7FFF"));
				CHECK(unpacker.unpack<int16_t>() == 32767);
			}
		}

		SECTION("uint32_t") {
			SECTION("min") {
				ss.str(hex_to_string("00000000"));
				CHECK(unpacker.unpack<uint32_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("00000001"));
				CHECK(unpacker.unpack<uint32_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("FFFFFFFF"));
				CHECK(unpacker.unpack<uint32_t>() == 4294967295);
			}
		}

		SECTION("int32_t") {
			SECTION("min") {
				ss.str(hex_to_string("80000000"));
				CHECK(unpacker.unpack<int32_t>() == -2147483648);
			}

			SECTION("zero") {
				ss.str(hex_to_string("00000000"));
				CHECK(unpacker.unpack<int32_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("00000001"));
				CHECK(unpacker.unpack<int32_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("7FFFFFFF"));
				CHECK(unpacker.unpack<int32_t>() == 2147483647);
			}
		}

		SECTION("uint64_t") {
			SECTION("min") {
				ss.str(hex_to_string("0000000000000000"));
				CHECK(unpacker.unpack<uint64_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("0000000000000001"));
				CHECK(unpacker.unpack<uint64_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("FFFFFFFFFFFFFFFF"));
				CHECK(unpacker.unpack<uint64_t>() == 18446744073709551615UL);
			}
		}

		SECTION("int64_t") {
			SECTION("min") {
				ss.str(hex_to_string("8000000000000000"));
				CHECK(unpacker.unpack<int64_t>() == -9223372036854775808L);
			}

			SECTION("zero") {
				ss.str(hex_to_string("0000000000000000"));
				CHECK(unpacker.unpack<int64_t>() == 0);
			}

			SECTION("one") {
				ss.str(hex_to_string("0000000000000001"));
				CHECK(unpacker.unpack<int64_t>() == 1);
			}

			SECTION("max") {
				ss.str(hex_to_string("7FFFFFFFFFFFFFFF"));
				CHECK(unpacker.unpack<int64_t>() == 9223372036854775807L);
			}
		}

		SECTION("bool") {
			SECTION("true") {
				ss.str(hex_to_string("01"));
				CHECK(unpacker.unpack<bool>() == true);
			}

			SECTION("false") {
				ss.str(hex_to_string("00"));
				CHECK(unpacker.unpack<bool>() == false);
			}
		}
	}

	SECTION("should throw an error on overflow") {
		PacketBuffer::Unpacker<std::istream> unpacker(ss);

//        REQUIRE_THROWS(unpacker.unpack<uint32_t>());
	}


}