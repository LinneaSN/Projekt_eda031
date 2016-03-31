#include <iostream>
#include <sstream>
#include <map>
#include <set>
//#include "connection.h"


using namespace std;

//vector<int> NGdata(3,0); //List for newsgroup, article and 
const set<string> correctCmds({"list","read","create","delete","exit"});
map<string,int> currentNewsGroup;//Stores names of newgroups and their numbers/indices

string parseCmd(string &input){
	istringstream s(input);
	string parse;
	string NG;
	int numbargs;
	while(s>>parse){
		if(parse=="list"){
			if(input.length()<5){
				cout<<"Listing NewsGroups.."<<endl;
//				clientListNG();
//				currentNewsGroup=clientRead();
			} else {
				s>>NG; //get string for article
				cout<<"Listing articles for NewsGroup :"<<NG<<endl;
//				clientListArt(currentNewsGroup[NG]);
			}
		}else if(parse=="read"){
			s>>parse;
			int article=static_cast<int>(parse[0]);
//			clientGetArt(currentNewsGroup[NG],article);	
		}else if(parse=="create"){
			s>>parse>>NG;
			if(parse=="Newsgroup"){
//				clientCreateNG(NG);
//				currentNewsGroups[NG];
			} else if(parse=="Article"){
				string title;
				string author;
				string text;
				s>>title>>author>>text;
//				clientCreateArt(currentNewsGroup[NG],title,author,text);
			}
		}
	}
}
	
int main(){
	//messageHandler MH=new messageHandler(Connection)
	//Read and parse data
	//istream i;
	string input;
	string sendstr;
	istringstream i;
	while(input!="exit"){
		getline(cin,input);
		istringstream i(input);
		string test;
		i>>test;
		cout<<input<<endl;
		if(correctCmds.find(test)!=correctCmds.end()){
			sendstr=parseCmd(input);	
		} else {
			cout<<"Errornous input: "<<input<<endl;
		}		
	}
}
