#include "professionnel.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QListWidget>
#include <QTextEdit>
#include <QFormLayout>
#include "database.h"

Professionnel::Professionnel(QWidget *parent, Database *db) : QWidget(parent)
{
    onglets = new QTabWidget(this);
    onglets->addTab(creationFormulaire(), "Ajouter");
    onglets->addTab(suppressionFormulaire(), "Supprimer");
    onglets->addTab(modificationFormulaire(), "Modifier");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(onglets);

    setLayout(layout);
}

QWidget *Professionnel::creationFormulaire()
{
    //Création d'un nouveau widget qui contiendra tous les éléments graphiques du formulaire.
    QWidget* widget = new QWidget(this);
    // Création de plusieurs QLineEdit pour les champs de nom et prénom.
    QLineEdit* nomLineEdit = new QLineEdit(widget);
    nomLineEdit->setPlaceholderText("Nom*");
    nomLineEdit->setStyleSheet("QLineEdit { height: 1em; color: black; font-size: 18px; background-color: white; } QLineEdit::cursor { width: 7px; background-color: red; }");
    QLineEdit* prenomLineEdit = new QLineEdit(widget);
    prenomLineEdit->setPlaceholderText("Prénom*");
    prenomLineEdit->setStyleSheet("QLineEdit { height: 1em; color: black; font-size: 18px; background-color: white; } QLineEdit::cursor { width: 7px; background-color: red; }");
    // Création d'une QComboBox pour sélectionner les compétences existantes dans la base de données.
    QComboBox* competencesComboBox = new QComboBox(widget);
    QStringList competences = db->recupererCompetences();
    competencesComboBox->addItems(competences);

    // Création d'un QPushButton pour ajouter la compétence sélectionnée à une liste.
    QPushButton* ajouterButton = new QPushButton("Ajouter", widget);

    // Création d'une QListWidget pour afficher les compétences sélectionnées.
    QListWidget* competencesListWidget = new QListWidget(widget);
    competencesListWidget->setStyleSheet("QListWidget {background-color: white;}");
    // Création d'un QPushButton pour ajouter le nouveau professionnel à la base de données.
    QPushButton* ajouterProfilButton = new QPushButton("Ajouter", widget);

    // Création d'un QLabel pour afficher une image du professionnel.
    QLabel* imageLabel = new QLabel(widget);
    QPixmap pixmap(":/images/images/pro.jpeg");
    imageLabel->setPixmap(pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);

    // Création d'un layout pour les champs de nom et prénom.
    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Nom* :", nomLineEdit);
    formLayout->addRow("Prenom* :", prenomLineEdit);
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
    connect(ajouterProfilButton, &QPushButton::clicked, this, [this, nomLineEdit, prenomLineEdit, competencesListWidget]() {
        QString nom = nomLineEdit->text();
        QString prenom = prenomLineEdit->text();
        if (nom.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires(*)");
            return;
        }

        QStringList competences;
        for (int i = 0; i < competencesListWidget->count(); i++) {
            competences.append(competencesListWidget->item(i)->text());
        }

        if (db->ajouterProfessionnel(nom, prenom, competences.join(";"))) {
            QMessageBox::information(this, "Succès", "Professionnel ajouté avec succès");
            nomLineEdit->clear();
            prenomLineEdit->clear();
            competencesListWidget->clear();
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

QWidget* Professionnel::suppressionFormulaire()
{
    QWidget* widget = new QWidget(this);
    // Création de la combobox pour sélectionner le professionnel à supprimer
    QComboBox* nomComboBox = new QComboBox(widget);
    nomComboBox->setObjectName("nomComboBox"); // ajouter un nom d'objet pour pouvoir appliquer un style
    nomComboBox->addItem("Choisissez un professionnel");

    // Remplir la combobox avec les noms-prénoms des professionnels existants
    QStringList professionnels = db->recupererProfessionnels();
    foreach(QString professionnel, professionnels) {
        nomComboBox->addItem(professionnel);
    }

    // Création du bouton de suppression
    QPushButton* supprimerButton = new QPushButton("Supprimer", widget);
    supprimerButton->setObjectName("supprimerButton"); // ajouter un nom d'objet pour pouvoir appliquer un style

    // Connexion du bouton à la fonction de suppression du professionnel sélectionné
    connect(supprimerButton, &QPushButton::clicked, this, [this, nomComboBox]() {
        if (nomComboBox->currentIndex() == 0) {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un professionnel");
            return;
        }

        QString nomComplet = nomComboBox->currentText();
        QStringList nomPrenom = nomComplet.split(" ");
        QString nom = nomPrenom.at(0);
        QString prenom = nomPrenom.at(1);
        if (nomPrenom.length() > 2) {
            for (int j = 2; j < nomPrenom.length(); j++) {
                prenom = prenom + " " + nomPrenom.at(j);
            }
        }

        // Suppression du professionnel sélectionné
        if (db->supprimerProfessionnel(nom, prenom)) {
            QMessageBox::information(this, "Succès", "Professionnel supprimé avec succès");
            nomComboBox->removeItem(nomComboBox->currentIndex());
        }
        else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la suppression du professionnel");
        }

        // Actualisation de l'onglet de modification des professionnels
        onglets->removeTab(2);
        onglets->addTab(modificationFormulaire(), "Modifier");
    });

    QLabel* imageLabel = new QLabel(widget);
    QPixmap pixmap(":/images/images/pro.jpeg");
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

QWidget *Professionnel::modificationFormulaire()
{
    QWidget *widget = new QWidget(this);
    QComboBox *nomComboBox = new QComboBox(widget);
    QListView *view = new QListView(nomComboBox);
    view->setMinimumWidth(200); // spécifiez une largeur plus grande pour les options
    nomComboBox->setView(view);
    QPushButton *modifierButton = new QPushButton("Modifier", widget);
    // Remplir la combobox avec les noms-prénoms des professionnels existants
    nomComboBox->addItem("Choisissez un professionnel");
    nomComboBox->setItemData(0, QVariant(0), Qt::UserRole);
    QStringList professionnels = db->recupererProfessionnels();
    foreach(QString professionnel, professionnels) {
        nomComboBox->addItem(professionnel);
    }
    QFormLayout *formLayout = new QFormLayout();
    QLabel *nomLabel = new QLabel("Nom:", widget);
    QLineEdit *nomLineEdit = new QLineEdit(widget);
    nomLineEdit->setStyleSheet("QLineEdit { height: 1em; color: black; font-size: 18px; background-color: white; } QLineEdit::cursor { width: 7px; background-color: red; }");
    formLayout->addRow(nomLabel, nomLineEdit);
    QLabel *prenomLabel = new QLabel("Prénom:", widget);
    QLineEdit *prenomLineEdit = new QLineEdit(widget);
    prenomLineEdit->setStyleSheet("QLineEdit { height: 1em; color: black; font-size: 18px; background-color: white; } QLineEdit::cursor { width: 7px; background-color: red; }");
    formLayout->addRow(prenomLabel, prenomLineEdit);
    QLabel *descriptionLabel = new QLabel("Description:", widget);
    QTextEdit *descriptionTextEdit = new QTextEdit(widget);
    descriptionTextEdit->setStyleSheet("QTextEdit { height: 1em; color: black; font-size: 18px; background-color: white; }");
    formLayout->addRow(descriptionLabel, descriptionTextEdit);
    QLabel *competencesLabel = new QLabel("Compétences:", widget);
    QComboBox *competencesComboBox = new QComboBox(widget);
    QStringList competences = db->recupererCompetences();
    foreach(QString competence, competences) {
        competencesComboBox->addItem(competence);
    }
    QListWidget *competencesListWidget = new QListWidget(widget);
    competencesListWidget->setStyleSheet("QListWidget {background-color: white;}");
    QPushButton *ajouterCompetenceButton = new QPushButton("Ajouter", widget);
    connect(ajouterCompetenceButton, &QPushButton::clicked, this, [this, nomLineEdit, prenomLineEdit, competencesComboBox, competencesListWidget]() {
        QString nom = nomLineEdit->text();
        QString prenom = prenomLineEdit->text();
        QString competence = competencesComboBox->currentText();
        if (competencesListWidget->findItems(competence, Qt::MatchExactly).count() > 0) {
            QMessageBox::warning(this, "Erreur", "Cette compétence a déjà été ajoutée");
            return;
        }
        if (db->ajouterCompetenceProfessionnel(nom, prenom, competence)) {
            QListWidgetItem *item = new QListWidgetItem(competence);
            competencesListWidget->addItem(item);
        }
        else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout de la compétence");
        }
    });
    QPushButton *supprimerCompetenceButton = new QPushButton("Supprimer", widget);
    connect(supprimerCompetenceButton, &QPushButton::clicked, this, [this, nomLineEdit, prenomLineEdit, competencesListWidget]() {
        QString nom = nomLineEdit->text();
        QString prenom = prenomLineEdit->text();
        QListWidgetItem *item = competencesListWidget->currentItem();
        if (!item) {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une compétence à supprimer");
            return;
        }
        QString competence = item->text();
        if (db->supprimerCompetenceProfessionnel(nom, prenom, competence)) {
            delete item;
        }
        else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la suppression de la compétence");
        }
    });
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

    QLabel* imageLabel = new QLabel(widget);
    QPixmap pixmap(":/images/images/pro.jpeg");
    imageLabel->setPixmap(pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addStretch();
    layout->addWidget(imageLabel);
    layout->addWidget(nomComboBox);
    layout->addLayout(formLayout);
    layout->addWidget(competencesLabel);
    layout->addLayout(competencesLayout);
    layout->addWidget(modifierButton);
    layout->addStretch();
    widget->setLayout(layout);

    connect(nomComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, nomComboBox, nomLineEdit, prenomLineEdit, descriptionTextEdit, competencesListWidget]() {
        int index = nomComboBox->currentIndex();
        if (index == 0) {
            nomLineEdit->clear();
            prenomLineEdit->clear();
            descriptionTextEdit->clear();
            competencesListWidget->clear();
            return;
        }
        QString nomComplet = nomComboBox->currentText();
        QStringList nomPrenom = nomComplet.split(" ");
        QString nom = nomPrenom.at(0);
        QString prenom = nomPrenom.at(1);
        if (nomPrenom.length()>2)
        {
            for (int j=2; j<nomPrenom.length(); j++)
            {
                prenom = prenom + " " + nomPrenom.at(j);
            }
        }
        QString description = db->recupererDescriptionProfessionnel(nom, prenom);
        QStringList competences = db->recupererCompetencesProfessionnel(nom, prenom);
        nomLineEdit->setText(nom);
        prenomLineEdit->setText(prenom);
        descriptionTextEdit->setText(description);
        competencesListWidget->clear();
        foreach(QString competence, competences) {
            QListWidgetItem *item = new QListWidgetItem(competence);
            competencesListWidget->addItem(item);
        }
    });

    connect(modifierButton, &QPushButton::clicked, this, [this, nomComboBox, nomLineEdit, prenomLineEdit, descriptionTextEdit]() {
        int index = nomComboBox->currentIndex();
        if (index == 0) {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un professionnel");
            return;
        }
        QString nomComplet = nomComboBox->currentText();
        QStringList nomPrenom = nomComplet.split(" ");
        QString nom = nomPrenom.at(0);
        QString prenom = nomPrenom.at(1);
        if (nomPrenom.length() > 2) {
            for (int j = 2; j < nomPrenom.length(); j++) {
                prenom = prenom + " " + nomPrenom.at(j);
            }
        }
        QString description = descriptionTextEdit->toPlainText();
        qDebug()<<nom <<prenom<<description;
        qDebug()<<prenomLineEdit->text();
        if (db->modifierProfessionnel(nom, prenom, nomLineEdit->text(), prenomLineEdit->text(), description)) {
            QMessageBox::information(this, "Succès", "Professionnel modifié avec succès");
            nomComboBox->setItemText(index, nomLineEdit->text() + " " + prenomLineEdit->text());
        }
        else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la modification du professionnel");
        }
        onglets->addTab(modificationFormulaire(), "Modifier");
        onglets->removeTab(2);
    });

    return widget;

}

