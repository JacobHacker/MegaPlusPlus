/*
* Copyright (C) 2014 Jacob Hacker
* License: http://www.gnu.org/licenses/gpl.html GPL version 3
*/
#include <iostream>
#include <sstream>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/adler32.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>

#include "crypto.h"

namespace MegaPP{

namespace bai = boost::archive::iterators;

std::string toHex(const std::string& str){
	std::string hex;
	CryptoPP::StringSource ss(str, true /*pumpAll*/,
		new CryptoPP::HexEncoder(
			new CryptoPP::StringSink(hex)
		)
	);
	return hex;
}

std::string toB64(const std::string& str) {
	namespace bai = boost::archive::iterators;
	const std::string base64_padding[] = {"", "==","="};

	std::stringstream os;

	typedef bai::base64_from_binary
		<bai::transform_width<const char *, 6, 8> > base64_enc;

	std::copy(base64_enc(str.c_str()),
			  base64_enc(str.c_str() + str.size()),
			  std::ostream_iterator<char>(os));

	return os.str() + base64_padding[str.size() % 3];
}

/*std::string base64_decode(const std::string& s) {
  namespace bai = boost::archive::iterators;

  std::stringstream os;

  typedef bai::transform_width<bai::binary_from_base64<const char *>, 8, 6> base64_dec;

  unsigned int size = s.size();

  // Remove the padding characters, cf. https://svn.boost.org/trac/boost/ticket/5629
  if (size && s[size - 1] == '=') {
    --size;
    if (size && s[size - 1] == '=') --size;
  }
  if (size == 0) return std::string();

  std::copy(base64_dec(s.data()), base64_dec(s.data() + size),
            std::ostream_iterator<char>(os));

  return os.str();
}*/

std::string b64UrlEncode(std::string b64){
	std::replace(b64.begin(), b64.end(), '+', '-');
	std::replace(b64.begin(), b64.end(), '/', '_');
	b64.erase(std::remove(b64.begin(), b64.end(), '='),b64.end());
	return b64;
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

std::string a32ToString(const std::vector<uint> &a32){
	std::string out{""};
	for(size_t i=0; i<a32.size(); ++i){
		out += (char)(a32[i] >> 8 * 3);
		out += (char)(a32[i] >> 8 * 2);
		out += (char)(a32[i] >> 8 * 1);
		out += (char)(a32[i]);
	}

	return out;
}

std::string aesCbcEncrypt(const std::string &data, const std::string &key,
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

std::vector<uint> aesCbcEncryptA32(const std::vector<uint> &data,
									 const std::vector<uint> &key){
	return stringToA32(aesCbcEncrypt(a32ToString(data), a32ToString(key)));
}

std::vector<uint> prepareKey(const std::vector<uint> &arr){
	std::vector<uint> pkey{0x93C467E3, 0x7DB0C7A4, 0xD1BE3F81, 0x0152CB56};
	for(size_t r = 0; r < 0x10000; ++r){
		for(size_t j = 0; j < arr.size(); j += 4){
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

std::string a32ToB64(const std::vector<uint> &data){
	return b64UrlEncode(toB64(a32ToString(data)));
}

std::string stringHash(const std::string &str, const std::vector<uint> &key){
	std::vector<uint> s32 = stringToA32(str);
	std::vector<uint> h32{0, 0, 0, 0};
	for(size_t i = 0; i < s32.size(); ++i){
		h32[i % 4] ^= s32[i];
	}
	for(int i=0; i<0x4000; ++i){
		h32 = aesCbcEncryptA32(h32, key);
	}
	return a32ToB64({h32[0], h32[2]});
}

}
