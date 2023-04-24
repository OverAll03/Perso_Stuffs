#include "database.h"
#include <QString>
#include <QtSql>
#include <QFile>
#include <QTextStream>
#include <QTableWidget>
#include <QFileDialog>
#include "functions.h"

Database::Database()
{
    bdd=QSqlDatabase::addDatabase("QSQLITE");
    bdd.setDatabaseName(chemin);
    if (!bdd.open())
        qDebug()<<"Your DB is not opened!!!";
    else
        qDebug()<<"No Error while opening the DB";

    QSqlQuery *query =new QSqlQuery(bdd);
    if (!execut("CREATE TABLE IF NOT EXISTS PRO (id INTEGER, nom_pro VARCHAR(50) NOT NULL, prenom VARCHAR(100) NOT NULL, nbre_cpt INT, liste_cpt TEXT DEFAULT 'Respirer', description TEXT DEFAULT 'Aucune',PRIMARY KEY(id AUTOINCREMENT))"))
        qDebug()<<"Error creating table PRO: "<<query->lastError().text();
    if (!execut("CREATE TABLE IF NOT EXISTS PFL (id INTEGER PRIMARY KEY AUTOINCREMENT, nom_pfl VARCHAR(50) NOT NULL, nbre_cpt INT, liste_cpt TEXT DEFAULT 'Respirer', description TEXT DEFAULT 'Aucune')"))
        qDebug()<<"Error creating table PFL: "<<query->lastError().text();
    if (!execut("CREATE TABLE IF NOT EXISTS CPT (id INTEGER PRIMARY KEY AUTOINCREMENT, nom VARCHAR(50));"))
        qDebug()<<"Error creating table CPT: "<<query->lastError().text();

}

bool Database::execut(const QString req)
{
    QSqlQuery *query =new QSqlQuery(bdd);
    query->prepare(req);
    if (!query->exec(req))
    {
        qDebug()<<"Query execution failed: "<<query->lastError().text();
        return false;
    }
    else
    {
        qDebug()<<"Query executed successfully";
        return true;
    }
}

void Database::import(const QString &path)
{
    QFile file(path);
    QSqlQuery *query =new QSqlQuery(bdd);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not open file for reading.";
        return;
    }
    QStringList liste;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << line;
        if (line == "PRO:"){
            QString nom_pro = in.readLine();
            nom_pro.remove("\t");
            QString prenom = in.readLine();
            prenom.remove("\t");
            QString liste_cpt="Marcher";
            int nbre_cpt = ((in.readLine()).toInt());
            for (int j=0; j<nbre_cpt; j++){
                liste_cpt=liste_cpt+";"+(in.readLine());
            }
            liste_cpt.remove("\t");
            liste.append(liste_cpt.split(";"));
            nbre_cpt = liste_cpt.split(";").length();
            query->prepare("INSERT INTO PRO (nom_pro, prenom, nbre_cpt, liste_cpt) VALUES (:1, :2, :3, :4)");
            query->bindValue(":1", nom_pro);
            query->bindValue(":2", prenom);
            query->bindValue(":3", nbre_cpt);
            query->bindValue(":4", liste_cpt);
            if (!query->exec()){
                qDebug() << "Insertion non Valide" << query->lastError();
            }
        }

        if (line == "PFL:"){
            QString nom_pfl = in.readLine();
            nom_pfl.remove("\t");
            QString liste_cpt="Marcher";
            int nbre_cpt = ((in.readLine()).toInt());
            for (int j=0; j<nbre_cpt; j++){
                liste_cpt=liste_cpt+";"+(in.readLine());
            }
            liste_cpt.remove("\t");
            liste.append(liste_cpt.split(";"));
            nbre_cpt = liste_cpt.split(";").length();
            query->prepare("INSERT INTO PFL (nom_pfl, nbre_cpt, liste_cpt) VALUES (:1, :2, :3)");
            query->bindValue(":1", nom_pfl);
            query->bindValue(":2", nbre_cpt);
            query->bindValue(":3", liste_cpt);
            if (!query->exec()){
                qDebug() << "Insertion non Valide" << query->lastError();
            }
        }

        if (line == "CPT:"){
            QString liste_cpt;
            int nbre_cpt = ((in.readLine()).toInt());
            for (int j=0; j<nbre_cpt; j++){
                liste_cpt=liste_cpt+";"+(in.readLine());
            }
            liste_cpt.remove("\t");
            liste.append(liste_cpt.split(";"));
            liste.sort();
            liste.removeDuplicates();
            liste.removeAll("");
            for (int i=0; i<liste.length(); i++)
            {
                query->prepare("INSERT INTO CPT (nom) VALUES (:1)");
                query->bindValue(":1", liste[i]);
                if (!query->exec()){
                    qDebug() << "Insertion non Valide" << query->lastError();
                }
            }
        }
    }
}

