#include <iostream>
#include <fstream>
#include <string>
#include "function.h"
#include "category.h"
#include "pro.h"
#include "pfl.h"

using namespace std;

int main()
{
    cout << "****** Gestion des Profils Et Compétences ****** \n" << endl << "Entrez le chemin d'accès du fichier de travail...." << endl;
    string abs_path("sample.cpt");
    int choice;
    vector<pro> pro_tab;
    vector<pfl> pfl_tab;
    vector<cpt> cpt_tab;
    load_file(abs_path, pro_tab, pfl_tab, cpt_tab);
    string path("save_gestion.cpt");
    ofstream n_save(path);
    do
    {
        cout << "Que comptez-vous faire ?" << endl << " 1 - Ajouter des données au fichier \n 2 - Lire des données dans du fichier \n 3 - Sauvegarder les modifications \n 0 - Quitter \n " << endl;
        cin >> choice;
        switch(choice)
        {
            case 1:
                cout << "Ajout de données...." << endl;
                writein_file(n_save, pro_tab, pfl_tab, cpt_tab);
                break;
            case 2:
                cout << "Lecture de données...." << endl;
                readin_file(pro_tab, pfl_tab, cpt_tab);
                break;
            case 3:
                cout << "Sauvegarde des données dans le fichier: "<<path<<"...." << endl;
                save(n_save, pro_tab, pfl_tab, cpt_tab);
                break;
            case 0:
                cout << "Vous quittez le programme...." << endl;
                break;
            default:
                continue;
                break;
        }
    }while(choice==1 || choice==2 || choice==3);
    return 0;
}
