#include "../headers/connection.h"
#include "../headers/client.h"
#include "../headers/messageHandlerdb.h"
#include <iostream>
#include <sstream>
#include <map>
#include <set>

using namespace std;
client::client(Connection& c): conn(c), handler(c){}

void client::printErrorMessage(){
	cout<<"Errornous command: no newsgroup selected! select a newgroup with 'list' command!"<<endl;
}

void client::listCMD(bool flagArt){
	updateMap(!flagArt);
	if(!flagArt){
		return;
	}
	if(!currentNewsGroup.count(NG)){
		cout<<"Newsgroup: "<<NG<<" does not exist!"<<endl;
		return;
	}
	handler.clientListArt(currentNewsGroup[NG]);
	vector<string> articles=handler.clientReadListArt(clearflag);
	if(clearflag && !articles.empty()){
		cout<<"Listing articles for NewsGroup: "<<NG<<endl;
		for(auto itr=articles.begin(); itr!=articles.end();++itr){
			cout<<*itr<<endl;
		}
		return;
	}
	cout<<"No articles in newsgroup "<<NG<<endl;
	return;
}

void client::readCMD(istringstream &ss){
	string title, author, text, error;
	int article;
	ss>>article;
	handler.clientGetArt(currentNewsGroup[NG],article);
	if(!handler.clientReadGetArt(title,author,text,error)){
		cout<<"Error retriving article: "<<error<<endl;
	} else {
        cout<<"Reading article: "<<article<<" in newsgroup: "<<NG<<endl;
		cout<<title<<"  From: "<<author<<"\n"<<endl;
		cout<<text<<endl;
	}
}

bool client::createCMD(istringstream &ss, string& parse){
	if(parse=="Newsgroup"||parse=="newsgroup"){
		ss>>NG;
        if(!currentNewsGroup.count(NG)){         
            handler.clientCreateNG(NG);
            if(handler.clientReadCreateNG()){
                updateMap(0);
                cout<<"NewsGroup: "<<NG<<" successfully created!"<<endl;
                return true;
            } else {
                cout<<"error! NewsgroupName must be unique"<<endl;
            }
        } else {
             cout<<"error! NewsgroupName must be unique"<<endl;
        }				
	}
	if(parse=="Article" || parse=="article"){
		if(!NG.empty()){
			string title, author, text;
			ss>>title>>author;
			getline(ss,text);
			handler.clientCreateArt(currentNewsGroup[NG],title,author,text.erase(0,1));
			if(handler.clientReadCreateArt()){
				cout<<"Creating Article in newsgroup:"<<NG<<" ::\n Title: "<<title<<" author: "<<author<<" text: "<<text<<endl;
				return true;						
			} else {
				cout<<"Error! Could not find NewsgroupName!"<<endl;
			}
		} else {
			printErrorMessage();
		}
	}
	return false;
}

bool client::deleteCMD(istringstream& s, string& parse){
	if(parse=="Newsgroup" || parse=="newsgroup"){
		s>>NG;
        if(currentNewsGroup.count(NG)){
		    handler.clientDeleteNG(currentNewsGroup[NG]);
		    if(handler.clientReadDeleteNG()){
			    cout<<"Newsgroup: "<<NG<<" removed"<<endl;
			    auto it=currentNewsGroup.find(NG);
			    currentNewsGroup.erase(it);
			    NG="";
		    } else {
			    cout<<"Error: Newsgroup does not exist!"<<endl;
		    }
        } else {
            cout<<"Error: Newsgroup does not exist!"<<endl;
        }
		return true;
	} else if(parse=="Article" || parse=="article"){
		if(!NG.empty()){
			int article;
			s>>article;
			if(s.fail()){
				return false;
			}
			string error;
			//int article=stoi(parse);
			handler.clientDeleteArt(currentNewsGroup[NG],article);
			if(handler.clientReadDeleteArt(error)){
				cout<<"Deleting Article: "<<article<<" in newsgroup: "<<NG<<endl;
			} else {
				cout<<error<<endl;
			}
		} else {
			client::printErrorMessage();	
		}
		return true;
	}
	return false;
}

void client::updateMap(bool printFlag){
	if(printFlag){
		cout<<"Listing NewsGroups.."<<endl;
	}
	handler.clientListNG();
	vector<string> NGs=handler.clientReadListNG();
	if(NGs.empty()){
		if(printFlag){
			cout<<"No newsgroups avaiable"<<endl;
		}
	} else {	
		for(auto itr=NGs.begin();itr!=NGs.end();++itr){
			currentNewsGroup[(*itr).substr((*itr).find(".")+2,(*itr).length())]=stoi((*itr).substr(0,(*itr).find(".")));	
			if(printFlag){
				cout<<*itr<<endl;
			}
		}
	}
}

void client::parseCmd(string &input){
	istringstream s(input);
	string parse;
	while(s>>parse){		
		if(parse=="list"){
			listCMD(s>>NG);
		}else if(parse=="read"){
			if(!NG.empty()){
				readCMD(s);
				return;
			}
			printErrorMessage();
		}else if(parse=="create"){
			s>>parse;
			if(!createCMD(s,parse)){
				cout<<"Errornous syntax: "<<input<<endl;
			}
		}else if(parse=="delete" ){
			s>>parse;
			if(!deleteCMD(s,parse)){
				cout<<"Errornous syntax: "<<input<<endl;	
			}
		}
	}
}


	
int main(int argc, char* argv[]){
	//messageHandler MH=new messageHandler(Connection)
	//Read and parse data
	//istream i;
	const set<string> correctCmds({"list","read","create","delete","exit","help"});
	string input;
	string sendstr;
	const string tooltip="\nCmds:\n list :Lists newsgroups.\n list NewsgroupName :List articles in the selected newgroup.\n \n read articleNbr :Read the selected article.\n \n create newsgroup NewsgroupName :creates a new Newsgroup. NewsgroupName may not contain any spaces.\n create article title author text :create an article in the lastly visited newsgroup. Title and author may not contain any spaces.\n \n delete newsgroup newgroupName :delete newsgroup.\n delete article articleNbr :Delete article in lastly visited newsgroup.\n\n exit :Exit program. \n\n help :Display this tooltip again.";
	istringstream i;	
	int port=1024;
	if(argc==2){
		try{
			port=stoi(argv[1]);
		}catch(exception &e){
			cout<<"Errornous port number"<<endl;
			exit(1);
		}
	}
	const char* b="127.0.0.1";
	Connection c(b,port);
	client myClient(c);
	cout<<"--NewsClient started--"<<tooltip<<endl;
	while(input!="exit"){
		if(c.isConnected()){
			getline(cin,input);
			istringstream i(input);
			string test;
			i>>test;
			if(correctCmds.find(test)!=correctCmds.end()){
				if(input=="help" || input=="Help"){
					cout<<tooltip<<endl;
				} else {
					try{
						myClient.parseCmd(input);
					}catch(exception &e){
						cout<<"Communication error with server!: "<<e.what()<<endl;
					}
				}	
			} else {
				cout<<"Errornous input: "<<input<<endl;
			}
		} else {
			input="exit";
			cout<<"Error: Not able to connect to server. Terminating client!"<<endl;
		}		
	}
}
