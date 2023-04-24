#include "pfl.h"
#include "category.h"
#include <iostream>

using namespace std;

pfl::pfl(int id_pfl, string nom_pfl, int ncpt_pfl, vector<string> cpt_pfl)
{
    //ctor
    id=id_pfl;
    name_pfl=nom_pfl;
    ncpt=ncpt_pfl;
    cpt_list=cpt_pfl;
    type="PFL";
}

void pfl::display()
{
    cout <<"PFL:"<<endl;
    cout <<"\t"<<name_pfl<<endl;
    cout <<"\t"<<ncpt<<endl;
    for (int i=0; i<ncpt; i++)
    {
        cout<<"\t"<< cpt_list[i] <<endl;
    }
}

string pfl::get_name(){return name_pfl;}

pfl::~pfl()
{
    //dtor
}
