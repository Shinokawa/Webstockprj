#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <string>
using namespace std;

class userManager {
private:
    string userName;
    string Password;
public:
    userManager(const string &userName, const string &password);
    ~userManager();
    string getUserName();
    string getPassword();

    void printUser() const;
    void setPassword(const string &newPassword);

    void setUsername(const string &newUsername);
};

#endif //USERMANAGER_H
