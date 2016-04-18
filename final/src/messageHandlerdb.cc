#include "messageHandlerdb.h"
#include "protocol.h"
#include <string>
#include <vector>
#include "connection.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace std;

MessageHandler::MessageHandler(Connection& c) : conn(c) {}

//Server
void MessageHandler::serverListNG(Database &database){
    vector<Newsgroup> &NG = database.newsgroups;
    recvCode(); // read COM_END

	unsigned int nbr = NG.size();
	sendCode(Protocol::ANS_LIST_NG);
	sendIntParameter(nbr); //nbr unsigned int, ok?
	
    for (auto n : NG) {
        sendIntParameter(n.getNbr());
        sendStringParameter(n.getName());
    }
    
    /*for(unsigned int i = 0; i<nbr; ++i){
		sendIntParameter(i); //i unsigned int, ok?
		sendStringParameter(NG[i].getName());
	}*/
	sendCode(Protocol::ANS_END);
}

void MessageHandler::serverCreateNG(Database &database) {
    vector<Newsgroup> &NG = database.newsgroups;
    bool exists = false;
    string name = recvStringParameter(); // read string
    recvCode(); // read COM_END

    if (find_if(NG.begin(), NG.end(), 
                [name](Newsgroup& n){ return n.getName() == name;} ) != NG.end()) {
        exists = true;
    } else {
        Newsgroup n(name);
        NG.push_back(n);
        database.createNewsgroup(n);
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

void MessageHandler::serverDeleteNG(Database &database) {
    vector<Newsgroup> &NG = database.newsgroups;
    bool removed = false;
    int id = recvIntParameter(); // read int
    recvCode(); // read COM_END

    auto it = remove_if(NG.begin(), NG.end(), 
            [id](Newsgroup& n){ return n.getNbr() == id;});
    if (it != NG.end()) {
        database.deleteNewsgroup(id, it->getName());
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

void MessageHandler::serverListArt(Database &database) {
    vector<Newsgroup> &NG = database.newsgroups;
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
	    string reply = a.getTitle();
            sendStringParameter(reply);

        }
    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    sendCode(Protocol::ANS_END);
}

void MessageHandler::serverCreateArt(Database &database) {
    vector<Newsgroup> &NG = database.newsgroups;
    int id = recvIntParameter();
    string title = recvStringParameter();
    string author = recvStringParameter();
    string text = recvStringParameter();
    recvCode(); // COM_END
    
    auto it = find_if(NG.begin(), NG.end(), [id](Newsgroup& n){ return n.getNbr() == id; }); 

    sendCode(Protocol::ANS_CREATE_ART);
    if(it != NG.end()){
        Article art(title, author, text);
        it->addArticle(art);
        database.createArticle(*it, art);
        sendCode(Protocol::ANS_ACK);
    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    sendCode(Protocol::ANS_END);
}

void MessageHandler::serverDeleteArt(Database &database){
    vector<Newsgroup> &NG = database.newsgroups;
    int newsID = recvIntParameter();
    int articleID = recvIntParameter();
    recvCode(); // COM_END

    auto it = find_if(NG.begin(), NG.end(), 
            [newsID](Newsgroup& n){ return n.getNbr() == newsID; });

    sendCode(Protocol::ANS_DELETE_ART);
    if (it != NG.end()) {
        try {
            it->deleteArticle(articleID);
            database.deleteArticle(*it, articleID);
            sendCode(Protocol::ANS_ACK);
        } catch (ERR_ART_DOES_NOT_EXIST &e) {
            sendCode(Protocol::ANS_NAK);
            sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
        }
    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    sendCode(Protocol::ANS_END);
}

void MessageHandler::serverGetArt(Database &database){
    vector<Newsgroup> &NG = database.newsgroups;
    int newsID = recvIntParameter();
    int articleID = recvIntParameter();
    recvCode(); // COM_END

    auto it = find_if(NG.begin(), NG.end(), 
            [newsID](Newsgroup& n){ return n.getNbr() == newsID; });
    
    sendCode(Protocol::ANS_GET_ART);
    if (it != NG.end()) {
        try {
            Article article = it->getArticle(articleID);
            sendCode(Protocol::ANS_ACK);
            sendStringParameter(article.getTitle());
            sendStringParameter(article.getAuthor());
            sendStringParameter(article.getText());
        } catch (ERR_ART_DOES_NOT_EXIST &e) {
            sendCode(Protocol::ANS_NAK);
            sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
        }

    } else {
        sendCode(Protocol::ANS_NAK);
        sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    sendCode(Protocol::ANS_END);
}

//Client write
void MessageHandler::clientListNG(){
	sendCode(Protocol::COM_LIST_NG);
	sendCode(Protocol::COM_END);
}

void MessageHandler::clientCreateNG(string NGname){
	sendCode(Protocol::COM_CREATE_NG);
	sendStringParameter(NGname);
	sendCode(Protocol::COM_END);
}

void MessageHandler::clientDeleteNG(int newsgroup){
	sendCode(Protocol::COM_DELETE_NG);
	sendIntParameter(newsgroup);
	sendCode(Protocol::COM_END);
}

void MessageHandler::clientListArt(int newsgroup){
	sendCode(Protocol::COM_LIST_ART);
	sendIntParameter(newsgroup);
	sendCode(Protocol::COM_END);
}

void MessageHandler::clientCreateArt(int newsgroup, string &title, string &author, string &text){
	sendCode(Protocol::COM_CREATE_ART);
	sendIntParameter(newsgroup);
	sendStringParameter(title);
	sendStringParameter(author);
	sendStringParameter(text);
	sendCode(Protocol::COM_END);
}

void MessageHandler::clientDeleteArt(int newsgroup, int article) {
	sendCode(Protocol::COM_DELETE_ART);
	sendIntParameter(newsgroup);
	sendIntParameter(article);
	sendCode(Protocol::COM_END);
}

void MessageHandler::clientGetArt(int newsgroup, int article) {
	sendCode(Protocol::COM_GET_ART);
	sendIntParameter(newsgroup);
	sendIntParameter(article);
	sendCode(Protocol::COM_END);
}

//ClientRead

vector<string> MessageHandler::clientReadListNG(){
    unsigned char ans = recvCode();
    vector<string> result;
    if(ans!=Protocol::ANS_LIST_NG){
        throw invalid_argument("Recieved an unexpected answer from server.");
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
        throw invalid_argument("Protocol error from server.");
    }
    return result;
}

bool MessageHandler::clientReadCreateNG(){
    unsigned char ans = recvCode();
    bool result;
    if(ans!=Protocol::ANS_CREATE_NG){
        throw invalid_argument("Recieved an unexpected answer from server.");
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
        throw invalid_argument("Protocol error from server.");
    }
    return result;
}

bool MessageHandler::clientReadDeleteNG(){
    unsigned char ans = recvCode();
    bool result;
    if(ans!=Protocol::ANS_DELETE_NG){
        throw invalid_argument("Recieved an unexpected answer from server.");
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
        throw invalid_argument("Protocol error from server.");
    }
    return result;
}

vector<string> MessageHandler::clientReadListArt(bool &ok){
    unsigned char ans = recvCode();
    vector<string> result;
    if(ans!=Protocol::ANS_LIST_ART){
        throw invalid_argument("Recieved an unexpected answer from server.");
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
        throw invalid_argument("Protocol error from server.");
    }
    return result;
}

bool MessageHandler::clientReadCreateArt(){
    unsigned char ans = recvCode();
    if(ans!=Protocol::ANS_CREATE_ART){
        throw invalid_argument("Recieved an unexpected answer from server.");
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
        throw invalid_argument("Protocol error from server.");
    }
    return result;
}

bool MessageHandler::clientReadDeleteArt(string &error){
    unsigned char ans = recvCode();
    if(ans!=Protocol::ANS_DELETE_ART){
        throw invalid_argument("Recieved an unexpected answer from server.");
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
        throw invalid_argument("Protocol error from server.");
    }
    return result;
}

bool MessageHandler::clientReadGetArt(string &title, string &author, string &text, string &error){
    unsigned char ans = recvCode();
    if(ans!=Protocol::ANS_GET_ART){
        throw invalid_argument("Recieved an unexpected answer from server.");
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
        throw invalid_argument("Protocol error from server.");
    }
    return result;
}
// Low level protocol functions 
void MessageHandler::sendByte(unsigned char code) {
    conn.write(code);
}

void MessageHandler::sendCode(unsigned char code) {
    sendByte(code);
}

void MessageHandler::sendInt(int value) {
    sendByte((value >> 24) & 0xFF);
    sendByte((value >> 16) & 0xFF);
    sendByte((value >> 8) & 0xFF);
    sendByte(value & 0xFF);
}

void MessageHandler::sendIntParameter(int param) {
    sendCode(Protocol::PAR_NUM);
    sendInt(param);
}

void MessageHandler::sendStringParameter(string param) {
    sendCode(Protocol::PAR_STRING);
    sendInt(param.length());
    for (const unsigned char c : param) {
        sendByte(c);
    } 
}

unsigned char MessageHandler::recvByte() {
    unsigned char code = conn.read();
    return code;
}

unsigned char MessageHandler::recvCode() {
    unsigned char code = recvByte();
    return code;
}

int MessageHandler::recvInt() {
    int b1 = recvByte();
    int b2 = recvByte();
    int b3 = recvByte();
    int b4 = recvByte();
    return b1 << 24 | b2 << 16 | b3 << 8 | b4;
}

int MessageHandler::recvIntParameter() {
    unsigned char code = recvCode();
    if (code != Protocol::PAR_NUM) {
        // throw protocol violation error
    }
    return recvInt();
}

string MessageHandler::recvStringParameter() {
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
