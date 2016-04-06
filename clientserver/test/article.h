#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>
#include <ctime>

class Article{
 public:
  Article(std::string tit, std::string aut, std::string text);

  std::string getAuthor();
  int getNbr();
  std::string getTitle();
  std::string getText();
  

 private:
  static int count;
  std::string title; 
  int nbr; //unique
  std::string articleText;
  std::string author;
};

#endif
