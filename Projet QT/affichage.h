#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <QWidget>
#include "database.h"

class affichage : public QWidget
{
    Q_OBJECT
public:
    explicit affichage(QWidget *parent = nullptr, Database *db = nullptr);
private:
    Database *db;
};

#endif // AFFICHAGE_H
