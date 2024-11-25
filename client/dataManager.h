#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <vector>

using namespace std;

class userManager;

class dataManager {
private:
    vector<userManager> userList;

public:
    dataManager();
    ~dataManager();
    vector<userManager> getUserList();
    void addUser(const userManager& newUser);

    void printUserList();
};

#endif //DATAMANAGER_H
