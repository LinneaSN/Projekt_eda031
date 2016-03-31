#include <iostream>
#include "connection.h"

using namespace std;

vector<int> NGdata(3,0);
const set<string> correctCmds={"list","read","create","delete","exit"};

string parseCmd(string &input){
	istringstream s(input);
	string parse;
	int numbargs;
	while(s>>parse){
		if(s=="list"){
			
		}
	}
}
	
int main(argv, argc[]){
	//Read and parse data
	//istream i;
	string input;
	string sendstr;
	while(input!="exit"){
		cin>>input;
		if(correctCmds.find(input)!=correctCmds.end()){
			sendstr=parseCmd(input);	
		} else {
			cout<<"Errornous input: "<<input<<endl;
		}		
	}
}
