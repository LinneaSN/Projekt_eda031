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
				if(NGs.empty()){
					cout<<"No newsgroups avaiable"<<endl;
				} else {	
					for(auto itr=NGs.begin();itr!=NGs.end();++itr){
						
						currentNewsGroup[(*itr).substr(3,(*itr).length())]=stoi((*itr).substr(0,1));
						cout<<*itr<<endl;
						//cout<<static_cast<int>(distance(NGs.begin(),itr))<<endl;
						//cout<<currentNewsGroup[(*itr).substr(3,(*itr).length())]<<endl;
					}
				}
			} else {
				bool ok=1;
				s>>NG; //get string for article
				cout<<"Eval count operator: "<<currentNewsGroup.count(NG)<<endl;
				if(!currentNewsGroup.count(NG)){
					cout<<"Newsgroup: "<<NG<<" does not exist!"<<endl;
					return;
				}
				handler.clientListArt(currentNewsGroup[NG]);
				vector<string> articles=handler.clientReadListArt(ok);
				if(ok && !articles.empty()){
					cout<<"Listing articles for NewsGroup: "<<NG<<endl;
					for(auto itr=articles.begin(); itr!=articles.end();++itr){
						cout<<*itr<<endl;
					}
					return;
				}
				cout<<"No articles in newsgroup "<<NG<<endl;
				return;
			}
		}else if(parse=="read"){
			if(!NG.empty()){
				s>>parse;
				cout<<"Reading article: "<<parse<<" in newsgroup: "<<NG<<endl;
				int article=stoi(parse);
				handler.clientGetArt(currentNewsGroup[NG],article);
				string title, author, text, error;
				if(!handler.clientReadGetArt(title,author,text,error)){
					cout<<"Error retriving article: "<<error<<endl;
				} else {
					cout<<title<<"  From: "<<author<<"\n"<<endl;
					cout<<text<<endl;
				}
			}else{
				client::printErrorMessage();
			}
		}else if(parse=="create"){
			s>>parse;
			if(parse=="Newsgroup"||parse=="newsgroup"){
				s>>NG;
				cout<<"Creating Newsgroup: "<<NG<<endl;
				handler.clientCreateNG(NG);
				if(handler.clientReadCreateNG()){
					currentNewsGroup[NG];
					cout<<"NewsGroup: "<<NG<<" successfully created!"<<endl;
				} else {
					cout<<"error!"<<endl;
				}				
			} else if(parse=="Article" || parse=="article"){
				if(!NG.empty()){
					string title;
					string author;
					string text;
					s>>title>>author;
					getline(s,text);
					handler.clientCreateArt(currentNewsGroup[NG],title,author,text.erase(0,1));
					if(handler.clientReadCreateArt()){
						cout<<"Creating Article: Title: "<<title<<" author: "<<author<<" text: "<<text<<endl;						
					} else {
						cout<<"error!"<<endl;
					}
				} else {
					client::printErrorMessage();
				}
			}
		}else if(parse=="delete" ){
			s>>parse;
			if(parse=="Newsgroup" || parse=="newsgroup"){
				s>>NG;
				cout<<"deleting Newsgroup: "<<NG<<"..."<<endl;
				handler.clientDeleteNG(currentNewsGroup[NG]);
				if(handler.clientReadDeleteNG()){
					cout<<"Newsgroup: "<<NG<<" removed"<<endl;
					NG="";
				} else {
					cout<<"Error: Delete unsuccessful!"<<endl;
				}
			} else if(parse=="Article" || parse=="article"){
				if(!NG.empty()){
					s>>parse;
					string error;
					int article=stoi(parse);
					handler.clientDeleteArt(currentNewsGroup[NG],article);
					if(handler.clientReadDeleteArt(error)){
						cout<<"Deleting Article: "<<article<<" in newsgroup: "<<NG<<endl;
					} else {
						cout<<error<<endl;
					}
				} else {
					client::printErrorMessage();	
				}
			} else {
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
	const string tooltip="\nCmds:\n list :Lists newsgroups\n list NewsgroupName :List articles in selected newgroup\n \n read articleNbr :Read selected article\n \n create newsgroup NewsgroupName :creates a new Newsgroup\n create article title author text :create an article in the lastly visited newsgroup\n \n delete newsgroup newgroupName :delete newsgroup\n delete article articleNbr :Delete article in lastly visited newsgroup\n\n exit :Exit program \n\n help :Display this tooltip again";
	istringstream i;	
	int port=1024;
	if(argc==2){
		try{
			port=stoi(argv[1]);
		}catch(exception &e){
			cout<<"Errornous port number, dumbass!"<<endl;
			exit(1);
		}
	}
	const char* b="127.0.0.1";
	Connection c(b,port);
	client myClient(c);
	cout<<"--NewsClient started--"<<tooltip<<endl;
	while(input!="exit"){
		getline(cin,input);
		istringstream i(input);
		string test;
		i>>test;
	//	cout<<input<<endl;
		if(correctCmds.find(test)!=correctCmds.end()){
			if(input=="help" || input=="Help"){
				cout<<tooltip<<endl;
			} else {
				myClient.parseCmd(input);
			}	
		} else {
			cout<<"Errornous input: "<<input<<endl;
		}		
	}
}