bool Database::xport(const QString &n_file, const QString &filtre)
{
    QFile file(n_file);
    QSqlQuery *query = new QSqlQuery(bdd);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Impossible d'ouvrir le fichier de sortie";
        return false;
    }
    QTextStream out(&file);
    if (filtre==NULL){
        query->exec("SELECT * FROM PRO;");
        while (query->next()) {
            out << "PRO:" << "\n"
                << "\t" << query->value(1).toString() << "\n"
                << "\t" << query->value(2).toString() << "\n"
                << "\t" << query->value(3).toString() << "\n";
            QString s = query->value(4).toString();
            QStringList s_list = s.split(";");
            for (int i=0; i<s_list.length(); i++) {
                out << "\t" << s_list[i] << "\n";
            }
        }
        query->exec("SELECT * FROM PFL;");
        while (query->next()) {
            out << "PFL:" << "\n"
                << "\t" << query->value(1).toString() << "\n"
                << "\t" << query->value(2).toString() << "\n";
            QString s = query->value(3).toString();
            QStringList s_list = s.split(";");
            for (int i=0; i<s_list.length(); i++) {
                out << "\t" << s_list[i] << "\n";
            }
        }
        query->exec("SELECT * FROM CPT");
        out << "CPT:" << "\n";
        while (query->next()) {
             out << "\t" << query->value(1).toString() << "\n";
        }
    }
    else {
        if(filtre.contains("PRO"))
        {
            query->exec("SELECT * FROM PRO;");
            while (query->next()) {
                out << "PRO:" << "\n"
                    << "\t" << query->value(1).toString() << "\n"
                    << "\t" << query->value(2).toString() << "\n"
                    << "\t" << query->value(3).toString() << "\n";
                QString s = query->value(4).toString();
                QStringList s_list = s.split(";");
                for (int i=0; i<s_list.length(); i++) {
                    out << "\t" << s_list[i] << "\n";
                }
            }
        }
        if(filtre.contains("PFL"))
        {
            query->exec("SELECT * FROM PFL;");
            while (query->next()) {
                out << "PFL:" << "\n"
                    << "\t" << query->value(1).toString() << "\n"
                    << "\t" << query->value(2).toString() << "\n";
                QString s = query->value(3).toString();
                QStringList s_list = s.split(";");
                for (int i=0; i<s_list.length(); i++) {
                    out << "\t" << s_list[i] << "\n";
                }
            }
        }
        if(filtre.contains("CPT"))
        {
            query->exec("SELECT * FROM CPT");
            out << "CPT:" << "\n";
            while (query->next()) {
                 out << "\t" << query->value(1).toString() << "\n";
            }
        }
    }
    return true;
}

void Database::re_init()
{
    execut("DELETE FROM PRO;");
    execut("DELETE FROM PFL;");
    execut("DELETE FROM CPT;");
}

QStringList Database::recupererProfessionnels()
{
    QStringList professionnelsList;
    QSqlQuery *query = new QSqlQuery();
    query->prepare("SELECT nom_pro, prenom FROM PRO;");
    query->exec();
    while (query->next()) {
        QString nom = query->value(0).toString();
        QString prenom = query->value(1).toString();
        nom.remove("\t");
        prenom.remove("\t");
        QString professionnel = nom + " " + prenom;
        professionnelsList.append(professionnel);
    }
    return professionnelsList;
}

QStringList Database::recupererCompetences()
{
    QStringList competences;
    QSqlQuery query;
    query.prepare("SELECT nom FROM CPT ORDER BY nom");
    if (query.exec()) {
        while (query.next()) {
            competences << query.value(0).toString();
        }
    }

    return competences;
}

