/*
* Copyright (C) 2014 Jacob Hacker
* License: http://www.gnu.org/licenses/gpl.html GPL version 3
*/
#include <array>

#include "crypto.h"
#include "mega.h"
namespace MegaPP{
Mega::Mega(std::string domain){
	m_domain = domain;
}

void Mega::login(std::string username, std::string password){
	std::vector<uint> key = prepareKey(stringToA32(password));
}
}
