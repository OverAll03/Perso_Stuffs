#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include <string>
#include <vector>
#include "category.h"
#include "pro.h"
#include "pfl.h"
#include "cpt.h"

using namespace std;

void readin_file(vector<pro> pro_vec, vector<pfl> pfl_vec, vector<cpt> cpt_vec);
void writein_file(ofstream& file, vector<pro> &pro_vec, vector<pfl> &pfl_vec, vector<cpt> &cpt_vec);
void load_file(string file, vector<pro> &pro_vec, vector<pfl> &pfl_vec, vector<cpt> &cpt_vec);
void add_pro(vector<pro> &pro_vec);
void add_pfl(vector<pfl> &pfl_vec);
void list_comp(vector<pro> pro_vec, vector<pfl> pfl_vec, vector<cpt> cpt_vec);
void list_pfl(vector<pfl> pfl_vec);
void list_pro(vector<pro> pro_vec);
void pfl_cpt(vector<pfl> pfl_vec);
void pro_cpt(vector<pro> pro_vec);
void pro_pfl(vector<pro> pro_vec, vector<pfl> pfl_vec);
void save(ofstream& file, vector<pro> &pro_vec, vector<pfl> &pfl_vec, vector<cpt> &cpt_vec);

#endif // FUNCTION_H_INCLUDED
