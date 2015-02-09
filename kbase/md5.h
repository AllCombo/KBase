/*
 @ Kingsley Chen
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef KBASE_MD5_H_
#define KBASE_MD5_H_

#include <array>

namespace kbase {

// Any 32-bit or wider unsigned integer data type will do
using MD5uint32 = unsigned int;

using MD5Digest = std::array<unsigned char, 16>;

struct MD5Context {
    MD5uint32 lo, hi;
    MD5uint32 a, b, c, d;
    unsigned char buffer[64];
    MD5uint32 block[16];
};

void MD5Init(MD5Context* context);

void MD5Update(MD5Context* context, const void* data, size_t size);

void MD5Final(MD5Context* context, MD5Digest* digest);

std::string MD5DigestToString(const MD5Digest& digest);

void MD5Sum(const void* data, size_t size, MD5Digest* digest);

std::string MD5String(const std::string str);

}   // namespace kbase

#endif  // KBASE_MD5_H_