#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "function.h"
#include "pro.h"
#include "category.h"
#include "pfl.h"
#include "cpt.h"
#include <algorithm>

using namespace std;

void load_file(string file, vector<pro> &pro_vec, vector<pfl> &pfl_vec, vector<cpt> &cpt_vec)
{
    ifstream r_stream(file);

    string line, name, f_name, num_cpt, comp;
    int id_pro(0), id_pfl(0), id_cpt(0);
    if (r_stream)
    {
        cout << "Fichier ouvert avec succès \n";
    }
    else
    {
            cout << "Quelque chose s'est mal passé \n";
            return;
    }

    while(getline(r_stream, line))
    {
        if (line=="PRO:")
        {
            vector<string> vec_cpt;
            getline(r_stream, name);
            name.erase(name.begin());
            getline(r_stream, f_name);
            f_name.erase(f_name.begin());
            getline(r_stream, num_cpt);
            num_cpt.erase(num_cpt.begin());
            int n=stoi(num_cpt);
            for (int i=0;i<n;i++)
            {
                getline(r_stream, comp);
                comp.erase(comp.begin());
                vec_cpt.push_back(comp);
            }
            pro_vec.push_back(pro(id_pro+1, name, f_name, n, vec_cpt));
            id_pro++;
        }
        if (line=="PFL:")
        {
            vector<string> vec_cpt;
            getline(r_stream, name);
            name.erase(name.begin());
            getline(r_stream, num_cpt);
            num_cpt.erase(num_cpt.begin());
            int n=stoi(num_cpt);
            for (int i=0;i<n;i++)
            {
                getline(r_stream, comp);
                comp.erase(comp.begin());
                vec_cpt.push_back(comp);
            }
            pfl_vec.push_back(pfl(id_pfl+1, name, n, vec_cpt));
            id_pfl++;
        }
        if (line=="CPT:")
        {
            vector<string> vec_cpt;
            getline(r_stream, num_cpt);
            num_cpt.erase(num_cpt.begin());
            int n=stoi(num_cpt);
            for (int i=0;i<n;i++)
            {
                getline(r_stream, comp);
                comp.erase(comp.begin());
                vec_cpt.push_back(comp);
            }
            cpt_vec.push_back(cpt(id_cpt+1, n, vec_cpt));
            id_cpt++;
        }

    }
}


void readin_file(vector<pro> pro_vec, vector<pfl> pfl_vec, vector<cpt> cpt_vec)
{
    cout << "**** Que voulez-vous faire ? **** \n" << endl;
    int choice;
    do
    {
        cout << " 1 - Lister toutes les compétences \n 2 - Lister tous les profils \n 3 - Lister tous les professionnels \n 4 - Lister toutes les compétences d'un profil spécifique \n 5 - Lister toutes les compétences d'un professionnel spécifique \n 6 - Lister tous les professionnels correspondant à un profil spécifique \n 0 - Revenir en arrière \n "<< endl;
        cin >> choice;
        cout << endl;
        switch (choice)
        {
            case 1:
                cout << "Listing des compétences....." <<endl;
                list_comp(pro_vec, pfl_vec, cpt_vec);
                break;
            case 2:
                cout << "Listing des profils....." <<endl;
                list_pfl(pfl_vec);
                break;
            case 3:
                cout << "Listing des professionnels....."<<endl;
                list_pro(pro_vec);
                break;
            case 4:
                cout << "Listing de compétences propres à un profil spécifique....."<<endl;
                pfl_cpt(pfl_vec);
                break;
            case 5:
                cout << "Listing de compétences propres à un professionnel spécifique....."<<endl;
                pro_cpt(pro_vec);
                break;
            case 6:
                cout << "Listing des profils correspondants à un professionnel sprécifique...."<<endl;
                pro_pfl(pro_vec, pfl_vec);
                break;
            case 0:
                cout << "Retour au menu précédent \n" <<endl;
                break;
            default:
                break;
        }
    }while(choice==1 || choice==2 || choice==3 || choice==4 || choice==5 || choice==6);
    return;
}

