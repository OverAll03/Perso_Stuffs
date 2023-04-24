#ifndef PROFESSIONNEL_H
#define PROFESSIONNEL_H
#include <QWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include "database.h"

class Professionnel : public QWidget
{
    Q_OBJECT

public:
    explicit Professionnel(QWidget *parent = nullptr, Database *db = nullptr);

private:
    QWidget *creationFormulaire();
    QWidget *suppressionFormulaire();
    QWidget *modificationFormulaire();

    QTabWidget *onglets;
    Database *db;
};

#endif // PROFESSIONNEL_H
