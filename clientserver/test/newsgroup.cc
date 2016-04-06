#include <string>
#include <iostream>
#include <ctime>
#include "article.h"
#include "newsgroup.h"

using namespace std;

Newsgroup::Newsgroup(string str){
  nbr = count;
  ++count;

  name = str;
  
  created=time(NULL);
}

/*Newsgroup::~Newsgroup(){
      for (vector<Article>::iterator it = articles.begin() ; it != articles.end(); ++it){
	articles.erase(it);
      }
}*/

int Newsgroup::count=0;

string Newsgroup::getName(){
  return name;
}

int Newsgroup::getNbr(){
  return nbr;
}

string Newsgroup::getDateAndTimeCreated(){
  return ctime(&created);
}

void Newsgroup::addArticle(Article art){
  articles.push_back(art);
}

void Newsgroup::deletArticle(int i){
    for (vector<Article>::iterator it = articles.begin() ; it != articles.end(); ++it){
      if(it->getNbr() == i){
	articles.erase(it);
	break;
      }
    }
}



vector<Article>& Newsgroup::listArticles(){
  return articles;
}

/* bool operator==(const Newsgroup& lhs, const Newsgroup& rhs){
  if(lhs.getNbr() == rhs.getNbr()){
      return true;
    }

    return false;

    }*/


