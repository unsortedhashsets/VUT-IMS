#include "get_params.hpp"

get_params::get_params(int argc, char *argv[]) {
    this->susceptible = 0;                               // -s --susceptible
    this->incubation_time = 5;
    this->disease_duration = 14;
    this->fraction_requiring_hospitalization = 0.13;
    this->infectivity = 0.025;
    this->contacts_rate = 0;                            // -c --contacts_rate
    this->hospital_capacity = 0;                        //-h --hospital_capacity
    this->fatality_rate = 0.03;

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
                {"contacts_rate", required_argument, 0, 'c'},
                {"hospital_capacity", required_argument, 0, 'h'},
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
                    this->susceptible = stoi(optarg);
                    break;
                case 'c':
                    this->contacts_rate = stoi(optarg);
                    break;
                case 'h':
                    this->hospital_capacity = stoi(optarg);
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
    cout << "\n\tSusceptible:\t\t\t\t" << this->susceptible << "\tPeople\t\tAssumed";
    cout << "\n\tIncubation time:\t\t\t" << this->incubation_time << "\tDays\t\tWu et al.(2020)";
    cout << "\n\tDisease duration:\t\t\t" << this->disease_duration << "\tDays\t\tWu et al.(2020)";
    cout << "\n\tFraction requiring hospitalization:\t" << this->fraction_requiring_hospitalization << "\t%\t\tWHO report 73 (2020), Li et al.(2020)";
    cout << "\n\tInfectivity:\t\t\t\t" << this->infectivity << "\tDimensionless\tEstimated with RO";
    cout << "\n\tContacts rate:\t\t\t\t" << this->contacts_rate << "\tContacts/person\tAssumed";
    cout << "\n\tHospital capacity:\t\t\t" << this->hospital_capacity << "\tBeds\t\tAssumed";
    cout << "\n\tFatality rate:\t\t\t\t" << this->fatality_rate << "\t%\t\tWHO report 73 (2020), Li et al.(2020)";
    cout << "\n\tVerbosity:\t\t\t\t" << this->verbose << endl;
}