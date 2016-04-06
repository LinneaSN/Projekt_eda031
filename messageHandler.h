#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <string>
#include <vector>
#include "./clientserver/connection.h"

class messageHandler{
	public:
		messageHandler(Connection& c);

		//int serverRead(string s,vector<std::string> &cmds);
		//std::string serverWrite(int stuff,);
		//En metod för varje commando
		void serverListNG(std::vector<std::string> &NG);
		void serverCreateNG(bool answer);
		void serverDeleteNG(bool answer);
		void serverListArt(std::vector<std::string> &articles);
		void serverCreateArt(bool answer);
		void serverDeleteArt(int answer);// answer<0 : wrong ng nbr, answer==0 : wrong article nbr, answer>0 ok!
		void serverGetArt(int status, std::string &title, std::string &author, std::string &text);

	//For client:
		//void clientRead(string answer);
		void clientListNG();
		void clientCreateNG(std::string NGname);
		void clientDeleteNG(int newsgroup);
		void clientListArt(int newsgroup);
		void clientCreateArt(int newsgroup, std::string &title, std::string &author, std::string &text);
		void clientDeleteArt(int newsgroup, int article);
		void clientGetArt(int newsgroup, int article);

        std::vector<std::string> clientReadListNG();
        void clientReadCreateNG();
        void clientReadDeleteNG();
        void clientReadListArt();
        void clientReadCreateArt();
        void clientReadDeleteArt();
        void clientReadGetArt();
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