QStringList Database::recupererProfils()
{
    QStringList profilsList;
    QSqlQuery *query = new QSqlQuery();
    query->prepare("SELECT nom_pfl FROM PFL;");
    query->exec();
    while (query->next()) {
        QString nom = query->value(0).toString();
        nom.remove("\t");
        QString profil = nom ;
        profilsList.append(profil);
    }
    return profilsList;
}

//Fonctions sur la table PRO
bool Database::supprimerProfessionnel(const QString &nom, const QString &prenom)
{
    QString req = "DELETE FROM PRO WHERE nom_pro=:nom AND prenom=:prenom";
    QSqlQuery query;
    query.prepare(req);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    if (!query.exec())
    {
        return false;
    }
    return true;
}

bool Database::ajouterProfessionnel(QString nom, QString prenom, QString liste_cpt)
{
    QSqlQuery query;
    liste_cpt = "Marcher;"+liste_cpt;
    int nbre_cpt = liste_cpt.split(";").length();
    query.prepare("INSERT INTO PRO (nom_pro, prenom, nbre_cpt, liste_cpt) VALUES (:nom, :prenom, :nbre, :liste)");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":nbre", nbre_cpt);
    query.bindValue(":liste", liste_cpt);
    if (query.exec()) {
        return true;
    }
    else {
        qDebug() << "Erreur lors de l'ajout du professionnel :" << query.lastError().text();
        return false;
    }
}

bool Database::ajouterCompetenceProfessionnel(QString nom, QString prenom, QString competences)
{
    QString listeCptPrecedente;
    int n_pre;
    QSqlQuery query;
    query.prepare("SELECT nbre_cpt, liste_cpt FROM PRO WHERE nom_pro = ? AND prenom = ?");
    query.bindValue(0, nom);
    query.bindValue(1, prenom);
    if (query.exec() && query.next()) {
        n_pre = query.value(0).toInt();
        listeCptPrecedente = query.value(1).toString();
    } else {
        qDebug()<< "Erreur lors de la récupération de la valeur de la colonne liste_cpt";
        return false;
    }

    // Construction de la nouvelle valeur de la colonne "liste_cpt"
    QString nouvelleListeCpt = listeCptPrecedente+";"+competences;
    int nbre = nouvelleListeCpt.split(";").length();
    // Mise à jour de la valeur de la colonne "liste_cpt"
    query.prepare("UPDATE PRO SET nbre_cpt = ?, liste_cpt = ? WHERE nom_pro = ? AND prenom = ?");
    query.bindValue(0, nbre);
    query.bindValue(1, nouvelleListeCpt);
    query.bindValue(2, nom);
    query.bindValue(3, prenom);

    if (query.exec()) {
        qDebug() << "La mise à jour a réussi";
        return true;
    } else {
        qDebug() << "La mise à jour a échoué";
        return false;
    }
}

bool Database::supprimerCompetenceProfessionnel(QString nom, QString prenom, QString competences)
{
    QString listeCptPrecedente;
        int n_pre;
        QSqlQuery query;
        query.prepare("SELECT nbre_cpt,liste_cpt FROM PRO WHERE nom_pro = ? AND prenom = ?");
        query.bindValue(0, nom);
        query.bindValue(1, prenom);
        if (query.exec() && query.next()) {
            listeCptPrecedente = query.value(1).toString();
            n_pre = query.value(0).toInt();
        } else {
            // Erreur lors de la récupération de la valeur de la colonne "liste_cpt"
            return false;
        }

        // Suppression de la compétence de la liste
        QStringList listeCpt = listeCptPrecedente.split(";");
        listeCpt.removeAll(competences);
        QString nouvelleListeCpt = listeCpt.join(";");

        // Mise à jour de la valeur de la colonne "liste_cpt"
        int nbre = listeCpt.length();
        query.prepare("UPDATE PRO SET nbre_cpt = ?, liste_cpt = ? WHERE nom_pro = ? AND prenom = ?");
        query.bindValue(0, nbre);
        query.bindValue(1, nouvelleListeCpt);
        query.bindValue(2, nom);
        query.bindValue(3, prenom);

        if (query.exec()) {
            qDebug() << "La mise à jour a réussi";
            return true;
        } else {
            qDebug() << "La mise à jour a échoué";
            return false;
        }
}

