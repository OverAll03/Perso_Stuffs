#ifndef GESTION_H
#define GESTION_H

#include <QMainWindow>
#include <QLineEdit>
#include <QStackedWidget>
#include <QPushButton>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Gestion; }
QT_END_NAMESPACE

class Gestion : public QMainWindow
{
    Q_OBJECT

public:
    Gestion(QWidget *parent = nullptr);
    void warn_reset();
    void load_file(Database *bdd);
    void warn_import();
    void warn_xport();
    void continue_to_newpage();
    ~Gestion();

private:
    Ui::Gestion *ui;
    Database *base;
    QStackedWidget *stack;
    QWidget *mainpage;
    QWidget *file_page;
};
#endif // GESTION_H
