#ifndef PFL_H
#define PFL_H

#include <string>
#include <vector>
#include "category.h"

using namespace std;

class pfl: public category
{
    public:
        pfl(int id_pfl, string nom_pfl, int ncpt_pfl, vector<string> cpt_pfl);
        string get_name();
        void display();
        virtual ~pfl();
    private:
        string name_pfl;
};

#endif // PFL_H
