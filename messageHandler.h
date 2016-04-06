#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <string>
#include <vector>
#include "./clientserver/connection.h"

class messageHandler{
	public:
		messageHandler(Connection& c);
		
        //En metod för varje commando
	    void serverListNG(std::vector<Newsgroup> &NG);
		void serverCreateNG(std::vector<Newsgroup> &NG);
		void serverDeleteNG(std::vector<Newsgroup> &NG);
		void serverListArt(std::vector<Newsgroup> &NG);
		void serverCreateArt(std::vector<Newsgroup> &NG);
		void serverDeleteArt(std::vector<Newsgroup> &NG);// answer<0 : wrong ng nbr, answer==0 : wrong article nbr, answer>0 ok!
		void serverGetArt(std::vector<Newsgroup> &NG);	

	//For client:
		void clientListNG();
		void clientCreateNG(std::string NGname);
		void clientDeleteNG(int newsgroup);
		void clientListArt(int newsgroup);
		void clientCreateArt(int newsgroup, std::string &title, std::string &author, std::string &text);
		void clientDeleteArt(int newsgroup, int article);
		void clientGetArt(int newsgroup, int article);

        std::vector<std::string> clientReadListNG();
        bool clientReadCreateNG();
        bool clientReadDeleteNG();
        std::vector<std::string> clientReadListArt(bool &ok);
        bool clientReadCreateArt();
        bool clientReadDeleteArt(std::string &error);
        bool clientReadGetArt(std::string &title, std::string &author, std::string &text, std::string &error);

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



