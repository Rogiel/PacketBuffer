#
# Copyright (c) 2017, Rogiel Sulzbach
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of Rogiel Sulzbach nor the names of contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#

cmake_minimum_required(VERSION 3.8)
project(PacketBuffer)

set(CMAKE_CXX_STANDARD 14)

add_library(PacketBuffer INTERFACE)
target_include_directories(PacketBuffer INTERFACE include)

option(PACKET_BUFFER_EXAMPLES "Enable to build examples" OFF)
if(PACKET_BUFFER_EXAMPLES)
    add_executable(PacketBuffer.Example example/main.cpp)
    target_link_libraries(PacketBuffer.Example PacketBuffer)
endif()

option(PACKET_BUFFER_TESTS "Enable to build tests" OFF)
if(PACKET_BUFFER_TESTS)
    file(GLOB_RECURSE TESTS_SRC tests/*.cpp)
    add_executable(PacketBuffer.Tests ${TESTS_SRC})
    target_link_libraries(PacketBuffer.Tests PacketBuffer)
    target_include_directories(PacketBuffer.Tests PRIVATE Catch/include)
endif()
