#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <string>
#include <vector>
#include "connection.h"
#include "newsgroup.h"
#include "article.h"
#include "ERR_ART_DOES_NOT_EXIST.h"

class messageHandler{
	public:
		messageHandler(Connection& c);

		//int serverRead(string s,vector<std::string> &cmds);
		//std::string serverWrite(int stuff,);
		//En metod för varje commando
		void serverListNG(std::vector<Newsgroup> &NG);
		void serverCreateNG(std::vector<Newsgroup> &NG);
		void serverDeleteNG(std::vector<Newsgroup> &NG);
		void serverListArt(std::vector<Newsgroup> &NG);
		void serverCreateArt(std::vector<Newsgroup> &NG);
		void serverDeleteArt(std::vector<Newsgroup> &NG);// answer<0 : wrong ng nbr, answer==0 : wrong article nbr, answer>0 ok!
		void serverGetArt(std::vector<Newsgroup> &NG);

	//For client:
		//void clientRead(string answer);
		void clientListNG();
		void clientCreateNG(std::string NGname);
		void clientDeleteNG(int newsgroup);
		void clientListArt(int newsgroup);
		void clientCreateArt(int newsgroup, std::string &title, std::string &author, std::string &text);
		void clientDeleteArt(int newsgroup, int article);
		void clientGetArt(int newsgroup, int article);
	private:
		Connection& conn;

        	void sendByte(unsigned char code);
        	void sendCode(unsigned char code);
        	void sendInt(int value);
        	void sendIntParameter(int param);
        	void sendStringParameter(std::string param);
        	unsigned char recvByte();
        	unsigned char recvCode();
        	int recvInt();
        	int recvIntParameter();
        	std::string recvStringParameter();
};
#endif
