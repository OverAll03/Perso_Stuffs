#include "gestion.h"
#include "ui_gestion.h"
#include "database.h"
#include <QtSql/QSql>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QDialogButtonBox>
#include "professionnel.h"
#include "profil.h"
#include "competence.h"
#include "affichage.h"
#include "functions.h"

Gestion::Gestion(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Gestion)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: #C0C0C0;");
    this->setFixedWidth(width());

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);

    base = new Database();
    stack = new QStackedWidget(this);
//Page d'accueil
    mainpage = new QWidget();
    QLabel *labelpic = new QLabel(this);
    QLabel *labeltext = new QLabel("Welcome to your Skill Managing app !!!",this);
    labeltext->setAlignment(Qt::AlignCenter);
    labeltext->setStyleSheet("QLabel { font-size: 25pt; font-weight: bold; color: #2c3e50;}");
    QPixmap pixmap(":images/images/skill.jpg");
    labelpic->setPixmap(pixmap);
    labelpic->setScaledContents(true);
    QVBoxLayout *menu = new QVBoxLayout(this);
    menu->addWidget(labeltext);
    menu->addWidget(labelpic);
    QPushButton *Start = new QPushButton("Start", this);
    Start->setIcon(QIcon(":/icons/get-started.png"));
    Start->setIconSize(QSize(20,20));
    Start->setStyleSheet("QPushButton {font-size: 20pt; background-color: #27ae60; color: white; border-radius: 10px; padding: 10px 20px;}"
                          "QPushButton:hover {background-color: #219f54;}");
    connect(Start, &QPushButton::clicked, this, [this](){stack->setCurrentIndex(1);});
    menu->addWidget(Start);
    mainpage->setLayout(menu);
    stack->insertWidget(0, mainpage);

//Choix du fichier de Travail
    file_page = new QWidget();
    QLabel *labelfch = new QLabel("Que faire de la base de données actuelle ?");
    labelfch->setAlignment(Qt::AlignCenter);
    labelfch->setStyleSheet("QLabel { font-size: 25pt; font-weight: bold; color: white; }");

    QVBoxLayout *btn_start = new QVBoxLayout (this);
    btn_start->setContentsMargins(20, 20, 20, 20);
    btn_start->setSpacing(50);

    QPushButton *reset = new QPushButton("Réinitialiser les données", this);
    reset->setIcon(QIcon(":/icons/reinit.png"));
    reset->setIconSize(QSize(20,20));
    reset->setStyleSheet("QPushButton { font-size: 15pt; background-color: #B71C1C; color: white; padding: 15px; border-radius: 5px; }");
    reset->setCursor(Qt::PointingHandCursor);
    connect(reset, &QPushButton::clicked, this, [this](){this->warn_reset();});

    QPushButton *import = new QPushButton("Importer de nouvelles données depuis un fichier", this);
    import->setIcon(QIcon(":/icons/import.png"));
    import->setIconSize(QSize(20,20));
    import->setStyleSheet("QPushButton { font-size: 15pt; background-color: #1B5E20; color: white; padding: 15px; border-radius: 5px; }");
    import->setCursor(Qt::PointingHandCursor);
    connect(import, &QPushButton::clicked, this, [this](){this->warn_import();});

    QPushButton *xport = new QPushButton("Exporter les données vers un fichier", this);
    xport->setIcon(QIcon(":/icons/export.png"));
    xport->setIconSize(QSize(20,20));
    xport->setStyleSheet("QPushButton { font-size: 15pt; background-color: #0D47A1; color: white; padding: 15px; border-radius: 5px; }");
    xport->setCursor(Qt::PointingHandCursor);
    connect(xport, &QPushButton::clicked, this, [this](){this->warn_xport();});

    QPushButton *save = new QPushButton("Conserver les données et Continuer", this);
    save->setIcon(QIcon(":/icons/dbadmin.png"));
    save->setIconSize(QSize(20,20));
    save->setStyleSheet("QPushButton { font-size: 15pt; background-color: #004D40; color: white; padding: 15px; border-radius: 5px; }");
    save->setCursor(Qt::PointingHandCursor);
    connect(save, &QPushButton::clicked, this, [this](){this->continue_to_newpage();});

    btn_start->addWidget(reset);
    btn_start->addWidget(import);
    btn_start->addWidget(xport);
    btn_start->addWidget(save);

    QVBoxLayout *choice = new QVBoxLayout(this);
    choice->setContentsMargins(50, 50, 50, 50);
    choice->setSpacing(50);
    choice->addStretch();
    choice->addWidget(labelfch);
    choice->addLayout(btn_start);
    choice->addStretch();

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addLayout(choice);
    hLayout->addStretch();

    file_page->setLayout(hLayout);
    stack->insertWidget(1, file_page);