QString Database::recupererDescriptionProfessionnel(QString nom, QString prenom)
{
    QSqlQuery query;
    query.prepare("SELECT description FROM PRO WHERE nom_pro = ? AND prenom = ?");
    query.bindValue(0, nom);
    query.bindValue(1, prenom);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        // Erreur lors de la récupération de la description
        return "";
    }
}

bool Database::modifierProfessionnel(QString nom, QString prenom, QString new_nom, QString new_prenom, QString description)
{
    QSqlQuery query;
    query.prepare("UPDATE PRO SET nom_pro = ?, prenom = ?, description = ? WHERE nom_pro = ? AND prenom = ?");
    query.addBindValue(new_nom);
    query.addBindValue(new_prenom);
    query.addBindValue(description);
    query.addBindValue(nom);
    query.addBindValue(prenom);
    if (query.exec()) {
        qDebug() << "La mise à jour a réussi";
        return true;
    } else {
        qDebug() << "La mise à jour a échoué";
        return false;
    }
}

QStringList Database::recupererCompetencesProfessionnel(QString nom, QString prenom)
{
    QStringList competences;
    QSqlQuery query;
    query.prepare("SELECT liste_cpt FROM PRO WHERE nom_pro = ? AND prenom = ?");
    query.bindValue(0, nom);
    query.bindValue(1, prenom);
    if (query.exec() && query.next()) {
        QString listeCpt = query.value(0).toString();
        competences = listeCpt.split(";");
    } else {
        // Erreur lors de la récupération de la valeur de la colonne "liste_cpt"
        qWarning() << "Impossible de récupérer les compétences du professionnel" << nom << prenom;
    }
    return competences;
}

//Fonctions sur la table PFL
bool Database::ajouterProfil(QString nom, QString liste_cpt)
{
    QSqlQuery query;
    liste_cpt = "Marcher;" + liste_cpt;
    int nbre_cpt = liste_cpt.split(";").length();
    query.prepare("INSERT INTO PFL (nom_pfl, nbre_cpt, liste_cpt) VALUES (:nom, :prenom, :nbre, :liste)");
    query.bindValue(":nom", nom);
    query.bindValue(":nbre", nbre_cpt);
    query.bindValue(":liste", liste_cpt);
    if (query.exec()) {
        return true;
    }
    else {
        qDebug() << "Erreur lors de l'ajout du profil :" << query.lastError().text();
        return false;
    }
}

bool Database::supprimerProfil(const QString &nom)
{
    QString req = "DELETE FROM PFL WHERE nom_pfl=:nom";
    QSqlQuery query;
    query.prepare(req);
    query.bindValue(":nom", nom);
    if (!query.exec())
    {
        return false;
    }
    return true;
}

bool Database::ajouterCompetenceProfil(QString nom, QString competences)
{
    QString listeCptPrecedente;
    int n_pre;
    QSqlQuery query;
    query.prepare("SELECT nbre_cpt, liste_cpt FROM PFL WHERE nom_pfl = ?");
    query.bindValue(0, nom);
    if (query.exec() && query.next()) {
        n_pre = query.value(0).toInt();
        listeCptPrecedente = query.value(1).toString();
    } else {
        qDebug()<< "Erreur lors de la récupération de la valeur de la colonne liste_cpt";
        return false;
    }

    // Construction de la nouvelle valeur de la colonne "liste_cpt"
    QString nouvelleListeCpt = listeCptPrecedente+";"+competences;
    int nbre = nouvelleListeCpt.split(";").length();
    // Mise à jour de la valeur de la colonne "liste_cpt"
    query.prepare("UPDATE PFL SET nbre_cpt = ?, liste_cpt = ? WHERE nom_pfl = ?");
    query.bindValue(0, nbre);
    query.bindValue(1, nouvelleListeCpt);
    query.bindValue(2, nom);

    if (query.exec()) {
        qDebug() << "La mise à jour a réussi";
        return true;
    } else {
        qDebug() << "La mise à jour a échoué";
        return false;
    }
}

