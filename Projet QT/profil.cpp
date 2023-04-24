#include "profil.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QListWidget>
#include <QTextEdit>
#include <QFormLayout>
#include "database.h"

Profil::Profil(QWidget *parent, Database *db) : QWidget(parent)
{
    onglets = new QTabWidget(this);
    onglets->addTab(creationFormulaire(), "Ajouter");
    onglets->addTab(suppressionFormulaire(), "Supprimer");
    onglets->addTab(modificationFormulaire(), "Modifier");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(onglets);
    setLayout(layout);
}

QWidget* Profil::creationFormulaire()
{
    // Création d'un nouveau widget qui contiendra tous les éléments graphiques du formulaire.
    QWidget* widget = new QWidget(this);

    // Création de plusieurs QLineEdit pour les champs de nom et prénom.
    QLineEdit* nomLineEdit = new QLineEdit(widget);
    nomLineEdit->setPlaceholderText("Nom*");
    nomLineEdit->setStyleSheet("QLineEdit { height: 1em; color: black; font-size: 18px; background-color: white; } QLineEdit::cursor { width: 7px; background-color: red; }");

    // Création d'une QComboBox pour sélectionner les compétences existantes dans la base de données.
    QComboBox* competencesComboBox = new QComboBox(widget);
    QStringList competences = db->recupererCompetences();
    competencesComboBox->addItems(competences);

    // Création d'un QPushButton pour ajouter la compétence sélectionnée à une liste.
    QPushButton* ajouterButton = new QPushButton("Ajouter", widget);

    // Création d'une QListWidget pour afficher les compétences sélectionnées.
    QListWidget* competencesListWidget = new QListWidget(widget);
    competencesListWidget->setStyleSheet("QListWidget {background-color: white;}");
    // Création d'un QPushButton pour ajouter le nouveau profil à la base de données.
    QPushButton* ajouterProfilButton = new QPushButton("Ajouter", widget);

    // Création d'un QLabel pour afficher une image du profil.
    QLabel* imageLabel = new QLabel(widget);
    QPixmap pixmap(":/images/images/pfl.webp");
    imageLabel->setPixmap(pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);

    // Création d'un layout pour les champs de nom et prénom.
    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Nom*", nomLineEdit);

    // Création d'un layout horizontal pour la QComboBox et le QPushButton d'ajout.
    QHBoxLayout* competencesLayout = new QHBoxLayout();
    competencesLayout->addWidget(competencesComboBox);
    competencesLayout->addWidget(ajouterButton);

    // Création d'un layout vertical pour les éléments du formulaire.
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(imageLabel);
    layout->addLayout(formLayout);
    layout->addLayout(competencesLayout);
    layout->addWidget(competencesListWidget);
    layout->addWidget(ajouterProfilButton);

    // Définition du layout principal pour le widget.
    widget->setLayout(layout);

    // Connexion du QPushButton d'ajout de compétence à la liste.
    connect(ajouterButton, &QPushButton::clicked, this, [competencesComboBox, competencesListWidget]() {
        QString competence = competencesComboBox->currentText();
        if (!competence.isEmpty()) {
            competencesListWidget->addItem(competence);
        }
    });

    // Connexion du QPushButton d'ajout de profil à la base de données.
    connect(ajouterProfilButton, &QPushButton::clicked, this, [this, nomLineEdit, competencesListWidget]() {
        QString nom = nomLineEdit->text();
        if (nom.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires(*)");
            return;
        }

        QStringList competences;
        for (int i = 0; i < competencesListWidget->count(); i++) {
            competences.append(competencesListWidget->item(i)->text());
        }

        if (db->ajouterProfil(nom, competences.join(";"))) {
            QMessageBox::information(this, "Succès", "Professionnel ajouté avec succès");
            nomLineEdit->clear();
        }
        else {
                QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout du professionnel");
            }
            onglets->removeTab(1);
            onglets->removeTab(1);
            onglets->addTab(suppressionFormulaire(), "Supprimer");
            onglets->addTab(modificationFormulaire(), "Modifier");
        });

        return widget;
}

