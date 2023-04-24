#include "category.h"
#include "pro.h"
#include "pfl.h"
#include "cpt.h"
#include <string>
#include <vector>

using namespace std;
category::category()
{

}
vector<string> category::get_cpt(){return cpt_list;}

void category::add_cpt(string n_cpt)
{
    id++;
    cpt_list.push_back(n_cpt);
}

int category::get_id(){return id;}


category::~category()
{
    //dtor
}
