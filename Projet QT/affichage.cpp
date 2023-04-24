#include "affichage.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>

affichage::affichage(QWidget *parent, Database *db) : QWidget(parent)
{
    QLineEdit *searchLineEdit = new QLineEdit();
    searchLineEdit->setStyleSheet("QLineEdit { height: 1em; color: black; font-size: 18px; background-color: white; } QLineEdit::cursor { width: 7px; background-color: red; }");
    QListWidget *ListWidget = new QListWidget();
    ListWidget->setStyleSheet("QListWidget {background-color: white;}");
    QCheckBox *professionnelsCheckBox = new QCheckBox("Professionnels");
    QCheckBox *profilsCheckBox = new QCheckBox("Profils");
    QCheckBox *competencesCheckBox = new QCheckBox("Compétences");
    QPushButton *searchButton = new QPushButton(QIcon(":/icons/search.png"), "Rechercher");
    QPushButton *resetButton = new QPushButton(QIcon(":/icons/reset.png"), "Réinitialiser");
    // Récupération des données depuis la base de données
    QStringList pro = db->recupererProfessionnels();
    QStringList pfl = db->recupererProfils();
    QStringList cpt = db->recupererCompetences();
    QStringList liste_search = pro;
    liste_search.append(cpt);
    liste_search.append(pfl);

    // Ajout des éléments à la QListWidget
    ListWidget->addItems(liste_search);

    // Connexion des signaux et des slots
    connect(searchButton, &QPushButton::clicked, this, [db, searchLineEdit, ListWidget, professionnelsCheckBox, profilsCheckBox, competencesCheckBox, liste_search](){
        ListWidget->clear();
        QString texte = searchLineEdit->text();
        searchLineEdit->clear();
        QStringList liste_resultats;

        // Ajout des résultats correspondant au texte pour chaque checkbox cochée
        if (professionnelsCheckBox->isChecked()){
            foreach (QString element, liste_search)
            {
                if (element.contains(texte, Qt::CaseInsensitive) && db->recupererProfessionnels().contains(element, Qt::CaseInsensitive)){
                    liste_resultats.append(element);
                }
            }
        }
        if (profilsCheckBox->isChecked()){
            foreach (QString element, liste_search)
            {
                if (element.contains(texte, Qt::CaseInsensitive) && db->recupererProfils().contains(element, Qt::CaseInsensitive)){
                    liste_resultats.append(element);
                }
            }
        }
        if (competencesCheckBox->isChecked()){
            foreach (QString element, liste_search)
            {
                if (element.contains(element, Qt::CaseInsensitive) && db->recupererCompetences().contains(element, Qt::CaseInsensitive)){
                    liste_resultats.append(element);
                }
            }
        }
        if (!professionnelsCheckBox->isChecked() && !profilsCheckBox->isChecked() && !competencesCheckBox->isChecked())
        {
            foreach (QString element, liste_search)
                    {
                        if (element.contains(texte, Qt::CaseInsensitive)){
                            liste_resultats.append(element);
                        }
                    }
        }


        // Ajout des résultats à la QListWidget
        ListWidget->addItems(liste_resultats);
        ListWidget->sortItems();
    });

    connect(resetButton, &QPushButton::clicked, this, [searchLineEdit, ListWidget, professionnelsCheckBox, profilsCheckBox, competencesCheckBox, liste_search](){
        ListWidget->clear();
        ListWidget->addItems(liste_search);
        searchLineEdit->clear();
        professionnelsCheckBox->setChecked(false);
        profilsCheckBox->setChecked(false);
        competencesCheckBox->setChecked(false);
    });

    // Création des layouts
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QHBoxLayout *checkboxLayout = new QHBoxLayout();

    // Ajout des éléments aux layouts
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(resetButton);
    searchLayout->setSpacing(10);
    searchLayout->setAlignment(Qt::AlignCenter);
    searchLayout->setContentsMargins(10, 10, 10, 0);

    checkboxLayout->addWidget(professionnelsCheckBox);
    checkboxLayout->addWidget(profilsCheckBox);
    checkboxLayout->addWidget(competencesCheckBox);
    mainLayout->addStretch();
    mainLayout->addWidget(ListWidget);
    mainLayout->addLayout(searchLayout);
    mainLayout->addLayout(checkboxLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
}
