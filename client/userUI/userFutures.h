#ifndef USERFUTURES_H
#define USERFUTURES_H

#include <QWidget>
#include "../userManager.h"

class QLabel;

class userFutures : public QWidget{

public:
    userManager user;
    QLabel *info;

public:
    explicit userFutures(const userManager& user);
    ~userFutures() override;

};

#endif //USERFUTURES_H
