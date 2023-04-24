#include "cpt.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
cpt::cpt(int id_cpt, int ncpt_cpt, vector<string> cpt_cpt)
{
    //ctor
    id=id_cpt;
    ncpt=ncpt_cpt;
    category::cpt_list=cpt_cpt;
    type="CPT";
}

void cpt::display()
{
    cout <<"CPT:"<<endl;
    cout <<"\t"<<ncpt<<endl;
    for (int i=0; i<ncpt; i++)
    {
        cout<<"\t"<< cpt_list[i] <<endl;
    }
}

cpt::~cpt()
{
    //dtor
}
