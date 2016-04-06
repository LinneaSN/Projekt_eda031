#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <ctime>
#include <vector>
#include "article.h"

class Newsgroup{
 public:
  Newsgroup(std::string str);
  // ~Newsgroup();
  std::string getName();
  int getNbr();
  std::string getDateAndTimeCreated();
  std::vector<Article>&  listArticles();
  Article getArticle(int i);
  void addArticle(Article art);
  void deleteArticle(int q);

  //friend bool operator==(const Newsgroup&, const Newsgroup&);


 private:
  static int count;
  std::string name; //unique
  int nbr; //unique
  time_t created;
  std::vector<Article> articles;
};

#endif
