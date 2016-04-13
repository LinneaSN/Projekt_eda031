#ifndef DATABASE_H
#define DATABASE_H

#include "newsgroup.h"
#include "article.h"

#include <vector>
#include <string>

class Database {
    friend class MessageHandler;

    public:
        Database();
        void createNewsgroup(Newsgroup &n);
        void deleteNewsgroup(int id, std::string name);
        void createArticle(Newsgroup &n, Article &a);
        void deleteArticle(Newsgroup &n, int id);
    private:
        std::vector<Newsgroup> newsgroups;
};


#endif
