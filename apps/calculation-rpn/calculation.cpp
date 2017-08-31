#include "calculation.h"
#include <string.h>
#include <math.h>
using namespace Poincare;

namespace RPNCalculation {

Calculation::Calculation() :
  m_outputText(""),
  m_input(nullptr),
  m_inputLayout(nullptr),
  m_output(nullptr),
  m_outputLayout(nullptr)
{
}

Calculation::~Calculation() {
  if (m_inputLayout != nullptr) {
    delete m_inputLayout;
    m_inputLayout = nullptr;
  }
  if (m_input != nullptr) {
    delete m_input;
    m_input = nullptr;
  }
  if (m_output != nullptr) {
    delete m_output;
    m_output = nullptr;
  }
  if (m_outputLayout != nullptr) {
    delete m_outputLayout;
    m_outputLayout = nullptr;
  }
}

Calculation& Calculation::operator=(const Calculation& other) {
  const char * otherOutputText = other.m_outputText;
  reset();
  strlcpy(m_outputText, otherOutputText, sizeof(m_outputText));
  return *this;
}

void Calculation::reset() {
  m_outputText[0] = 0;
  tidy();
}

void Calculation::setContent(Poincare::Evaluation<double> * evaluation, Context * context) {
  reset();
  m_input = evaluation;
  evaluation->writeTextInBuffer(m_outputText, sizeof(m_outputText));
}

void Calculation::setContent(const char * c, Context * context) {
  reset();
  m_input = Expression::parse(c);
  Evaluation<double> * evaluation = m_input->evaluate<double>(*context);
  evaluation->writeTextInBuffer(m_outputText, sizeof(m_outputText));
  delete evaluation;
}

const char * Calculation::outputText() {
  return m_outputText;
}

Expression * Calculation::input() {
  return m_input;
}

ExpressionLayout * Calculation::inputLayout() {
  if (m_inputLayout == nullptr && input() != nullptr) {
    m_inputLayout = input()->createLayout(Expression::FloatDisplayMode::Decimal, Expression::ComplexFormat::Cartesian);
  }
  return m_inputLayout;
}

Evaluation<double> * Calculation::output(Context * context) {
  if (m_output == nullptr) {
    /* To ensure that the expression 'm_output' is a matrix or a complex, we
     * call 'evaluate'. */
    Expression * exp = Expression::parse(m_outputText);
    if (exp != nullptr) {
      m_output = exp->evaluate<double>(*context);
      delete exp;
    } else {
      m_output = new Complex<double>(Complex<double>::Float(NAN));
    }
  }
  return m_output;
}

ExpressionLayout * Calculation::outputLayout(Context * context) {
  if (m_outputLayout == nullptr && output(context) != nullptr) {
    m_outputLayout = output(context)->createLayout();
  }
  return m_outputLayout;
}

bool Calculation::isEmpty() {
  /* To test if a calculation is empty, we need to test either m_inputText or
   * m_outputText, the only two fields that are not lazy-loaded. We choose
   * m_outputText to consider that a calculation being added is still empty
   * until the end of the method 'setContent'. Indeed, during 'setContent'
   * method, 'ans' evaluation calls the evaluation of the last calculation
   * only if the calculation being filled is not taken into account.*/
  if (strlen(m_outputText) == 0) {
    return true;
  }
  return false;
}

void Calculation::tidy() {
  if (m_input != nullptr) {
    delete m_input;
  }
  m_input = nullptr;
  if (m_inputLayout != nullptr) {
    delete m_inputLayout;
  }
  m_inputLayout = nullptr;
  if (m_output != nullptr) {
    delete m_output;
  }
  m_output = nullptr;
  if (m_outputLayout != nullptr) {
    delete m_outputLayout;
  }
  m_outputLayout = nullptr;
}

}
