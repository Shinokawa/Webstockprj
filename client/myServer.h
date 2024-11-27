#ifndef MYSERVER_H
#define MYSERVER_H

#include <QtNetwork/QTcpSocket>
#include "userManager.h"

class myServer {

public:
    userManager getUserFromServer(string usrname, string password);

};

#endif //MYSERVER_H
