#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
class messageHandler{
	public:
		int serverRead(string s,vector<std::string> &cmds);
		//std::string serverWrite(int stuff,);
		//En metod för varje commando
		std::string serverListNG(std::vector<std::string> NG);
		std::string serverCreateNG(bool answer);
		std::string serverDeleteNG(bool answer);
		std::string serverListArt(std::vector<std::string> articles);
		std::string serverCreateArt(bool answer);
		std::string serverDeleteArt(int answer);
		std::string serverGetArt(std::string &title, std::string &author, std::string &text);

	//For client:
		void clientRead(string answer);
		std::string clientListNG();
		std::string clientCreateNG(std::string NGname);
		std::string clientDeleteNG(int newsgroup);
		std::string clientListArt(int newsgroup);
		std::string clientCreateArt(int newsgroup, std::string &title, std::string &author, std::string &text);
		void clientDeleteArt(int newsgroup, int article);
		void clientGetArt(int newsgroup, int article);
	private:
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
