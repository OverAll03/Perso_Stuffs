#ifndef PROFIL_H
#define PROFIL_H
#include <QWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include "database.h"

class Profil : public QWidget
{
    Q_OBJECT
public:
    explicit Profil(QWidget *parent = nullptr, Database *db = nullptr);
    QWidget *creationFormulaire();
    QWidget *suppressionFormulaire();
    QWidget *modificationFormulaire();

    QTabWidget *onglets;
    Database *db;
};

#endif // PROFIL_H
