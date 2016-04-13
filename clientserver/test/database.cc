#include "database.h"

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>


#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

Database::Database() {
    vector<pair<int, string> > dirNames;
    dirent* de;
    DIR* dp;
    dp = opendir("./database");
    if (dp) {
        while (true) {
            de = readdir(dp);
            if (de == NULL) break;
            string s(de->d_name);
            if (s[0] != '.') {
                size_t pos = s.find('-');
                int id = stoi(s.substr(0, pos));
                string name = s.substr(pos+1, s.size());
                dirNames.push_back(make_pair(id, name));
            }
        }
        sort(dirNames.begin(), dirNames.end());
        
        for (auto &s : dirNames) {
            Newsgroup ng(s.second, s.first);
 
            vector<int> artNames; 
            dirent* entry;
            DIR* dir;
            string article("./database/" + to_string(s.first) + "-" + s.second);
            cout << article << endl;
            dir = opendir(article.c_str());
            if (dir) {
                while (true) {
                    entry = readdir(dir);
                    if (entry == NULL) break;
                    string str(entry->d_name);
                    if (str[0] != '.') {
                        artNames.push_back(stoi(str));
                    }
                }
                sort(artNames.begin(), artNames.end());

                for (auto &art : artNames) {
                    ifstream s(article + "/" + to_string(art));
                    string title;
                    string author;
                    string text;

                    getline(s, title);
                    getline(s, author);

                    string line;
                    while (getline(s, line)) {
                        text += line + "\n";
                    }
                    ng.addArticle(Article(title, author, text, art));
                }
                newsgroups.push_back(ng);
                closedir(dir);
            } else {
                cerr << "error" << endl;
            }
        }
        closedir(dp);
    } else {
        cerr << "error" << endl;
    }
}

void Database::createNewsgroup(Newsgroup &n) {
    string dirName = "./database/" + to_string(n.getNbr()) 
        + "-" + n.getName();
    mkdir(dirName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
}

void Database::deleteNewsgroup() {}

void Database::createArticle() {}

void Database::deleteArticle() {}
