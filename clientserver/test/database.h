#ifndef DATABASE_H
#define DATABASE_H

#include "newsgroup.h"
#include "article.h"

#include <vector>

class Database {
    friend class MessageHandler;

    public:
        Database();
        void createNewsgroup(Newsgroup &n);
        void deleteNewsgroup();
        void createArticle();
        void deleteArticle();
    private:
        std::vector<Newsgroup> newsgroups;
};


#endif
