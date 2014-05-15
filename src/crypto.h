/*
* Copyright (C) 2014 Jacob Hacker
* License: http://www.gnu.org/licenses/gpl.html GPL version 3
*/
#ifndef MEGAPP_CRYPTO_H
#define MEGAPP_CRYPTO_H

#include <vector>
#include <string>

namespace MegaPP{

typedef unsigned int uint;
typedef unsigned int ulint;
typedef unsigned char byte;
typedef unsigned char uchar;
typedef std::vector<uint> a32;

std::string toB64(const std::string& str);
std::string toHex(const std::string& str);
std::string b64UrlEncode(std::string b64);
std::string a32ToB64(std::vector<uint> data);

std::vector<uint> stringToA32(std::string str);
std::string a32ToString(std::vector<uint> a32);

std::string aesCbcEncrypt(std::string data, std::string key,
						  std::vector<byte> iv={});

std::vector<uint> aesCbcEncryptA32(std::vector<uint> data,
								   std::vector<uint> key);

std::vector<uint> prepareKey(std::vector<uint> arr);

/*def stringhash(str, aeskey):
    s32 = str_to_a32(str)
    h32 = [0, 0, 0, 0]
    for i in range(len(s32)):
        h32[i % 4] ^= s32[i]
    for r in range(0x4000):
        h32 = aes_cbc_encrypt_a32(h32, aeskey)
    return a32_to_base64((h32[0], h32[2]))*/
std::string stringHash(std::string str, std::vector<uint> key);

}
#endif