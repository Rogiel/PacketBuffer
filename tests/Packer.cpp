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

TEST_CASE("Packer", "[packer]") {

	std::stringstream ss;

	SECTION("little endian") {
		PacketBuffer::Packer<std::ostream, boost::endian::order::little> packer(ss);

		SECTION("uint8_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<uint8_t>::min());
				CHECK(string_to_hex(ss.str()) == "00");
			}

			SECTION("one") {
				packer.pack(uint8_t(1));
				CHECK(string_to_hex(ss.str()) == "01");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<uint8_t>::max());
				CHECK(string_to_hex(ss.str()) == "FF");
			}
		}

		SECTION("int8_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<int8_t>::min());
				CHECK(string_to_hex(ss.str()) == "80");
			}

			SECTION("zero") {
				packer.pack(int8_t(0));
				CHECK(string_to_hex(ss.str()) == "00");
			}

			SECTION("one") {
				packer.pack(int8_t(1));
				CHECK(string_to_hex(ss.str()) == "01");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<int8_t>::max());
				CHECK(string_to_hex(ss.str()) == "7F");
			}
		}

		SECTION("uint16_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<uint16_t>::min());
				CHECK(string_to_hex(ss.str()) == "0000");
			}

			SECTION("one") {
				packer.pack(uint16_t(1));
				CHECK(string_to_hex(ss.str()) == "0100");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<uint16_t>::max());
				CHECK(string_to_hex(ss.str()) == "FFFF");
			}
		}

		SECTION("int16_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<int16_t>::min());
				CHECK(string_to_hex(ss.str()) == "0080");
			}

			SECTION("zero") {
				packer.pack(int16_t(0));
				CHECK(string_to_hex(ss.str()) == "0000");
			}

			SECTION("one") {
				packer.pack(int16_t(1));
				CHECK(string_to_hex(ss.str()) == "0100");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<int16_t>::max());
				CHECK(string_to_hex(ss.str()) == "FF7F");
			}
		}

		SECTION("uint32_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<uint32_t>::min());
				CHECK(string_to_hex(ss.str()) == "00000000");
			}

			SECTION("one") {
				packer.pack(uint32_t(1));
				CHECK(string_to_hex(ss.str()) == "01000000");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<uint32_t>::max());
				CHECK(string_to_hex(ss.str()) == "FFFFFFFF");
			}
		}

		SECTION("int32_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<int32_t>::min());
				CHECK(string_to_hex(ss.str()) == "00000080");
			}

			SECTION("zero") {
				packer.pack(int32_t(0));
				CHECK(string_to_hex(ss.str()) == "00000000");
			}

			SECTION("one") {
				packer.pack(int32_t(1));
				CHECK(string_to_hex(ss.str()) == "01000000");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<int32_t>::max());
				CHECK(string_to_hex(ss.str()) == "FFFFFF7F");
			}
		}

		SECTION("uint64_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<uint64_t>::min());
				CHECK(string_to_hex(ss.str()) == "0000000000000000");
			}

			SECTION("one") {
				packer.pack(uint64_t(1));
				CHECK(string_to_hex(ss.str()) == "0100000000000000");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<uint64_t>::max());
				CHECK(string_to_hex(ss.str()) == "FFFFFFFFFFFFFFFF");
			}
		}

		SECTION("int64_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<int64_t>::min());
				CHECK(string_to_hex(ss.str()) == "0000000000000080");
			}

			SECTION("zero") {
				packer.pack(int64_t(0));
				CHECK(string_to_hex(ss.str()) == "0000000000000000");
			}

			SECTION("one") {
				packer.pack(int64_t(1));
				CHECK(string_to_hex(ss.str()) == "0100000000000000");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<int64_t>::max());
				CHECK(string_to_hex(ss.str()) == "FFFFFFFFFFFFFF7F");
			}
		}

		SECTION("bool") {
			SECTION("true") {
				packer.pack(true);
				CHECK(string_to_hex(ss.str()) == "01");
			}

			SECTION("false") {
				packer.pack(false);
				CHECK(string_to_hex(ss.str()) == "00");
			}
		}
	}

	SECTION("big endian") {
		PacketBuffer::Packer<std::ostream, boost::endian::order::big> packer(ss);

		SECTION("uint8_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<uint8_t>::min());
				CHECK(string_to_hex(ss.str()) == "00");
			}

			SECTION("one") {
				packer.pack(uint8_t(1));
				CHECK(string_to_hex(ss.str()) == "01");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<uint8_t>::max());
				CHECK(string_to_hex(ss.str()) == "FF");
			}
		}

		SECTION("int8_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<int8_t>::min());
				CHECK(string_to_hex(ss.str()) == "80");
			}

			SECTION("zero") {
				packer.pack(int8_t(0));
				CHECK(string_to_hex(ss.str()) == "00");
			}

			SECTION("one") {
				packer.pack(int8_t(1));
				CHECK(string_to_hex(ss.str()) == "01");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<int8_t>::max());
				CHECK(string_to_hex(ss.str()) == "7F");
			}
		}

		SECTION("uint16_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<uint16_t>::min());
				CHECK(string_to_hex(ss.str()) == "0000");
			}

			SECTION("one") {
				packer.pack(uint16_t(1));
				CHECK(string_to_hex(ss.str()) == "0001");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<uint16_t>::max());
				CHECK(string_to_hex(ss.str()) == "FFFF");
			}
		}

		SECTION("int16_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<int16_t>::min());
				CHECK(string_to_hex(ss.str()) == "8000");
			}

			SECTION("zero") {
				packer.pack(int16_t(0));
				CHECK(string_to_hex(ss.str()) == "0000");
			}

			SECTION("one") {
				packer.pack(int16_t(1));
				CHECK(string_to_hex(ss.str()) == "0001");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<int16_t>::max());
				CHECK(string_to_hex(ss.str()) == "7FFF");
			}
		}

		SECTION("uint32_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<uint32_t>::min());
				CHECK(string_to_hex(ss.str()) == "00000000");
			}

			SECTION("one") {
				packer.pack(uint32_t(1));
				CHECK(string_to_hex(ss.str()) == "00000001");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<uint32_t>::max());
				CHECK(string_to_hex(ss.str()) == "FFFFFFFF");
			}
		}

		SECTION("int32_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<int32_t>::min());
				CHECK(string_to_hex(ss.str()) == "80000000");
			}

			SECTION("zero") {
				packer.pack(int32_t(0));
				CHECK(string_to_hex(ss.str()) == "00000000");
			}

			SECTION("one") {
				packer.pack(int32_t(1));
				CHECK(string_to_hex(ss.str()) == "00000001");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<int32_t>::max());
				CHECK(string_to_hex(ss.str()) == "7FFFFFFF");
			}
		}

		SECTION("uint64_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<uint64_t>::min());
				CHECK(string_to_hex(ss.str()) == "0000000000000000");
			}

			SECTION("one") {
				packer.pack(uint64_t(1));
				CHECK(string_to_hex(ss.str()) == "0000000000000001");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<uint64_t>::max());
				CHECK(string_to_hex(ss.str()) == "FFFFFFFFFFFFFFFF");
			}
		}

		SECTION("int64_t") {
			SECTION("min") {
				packer.pack(std::numeric_limits<int64_t>::min());
				CHECK(string_to_hex(ss.str()) == "8000000000000000");
			}

			SECTION("zero") {
				packer.pack(int64_t(0));
				CHECK(string_to_hex(ss.str()) == "0000000000000000");
			}

			SECTION("one") {
				packer.pack(int64_t(1));
				CHECK(string_to_hex(ss.str()) == "0000000000000001");
			}

			SECTION("max") {
				packer.pack(std::numeric_limits<int64_t>::max());
				CHECK(string_to_hex(ss.str()) == "7FFFFFFFFFFFFFFF");
			}
		}

		SECTION("bool") {
			SECTION("true") {
				packer.pack(true);
				CHECK(string_to_hex(ss.str()) == "01");
			}

			SECTION("false") {
				packer.pack(false);
				CHECK(string_to_hex(ss.str()) == "00");
			}
		}
	}

}