#ifndef RPNCALCULATION_CALCULATION_H
#define RPNCALCULATION_CALCULATION_H

#include <escher.h>
#include <poincare.h>

namespace RPNCalculation {

class Calculation {
public:
  Calculation();
  ~Calculation(); // Delete expression and layout, if needed
  Calculation& operator=(const Calculation& other);
  Calculation(const Calculation& other) = delete;
  Calculation(Calculation&& other) = delete;
  Calculation& operator=(Calculation&& other) = delete;
  /* c.reset() is the equivalent of c = Calculation() without copy assingment. */
  void reset();
  const char * inputText();
  const char * outputText();
  Poincare::Expression * input();
  Poincare::ExpressionLayout * inputLayout();
  Poincare::Evaluation<double> * output(Poincare::Context * context);
  Poincare::ExpressionLayout * outputLayout(Poincare::Context * context);
  void setContent(const char * c, Poincare::Context * context);
  void setContent(Poincare::Evaluation<double> *, Poincare::Context * context);
  bool isEmpty();
  void tidy();
private:
  char m_outputText[2*::TextField::maxBufferSize()];
  Poincare::Expression * m_input;
  Poincare::ExpressionLayout * m_inputLayout;
  Poincare::Evaluation<double> * m_output;
  Poincare::ExpressionLayout * m_outputLayout;
};

}

#endif
