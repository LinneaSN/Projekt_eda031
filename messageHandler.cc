#include "messageHandler.h"
#include "protocol.h"
#include <string>
#include <vector>

using namnspace std;

messageHandler::messageHandler(Connection c) : conn(c) {}

//Server
void messageHandler::serverListNG(vector<string> &NG){
	//string s;
	unsigned int nbr = NG.size();
	//s.append(to_string(ANS_LIST_NG) + ' ' + to_string(nbr) + ' ');
	sendByte(ANS_LIST_NG);
	sendInt(nbr); //nbr unsigned int, ok?
	for(unsigned int i = 0; i<nbr; ++i){
	//	s.append(to_string(i) + ' ' NG[i] + ' ');
		sendIntParameter(i); //i unsigned int, ok?
		sendStringParameter(NG[i]);
	}
	//s.append(
	sendByte(ANS_END);
}

void messageHandler::serverCreateNG(bool answer) {
    sendCode(ANS_CREATE_NG);
    if(answer){
        sendCode(ANS_ACK);
    } else {
        sendCode(ANS_NAK);
        sendCode(ERR_NG_ALREADY_EXISTS);
    }
    sendCode(ANS_END);
}

void messageHandler::serverDeleteNG(bool answer) {
    sendCode(ANS_Delete_NG);
    if(answer){
        sendCode(ANS_ACK);
    } else {
        sendCode(ANS_NAK);
        sendCode(ERR_NG_DOES_NOT_EXIST);
    }
    sendCode(ANS_END);
}

void messageHandler::serverListArt(vector<string> &articles) {
    sendCode(ANS_LIST_ART);
    unsigned int nbr = articles.size();
    if(nbr>0){
        sendCode(ANS_ACK);
        sendIntParameter(nbr); //nbr is unsigned int, ok?
        for(unsigned int i =0;i<nbr;++i){ //i unsigned int, ok?
            sendIntParameter(i);
            sendStringParameter(articles[i]);
        }
    } else {
        sendCode(ANS_NAK);
        sendCode(ERR_NG_DOES_NOT_EXIST);
    }
    sendCode(ANS_END);
}

//check difference between byte and code !!!!!!!!!!!!!!!!!!!!!

//Client
void messageHandler::clientListNG(){
	//string s;
	//s.append(to_string(COM_LIST_NG) + ' ' + to_string(COM_END));
	sendByte(COM_LIST_NG);
	sendByte(COM_END);
}

void messageHandler::clientCreateNG(string NGname){
	//NGname.insert(0,to_string(COM_CREATE_NG) + ' ');
	//NGname.append(' ' + to_string(COM_END));
	sendByte(COM_CREATE_NG);
	sendStringParameter(NGname);
	sendByte(COM_END);
}

void messageHandler::clientDeleteNG(int newsgroup){
	//string s;
	//s.append(to_string(COM_DELETE_NG) + ' ' + to_string(newsgroup) + ' ' + to_string(COM_END));
	sendByte(COM_DELETE_NG);
	sendIntParameter(newsgroup);
	sendByte(COM_END);
}

void messageHandler::clientListArt(int newsgroup){
	//string s;
	//s.append(to_string(COM_LIST_ART) + ' ' + to_string(newsgroup) + ' ' + to_string(COM_END));
	sendByte(COM_LIST_ART);
	sendIntParameter(newsgroup);
	sendByte(COM_END);
}

void messageHandler::clienCreateArt(int newsgroup, string &title, string &author, string &text){
	//string s;
	//s.append(to_string(COM_CREATE_ART) + ' ' + to_string(newsgroup) + ' ');
	//s.append(title + ' ' + author + ' ' + text + ' ' + to_string(COM_END));
	sendByte(COM_CREATE_ART);
	sendIntParameter(newsgroup);
	sendStringParameter(title);
	sendStringParameter(author);
	sendStringParameter(text);
	sendByte(COM_END);
}

void messageHandler::clientDeleteArt(int newsgroup, int article) {
	//string s;
	//s.append(to_string(COM_DELETE_ART) + ' ' + to_string(newsgroup) + ' ' + to_string(article) + ' ' + to_string(COM_END));
	sendByte(COM_DELETE_ART);
	sendIntParameter(newsgroup);
	sendIntParameter(article);
	sendByte(COM_END);
}

void mesageHandler::clientGetArt(int newsgroup, int article) {
	//string s;
	//s.append(to_string(COM_GET_ART) + ' ' + to_string(newsgroup) + ' ' + to_string(article) + ' ' + to_string(COM_END));
	sendByte(COM_GET_ART);
	sendIntParameter(newsgroup);
	sendIntParameter(article);
	sendByte(COM_END);
}

// Low level protocol functions 
void messageHandler::sendByte(unsigned char code) {
    conn.write(code);
}

void messageHandler::sendCode(unsigned char code) {
    sendByte(code);
}

void messageHandler::sendInt(int value) {
    sendByte((value >> 24) & 0xFF);
    sendByte((value >> 16) & 0xFF);
    sendByte((value >> 8) & 0xFF);
    sendByte(value & 0xFF);
}

void messageHandler::sendIntParameter(int param) {
    sendCode(Protocol::PAR_NUM);
    sendInt(param);
}

void messageHandler::sendStringParameter(string param) {
    sendCode(Protocol::PAR_STRING);
    sendInt(param.length());
    for (const unsigned char c : param) {
        sendByte(c);
    } 
}

unsigned char messageHandler::recvByte() {
    unsigned char code = conn.read();
    return code;
}

unsigned char messageHandler::recvCode() {
    unsigned char code = recvByte();
    return code;
}

int messageHandler::recvInt() {
    int b1 = recvByte();
    int b2 = recvByte();
    int b3 = recvByte();
    int b4 = recvByte();
    return b1 << 24 | b2 << 16 | b3 << 8 | b4;
}

int messageHandler::recvIntParameter() {
    unsigned char code = recvCode();
    if (code != Protocol::PAR_NUM) {
        // throw protocol violation error
    }
    return recvInt();
}

string messageHandler::recvStringParameter() {
    unsigned char code = recvCode();
    if (code != Protocol::PAR_STRING) {
        // throw protocol violation error
    }
    int n = recvInt();
    if (n < 0) {
        // throw protocol violation error
    }
    string result;
    for (int i = 0; i < n; ++i) {
        unsigned char c = conn.read();
        result += c;
    }
    return result;
}
