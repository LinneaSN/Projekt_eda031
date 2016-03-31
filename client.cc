#include <iostream>
#include "connection.h"

using namespace std;

vector<int> NGdata(3,0); //List for newsgroup, article and 
const set<string> correctCmds={"list","read","create","delete","exit"};
map<string,int> currentNewsGroup;

string parseCmd(string &input){
	istringstream s(input);
	string parse;
	int numbargs;
	while(s>>parse){
		if(parse=="list"){
			if(input.size()<5){
				clientListNG();
				currentNewsGroup=clientRead();
			} else {
				s>>parse; //get string for newsgroup
				clientListArt(currentNewsGroup[parse]);
			}
		}else if(parse=="read"){
			
		}
	}
}
	
int main(argv, argc[]){
	//Read and parse data
	//istream i;
	string input;
	string sendstr;
	istringstream i;
	while(input!="exit"){
		cin>>input;
		istringstream i(input);
		string test;
		if(correctCmds.find(i>>test)!=correctCmds.end()){
			sendstr=parseCmd(input);	
		} else {
			cout<<"Errornous input: "<<input<<endl;
		}		
	}
}
