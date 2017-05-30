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

#include <PacketBuffer/PacketBuffer.h>
#include <PacketBuffer/Serializer/Std/Experimental.h>

#include <iostream>
#include <sstream>

using namespace PacketBuffer;

struct HELLO_MESSAGE {
	uint8_t id;
	uint8_t kind;

	unsigned char payload[64];
	std::array<uint8_t, 16> test;

	std::vector<int> vec;

	std::tuple<uint8_t, uint32_t, uint64_t> tuple;
	std::pair<uint8_t, uint32_t> pair;

	std::map<uint8_t, uint64_t> mapping;
	std::unordered_map<uint8_t, uint64_t> umapping;

	std::list<uint8_t> list;
	std::list<uint8_t> flist;
	std::set<uint8_t> set;
	std::unordered_set<uint8_t> uset;

	std::chrono::seconds durationSec;
	std::chrono::hours durationHour;

	std::chrono::system_clock::time_point time;

	std::experimental::optional<std::string> oName;

	std::string name;

	template<typename Packer>
	void pack(Packer& packer) const {
		packer(
				id, kind, payload, test, vec, tuple, pair, mapping, umapping, list, flist, set, uset, name, durationSec,
				durationHour, time, oName
		);
	}

	template<typename Unpacker>
	void unpack(Unpacker& unpacker) {
		unpacker(
				id, kind, payload, test, vec, tuple, pair, mapping, umapping, list, flist, set, uset, name, durationSec,
				durationHour, time, oName
		);
	}
};

int main(int argc, const char** argv) {
	std::stringstream ss;
	Packer<std::ostream> packer(ss);

	{
		HELLO_MESSAGE hello;
		hello.id = 20;
		hello.kind = 2;
		memset(hello.payload, 'A', sizeof(hello.payload));

		hello.vec = {1, 2, 3};
		hello.mapping[1] = 200;
		hello.mapping[2] = 300;

		hello.name = "It Works!";
//        hello.wname = L"Teste";
		hello.oName = std::string(";)");

		packer.pack(hello);
	}

	std::cout << "Serialized message has " << ss.str().size() << std::endl;

	{
		Unpacker<std::istream> unpacker(ss);
		HELLO_MESSAGE unpacked;

		unpacker.unpack(unpacked);

		std::cout << "id:      " << (int) unpacked.id << std::endl;
		std::cout << "kind:    " << (int) unpacked.kind << std::endl;
		std::cout << "payload: " << std::string((char*) unpacked.payload, 64) << std::endl;
		std::cout << "vec:     " << unpacked.vec.size() << std::endl;
		std::cout << "name:    " << unpacked.name << std::endl;

		std::cout << "mapping: " << std::endl;
		for(auto& entry : unpacked.mapping) {
			std::cout << "                  " << (int) entry.first << " = " << entry.second << std::endl;
		}

	}
}