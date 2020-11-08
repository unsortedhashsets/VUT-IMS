#ifndef GET_PARAMS_H
#define GET_PARAMS_H

#include <iostream>
#include <getopt.h>
#include <string.h>

using namespace std;

class get_params {
   public:
    int susceptible;
    int incubation_time;
    int disease_duration;
    float fraction_requiring_hospitalization;
    float infectivity;
    int contacts_rate;
    int hospital_capacity;
    float fatality_rate;
    bool verbose;
    get_params(int argc, char *argv[]);
    get_params();
   private:
    void get_arguments(int argc, char *argv[]);
    void print_params();
};

#endif