#include "competence.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QListWidget>
#include <QTextEdit>
#include <QFormLayout>
#include "database.h"

Competence::Competence(QWidget *parent, Database *db) : QWidget(parent)
{
    onglets = new QTabWidget(this);
    onglets->addTab(creationFormulaire(), "Ajouter");
    onglets->addTab(suppressionFormulaire(), "Supprimer");
    onglets->addTab(modificationFormulaire(), "Modifier");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(onglets);
    setLayout(layout);
}

QWidget* Competence::creationFormulaire()
{
    // Création d'un nouveau widget qui contiendra tous les éléments graphiques du formulaire.
    QWidget* widget = new QWidget(this);

    // Création d'un QLineEdit pour le champ de saisie du nom de la compétence.
    QLineEdit* nomLineEdit = new QLineEdit(widget);
    nomLineEdit->setPlaceholderText("Nom de la compétence*");
    nomLineEdit->setStyleSheet("QLineEdit {  cursor: crosshair; }");

    // Création d'un QPushButton pour ajouter la nouvelle compétence à la base de données.
    QPushButton* ajouterButton = new QPushButton("Ajouter", widget);

    // Création d'un layout pour les champs de nom de compétence et bouton d'ajout.
    QHBoxLayout* formLayout = new QHBoxLayout();
    formLayout->addWidget(nomLineEdit);
    formLayout->addWidget(ajouterButton);

    QLabel* imageLabel = new QLabel(widget);
    QPixmap pixmap(":/images/images/cpt.jpeg");
    imageLabel->setPixmap(pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);

    // Création du layout principal qui contient tous les éléments graphiques du formulaire.
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addStretch();
    layout->addWidget(imageLabel);
    layout->addLayout(formLayout);
    layout->addStretch();

    // Définition du layout principal pour le widget.
    widget->setLayout(layout);

    // Connexion du QPushButton à la fonction qui ajoute la nouvelle compétence à la base de données.
    connect(ajouterButton, &QPushButton::clicked, this, [this, nomLineEdit](){
        QString nom = nomLineEdit->text();
        if (nom.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir le champ obligatoire(*)");
            return;
        }

        if (db->ajouterCompetence(nom)) {
            QMessageBox::information(this, "Succès", "Compétence ajoutée avec succès");
            nomLineEdit->clear();
        }
        else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout de la compétence");
        }
    });

    return widget;
}

QWidget* Competence::suppressionFormulaire()
{
    QWidget* widget = new QWidget(this);
    // Création de la combobox pour sélectionner le profil à supprimer
    QComboBox* nomComboBox = new QComboBox(widget);
    nomComboBox->setObjectName("nomComboBox"); // ajouter un nom d'objet pour pouvoir appliquer un style
    nomComboBox->addItem("Choisissez une compétence");
    nomComboBox->setItemData(0, QVariant(0), Qt::UserRole);
    // Remplir la combobox avec les noms des profils existants
    QStringList competences = db->recupererCompetences();
    foreach(QString cpt, competences) {
        nomComboBox->addItem(cpt);
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
        if (db->supprimerCompetence(nom)) {
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
    QPixmap pixmap(":/images/images/cpt.jpeg");
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

QWidget* Competence::modificationFormulaire()
{
    QWidget *widget = new QWidget(this);
    QComboBox *nomComboBox = new QComboBox(widget);
    QListView *view = new QListView(nomComboBox);
    view->setMaximumWidth(200); // spécifiez une largeur plus grande pour les options
    nomComboBox->setView(view);
    QPushButton *modifierButton = new QPushButton("Modifier", widget);
    // Remplir la combobox avec les competences existantes
    nomComboBox->addItem("Choisissez une competences");
    nomComboBox->setItemData(0, QVariant(0), Qt::UserRole);
    QStringList competences = db->recupererCompetences();
    foreach(QString cpt, competences ) {
        nomComboBox->addItem(cpt);
    }
    QFormLayout *formLayout = new QFormLayout();
    QLineEdit *nomLineEdit = new QLineEdit(widget);
    nomLineEdit->setStyleSheet("QLineEdit { height: 1em; color: black; font-size: 18px; background-color: white; } QLineEdit::cursor { width: 7px; background-color: red; }");
    nomLineEdit->setPlaceholderText("Renommez la compétence");
    nomLineEdit->setStyleSheet("QLineEdit {  cursor: crosshair; }");
    connect(nomComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [nomComboBox, nomLineEdit]() {
        int index = nomComboBox->currentIndex();
        if (index == 0) {
            nomLineEdit->clear();
            return;
        }
        QString nom = nomComboBox->currentText();
        nomLineEdit->setText(nom);
    });

    connect(modifierButton, &QPushButton::clicked, this, [this, nomComboBox, nomLineEdit]() {
        int index = nomComboBox->currentIndex();
        if (index == 0) {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une Compétence");
            return;
        }
        QString nom = nomComboBox->currentText();
        qDebug()<<nom;
        if (db->modifierCompetence(nom, nomLineEdit->text())) {
            QMessageBox::information(this, "Succès", "Compétence modifié avec succès");
            nomComboBox->setItemText(index, nomLineEdit->text());
        }
        else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la modification de la compétence");
        }
        onglets->addTab(modificationFormulaire(), "Modifier");
        onglets->removeTab(2);
    });
    QLabel *imageLabel = new QLabel(widget);
    QPixmap pixmap(":/images/images/cpt.jpeg");
    imageLabel->setPixmap(pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(imageLabel);
    formLayout->addRow("Compétences à modifer",nomComboBox);
    formLayout->addRow("Nouveau nom",nomLineEdit);
    widget->setLayout(formLayout);
    return widget;
}
