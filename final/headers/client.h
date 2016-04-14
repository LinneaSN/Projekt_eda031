#ifndef CLIENT_H
#define CLIENT_H
#include "./messageHandler.h"
#include "./clientserver/connection.h"
#include <iostream>
#include <map>
#include <set>

class client{
	public: 
		client(Connection &c);
		void parseCmd(std::string& input);
		void printErrorMessage();
	private:
		Connection& conn;
		messageHandler handler;	
		std::map<std::string,int> currentNewsGroup;
		std::string NG;
		bool clearflag=1;
		void listCMD(bool flagArt);
		void updateMap(bool printFlag);
		void readCMD(std::istringstream& ss);
		bool createCMD(std::istringstream& ss,std::string& parse);
		bool deleteCMD(std::istringstream& ss,std::string& parse);					
};
#endif
