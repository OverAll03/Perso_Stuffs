#include "gestion.h"
#include "database.h"
#include <QApplication>
#include <QtSql/QSql>
#include <QDesktopWidget>
#include <QLineEdit>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gestion w;
    w.setWindowTitle("Skill Managing App");
    QRect screenGeometry = QApplication::desktop()->screenGeometry();

    w.setWindowIcon(QIcon(":/icons/skill-manage.png"));
    w.move(screenGeometry.topLeft());
    w.show();
    return a.exec();
}
