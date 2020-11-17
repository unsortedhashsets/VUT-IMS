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

const double StepPrn = 1; // step of output
get_params params;

class Covid19 {
   public:
    Parameter Fr, Ci;
    Integrator S, I, R, D;
    Covid19()
        : Fr(params.Fr),
          Ci(0),
          S(-params.beta * Ci / params.it, params.S),
          I (params.beta * Ci / params.it - I / params.Dd, params.I),
          R(I / params.Dd * (1 - Fr), params.R),
          D(I / params.Dd * Fr, params.D) {}

    void SetParameters() {
        Fr = params.Fr;
        S.Init(params.S);
        I.Init(params.I);
        R.Init(params.R);
        D.Init(params.D);
    }

    void Conditions() {
      double SC = I.Value() * params.Fh;
      double HiC = SC / params.HC;
      if ( HiC > 16.0 ){
          Fr = 0.15;
      } else if ( HiC > 8.0 ){
          Fr = 0.12;
      } else if ( HiC > 4.0 ){
          Fr = 0.07;
      } else{
          Fr = params.Fr;
      }
      
      double F = S.Value() / params.S;
      switch (params.type) {
        case 1:
            if (Time < 25) {  // Before lockdowns
                Ci = (I.Value() * params.mu * F);
            } 
            else if ((Time >= 25) && (Time <= 85)) {  // One short
                Ci = (I.Value() * params.mu * params.hi * F);  // hi = 0.1 is DEF_HI FIXME:
            } 
            else {                                  // After short lockdown
                Ci = (I.Value() * params.mu * params.q * F);  // q = 0.5 is DEF_HI FIXME:
            }
            break;
        case 2:
            if (Time <= 24) {  // Before lockdowns
                Ci = (I.Value() * params.mu * F);
            } else if ((Time >= 25) && (Time <= 55)) {  // Short lockdown
                Ci = (I.Value() * params.mu * params.hi * F);
            } else if ((Time >= 56) && (Time <= 85)) {  // Smart lockdown
                Ci = (I.Value() * params.mu * params.k * F);
            } else if ((Time >= 86) && (Time <= 115)) {  // Short lockdown
                Ci = (I.Value() * params.mu * params.hi * F);
            } else {  // After all lockdowns
                Ci = (I.Value() * params.mu * params.q * F);
            }
            break;
        case 3:
            if (Time <= 24) {  // Before lockdowns
                Ci = (I.Value() * params.mu * F);
            } else if ((Time >= 25) && (Time <= 65)) {   // One medium
                Ci = (I.Value() * params.mu * params.hi * F);
            } else if ((Time >= 66) && (Time <= 106)) {  // One smart
                Ci = (I.Value() * params.mu * params.k * F);
            } else {  // After all lockdowns
                Ci = (I.Value() * params.mu * params.q * F);
            }
            break;
        case 4:
            if (Time < 10) {
                Ci = (I.Value() * params.mu * F);
            } else if ((Time >= 10) && (Time < 21)) {
                Ci = (I.Value() * params.mu * F * (1 - 0.05));
            } else if ((Time >= 21) && (Time < 35)) {
                Ci = (I.Value() * params.mu * F * (1 - 0.05 - 0.03));
            } else if ((Time >= 35) && (Time < 39)) {
                Ci = (I.Value() * params.mu * F * (1 - 0.05 - 0.03 - 0.03));
            } else if ((Time >= 39) && (Time < 42)) {
                Ci = (I.Value() * params.mu * F * (1 - 0.05 - 0.03 - 0.03 - 0.02));
            } else if ((Time >= 42) && (Time < 51)) {
                Ci = (I.Value() * params.mu * F * (1 - 0.05 - 0.03 - 0.03 - 0.02 - 0.26));
            } else if ((Time >= 51) && (Time < 58)) {
                Ci = (I.Value() * params.mu * F * (1 - 0.05 - 0.03 - 0.03 - 0.02 - 0.26 - 0.05));
            } else if ((Time >= 58) && (Time < 79)) {
                Ci = (I.Value() * params.mu * F * (1 - 0.05 - 0.03 - 0.03 - 0.02 - 0.26 - 0.05 - 0.05));
            } else {
                Ci = (I.Value() * params.mu * F * (1 - 0.05 - 0.03 - 0.03 - 0.02 - 0.26 - 0.05 - 0.05 - 0.01));
            }
            break;
        default:
            Ci = (I.Value() * params.mu * F);
            break;
      }
    }

    void Out() {
      Print("%g;%g;%g;%g;%g\n", Time, S.Value(), I.Value(), R.Value(), D.Value());
      if (params.verbose) {
         cout << Time << '\t' << S.Value() << '\t' << I.Value() << '\t' << R.Value() << '\t' << D.Value() << '\n';
      }
    }
};


Covid19 c19;

void Sample() { 
  c19.Out();
  c19.Conditions();
}

Sampler S(Sample, StepPrn);

// experiment description:
int main(int argc, char *argv[]) {
  params.get_arguments(argc, argv);
  c19.SetParameters();
  SetOutput("covid19.csv");
//   Print("# Modeling containing covid-19 infection. A conceptual model.\n");
  if (params.verbose) {
    cout << "# Modeling containing covid-19 infection. A conceptual model.\n";
  }
  SetStep(1e-8,1e-3);   // set step size range
  Init(0,params.range); // experiment initialization 
  Run();                // simulation
}