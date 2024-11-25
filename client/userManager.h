#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <string>
using namespace std;

class userManager {
private:
    string userName;
    string Password;
public:
    userManager(string userName,string password);
    ~userManager();
    string getUserName();
    string getPassword();

    void printUser();
};

#endif //USERMANAGER_H
