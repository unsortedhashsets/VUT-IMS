#include "get_params.hpp"



get_params::get_params(int argc, char *argv[]) {

    this->S = DEF_S;   // -s --susceptible
    this->mu = DEF_MU;  // -c --mu
    this->HC = DEF_HC;  //-h --HC

    this->it = DEF_IT;
    this->Dd = DEF_DD;
    this->Fh = DEF_FH;
    this->beta = DEF_BETA;
    this->Fr = DEF_FR;
    this->type = DEF_LOCK_TYPE;
    this->hi = DEF_HI;

    this->verbose = false;
    this->range = 150;

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
                {"hc", required_argument, 0, 'h'},
                {"range", required_argument, 0, 'r'},
                {"verbose", no_argument, 0, 'v'},
                {"type", no_argument, 0, 't'}
            };

        c = getopt_long(argc, argv, "vs:c:h:r:t:", long_options, nullptr);

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
                case 't':
                    this->type = stod(optarg);
                    break;
                case 'r':
                    this->range = stoi(optarg);
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
    cout << "\n\tRange:\t\t\t\t\t" << this->range << "\tDays";
    cout << "\n\tType of lockdown:\t\t\t" << this->type;
    cout << "\n\tVerbosity:\t\t\t\t" << this->verbose << endl << endl;
}