bool Database::supprimerCompetenceProfil(QString nom, QString competences)
{
    QString listeCptPrecedente;
        int n_pre;
        QSqlQuery query;
        query.prepare("SELECT nbre_cpt,liste_cpt FROM PFL WHERE nom_pfl = ?");
        query.bindValue(0, nom);
        if (query.exec() && query.next()) {
            listeCptPrecedente = query.value(1).toString();
            n_pre = query.value(0).toInt();
        } else {
            // Erreur lors de la récupération de la valeur de la colonne "liste_cpt"
            return false;
        }

        // Suppression de la compétence de la liste
        QStringList listeCpt = listeCptPrecedente.split(";");
        listeCpt.removeAll(competences);
        QString nouvelleListeCpt = listeCpt.join(";");

        // Mise à jour de la valeur de la colonne "liste_cpt"
        int nbre = listeCpt.length();
        query.prepare("UPDATE PFL SET nbre_cpt = ?, liste_cpt = ? WHERE nom_pro = ?");
        query.bindValue(0, nbre);
        query.bindValue(1, nouvelleListeCpt);
        query.bindValue(2, nom);

        if (query.exec()) {
            qDebug() << "La mise à jour a réussi";
            return true;
        } else {
            qDebug() << "La mise à jour a échoué";
            return false;
        }
}

QString Database::recupererDescriptionProfil(QString nom)
{
    QSqlQuery query;
    query.prepare("SELECT description FROM PFL WHERE nom_pfl = ?");
    query.bindValue(0, nom);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        // Erreur lors de la récupération de la description
        return "";
    }
}

bool Database::modifierProfil(QString nom, QString new_nom, QString description)
{
    QSqlQuery query;
    query.prepare("UPDATE PFL SET nom_pfl = ?, description = ? WHERE nom_pfl = ?");
    query.addBindValue(new_nom);
    query.addBindValue(description);
    query.addBindValue(nom);
    if (query.exec()) {
        qDebug() << "La mise à jour a réussi";
        return true;
    } else {
        qDebug() << "La mise à jour a échoué";
        return false;
    }
}

QStringList Database::recupererCompetencesProfil(QString nom)
{
    QStringList competences;
    QSqlQuery query;
    query.prepare("SELECT liste_cpt FROM PFL WHERE nom_pfl = ?");
    query.bindValue(0, nom);
    if (query.exec() && query.next()) {
        QString listeCpt = query.value(0).toString();
        competences = listeCpt.split(";");
    } else {
        // Erreur lors de la récupération de la valeur de la colonne "liste_cpt"
        qWarning() << "Impossible de récupérer les compétences du profil" << nom;
    }
    return competences;
}

//Fonctions sur la table CPT
bool Database::ajouterCompetence(QString nom)
{
    QSqlQuery query;
    query.prepare("INSERT INTO CPT (nom) VALUES (:nom)");
    query.bindValue(":nom", nom);
    if (query.exec()) {
        return true;
    }
    else {
        qDebug() << "Erreur lors de l'ajout de la comptence :" << query.lastError().text();
        return false;
    }
}

bool Database::supprimerCompetence(QString nom)
{
    QString req = "DELETE FROM CPT WHERE nom=:nom";
    QSqlQuery query;
    query.prepare(req);
    query.bindValue(":nom", nom);
    if (!query.exec())
    {
        return false;
    }
    return true;
}

bool Database::modifierCompetence(QString nom, QString new_nom)
{
    QSqlQuery query;
    query.prepare("UPDATE CPT SET nom = ? WHERE nom = ?");
    query.addBindValue(new_nom);
    query.addBindValue(nom);
    if (query.exec()) {
        qDebug() << "La mise à jour a réussi";
        return true;
    } else {
        qDebug() << "La mise à jour a échoué";
        return false;
    }
}

QStringList Database::matchProPfl(QString profil_test)
{
    QStringList professionnels = recupererProfessionnels();
    QStringList list_pro;
    foreach(QString pro, professionnels)
    {
        QStringList nomComplet = pro.split(" ");
        QString nom = nomComplet[0];
        QString prenom = nomComplet[1];
        for (int i=2;i<nomComplet.length();i++)
        {
            prenom = prenom + " " + nomComplet[i];
        }
        bool test = true;
        foreach(const QString& element, recupererCompetencesProfil(profil_test)) {
            if (!recupererCompetencesProfessionnel(nom, prenom).contains(element)) {
                test = false;
                break;
            }
        }
        if (test) {
            list_pro.append(nom + " " + prenom);
        }
    }
    return list_pro;
}
