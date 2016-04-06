#include "messageHandler.h"
#include "protocol.h"
#include <string>
#include <vector>
#include "connection.h"
#include <iostream>
#include <algorithm>

using namespace std;

messageHandler::messageHandler(Connection& c) : conn(c) {}

//Server
void messageHandler::serverListNG(vector<Newsgroup> &NG){
    recvCode(); // read COM_END

	unsigned int nbr = NG.size();
	sendCode(Protocol::ANS_LIST_NG);
	sendIntParameter(nbr); //nbr unsigned int, ok?
	for(unsigned int i = 0; i<nbr; ++i){
		sendIntParameter(i); //i unsigned int, ok?
		sendStringParameter(NG[i].getName());
	}
	sendCode(Protocol::ANS_END);
}

void messageHandler::serverCreateNG(vector<Newsgroup> &NG) {
    bool exists = false;
    string name = recvStringParameter(); // read string
    recvCode(); // read COM_END

    Newsgroup n(name);
    if (find_if(NG.begin(), NG.end(), 
                [name](Newsgroup& n){ return n.getName() == name;} ) != NG.end()) {
        exists = true;
    } else {
        NG.push_back(n);
    }

    sendCode(Protocol::ANS_CREATE_NG);
    if(!exists){
        sendCode(Protocol::ANS_ACK);
    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
    }
    sendCode(Protocol::ANS_END);
}

void messageHandler::serverDeleteNG(vector<Newsgroup> &NG) {
    bool removed = false;
    int id = recvIntParameter(); // read int
    recvCode(); // read COM_END

    auto it = remove_if(NG.begin(), NG.end(), 
            [id](Newsgroup& n){ return n.getNbr() == id;});
    if (it != NG.end()) {
        NG.erase(it);
        removed = true;
    }
    
    sendCode(Protocol::ANS_DELETE_NG);
    if(removed){
        sendCode(Protocol::ANS_ACK);
    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    sendCode(Protocol::ANS_END);
}

void messageHandler::serverListArt(vector<Newsgroup> &NG) {
    int id = recvIntParameter(); // read int
    recvCode(); // read COM_END

    auto it = find_if(NG.begin(), NG.end(), [id](Newsgroup& n){ return n.getNbr() == id; }); 
    
    sendCode(Protocol::ANS_LIST_ART);
    if (it != NG.end()) {
        vector<Article>& articles = it->listArticles();
        
        sendCode(Protocol::ANS_ACK);
        sendIntParameter(articles.size());
        for (auto &a : articles) {
            sendIntParameter(a.getNbr());
            sendStringParameter(a.getTitle());
        }
    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    sendCode(Protocol::ANS_END);
}

void messageHandler::serverCreateArt(bool answer) {
    sendCode(Protocol::ANS_CREATE_ART);
    if(answer){
        sendCode(Protocol::ANS_ACK);
    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    sendCode(Protocol::ANS_END);
}

void messageHandler::serverDeleteArt(int answer){
    sendCode(Protocol::ANS_DELETE_ART);
    if(answer>0){
        sendCode(Protocol::ANS_ACK);
    } else if(answer<0){
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
    }
    sendCode(Protocol::ANS_END);
}

void messageHandler::serverGetArt(int status, string &title, string &author, string &text){
    sendCode(Protocol::ANS_GET_ART);
    if(status>0){
        sendCode(Protocol::ANS_ACK);
        sendStringParameter(title);
        sendStringParameter(author);
        sendStringParameter(text);
    } else if(status<0){
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
    }
    sendCode(Protocol::ANS_END);
}

//check difference between byte and code !!!!!!!!!!!!!!!!!!!!!

//Client
void messageHandler::clientListNG(){
	sendCode(Protocol::COM_LIST_NG);
	sendCode(Protocol::COM_END);
}

void messageHandler::clientCreateNG(string NGname){
	sendCode(Protocol::COM_CREATE_NG);
	sendStringParameter(NGname);
	sendCode(Protocol::COM_END);
}

void messageHandler::clientDeleteNG(int newsgroup){
	sendCode(Protocol::COM_DELETE_NG);
	sendIntParameter(newsgroup);
	sendCode(Protocol::COM_END);
}

void messageHandler::clientListArt(int newsgroup){
	sendCode(Protocol::COM_LIST_ART);
	sendIntParameter(newsgroup);
	sendCode(Protocol::COM_END);
}

void messageHandler::clientCreateArt(int newsgroup, string &title, string &author, string &text){
	sendCode(Protocol::COM_CREATE_ART);
	sendIntParameter(newsgroup);
	sendStringParameter(title);
	sendStringParameter(author);
	sendStringParameter(text);
	sendCode(Protocol::COM_END);
}

void messageHandler::clientDeleteArt(int newsgroup, int article) {
	sendCode(Protocol::COM_DELETE_ART);
	sendIntParameter(newsgroup);
	sendIntParameter(article);
	sendCode(Protocol::COM_END);
}

void messageHandler::clientGetArt(int newsgroup, int article) {
	sendCode(Protocol::COM_GET_ART);
	sendIntParameter(newsgroup);
	sendIntParameter(article);
	sendCode(Protocol::COM_END);
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


//int main(){
//    cout<<"test"<<endl;
//}
