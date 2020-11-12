#ifndef GET_PARAMS_H
#define GET_PARAMS_H

#include <iostream>
#include <getopt.h>
#include <string.h>

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
    bool verbose;
    get_params(int argc, char *argv[]);
    get_params();
   private:
    void get_arguments(int argc, char *argv[]);
    void print_params();
};

#endif