void writein_file(ofstream &file, vector<pro> &pro_vec, vector<pfl> &pfl_vec, vector<cpt> &cpt_vec)
{
    cout << "**** Que voulez-vous faire ? **** \n" << endl;
    int choice;
    do
    {
        cout << " 1 - Ajouter des compétences à un profil \n 2 - Ajouter des compétences à un professionnels \n 0 - Revenir en arrière \n " << endl;
        cin >> choice;
        switch(choice)
        {
            case 1:
                cout << " Ajout de compétences à un professionnel \n" << endl;
                add_pro(pro_vec);
                break;
            case 2:
                cout << "Ajout de compétences à un profil \n " << endl;
                add_pfl(pfl_vec);
                break;
            case 0:
                cout << "Leaving the loop \n" <<endl;
                break;
            default:
                break;
        }
        cout << "Tapez 'O' pour sauvegarder" << endl;
        char t;
        cin >> t;
        if (t=='O' || t=='o')
        {
            save(file, pro_vec, pfl_vec, cpt_vec);
            cout << "Sauvegarde effectuée avec succès..."<< endl;
        }
        else
        {
            cout << "Erreur lors de la sauvegarde..."<< endl;
        }
    }while(choice==1 || choice==2);
    return;
}

void list_comp(vector<pro> pro_vec, vector<pfl> pfl_vec, vector<cpt> cpt_vec)
{
    vector<string> tab;
    for(int i=0;i<pro_vec.size();i++)
    {
        for(int j=0;j<pro_vec[i].get_cpt().size();j++)
        {
            tab.push_back(pro_vec[i].get_cpt()[j]);
        }
    }
    for(int i=0;i<pfl_vec.size();i++)
    {
        for(int j=0;j<pfl_vec[i].get_cpt().size();j++)
        {
            tab.push_back(pfl_vec[i].get_cpt()[j]);
        }
    }
    for(int i=0;i<cpt_vec.size();i++)
    {
        for(int j=0;j<cpt_vec[i].get_cpt().size();j++)
        {
            tab.push_back(cpt_vec[i].get_cpt()[j]);
        }
    }
    sort(tab.begin(),tab.end());
    tab.erase(unique(tab.begin(),tab.end()),tab.end());
    for(int i=0;i<tab.size();i++)
    {
        cout<<tab[i]<<endl;
    }
}

void list_pfl(vector<pfl> pfl_vec)
{
    for (int i=0; i<pfl_vec.size(); i++)
    {
        cout << pfl_vec[i].get_name() <<endl;
    }
}

void list_pro(vector<pro> pro_vec)
{
    vector<string> tab;
    for (int i=0; i<pro_vec.size();i++)
    {
        tab.push_back(pro_vec[i].get_name());
    }
    sort(tab.begin(),tab.end());
    for (int i=0; i<tab.size();i++)
    {
        cout << pro_vec[i].get_id() << " - " << tab[i] << endl << endl;
    }
}

void pfl_cpt(vector<pfl> pfl_vec)
{
    cout << " Les compétences de quels profils voulez-vous afficher ?" << endl;
    for(int i=0; i<pfl_vec.size(); i++)
    {
        cout << pfl_vec[i].get_id();
        cout << " - " << pfl_vec[i].get_name()<<endl;
    }
    int c;
    cin >> c;
    for(int i=0; i<pfl_vec.size(); i++)
    {
        if (pfl_vec[i].get_id()==c)
        {
            for (int j=0;j<pfl_vec[i].get_cpt().size();j++)
            {
                cout << pfl_vec[i].get_cpt()[j] << endl;
            }
        }
    }
}

void pro_cpt(vector<pro> pro_vec)
{
    cout << " Les compétences de quels profils voulez-vous afficher ?" << endl;
    for(int i=0; i<pro_vec.size(); i++)
    {
        cout << pro_vec[i].get_id();
        cout << " - " << pro_vec[i].get_name()<<endl;
    }
    int c;
    cin >> c;
    for(int i=0; i<pro_vec.size(); i++)
    {
        if (pro_vec[i].get_id()==c)
        {
            for (int j=0;j<pro_vec[i].get_cpt().size();j++)
            {
                cout << pro_vec[i].get_cpt()[j] << endl;
            }
        }
    }
}

