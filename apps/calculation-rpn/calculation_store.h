#ifndef RPNCALCULATION_CALCULATION_STORE_H
#define RPNCALCULATION_CALCULATION_STORE_H

#include "calculation.h"

namespace RPNCalculation {

class CalculationStore {
public:
  CalculationStore();
  Calculation * calculationAtIndex(int i);
  Calculation * push(const char * text, Poincare::Context * context);
  Calculation * push(Poincare::Evaluation<double> * evaluation, Poincare::Context * context);
  void deleteCalculationAtIndex(int i);
  void deleteAll();
  int numberOfCalculations();
  void tidy();
  static constexpr int k_MaxStackSize = 64;
private:
  int m_startIndex;
  Calculation m_calculations[k_MaxStackSize];
};

}

#endif
