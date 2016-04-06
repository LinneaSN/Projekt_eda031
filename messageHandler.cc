#include "messageHandler.h"
#include "./clientserver/protocol.h"
#include <string>
#include <vector>
#include "./clientserver/connection.h"
#include <iostream>

using namespace std;

messageHandler::messageHandler(Connection& c) : conn(c) {}

//Server
void messageHandler::serverListNG(vector<string> &NG){
	unsigned int nbr = NG.size();
	sendCode(Protocol::ANS_LIST_NG);
	sendIntParameter(nbr); //nbr unsigned int, ok?
	for(unsigned int i = 0; i<nbr; ++i){
		sendIntParameter(i); //i unsigned int, ok?
		sendStringParameter(NG[i]);
	}
	sendCode(Protocol::ANS_END);
}

void messageHandler::serverCreateNG(bool answer) {
    sendCode(Protocol::ANS_CREATE_NG);
    if(answer){
        sendCode(Protocol::ANS_ACK);
    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
    }
    sendCode(Protocol::ANS_END);
}

void messageHandler::serverDeleteNG(bool answer) {
    sendCode(Protocol::ANS_DELETE_NG);
    if(answer){
        sendCode(Protocol::ANS_ACK);
    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    sendCode(Protocol::ANS_END);
}

void messageHandler::serverListArt(vector<string> &articles) {
    sendCode(Protocol::ANS_LIST_ART);
    unsigned int nbr = articles.size();
    if(nbr>0){
        sendCode(Protocol::ANS_ACK);
        sendIntParameter(nbr); //nbr is unsigned int, ok?
        for(unsigned int i =0;i<nbr;++i){ //i unsigned int, ok?
            sendIntParameter(i);
            sendStringParameter(articles[i]);
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


//ClientWrite
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

//ClientRead

vector<string> messageHandler::clientReadListNG(){
    unsigned char ans = recvCode();
    vector<string> result;
    if(ans!=Protocol::ANS_LIST_NG){
        //Do something until ANS_END is read, throw something?
        return result;
    }
    int nbr = recvIntParameter();
    string name;
    int id;
    for(int i = 0;i<nbr;++i){
        id = recvIntParameter();
        name = recvStringParameter();
        name.insert(0,". ");
        result.push_back(name.insert(0, to_string(id)));
    }
    ans = recvCode();
    if(ans!=Protocol::ANS_END){
        //do something, throw something?
    }
    return result;
}

bool messageHandler::clientReadCreateNG(){
    unsigned char ans = recvCode();
    bool result;
    if(ans!=Protocol::ANS_CREATE_NG){
        //do something until ANS_END, throw something
        return false;
    }
    ans = recvCode();
    if(ans==Protocol::ANS_ACK){
        result=true;
    } else {
        ans=recvCode();
        result=false;
    }
    ans=recvCode();
    if(ans!=Protocol::ANS_END){
        //do something, throw something?
    }
    return result;
}

bool messageHandler::clientReadDeleteNG(){
    unsigned char ans = recvCode();
    bool result;
    if(ans!=Protocol::ANS_DELETE_NG){
        //do something until ANS_END, throw something
        return false;
    }
    ans = recvCode();
    if(ans==Protocol::ANS_ACK){
        result=true;
    } else {
        ans=recvCode();
        result=false;
    }
    ans=recvCode();
    if(ans!=Protocol::ANS_END){
        //do something, throw something?
    }
    return result;
}

vector<string> messageHandler::clientReadListArt(bool &ok){
    unsigned char ans = recvCode();
    vector<string> result;
    if(ans!=Protocol::ANS_LIST_ART){
        //do something until ANS_END, throw something?
        return result;
    }
    ans = recvCode();
    if(ans!=Protocol::ANS_ACK){
        ok=false;
        ans = recvCode();
    } else {
        ok = true;
        int nbr = recvIntParameter();
        string name;
        int id;
        for(int i = 0;i<nbr;++i){
            id = recvIntParameter();
            name = recvStringParameter();
            name.insert(0,". ");
            result.push_back(name.insert(0,to_string(id)));
        }
    }
    ans=recvCode();
    if(ans!=Protocol::ANS_END){
        //do something, throw something?
    }
    return result;
}

bool messageHandler::clientReadCreateArt(){
    unsigned char ans = recvCode();
    if(ans!=Protocol::ANS_CREATE_ART){
        //do something until ANS_END, throw something?
        return false;
    }
    bool result;
    ans = recvCode();
    if(ans==Protocol::ANS_ACK){
        result=true;
    } else {
        result = false;
        ans = recvCode();
    }
    ans=recvCode();
    if(ans!=Protocol::ANS_END){
        //do something, throw something?
    }
    return result;
}

bool messageHandler::clientReadDeleteArt(string &error){
    unsigned char ans = recvCode();
    if(ans!=Protocol::ANS_DELETE_ART){
        //do something until ANS_END, throw something?
        return false;
    }
    bool result;
    ans = recvCode();
    if(ans==Protocol::ANS_ACK){
        result = true;
    } else {
        result = false;
        ans= recvCode();
        error = ans==Protocol::ERR_NG_DOES_NOT_EXIST ? "Error: Newsgroup does not exist." : "Error: Article does not exist.";
    }
    ans=recvCode();
    if(ans!=Protocol::ANS_END){
        //do something, throw something?
    }
    return result;
}

bool messageHandler::clientReadGetArt(string &title, string &author, string &text, string &error){
    unsigned char ans = recvCode();
    if(ans!=Protocol::ANS_GET_ART){
        //do something until ANS_END, throw something?
        return false;
    }
    bool result;
    ans = recvCode();
    if(ans == Protocol::ANS_ACK){
        title = recvStringParameter();
        author = recvStringParameter();
        text = recvStringParameter();
        result = true;
    } else {
        result = false;
        ans = recvCode();
        error = ans==Protocol::ERR_NG_DOES_NOT_EXIST ? "Error: Newsgroup does not exist." : "Error: Article does not exist.";
    }
    ans=recvCode();
    if(ans!=Protocol::ANS_END){
        //do something, throw something?
    }
    return result;
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


int main(){
    cout<<"test"<<endl;
}
