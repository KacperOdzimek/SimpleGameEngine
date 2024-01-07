#pragma once
#include <cstdint>
#include <string>

namespace utilities
{
    namespace
    {
        //requires string to be null terminated
        uint32_t hash_string(std::string str)
        {
            uint32_t hash = 0;
            const char* x = str.c_str();
            for (; *x; ++x)
            {
                hash += *x;
                hash += (hash << 10);
                hash ^= (hash >> 6);
            }

            hash += (hash << 3);
            hash ^= (hash >> 11);
            hash += (hash << 15);

            return hash;
        }

        //requires string to be null terminated
        constexpr uint32_t hash_string(const char* str)
        {
            uint32_t hash = 0;
            for (; *str; ++str)
            {
                hash += *str;
                hash += (hash << 10);
                hash ^= (hash >> 6);
            }

            hash += (hash << 3);
            hash ^= (hash >> 11);
            hash += (hash << 15);

            return hash;
        }
    }
}