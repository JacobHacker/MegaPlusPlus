/*
* Copyright (C) 2014 Jacob Hacker
* License: http://www.gnu.org/licenses/gpl.html GPL version 3
*/
#include <array>
#include <map>
#include <random>
#include <limits>

#include <jsoncpp/json/writer.h>
#define BOOST_NETWORK_ENABLE_HTTPS
#include <boost/network/protocol/http/client.hpp>

#include "crypto.h"
#include "mega.h"

namespace MegaPP{

namespace bn = boost::network;
namespace bnh = boost::network::http;
namespace bnhc = boost::network::http::client;

Mega::Mega(const std::string &domain){
	m_domain = domain;
	m_loggedIn = false;

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<uint> ud(0, std::numeric_limits<uint>::max());
	m_sequenceNum = ud(eng);
}

void Mega::login(const std::string &username, const std::string &password){
	std::vector<uint> key{prepareKey(stringToA32(password))};
	std::string uh{stringHash(username, key)};

	Json::Value root;
	root[0]["a"] = "us";
	root[0]["user"] = username;
	root[0]["uh"] = uh;
	megaRequest(Json::FastWriter().write(root));
}

void Mega::megaRequest(const std::string &data){
	std::string uri{m_domain + "/cs?id=" + std::to_string(m_sequenceNum)};
	++m_sequenceNum;

	bnh::client client;
	bnhc::request req = bnhc::request{uri};
	bnhc::response res = client.post(req, data);
}

bool Mega::loggedIn(){
	return m_loggedIn;
}

}
