#ifndef MESSAGEHANDLERDB_H
#define MESSAGEHANDLERDB_H
#include <string>
#include <vector>
#include "connection.h"
#include "newsgroup.h"
#include "article.h"
#include "ERR_ART_DOES_NOT_EXIST.h"
#include "database.h"

class MessageHandler{
	public:
		MessageHandler(Connection& c);

		void serverListNG(Database &database);
		void serverCreateNG(Database &database);
		void serverDeleteNG(Database &database);
		void serverListArt(Database &database);
		void serverCreateArt(Database &database);
		void serverDeleteArt(Database &database);
		void serverGetArt(Database &database);

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