//Page de gestion
    QWidget *page1 = new QWidget(this);
    QLabel *labelpage1 = new QLabel("Gestion des Informations",this);
    labelpage1->setAlignment(Qt::AlignCenter);
    labelpage1->setStyleSheet("QLabel { font-size: 30pt; font-weight: bold; color: white;}");
    QVBoxLayout *page1_lay = new QVBoxLayout (this);
    QFrame* framepro = new QFrame();
    QFrame* framepfl = new QFrame();
    QFrame* framecpt = new QFrame();
    framepro->setFixedHeight(this->height()*1.1);
    framepfl->setFixedHeight(this->height()*1.1);
    framecpt->setFixedHeight(this->height()*1.1);
    QPixmap imgpro(":/images/images/pro.jpeg");
    QPixmap imgpfl(":/images/images/pfl.webp");
    QPixmap imgcpt(":/images/images/cpt.jpeg");
    framepro->setStyleSheet("QFrame {border: 2px solid black; border-radius: 5px;}");
    framepfl->setStyleSheet("QFrame {border: 2px solid black; border-radius: 5px;}");
    framecpt->setStyleSheet("QFrame {border: 2px solid black; border-radius: 5px;}");
    QPushButton* btnpro = new QPushButton("Professionnels", this);
    connect(btnpro, &QPushButton::clicked, this, [this](){Professionnel *pagepro = new Professionnel(this,base);
        stack->insertWidget(4, pagepro);stack->setCurrentIndex(4);});
    btnpro->setIcon(QIcon(":/icons/pro.png"));
    btnpro->setIconSize(QSize(20,20));
    QPushButton* btnpfl = new QPushButton("Profils", this);
    connect(btnpfl, &QPushButton::clicked, this, [this](){Profil *pagepfl = new Profil(this,base);
        stack->insertWidget(4, pagepfl);stack->setCurrentIndex(4);});
    btnpfl->setIcon(QIcon(":/icons/pfl.png"));
    btnpfl->setIconSize(QSize(20,20));

    QPushButton* btncpt = new QPushButton("Compétences", this);
    connect(btncpt, &QPushButton::clicked, this, [this](){Competence *pagecpt = new Competence(this,base);
        stack->insertWidget(4, pagecpt);stack->setCurrentIndex(4);});
    btncpt->setIcon(QIcon(":/icons/cpt.png"));
    btncpt->setIconSize(QSize(20,20));

    QVBoxLayout* layout1 = new QVBoxLayout(framepro);
    QLabel* label1 = new QLabel();
    label1->setPixmap(imgpro);
    label1->setScaledContents(true);
    layout1->addWidget(label1);
    layout1->addWidget(btnpro);
    QVBoxLayout* layout2 = new QVBoxLayout(framepfl);
    QLabel* label2 = new QLabel();
    label2->setPixmap(imgpfl);
    label2->setScaledContents(true);
    layout2->addWidget(label2);
    layout2->addWidget(btnpfl);
    QVBoxLayout* layout3 = new QVBoxLayout(framecpt);
    QLabel* label3 = new QLabel();
    label3->setPixmap(imgcpt);
    label3->setScaledContents(true);
    layout3->addWidget(label3);
    layout3->addWidget(btncpt);
    QHBoxLayout* frameLayout = new QHBoxLayout(this);
    frameLayout->addWidget(framepro);
    frameLayout->addWidget(framepfl);
    frameLayout->addWidget(framecpt);
    page1->setStyleSheet("QWidget {background-color: grey;} QPushButton {color: white;font-size: 11pt; font-weight: bold; background-color: #333333; border-radius: 10px; padding: 20px; margin: 10px;} QPushButton:hover {background-color: #666666;}");
    page1_lay->addStretch();
    page1_lay->addWidget(labelpage1);
    page1_lay->addLayout(frameLayout);
    page1_lay->addStretch();
    page1->setLayout(page1_lay);
    setCentralWidget(stack);
    stack->insertWidget(2, page1);

