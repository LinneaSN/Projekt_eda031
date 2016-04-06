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


void Newsgroup::listArticles(){
  for (vector<Article>::iterator it = articles.begin() ; it != articles.end(); ++it){
    cout << it-> getTitle() << ' ' << it->getAuthor() << "\n" << it->getText() << endl;;
  }

}
