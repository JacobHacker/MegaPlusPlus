/*
* Copyright (C) 2014 Jacob Hacker
* License: http://www.gnu.org/licenses/gpl.html GPL version 3
*/
#ifndef MEGAPP_MEGA_H
#define MEGAPP_MEGA_H

#include <iostream>

namespace MegaPP{

const std::string DEFAULT_MEGA_DOMAIN = "https://mega.co.nz";

class Mega{
	public:
		Mega(const std::string &domain = DEFAULT_MEGA_DOMAIN);

		void login(const std::string &username, const std::string &password);
	private:
		std::string m_domain;
};

}
#endif