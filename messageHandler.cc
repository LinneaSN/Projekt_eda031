#include "messageHandler.h"
#include "protocol.h"
#include <string>
#include <vector>

using namnspace std;

//Server
string messageHandler::serverListNG(vector<string> NG){
	string s;
	unsigned int nbr = NG.size();
	s.append(to_string(ANS_LIST_NG) + ' ' + to_string(nbr) + ' ');
	for(unsigned int i = 0; i<nbr; ++i){
		s.append(to_string(i) + ' ' NG[i] + ' ');
	}
	return s.append(
}



//Client
string messageHandler::clientListNG(){
	string s;
	return s.append(to_string(COM_LIST_NG) + ' ' + to_string(COM_END));
}

string messageHandler::clientCreateNG(string NGname){
	NGname.insert(0,to_string(COM_CREATE_NG) + ' ');
	return NGname.append(' ' + to_string(COM_END));
}

string messageHandler::clientDeleteNG(int newsgroup){
	string s;
	return s.append(to_string(COM_DELETE_NG) + ' ' + to_string(newsgroup) + ' ' + to_string(COM_END));
}

string messageHandler::clientListArt(int newsgroup){
	string s;
	return s.append(to_string(COM_LIST_ART) + ' ' + to_string(newsgroup) + ' ' + to_string(COM_END));
}

string messageHandler::clienCreateArt(int newsgroup, string &title, string &author, string &text){
	string s;
	s.append(to_string(COM_CREATE_ART) + ' ' + to_string(newsgroup) + ' ');
	return s.append(title + ' ' + author + ' ' + text + ' ' + to_string(COM_END));
}

string messageHandler::clientDeleteArt(int newsgroup, int article) {
	string s;
	return s.append(to_string(COM_DELETE_ART) + ' ' + to_string(newsgroup) + ' ' + to_string(article) + ' ' + to_string(COM_END));
}

string mesageHandler::clientGetArt(int newsgroup, int article) {
	string s;
	return s.append(to_string(COM_GET_ART) + ' ' + to_string(newsgroup) + ' ' + to_string(article) + ' ' + to_string(COM_END));
}

