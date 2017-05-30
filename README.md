PacketBuffer - Lightning fast packet parsing library
========================================================

PacketBuffer is a C++14 header-only library designed specifically to be really fast at processing binary network packets. It supports many of the C++ standard containers, including:

 * [`std::array`](include/PacketBuffer/Serializer/Std/Array.h)
 * [`T[S]` (statically sized arrays)](include/PacketBuffer/Serializer/Std/Array.h)
 * [`std::chrono::duration`](include/PacketBuffer/Serializer/Std/Chrono.h)
 * [`std::chrono::time_point`](include/PacketBuffer/Serializer/Std/Chrono.h)
 * [`std::vector`](include/PacketBuffer/Serializer/Std/Vector.h)
 * [`std::map`](include/PacketBuffer/Serializer/Std/Map.h)
 * [`std::unordered_map`](include/PacketBuffer/Serializer/Std/Map.h)
 * [`std::list`](include/PacketBuffer/Serializer/Std/List.h)
 * [`std::set`](include/PacketBuffer/Serializer/Std/Set.h)
 * [`std::unoredered_set`](include/PacketBuffer/Serializer/Std/Set.h)
 * [`std::string`](include/PacketBuffer/Serializer/Std/String.h)
 * [`std::tuple`](include/PacketBuffer/Serializer/Std/Tuple.h)
 * [`std::pair`](include/PacketBuffer/Serializer/Std/Pair.h)
 * [`std::experimental::optional`](include/PacketBuffer/Serializer/Std/Experimental/Optional.h)

It also supports endian swapping the following types: 
    `uint8_t`,  `int8_t`, 
    `uint16_t`, `int16_t`, 
    `uint32_t`, `int32_t`, 
    `uint64_t`, `int64_t`.
    
## Getting Started
````````
using namespace PacketBuffer;

std::stringstream ss;
Packer<std::stringstream> packer(ss);
packer.pack(uint8_t(100));
````````

That's it!

But wait, you probably want something more than just packing a `uint8_t`, right? What about some custom structs?

### Packing structs
``` c++
using namespace PacketBuffer;

struct MyPacket {
    uint8_t id;
    std::string name;
    uint8_t age;
    
    template<typename Packer>
    void pack(Packer& packer) const { packer(id, name, age); }
    
    template<typename Unpacker>
    void unpack(Unpacker& unpacker) { unpacker(id, name, age); }
};

std::stringstream ss;
Packer<std::stringstream> packer(ss);

MyPacket packet;
packer.pack(packet);
```

Seriously, that is it! You can now serialize and deserialize your structure on any machine with whatever byte order it has!