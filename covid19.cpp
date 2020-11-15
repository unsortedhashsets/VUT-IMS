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
//           | I * mu * hi            if standard-lockdown
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
int type = 0;
bool verbose = false;
const double StepPrn = 1; // step of output

class Covid19 {
   public:
    Parameter s_init, beta, mu, it, Dd, Fr, HC, Fh, Ci;
    Integrator S, I, R, D;
    Covid19(double _beta, double _mu, double _s_init, double _it, double _Dd,
            double _Fr, double _HC, double _Fh, double _Ci)
        : s_init(_s_init),
          beta(_beta),
          mu(_mu),
          it(_it),
          Dd(_Dd),
          Fr(_Fr),
          HC(_HC),
          Fh(_Fh),
          Ci(_Ci),
          S(-beta * Ci / it, s_init.Value()),
          I(beta * Ci / it - I / Dd, 40),
          R(I / Dd * (1 - Fr)),
          D(I / Dd * Fr) {}

    void SetParameters(double _beta, double _mu, double _s_init, double _it,
                       double _Dd, double _Fr, double _HC, double _Fh,
                       double _Ci) {
        s_init = _s_init;
        beta = _beta;
        mu = _mu;
        it = _it;
        Dd = _Dd;
        Fr = _Fr;
        HC = _HC;
        Fh = _Fh;
        Ci = _Ci;
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
      double F = S.Value() / s_init.Value();
      
      switch (type) {
        case 0:
            Ci = (I.Value() * mu.Value() * F);
            break;
        case 1:
            if (Time < 25) {  // Before lockdowns
                Ci = (I.Value() * mu.Value() * F);
            } 
            else if ((Time >= 25) && (Time <= 85)) {  // One short
                Ci = (I.Value() * mu.Value() * 0.1 * F);  // hi = 0.1 is DEF_HI FIXME:
            } 
            else {                                  // After short lockdown
                Ci = (I.Value() * mu.Value() * 0.5 * F);  // q = 0.5 is DEF_HI FIXME:
            }
            break;
        case 2:
            if (Time <= 24) {  // Before lockdowns
                Ci = (I.Value() * mu.Value() * F);
            } else if ((Time >= 25) && (Time <= 55)) {  // Short lockdown
                Ci = (I.Value() * mu.Value() * 0.1 * F);
            } else if ((Time >= 56) && (Time <= 85)) {  // Smart lockdown
                Ci = (I.Value() * mu.Value() * 0.6 * F);
            } else if ((Time >= 86) && (Time <= 115)) {  // Short lockdown
                Ci = (I.Value() * mu.Value() * 0.1 * F);
            } else {  // After all lockdowns
                Ci = (I.Value() * mu.Value() * 0.5 * F);
            }
            break;
        case 3:
            if (Time <= 24) {  // Before lockdowns
                Ci = (I.Value() * mu.Value() * (S.Value() / s_init.Value()));
            } else if ((Time >= 25) && (Time <= 65)) {   // One medium
                Ci = (I.Value() * mu.Value() * 0.1 * F);
            } else if ((Time >= 66) && (Time <= 106)) {  // One smart
                Ci = (I.Value() * mu.Value() * 0.6 * F);
            } else {  // After all lockdowns
                Ci = (I.Value() * mu.Value() * 0.5 * F);
            }

            break;

        default:
            break;
      }
    }

    void Out() {
      Print("%g;%g;%g;%g;%g\n", Time, S.Value(), I.Value(), R.Value(), D.Value());
      if (verbose) {
         cout << Time << ' ' << S.Value() << ' ' << I.Value() << ' ' << R.Value() << ' ' << D.Value() << '\n';
      }
    }
};

Covid19 c19(0,0,0,0,0,0,0,0, 0);

void Sample() { 
  c19.Out();
  c19.Conditions();
}

Sampler S(Sample, StepPrn);

// experiment description:
int main(int argc, char *argv[]) {  
  get_params params(argc, argv);
  c19.SetParameters(params.beta, params.mu, params.S, params.it, params.Dd, params.Fr, params.HC, params.Fh, 0);
  verbose = params.verbose;
  type = params.type;
  SetOutput("covid19.csv");
//   Print("# Modeling containing covid-19 infection. A conceptual model.\n");
  if (verbose) {
    cout << "# Modeling containing covid-19 infection. A conceptual model.\n";
  }
  SetStep(1e-8,1e-3);   // set step size range
  Init(0,params.range); // experiment initialization 
  Run();                // simulation
}