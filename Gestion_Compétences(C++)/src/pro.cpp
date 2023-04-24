#include "pro.h"
#include <string>
#include <vector>
#include "category.h"
#include <iostream>

using namespace std;

pro::pro(int id_pro, string nom_pro, string fnom_pro, int ncpt_pro, vector<string> cpt_pro)
{
    //ctor
    id=id_pro;
    name_pro=nom_pro;
    f_name_pro=fnom_pro;
    ncpt=ncpt_pro;
    cpt_list=cpt_pro;
    type="PRO";
}

void pro::display()
{
    cout <<"PRO:"<<endl;
    cout <<"\t"<<name_pro<<endl;
    cout <<"\t"<<f_name_pro<<endl;
    cout <<"\t"<<ncpt<<endl;
    for (int i=0; i<ncpt; i++)
    {
        cout<<"\t"<< cpt_list[i] <<endl;
    }
}

string pro::get_name(){return name_pro+"\n"+f_name_pro;}

pro::~pro()
{
    //dtor
}
