#ifndef CLIENT_H
#define CLIENT_H
#include "./messageHandler.h"
#include "./clientserver/connection.h"
#include <iostream>
#include <map>
#include <set>
//class messageHandler;

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
};
#endif
