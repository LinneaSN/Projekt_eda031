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

Database::Database(bool useDB) : useDatabase(useDB) {
    if (useDatabase) {
        loadDatabase();
    }
}

void Database::loadDatabase() {
    vector<pair<int, string> > dirNames;
    
    dirent* newsgroupEntry;
    DIR* rootDir;
    rootDir = opendir(DB_PATH);
    if (rootDir) {
        while (true) {
            newsgroupEntry = readdir(rootDir);
            if (newsgroupEntry == NULL) break;
            string s(newsgroupEntry->d_name);
            if (s[0] != '.') {
                // Ignore current and parent directory
                size_t pos = s.find(DELIMITER);
                int id = stoi(s.substr(0, pos));
                string name = s.substr(pos+1, s.size());
                dirNames.push_back(make_pair(id, name));
            }
        }
        sort(dirNames.begin(), dirNames.end());
        closedir(rootDir);
    } else {
        cerr << "Could not open database directory." << endl;
    }

        for (auto &folderName : dirNames) {
            Newsgroup ng(folderName.second, folderName.first);
 
            vector<int> articleIDs; 
            dirent* articleEntry;
            DIR* newsgroupDir;
            string newsgroupPath(DB_PATH + to_string(folderName.first) + DELIMITER + folderName.second);
            cout << newsgroupPath << endl;
            newsgroupDir = opendir(newsgroupPath.c_str());
            if (newsgroupDir) {
                while (true) {
                    articleEntry = readdir(newsgroupDir);
                    if (articleEntry == NULL) break;
                    string str(articleEntry->d_name);
                    if (str[0] != '.') {
                        articleIDs.push_back(stoi(str));
                    }
                }
                sort(articleIDs.begin(), articleIDs.end());

                for (auto &id : articleIDs) {
                    ifstream s(newsgroupPath + "/" + to_string(id));
                    string title, author, text;

                    getline(s, title);
                    getline(s, author);

                    string line;
                    while (getline(s, line)) {
                        text += line + "\n";
                    }
                    ng.addArticle(Article(title, author, text, id));
                }
                newsgroups.push_back(ng);
                closedir(newsgroupDir);
            } else {
                cerr << "Could not open newsgroup directory." << endl;
            }
        }
}

void Database::createNewsgroup(Newsgroup &n) {
    if (!useDatabase) return;
    string dirName = DB_PATH + to_string(n.getNbr()) 
        + DELIMITER + n.getName();
    mkdir(dirName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
}

void Database::deleteNewsgroup(int id, string name) {
    if (!useDatabase) return;
    string dirName = DB_PATH + to_string(id) + DELIMITER + name;

    dirent* entry;
    DIR* dir;
    dir = opendir(dirName.c_str());
    if (dir) {
        while (true) {
            entry = readdir(dir);
            if (entry == NULL) break;
            string s(entry->d_name);
            if (s[0] != '.') {
                string path(dirName + "/" + s);
                remove(path.c_str());
            }
        }

        remove(dirName.c_str());
        closedir(dir);
    } else {
        cerr << "error3" << endl;
    }
}

void Database::createArticle(Newsgroup &n, Article &a) {
    if (!useDatabase) return;
    string filename = DB_PATH + to_string(n.getNbr()) + DELIMITER + n.getName() + 
        "/" + to_string(n.getLast().getNbr());
    
    ofstream file(filename);
    file << a.getTitle() << endl;
    file << a.getAuthor() << endl;
    file << a.getText() << endl;    
}

void Database::deleteArticle(Newsgroup &n, int id) {
    if (!useDatabase) return;
    string filename = DB_PATH + to_string(n.getNbr()) + DELIMITER + n.getName() + "/" + 
        to_string(id);
    remove(filename.c_str());
}
