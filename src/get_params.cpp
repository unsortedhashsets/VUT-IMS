#include "get_params.hpp"

#define IT   5;       //  Incubation time:                    5     Days            Wu et al.(2020)
#define DD   14;      //  Disease duration:                   14    Days            Wu et al.(2020)
#define FH   0.13;    //  Fraction requiring hospitalization: 0.13  %               WHO report 73 (2020), Li et al.(2020)
#define BETA 0.025;   //  Infectivity:                        0.025 Dimensionless   Estimated with RO
#define FR   0.03;    //  Fatality rate:                      0.03  %               WHO report 73 (2020), Li et al.(2020)

get_params::get_params(int argc, char *argv[]) {
    this->S = 0;   // -s --susceptible
    this->mu = 0;  // -c --mu
    this->HC = 0;  //-h --HC

    this->it = IT;
    this->Dd = DD;
    this->Fh = FH;
    this->beta = BETA;
    this->Fr = FR;

    this->verbose = false;

    get_arguments(argc, argv);
}

void get_params::get_arguments(int argc, char *argv[]) {
    int c;
    while (1) {
        // getopt_long options
        static struct option long_options[] =
            {
                {"susceptible", required_argument, 0, 's'},
                {"mu", required_argument, 0, 'c'},
                {"HC", required_argument, 0, 'h'},
                {"verbose", no_argument, 0, 'v'}
            };

        c = getopt_long(argc, argv, "vs:c:h:", long_options, nullptr);

        // detect the end of the options
        if (c == -1) {
            break;
        }
        try{
            switch (c) {
                case 's':
                    this->S = stod(optarg);
                    break;
                case 'c':
                    this->mu = stod(optarg);
                    break;
                case 'h':
                    this->HC = stod(optarg);
                    break;
                case 'v':
                    this->verbose = true;
                    break;
                default:
                    cerr << "get_arguments(): wrong argument \"" << optarg << "\"!" << endl;
                    exit(1);
            }
        }
        catch(invalid_argument& e){
            cerr << "get_arguments(): wrong argument \"" << optarg << "\"!" << endl;
            exit(1);
        }
        catch(out_of_range& e){
            cerr << "get_arguments(): wrong argument \"" << optarg << "\"!" << endl;
            exit(1);
        }
    }

    if (optind != argc) {
        cerr << "get_arguments(): wrong number of arguments!" << endl;
        exit(1);
    }

    if (this->verbose) {
        cout << "\nProgram arguments: " << endl;
        for (int i = 0; i < argc; i++) {
            cout << "\t" << i << ": " << argv[i] << endl;
        }
    }

    if (this->verbose) this->print_params();
}

void get_params::print_params() {
    cout << "\nParameters:";
    cout << "\n\tSusceptible:\t\t\t\t" << this->S << "\tPeople\t\tAssumed";
    cout << "\n\tIncubation time:\t\t\t" << this->it << "\tDays\t\tWu et al.(2020)";
    cout << "\n\tDisease duration:\t\t\t" << this->Dd << "\tDays\t\tWu et al.(2020)";
    cout << "\n\tFraction requiring hospitalization:\t" << this->Fh << "\t%\t\tWHO report 73 (2020), Li et al.(2020)";
    cout << "\n\tInfectivity:\t\t\t\t" << this->beta << "\tDimensionless\tEstimated with RO";
    cout << "\n\tContacts rate:\t\t\t\t" << this->mu << "\tContacts/person\tAssumed";
    cout << "\n\tHospital capacity:\t\t\t" << this->HC << "\tBeds\t\tAssumed";
    cout << "\n\tFatality rate:\t\t\t\t" << this->Fr << "\t%\t\tWHO report 73 (2020), Li et al.(2020)";
    cout << "\n\tVerbosity:\t\t\t\t" << this->verbose << endl << endl;
}