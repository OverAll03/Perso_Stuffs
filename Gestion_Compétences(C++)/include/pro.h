#ifndef PRO_H
#define PRO_H
#include <string>
#include <vector>
#include "pfl.h"
#include "category.h"

using namespace std;

class pro : public category
{
    public:
        pro(int id_pro, string nom_pro, string fnom_pro, int ncpt_pro, vector<string> cpt_pro);
        string get_name();
        void display();
        virtual ~pro();
    private:
        string name_pro;
        string f_name_pro;
};

#endif // PRO_H
