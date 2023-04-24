#ifndef DATABASE_H
#define DATABASE_H
#include <QString>
#include <QtSql>
#include <QFile>
#include <QTableWidget>

class Database
{
public:
    Database();
    bool execut(const QString req);
    void import(const QString &path);
    bool xport(const QString &n_file, const QString &filtre = NULL);
    void re_init();

    bool supprimerProfessionnel(const QString &nom, const QString &prenom);
    bool ajouterProfessionnel(QString nom, QString prenom, QString liste_cpt);
    bool ajouterCompetenceProfessionnel(QString nom, QString prenom, QString competences);
    bool supprimerCompetenceProfessionnel(QString nom, QString prenom, QString competences);
    QString recupererDescriptionProfessionnel(QString nom,QString prenom);
    QStringList recupererCompetencesProfessionnel(QString nom, QString prenom);
    bool modifierProfessionnel(QString nom, QString prenom, QString new_nom, QString new_prenom, QString description);

    bool ajouterProfil(QString nom, QString liste_cpt);
    bool supprimerProfil(const QString &nom);
    bool ajouterCompetenceProfil(QString nom, QString competences);
    bool supprimerCompetenceProfil(QString nom, QString competences);
    QString recupererDescriptionProfil(QString nom);
    QStringList recupererCompetencesProfil(QString nom);
    bool modifierProfil(QString nom, QString new_nom, QString description);

    bool ajouterCompetence(QString nom);
    bool supprimerCompetence(QString nom);
    bool modifierCompetence(QString nom, QString new_nom);

    QStringList recupererProfessionnels();
    QStringList recupererCompetences();
    QStringList recupererProfils();

    QStringList matchProPfl(QString profil_test);

private:
    QSqlDatabase bdd;
    const QString chemin = "./test.db";
};

#endif // DATABASE_H