QWidget* Profil::suppressionFormulaire()
{
    QWidget* widget = new QWidget(this);
    // Création de la combobox pour sélectionner le profil à supprimer
    QComboBox* nomComboBox = new QComboBox(widget);
    nomComboBox->setObjectName("nomComboBox"); // ajouter un nom d'objet pour pouvoir appliquer un style
    nomComboBox->addItem("Choisissez un profil");

    // Remplir la combobox avec les noms des profils existants
    QStringList profils = db->recupererProfils();
    foreach(QString profil, profils) {
        nomComboBox->addItem(profil);
    }

    // Création du bouton de suppression
    QPushButton* supprimerButton = new QPushButton("Supprimer", widget);
    supprimerButton->setObjectName("supprimerButton"); // ajouter un nom d'objet pour pouvoir appliquer un style

    // Connexion du bouton à la fonction de suppression du profil sélectionné
    connect(supprimerButton, &QPushButton::clicked, this, [this, nomComboBox]() {
        if (nomComboBox->currentIndex() == 0) {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un professionnel");
            return;
        }

        QString nom = nomComboBox->currentText();
        // Suppression du profil sélectionné
        if (db->supprimerProfil(nom)) {
            QMessageBox::information(this, "Succès", "Professionnel supprimé avec succès");
            nomComboBox->removeItem(nomComboBox->currentIndex());
        }
        else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la suppression du professionnel");
        }

        // Actualisation de l'onglet de modification des profils
        onglets->removeTab(2);
        onglets->addTab(modificationFormulaire(), "Modifier");
    });
    QLabel* imageLabel = new QLabel(widget);
    QPixmap pixmap(":/images/images/pfl.webp");
    imageLabel->setPixmap(pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);

    // Création et paramétrage du layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(imageLabel);
    layout->addWidget(nomComboBox);
    layout->addWidget(supprimerButton);
    layout->setAlignment(Qt::AlignCenter); // aligner les widgets au centre

    // Application d'un style
    widget->setLayout(layout);
    widget->setStyleSheet("QWidget#nomComboBox, QPushButton#supprimerButton { padding: 10px; font-size: 16px; }");

    return widget;

}

