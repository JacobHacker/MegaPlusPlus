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
std::string a32ToB64(const std::vector<uint> &data);

std::vector<uint> stringToA32(std::string str);
std::string a32ToString(const std::vector<uint> &a32);

std::string aesCbcEncrypt(const std::string &data, const std::string &key,
						  std::vector<byte> iv={});

std::vector<uint> aesCbcEncryptA32(const std::vector<uint> &data,
								   const std::vector<uint> &key);

std::vector<uint> prepareKey(const std::vector<uint> &arr);

std::string stringHash(const std::string &str, const std::vector<uint> &key);

}

#endif