void pro_pfl(vector<pro> pro_vec, vector<pfl> pfl_vec)
{
    cout << " Les professionnels liés à quels profils voulez-vous afficher ?" << endl;
    for(int i=0; i<pfl_vec.size(); i++)
    {
        cout << pfl_vec[i].get_id();
        cout << " - " << pfl_vec[i].get_name()<<endl;
    }

    int choice;
    string pfl_test;
    vector<string> test_list, pro_list;
    cin >> choice;
    cout << "Les professionnels correspondants sont: \n" << endl;
    for(int i=0; i<pfl_vec.size(); i++)
    {
        if (pfl_vec[i].get_id()==choice)
        {
            test_list=pfl_vec[i].get_cpt();
            pfl_test=pfl_vec[i].get_name();
        }
    }

    for (int j=0; j<pro_vec.size(); j++)
    {
        int c= 0;
        pro_list=pro_vec[j].get_cpt();
        for (int k=0; k<test_list.size(); k++)
        {
            for (int t=0; t<pro_list.size(); t++)
            {
                if (test_list[k]==pro_list[t])
                {
                    c=c+1;
                    break;
                }

            }
        }

        if (c>=(test_list.size()/2))
        {
            cout<<pro_vec[j].get_name() << endl << endl;
        }
        continue;
    }

}

void add_pro (vector<pro> &pro_vec)
{
    cout << "Choix du professionnel: \n"<< endl;
    list_pro(pro_vec);
    int choice;
    cin >> choice;
    string new_cpt;
    cout << "Compétence à ajouter: \n"<< endl;
    cin >> new_cpt;
    for (int i=0; i<pro_vec.size(); i++)
    {
        if (pro_vec[i].get_id()==choice)
        {
            pro_vec[i].add_cpt(new_cpt);
            for (int j=0; j<pro_vec[i].get_cpt().size(); j++)
            {
                cout << pro_vec[i].get_cpt()[j] << endl;
            }
            break;
        }
    }
}

void add_pfl (vector<pfl> &pfl_vec)
{
    cout << "Choix du profil: \n" <<endl;
    list_pfl(pfl_vec);
    int choice;
    cin >> choice;
    string new_cpt;
    cout << "Compétence à ajouter: \n" <<endl;
    cin >> new_cpt;
    for (int i=0; i<pfl_vec.size(); i++)
    {
        if (pfl_vec[i].get_id()==choice)
        {
            pfl_vec[i].add_cpt(new_cpt);
            for (int j=0; j<pfl_vec[i].get_cpt().size(); j++)
            {
                cout << pfl_vec[i].get_cpt()[j] << endl;
            }
            break;
        }
    }
}

void save(ofstream& n_file, vector<pro> &pro_vec, vector<pfl> &pfl_vec, vector<cpt> &cpt_vec)
{

    for (int i=0; i<pro_vec.size(); i++)
    {
        n_file<<"PRO:\n"<< pro_vec[i].get_name() << pro_vec[i].get_cpt().size()<<endl;
        for (int j=0; j<pro_vec[i].get_cpt().size(); j++)
        {
            n_file<<pro_vec[i].get_cpt()[j]<<endl;
        }
    }
     n_file << endl;
    for (int i=0; i<pfl_vec.size(); i++)
    {
        n_file<<"PFL:\n"<< pfl_vec[i].get_name() << pfl_vec[i].get_cpt().size()<<endl;
        for (int j=0; j<pfl_vec[i].get_cpt().size(); j++)
        {
            n_file<<pfl_vec[i].get_cpt()[j]<<endl;
        }
    }
    n_file<< endl;
    for (int i=0; i<cpt_vec.size(); i++)
    {
        n_file<<"CPT:\n"<< pro_vec[i].get_cpt().size()<<endl;
        for (int j=0; j<pro_vec[i].get_cpt().size(); j++)
        {
            n_file<<pro_vec[i].get_cpt()[j]<<endl;
        }
    }
    return;
}
