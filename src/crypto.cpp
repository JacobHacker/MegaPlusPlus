/*
* Copyright (C) 2014 Jacob Hacker
* License: http://www.gnu.org/licenses/gpl.html GPL version 3
*/
#include <iostream>

#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/adler32.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>

#include "crypto.h"

std::string toHex(std::string str){
	std::string hex;
	CryptoPP::StringSource ss(str, true /*pumpAll*/,
		new CryptoPP::HexEncoder(
			new CryptoPP::StringSink(hex)
		)
	);
	return hex;
}

// Unpack string into unsigned ints
std::vector<ulint> stringToA32(std::string str){
	// Make string divisible by 4
	if(str.size()%4){
		str += std::string(4-str.size()%4, '\0');
	}

	std::vector<ulint> out;
	for(size_t i=0; i<str.size(); i+=4){
		out.push_back((uchar)str[i  ] << 8*3 |
					  (uchar)str[i+1] << 8*2 |
					  (uchar)str[i+2] << 8*1 |
					  (uchar)str[i+3]);
	}
	return out;
}

std::string a32ToString(std::vector<uint> a32){
	std::string out("");
	for(size_t i=0; i<a32.size(); ++i){
		out += (char)(a32[i] >> 8 * 3);
		out += (char)(a32[i] >> 8 * 2);
		out += (char)(a32[i] >> 8 * 1);
		out += (char)(a32[i]);
	}

	return out;
}

std::string aesCbcEncrypt(std::string data, std::string key,
						  std::vector<byte> iv){
	CryptoPP::AutoSeededRandomPool prng;

	// TODO: BUG?: IV should be random
	// TODO: prng.GenerateBlock( iv, sizeof(iv) );
	if(iv.size() == 0) iv.assign(16, '\0');

	std::vector<byte> k;
	for(size_t i=0; i<key.size(); ++i){
		k.push_back(key[i]);
	}
	
	std::string out;
	try{
		CryptoPP::CBC_Mode< CryptoPP::AES >::Encryption e;

		e.SetKeyWithIV(k.data(), k.size(), iv.data(), iv.size());
		CryptoPP::StringSource ss(data, true,
			new CryptoPP::StreamTransformationFilter( e,
				new CryptoPP::StringSink(out),
				CryptoPP::StreamTransformationFilter::BlockPaddingScheme::NO_PADDING
			)
		);
	}catch(const CryptoPP::Exception& e){
		std::cerr << "Exception: " << e.what() << std::endl;
		exit(1);
	}
	
	return out;
}

std::vector<uint> aesCbcEncryptA32(std::vector<uint> data,
									 std::vector<uint> key){
	return stringToA32(aesCbcEncrypt(a32ToString(data), a32ToString(key)));
}

std::vector<uint> prepareKey(std::vector<uint> arr){
	std::vector<uint> pkey{0x93C467E3, 0x7DB0C7A4, 0xD1BE3F81, 0x0152CB56};
	for(size_t r = 0; r < 0x10000; ++r){
		for(size_t j = 0; j < 4; j += 4){
			std::vector<uint> key{0, 0, 0, 0};
			for(int i = 0; i < 4; ++i){
				if(i + j < arr.size()){
					key[i] = arr[i + j];
				}
			}
			pkey = aesCbcEncryptA32(pkey, key);
		}
	}
	return pkey;
}