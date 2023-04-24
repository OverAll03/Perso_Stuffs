#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <vector>


using namespace std;

class category
{
    public:
        category();
        vector<string> get_cpt();
        int get_id();
        void add_cpt(string n_cpt);
        virtual ~category();

    protected:
        int id;
        int ncpt;
        string type;
        vector<string> cpt_list;
};

#endif // CATEGORY_H