//Bouton de retour
    QPushButton *back = new QPushButton ("Retour",this);
    back->move(this->width()-back->width(),1);
    back->setStyleSheet("QPushButton {"
                        "border: 2px solid gray;"
                        "border-radius: 5px;"
                        "color: white;"
                        "background-color: #FFD162;"
                        "font-size: 16px;"
                        "padding: 6px 12px;"
                        "}"
                        "QPushButton:hover {"
                        "background-color:#E6B800;"
                        "}");
    connect(back, &QPushButton::clicked, this, [this](){
        if (stack->currentIndex()==0)
        {
            return;
        } else {
            stack->setCurrentIndex(0);
        }
    });
}

void Gestion::warn_reset()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, "Attention", "Les données seront complètement irrécupérables. Voulez-vous continuer?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        base->re_init();
        QMessageBox::information(this, "Réinitialisation de la base de données", "La base de données a été réinitialisée avec succès");
    } else {

    }
}

void Gestion::load_file(Database *bdd)
{
    QString fileName = QFileDialog::getOpenFileName(this,
            "Ouvrir fichier", QDir::homePath(),
            "Fichiers cpt (*.cpt);;Tous les fichiers (*)");
    qDebug()<<fileName;
    bdd->import(fileName);
    qDebug() << "Loaded successfully";
}

void Gestion::warn_import()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, "Attention", "L'importation de données implique que les données actuelles seront irrécupérables. Voulez-vous continuer?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        base->re_init();
        load_file(base);
        QMessageBox::information(this, "Importation depuis un fichier", "La base de données a été réinitialisée avec succès");
    } else {

    }
}

void Gestion::warn_xport()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Sélectionner un fichier", QDir::homePath(), "Fichier CPT (*.cpt)");
    if (fileName.isEmpty()) {
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Exporter la base de données");

    QCheckBox* proCheckbox = new QCheckBox("Professionnels", &dialog);
    QCheckBox* pflCheckbox = new QCheckBox("Profils", &dialog);
    QCheckBox* cptCheckbox = new QCheckBox("Compétences", &dialog);

    QVBoxLayout layout(&dialog);
    layout.addWidget(proCheckbox);
    layout.addWidget(pflCheckbox);
    layout.addWidget(cptCheckbox);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    layout.addWidget(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString filter;
        if (proCheckbox->isChecked()) {
            filter += "PRO";
        }
        if (pflCheckbox->isChecked()) {
            if (!filter.isEmpty()) {
                filter += "|";
            }
            filter += "PFL";
        }
        if (cptCheckbox->isChecked()) {
            if (!filter.isEmpty()) {
                filter += "|";
            }
            filter += "CPT";
        }

        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "Exportation", "Vous vous apprêtez à exporter les données. Prêt ?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            base->xport(fileName, filter);
            QMessageBox::information(this, "Exportation vers un fichier", "La base de données a été exportée avec succès");
        } else {
            QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de l'exportation de la base de données");
            return;
        }
    }
}

void Gestion::continue_to_newpage(){
    QMessageBox msgBox;
    affichage *display = new affichage(this, base);
    stack->insertWidget(3,display);

    msgBox.setText("Que souhaitez-vous faire ?");
    msgBox.setInformativeText("Veuillez sélectionner une option:");
    QPushButton* btn1 = msgBox.addButton("Parcourir les données", QMessageBox::YesRole);
    QPushButton* btn2 = msgBox.addButton("Modifier les données", QMessageBox::NoRole);
    msgBox.setDefaultButton(btn1);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStyleSheet("QMessageBox {background-color: #F5F5F5;}");
    btn1->setStyleSheet("QPushButton{background-color: #A9A9A9;}");
    btn2->setStyleSheet("QPushButton{background-color: #A9A9A9;}");
    msgBox.exec();

    if (msgBox.clickedButton() == btn2) {
        // Code pour la première option
        stack->setCurrentIndex(2);
        QMessageBox::information(this, "Bienvenue", "Bienvenue sur la page de Gestion. C'est ici que vous pourrez modifier les informations de votre base de données");
    }
    else if (msgBox.clickedButton() == btn1) {
        // Code pour la deuxième option
        stack->setCurrentIndex(3);
        QMessageBox::information(this, "Bienvenue", "Bienvenue sur la page d'affichage des données. C'est ici que vous pourrez parcourir informations de votre base de données");

    }
    else {
        stack->setCurrentIndex(1);
    }
}


Gestion::~Gestion()
{
    delete ui;
}
