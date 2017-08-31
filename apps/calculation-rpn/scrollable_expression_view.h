#ifndef RPNCALCULATION_SCROLLABLE_EXPRESSION_VIEW_H
#define RPNCALCULATION_SCROLLABLE_EXPRESSION_VIEW_H

#include <escher.h>

namespace RPNCalculation {

class ScrollableExpressionView : public ScrollableView, public ScrollViewDataSource {
public:
  ScrollableExpressionView(Responder * parentResponder);
  void setExpression(Poincare::ExpressionLayout * expressionLayout);
  void setBackgroundColor(KDColor backgroundColor);
  KDSize minimalSizeForOptimalDisplay() const override;
private:
  ExpressionView m_expressionView;
};

}

#endif