QWidget *Profil::modificationFormulaire()
{
    QWidget *widget = new QWidget(this);
    QComboBox *nomComboBox = new QComboBox(widget);
    QListView *view = new QListView(nomComboBox);
    view->setMinimumWidth(200); // spécifiez une largeur plus grande pour les options
    nomComboBox->setView(view);
    QPushButton *modifierButton = new QPushButton("Modifier", widget);
    // Remplir la combobox avec les noms des profils existants
    nomComboBox->addItem("Choisissez un profil");
    nomComboBox->setItemData(0, QVariant(0), Qt::UserRole);
    QStringList profils = db->recupererProfils();
    foreach(QString profil, profils) {
        nomComboBox->addItem(profil);
    }
    QFormLayout *formLayout = new QFormLayout();
    QLabel *nomLabel = new QLabel("Nom:", widget);
    QLineEdit *nomLineEdit = new QLineEdit(widget);
    nomLineEdit->setStyleSheet("QLineEdit { height: 1em; color: black; font-size: 18px; background-color: white; } QLineEdit::cursor { width: 7px; background-color: red; }");
    formLayout->addRow(nomLabel, nomLineEdit);
    QLabel *descriptionLabel = new QLabel("Description:", widget);
    QTextEdit *descriptionTextEdit = new QTextEdit(widget);
    descriptionTextEdit->setStyleSheet("QTextEdit { height: 1em; color: black; font-size: 18px; background-color: white; } QLineEdit::cursor { width: 7px; background-color: red; }");
    formLayout->addRow(descriptionLabel, descriptionTextEdit);
    QLabel *competencesLabel = new QLabel("Compétences:", widget);
    QComboBox *competencesComboBox = new QComboBox(widget);
    QStringList competences = db->recupererCompetences();
    foreach(QString competence, competences) {
        competencesComboBox->addItem(competence);
    }
    QListWidget *matchingList = new QListWidget(widget);
    matchingList->setStyleSheet("QListWidget {background-color: white;}");
    QListWidget *competencesListWidget = new QListWidget(widget);
    competencesListWidget->setStyleSheet("QListWidget {background-color: white;}");
    QPushButton *ajouterCompetenceButton = new QPushButton("Ajouter", widget);
    connect(ajouterCompetenceButton, &QPushButton::clicked, this, [this, nomLineEdit, competencesComboBox, competencesListWidget]() {
        QString nom = nomLineEdit->text();
        QString competence = competencesComboBox->currentText();
        if (competencesListWidget->findItems(competence, Qt::MatchExactly).count() > 0) {
            QMessageBox::warning(this, "Erreur", "Cette compétence a déjà été ajoutée");
            return;
        }
        if (db->ajouterCompetenceProfil(nom, competence)) {
            QListWidgetItem *item = new QListWidgetItem(competence);
            competencesListWidget->addItem(item);
        }
        else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout de la compétence");
        }
    });
    QPushButton *supprimerCompetenceButton = new QPushButton("Supprimer", widget);
    connect(supprimerCompetenceButton, &QPushButton::clicked, this, [this, nomLineEdit, competencesListWidget]() {
        QString nom = nomLineEdit->text();
        QListWidgetItem *item = competencesListWidget->currentItem();
        if (!item) {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une compétence à supprimer");
            return;
        }
        QString competence = item->text();
        if (db->supprimerCompetenceProfil(nom, competence)) {
            delete item;
        }
        else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la suppression de la compétence");
        }
    });
    QLabel* imageLabel = new QLabel(widget);
    QPixmap pixmap(":/images/images/pfl.webp");
    imageLabel->setPixmap(pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *competencesLayout = new QHBoxLayout();
    QVBoxLayout *competencesLeftLayout = new QVBoxLayout();
    competencesLeftLayout->addWidget(competencesComboBox);
    competencesLeftLayout->addWidget(ajouterCompetenceButton);
    competencesLayout->addLayout(competencesLeftLayout);
    competencesLayout->addWidget(competencesListWidget);
    QVBoxLayout *competencesRightLayout = new QVBoxLayout();
    competencesRightLayout->addWidget(supprimerCompetenceButton);
    competencesRightLayout->addStretch();
    competencesLayout->addLayout(competencesRightLayout);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addStretch();
    layout->addWidget(imageLabel);
    layout->addWidget(nomComboBox);
    layout->addLayout(formLayout);
    layout->addWidget(competencesLabel);
    layout->addLayout(competencesLayout);
    layout->addWidget(modifierButton);
    layout->addStretch();
    QHBoxLayout *final_lay = new QHBoxLayout();
    QVBoxLayout *matchListlay = new QVBoxLayout();
    QLabel *matchListLabel = new QLabel("Professionnels Correspondants");
    matchListlay->addWidget(matchListLabel);
    matchListlay->addWidget(matchingList);
    final_lay->addLayout(layout);
    final_lay->addLayout(matchListlay);
    widget->setLayout(final_lay);

    connect(nomComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, nomComboBox, nomLineEdit, descriptionTextEdit, competencesListWidget, matchingList]() {
        int index = nomComboBox->currentIndex();
        if (index == 0) {
            nomLineEdit->clear();
            descriptionTextEdit->clear();
            competencesListWidget->clear();
            matchingList->clear();
            return;
        }
        QString nom = nomComboBox->currentText();
        QString description = db->recupererDescriptionProfil(nom);
        QStringList competences = db->recupererCompetencesProfil(nom);
        nomLineEdit->setText(nom);
        descriptionTextEdit->setText(description);
        competencesListWidget->clear();
        foreach(QString competence, competences) {
            QListWidgetItem *item = new QListWidgetItem(competence);
            competencesListWidget->addItem(item);
        }
        matchingList->addItems(db->matchProPfl(nomComboBox->currentText()));
    });

    connect(modifierButton, &QPushButton::clicked, this, [this, nomComboBox, nomLineEdit, descriptionTextEdit]() {
        int index = nomComboBox->currentIndex();
        if (index == 0) {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un professionnel");
            return;
        }
        QString nom = nomComboBox->currentText();
        QString description = descriptionTextEdit->toPlainText();
        qDebug()<<nom<<description;
        if (db->modifierProfil(nom, nomLineEdit->text(), description)) {
            QMessageBox::information(this, "Succès", "Professionnel modifié avec succès");
            nomComboBox->setItemText(index, nomLineEdit->text());
        }
        else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la modification du professionnel");
        }
        onglets->addTab(modificationFormulaire(), "Modifier");
        onglets->removeTab(2);
    });

    return widget;

}

