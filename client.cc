#include <iostream>
#include <sstream>
#include <map>
#include <set>
//#include "connection.h"


using namespace std;

//vector<int> NGdata(3,0); //List for newsgroup, article and 
const set<string> correctCmds({"list","read","create","delete","exit"});
map<string,int> currentNewsGroup;//Stores names of newgroups and their numbers/indices
string NG;

void printErrorMessage(){
	cout<<"errornous command: no newsgroup selected! select a newgroup with 'list' command!"<<endl;
}

void parseCmd(string &input){
	istringstream s(input);
	string parse;
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
			if(!NG.empty()){
				s>>parse;
				cout<<"Reading article: "<<parse<<"in newsgroup:"<<NG<<endl;
				int article=static_cast<int>(parse[0]);
	//			clientGetArt(currentNewsGroup[NG],article);
			}else{
				printErrorMessage();
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
					printErrorMessage();
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
					printErrorMessage();	
				}
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
	//	cout<<input<<endl;
		if(correctCmds.find(test)!=correctCmds.end()){
			parseCmd(input);	
		} else {
			cout<<"Errornous input: "<<input<<endl;
		}		
	}
}
