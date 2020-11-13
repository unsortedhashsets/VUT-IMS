////////////////////////////////////////////////////////////////////////////
// Lockdown, one, two, none, or smart.                            SIMLIB/C++
// Modeling containing covid-19 infection.
// A conceptual model              
//
// Model equations:
//
//      dS/dt = -beta * Ci * I / it
//      dI/dt =  beta * Ci - I / Dd
//      dR/dt =  I / Dd * (1-Fr)
//      dD/dt =  I / Dd * Fr
//
// Auxiliar equations:
//
//      Ci  = C * F 
//      F   = (S_init)/S
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
//           | I * mu * q             if post-lockdown
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

int range = 150;
bool verbose = false;
const double StepPrn = 1; // step of output

class Covid19{
 public:
  Parameter s_init, beta, mu, it, Dd, Fr, HC, Fh;
  Integrator S, I, R, D;
  Covid19(double _beta, double _mu, double _s_init, double _it, double _Dd, double _Fr, double _HC, double _Fh  ) :

    s_init(_s_init), beta(_beta), mu(_mu), it(_it), Dd(_Dd), Fr(_Fr), HC(_HC), Fh(_Fh),
    S (-beta * I * mu * (S / s_init) / it , s_init.Value()),
    I ( beta * I * mu * (S / s_init) / it - I / Dd , 25.0),
    R ( I / Dd * (1-Fr)),
    D ( I / Dd * Fr) {}

    void SetParameters(double _beta, double _mu, double _s_init, double _it, double _Dd, double _Fr, double _HC, double _Fh ) { 
        s_init = _s_init;
        beta = _beta;
        mu = _mu;
        it = _it;
        Dd = _Dd;
        Fr = _Fr;
        HC = _HC;
        Fh = _Fh;
        S.Init(_s_init);
    }

    void Conditions() {
      double SC = I.Value() * Fh.Value();
      double HiC = SC / HC.Value();
      if ( HiC > 30.0 ){
          Fr = 0.10;
      } else if ( HiC > 4.0 )
          Fr = 0.07;
      else{
          Fr = 0.03;
      }
    }

    void Out() {
      Print("%g;%g;%g;%g;%g\n", Time, S.Value(), I.Value(), R.Value(), D.Value());
      if (verbose) {
         cout << Time << ' ' << S.Value() << ' ' << I.Value() << ' ' << R.Value() << ' ' << D.Value() << '\n';
      }
    }
    
};

Covid19 c19(0,0,0,0,0,0,0,0);

void Sample() { 
  c19.Out();
  c19.Conditions();
}

Sampler S(Sample, StepPrn);

// experiment description:
int main(int argc, char *argv[]) {  
  get_params params(argc, argv);
  c19.SetParameters(params.beta, params.mu, params.S, params.it, params.Dd, params.Fr, params.HC, params.Fh);
  verbose = params.verbose;
  SetOutput("covid19.csv");
  Print("# Modeling containing covid-19 infection. A conceptual model.\n");
  if (verbose) {
    cout << "# Modeling containing covid-19 infection. A conceptual model.\n";
  }
  SetStep(1e-8,1e-3);   // set step size range
  Init(0,params.range);        // experiment initialization 
  Run();                // simulation
}