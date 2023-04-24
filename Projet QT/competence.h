#ifndef COMPETENCE_H
#define COMPETENCE_H
#include <QWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include "database.h"

class Competence : public QWidget
{
    Q_OBJECT
public:
    explicit Competence(QWidget *parent = nullptr, Database *db = nullptr);

private:
    QWidget *creationFormulaire();
    QWidget *suppressionFormulaire();
    QWidget *modificationFormulaire();
    QTabWidget *onglets;
    Database *db;

};

#endif // COMPETENCE_H
