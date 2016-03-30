#include "messageHandler.h"
#include "protocol.h"
#include <string>
#include <vector>

using namnspace std;

messageHandler::messageHandler(connection c) : conn(c) {}

//Server
void messageHandler::serverListNG(vector<string> NG){
	//string s;
	unsigned int nbr = NG.size();
	//s.append(to_string(ANS_LIST_NG) + ' ' + to_string(nbr) + ' ');
	sendByte(ANS_LIST_NG);
	sendInt(nbr);
	for(unsigned int i = 0; i<nbr; ++i){
	//	s.append(to_string(i) + ' ' NG[i] + ' ');
		sendIntParameter(i);
		sendStringParameter(NG[i]);
	}
	//s.append(
	sendByte(ANS_END);
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

