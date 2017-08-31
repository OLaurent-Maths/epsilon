#ifndef RPNCALCULATION_HISTORY_VIEW_CELL_H
#define RPNCALCULATION_HISTORY_VIEW_CELL_H

#include <escher.h>
#include "calculation.h"
#include "scrollable_expression_view.h"

namespace RPNCalculation {

class StackViewCell : public ::EvenOddCell, public Responder {
public:
  StackViewCell(Responder * parentResponder);
  void reloadCell() override;
  KDColor backgroundColor() const override;
  void setCalculation(Calculation * calculation);
  int numberOfSubviews() const override;
  View * subviewAtIndex(int index) override;
  void layoutSubviews() override;
  void didBecomeFirstResponder() override;
  bool handleEvent(Ion::Events::Event event) override;
  constexpr static KDCoordinate k_digitHorizontalMargin = 10;
  constexpr static KDCoordinate k_digitVerticalMargin = 5;
private:
  constexpr static KDCoordinate k_resultWidth = 80;
  ScrollableExpressionView m_outputView;
};

}

#endif
