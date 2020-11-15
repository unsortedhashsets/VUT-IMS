#ifndef GET_PARAMS_H
#define GET_PARAMS_H

#include <iostream>
#include <getopt.h>
#include <string.h>

#define DEF_S    100000;  //  Susceptible:       X People          Assumed
#define DEF_MU   70;      //  Contacts rate:     Y Contacts/person Assumed
#define DEF_HC   1000;    //  Hospital capacity: Z Beds            Assumed

#define DEF_IT   5;       //  Incubation time:                    5     Days            Wu et al.(2020)
#define DEF_DD   14;      //  Disease duration:                   14    Days            Wu et al.(2020)
#define DEF_FH   0.13;    //  Fraction requiring hospitalization: 0.13  %               WHO report 73 (2020), Li et al.(2020)
#define DEF_BETA 0.025;   //  Infectivity:                        0.025 Dimensionless   Estimated with RO
#define DEF_FR   0.03;    //  Fatality rate:                      0.03  %               WHO report 73 (2020), Li et al.(2020)
#define DEF_HI   0.1;     //  Fatality rate:                      0.03  %               WHO report 73 (2020), Li et al.(2020)

#define DEF_LOCK_TYPE   0;  //  Type of lockdown: 0 - without any lockdown; 
                            //                    1 - basic lockdown (from 25 to 85 day); 
                            //                    2 - two short lockdown and one smart (25  -  55,  85  -  115) 
                            //                                                            Short       Short
                            //                    3 - one medium and one smart locdown (25  -  65, 66  -  106  )  
                            //                                                             Medium     Smart

using namespace std;

class get_params {
   public:
    double S;
    double it;
    double Dd;
    double Fh;
    double beta;
    double mu;
    double HC;
    double Fr;
    int range;
    int type;
    double hi;
    bool verbose;
    get_params(int argc, char *argv[]);
    get_params();
   private:
    void get_arguments(int argc, char *argv[]);
    void print_params();
};

#endif