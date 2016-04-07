#include "./clientserver/connection.h"
#include "client.h"
#include "messageHandler.h"
#include <iostream>
#include <sstream>
#include <map>
#include <set>

using namespace std;

//vector<int> NGdata(3,0); //List for newsgroup, article and 
//map<string,int> currentNewsGroup;//Stores names of newsgroups and their numbers/indices
//string NG;
//Connection c;
//messageHandler handler(c);


client::client(Connection& c): conn(c), handler(c){}

void client::printErrorMessage(){
	cout<<"errornous command: no newsgroup selected! select a newgroup with 'list' command!"<<endl;
}

void client::parseCmd(string &input){
	istringstream s(input);
	string parse;
	int numbargs;
	while(s>>parse){
		if(parse=="list"){
			if(input.length()<5){
				cout<<"Listing NewsGroups.."<<endl;
				handler.clientListNG();
				vector<string> NGs=handler.clientReadListNG();
					for(auto itr=NGs.begin();itr!=NGs.end();++itr){
						currentNewsGroup[*itr]=static_cast<int>(distance(NGs.begin(),itr));
					}
			} else {
				bool ok=1;
				s>>NG; //get string for article
				cout<<"Listing articles for NewsGroup :"<<NG<<endl;
				handler.clientListArt(client::currentNewsGroup[NG]);
				vector<string> articles=handler.clientReadListArt(ok);
				if(ok){
					for(auto itr=articles.begin(); itr!=articles.end();++itr){
						cout<<static_cast<int>(distance(articles.begin(),itr))<<" "<<*itr<<endl;
					}
				}
			}
		}else if(parse=="read"){
			if(!NG.empty()){
				s>>parse;
				cout<<"Reading article: "<<parse<<"in newsgroup:"<<NG<<endl;
				int article=static_cast<int>(parse[0]);
				handler.clientGetArt(currentNewsGroup[NG],article);
			}else{
				client::printErrorMessage();
			}
		}else if(parse=="create"){
			s>>parse;
			if(parse=="Newsgroup"||parse=="newsgroup"){
				s>>NG;
				cout<<"Creating Newsgroup: "<<NG<<endl;
//				clientCreateNG(NG);
//				currentNewsGroups[NG];
			} else if(parse=="Article" || parse=="article"){
				if(!NG.empty()){
					string title;
					string author;
					string text;
					s>>title>>author>>text;
	//				clientCreateArt(currentNewsGroup[NG],title,author,text);
					cout<<"Creating Article: Title: "<<title<<" author: "<<author<<" text: "<<text<<endl;	
				} else {
					client::printErrorMessage();
				}
			}
		}else if(parse=="delete" ){
			s>>parse;
			if(parse=="Newsgroup" || parse=="newsgroup"){
				s>>NG;
				cout<<"deleting Newsgroup: "<<parse<<endl;
				//clientDeleteNG(currentNewsGroup[NG]);
			} else if(parse=="Article" || parse=="article"){
				if(!NG.empty()){
					s>>parse;
					int article=static_cast<int>(parse[0]);
					//clientDeleteArt(currentNewsGroup[NG],article);
					cout<<"Deleting Article: "<<article<<" in newsgroup: "<<NG<<endl;
				} else {
					client::printErrorMessage();	
				}
			}
		}
	}
}


	
int main(){
	//messageHandler MH=new messageHandler(Connection)
	//Read and parse data
	//istream i;
	const set<string> correctCmds({"list","read","create","delete","exit"});
	string input;
	string sendstr;
	istringstream i;
	const char* b="127.0.0.1";
	Connection c(b,2);
	client myClient(c);
	cout<<"--NewsClient started--\nCmds:\n list :Lists newsgroups\n list NewsgroupName :List articles in selected newgroup\n read articleNbr :Read selected article\n create NewsgroupName :creates a new Newsgroup\n "<<endl;
	while(input!="exit"){
		getline(cin,input);
		istringstream i(input);
		string test;
		i>>test;
	//	cout<<input<<endl;
		if(correctCmds.find(test)!=correctCmds.end()){
			myClient.parseCmd(input);	
		} else {
			cout<<"Errornous input: "<<input<<endl;
		}		
	}
}
