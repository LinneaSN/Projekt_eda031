#include <string>
#include <iostream>
#include <ctime>
#include "article.h"
#include "newsgroup.h"
#include "ERR_ART_DOES_NOT_EXIST.h"

using namespace std;

Newsgroup::Newsgroup(string str, int n){
  nbr = n;
  count = n+1;

  name = str;
  
  created=time(NULL);
}

Newsgroup::Newsgroup(string str){
  nbr = count;
  ++count;

  name = str;
  
  created=time(NULL);
}

/*Newsgroup::~Newsgroup(){
 --count;
}*/

int Newsgroup::count=1;

string Newsgroup::getName(){
  return name;
}

int Newsgroup::getNbr(){
  return nbr;
}

string Newsgroup::getDateAndTimeCreated(){
  return ctime(&created);
}

Article Newsgroup::getArticle(int i){
  for(vector<Article>::iterator it = articles.begin(); it != articles.end();++it){
    if(it->getNbr() == i){
      return *it;
    }
  }
  throw ERR_ART_DOES_NOT_EXIST();
}

void Newsgroup::addArticle(Article art){
  articles.push_back(art);
}

void Newsgroup::deleteArticle(int i){
    for (vector<Article>::iterator it = articles.begin() ; it != articles.end(); ++it){
      if(it->getNbr() == i){
	articles.erase(it);
	return;
      }
    }
    throw ERR_ART_DOES_NOT_EXIST();
}



vector<Article>& Newsgroup::listArticles(){
  return articles;
}


