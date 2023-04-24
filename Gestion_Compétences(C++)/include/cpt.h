#ifndef CPT_H
#define CPT_H
#include <string>
#include <vector>
#include "category.h"

using namespace std;
class cpt: public category
{
    public:
        cpt(int id_cpt, int ncpt_cpt, vector<string> cpt_cpt);
        void display();
        virtual ~cpt();
};

#endif // CPT_H
