#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <ctime>
#include <vector>
#include "article.h"

class Newsgroup{
 public:
  Newsgroup( std::string str);

  std::string getName();
  int getNbr();
  std::string getDateAndTimeCreated();
  std::vector<Article>&  listArticles();
  void addArticle(Article art);
  void deletArticle(int q);

 private:
  static int count;
  std::string name; //unique
  int nbr; //unique
  time_t created;
  std::vector<Article> articles;
};

#endif
