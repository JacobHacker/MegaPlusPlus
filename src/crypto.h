/*
* Copyright (C) 2014 Jacob Hacker
* License: http://www.gnu.org/licenses/gpl.html GPL version 3
*/
#ifndef MEGAPP_CRYPTO_H
#define MEGAPP_CRYPTO_H

#include <vector>
#include <string>
typedef unsigned int uint;
typedef unsigned int ulint;
typedef unsigned char byte;
typedef unsigned char uchar;

std::string toHex(std::string str);

std::vector<uint> stringToA32(std::string str);
std::string a32ToString(std::vector<uint> a32);

std::string aesCbcEncrypt(std::string data, std::string key,
						  std::vector<byte> iv={});

std::vector<uint> aesCbcEncryptA32(std::vector<uint> data,
								   std::vector<uint> key);

std::vector<uint> prepareKey(std::vector<uint> arr);
#endif