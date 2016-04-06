#include <string>
#include "article.h"

using namespace std;

Article::Article(string tit, string aut,string text){
  nbr = count;
  ++count;
  title = tit;
  author = aut;
  articleText = text;
}
int Article::count=0;

string Article::getAuthor(){
  return author;
}

string Article::getTitle(){
  return title;
}

string Article::getText(){
  return articleText;
}

int Article::getNbr(){
  return nbr;
}
