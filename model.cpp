////////////////////////////////////////////////////////////////////////////
// Model BLABLA-test               SIMLIB/C++
//
// BLABLA equations:
//
//      dS/dt = -beta * Ci * I / it
//      dI/dt =  beta * C - I / Dd * (1-FR)
//      dR/dt =  I / Dd * (1-FR)
//      dD/dt =  I / Dd * FR
//
// Auxiliar equations:
//
//      Ci  = C * F
//      F   = (S-I)/S
//      HiC = SC / HC
//      SC  = I * Fh
//
//           | 3%  if Hic < 5
//      Fr = | 7%  if 5 < HiC < 30
//           | 10% if 30 < HiC
//
//           | I * mu                 if no-lockdown
//           | I * mu * barred_lambda if standard-lockdown
//      C  = | I * mu * k             if smart-lockdown
//           | I * mu * q             if Post-lockdown
//
// where:
//       Parameters:
//                  it            -- Incubation time 
//                  Dd            -- Disease duration 
//                  Fh            -- Fraction requiring hospitalization 
//                  beta          -- Infectivity 
//                  HC            -- Hospital capacity 
//                  barred_lambda -- Lockdown effectivity
//                  k             -- Smart lockdown effectivity 	
//                  q             -- Post lockdown effectivity 
//                  SC            -- Serious cases 
//       Auxiliary:
//                  mu  -- Contacts rate
//                  Fr  -- Fatality rate
//                  HiC -- Hospital capacity strain index
//       Stocks:
//                  S -- Stock Susceptible S
//                  I -- Stock Infected I	
//                  R -- Stock Recovered R
//                  D -- Stock Deaths D
//
// initial conditions:
//
// Source: 
//

#include "simlib.h"
#include "src/get_params.hpp"
#include <iostream>
#include <cmath>
using namespace std;

bool verbose = false;
const double StepPrn = 1; // step of output

class BLA {
 public:
  Parameter s_init, beta, mu, it, Dd, Fr;
  Integrator S, I, R, D;
  BLA(double _beta, double _mu, double _s_init, double _it, double _Dd, double _Fr ) :
    s_init(_s_init), beta(_beta), mu(_mu), it(_it), Dd(_Dd), Fr(_Fr),
    S (-beta * I * mu * (S/s_init) / it , s_init.Value()),
    I ( beta * I * mu * (S/s_init) / it - I / Dd , 25.0),
    R ( I / Dd * (1-Fr)),
    D ( I / Dd * Fr) {}

    void SetParameters(double _beta, double _mu, double _s_init, double _it, double _Dd, double _Fr ) { 
        s_init = _s_init;
        beta = _beta;
        mu = _mu;
        it = _it;
        Dd = _Dd;
        Fr = _Fr;
        S.Init(_s_init);
    }

};

BLA e(0.025,70.0,100000.0,5.0,14.0,0.03);

void Sample() {
  Print("%g;%g;%g;%g;%g\n", Time, e.S.Value(), e.I.Value(), e.R.Value(), e.D.Value());
  if (verbose) {
    cout << Time << ' ' << e.S.Value() << ' ' << e.I.Value() << ' ' << e.R.Value() << ' ' << e.D.Value() << '\n';
  }
}

Sampler S(Sample, StepPrn);

// experiment description:
int main(int argc, char *argv[]) {  
  if (argc > 1){
    get_params params(argc, argv);
    e.SetParameters(params.beta, params.mu, params.S, params.it, params.Dd, params.Fr);
    verbose = params.verbose;
  }
  SetOutput("model.csv");
  Print("# BLA equation output \n");
  if (verbose) {
    cout << "# BLA equation output \n";
  }
  SetStep(1e-8,1e-3);   // set step size range
  Init(0,150);          // experiment initialization 
  Run();                // simulation
}