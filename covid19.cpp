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
//      F   = S/S_init
//      HiC = SC / HC
//      SC  = I * Fh
//
//           | 3%  if Hic < 4
//           | 7%  if 4 <= HiC < 8
//      Fr = | 12%  if 8 <= HiC < 16
//           | 15% if 16 <= HiC
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
      if ( HiC > 30.0 ){
          Fr = 0.1;
      } else if ( HiC > 5 ){
          Fr = 0.07;
      } else{
          Fr = params.Fr;
      }
      
      double C;
      switch (params.type) {
        case 1:
            if (Time < 25) {  // Before lockdowns
                C = (I.Value() * params.mu);
            } 
            else if ((Time >= 25) && (Time <= 85)) {  // One short
                C = (I.Value() * params.mu * params.hi);  // hi = 0.1 is DEF_HI FIXME:
            } 
            else {                                  // After short lockdown
                C = (I.Value() * params.mu * params.q);  // q = 0.5 is DEF_HI FIXME:
            }
            break;
        case 2:
            if (Time <= 24) {  // Before lockdowns
                C = (I.Value() * params.mu);
            } else if ((Time >= 25) && (Time <= 55)) {  // Short lockdown
                C = (I.Value() * params.mu * params.hi);
            } else if ((Time >= 56) && (Time <= 85)) {  // Smart lockdown
                C = (I.Value() * params.mu * params.k);
            } else if ((Time >= 86) && (Time <= 115)) {  // Short lockdown
                C = (I.Value() * params.mu * params.hi);
            } else {  // After all lockdowns
                C = (I.Value() * params.mu * params.q);
            }
            break;
        case 3:
            if (Time <= 24) {  // Before lockdowns
                C = (I.Value() * params.mu);
            } else if ((Time >= 25) && (Time <= 65)) {   // One medium
                C = (I.Value() * params.mu * params.hi);
            } else if ((Time >= 66) && (Time <= 106)) {  // One smart
                C = (I.Value() * params.mu * params.k);
            } else {  // After all lockdowns
                C = (I.Value() * params.mu * params.q);
            }
            break;
        case 4:
            if (Time < 10) {
                C = (I.Value() * params.mu);
            } else if ((Time >= 10) && (Time < 21)) {
                C = (I.Value() * params.mu * (1 - params.o1));
            } else if ((Time >= 21) && (Time < 35)) {
                C = (I.Value() * params.mu * (1 - params.o1 - params.o2));
            } else if ((Time >= 35) && (Time < 39)) {
                C = (I.Value() * params.mu * (1 - params.o1 - params.o2 - params.o3));
            } else if ((Time >= 39) && (Time < 42)) {
                C = (I.Value() * params.mu * (1 - params.o1 - params.o2 - params.o3 - params.o4));
            } else if ((Time >= 42) && (Time < 51)) {
                C = (I.Value() * params.mu * (1 - params.o1 - params.o2 - params.o3 - params.o4 - params.o5));
            } else if ((Time >= 51) && (Time < 58)) {
                C = (I.Value() * params.mu * (1 - params.o1 - params.o2 - params.o3 - params.o4 - params.o5 - params.o6));
            } else if ((Time >= 58) && (Time < 79)) {
                C = (I.Value() * params.mu * (1 - params.o1 - params.o2 - params.o3 - params.o4 - params.o5 - params.o6 - params.o7));
            } else {
                C = (I.Value() * params.mu * (1 - params.o1 - params.o2 - params.o3 - params.o4 - params.o5 - params.o6 - params.o7 - params.o8));
            }
            break;
        case 0:
        default:
            C = (I.Value() * params.mu);
            break;
      }
      double F = S.Value() / params.S;
      Ci = C * F;
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
  Init(0,params.range); // experiment initialization 
  Run();                // simulation
}