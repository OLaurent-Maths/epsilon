#include "stack_view_cell.h"

#include "app.h"
#include "../constant.h"
#include "selectable_table_view.h"
#include <assert.h>
#include <string.h>

namespace RPNCalculation {

StackViewCell::StackViewCell(Responder * parentResponder) :
  Responder(parentResponder),
  m_outputView(this)
{
}

KDColor StackViewCell::backgroundColor() const {
  KDColor background = m_even ? Palette::WallScreen : KDColorWhite;
  return background;
}


int StackViewCell::numberOfSubviews() const {
  return 1;
}

View * StackViewCell::subviewAtIndex(int index) {
  View * views[1] = {&m_outputView};
  return views[index];
}

void StackViewCell::layoutSubviews() {
  KDCoordinate width = bounds().width();
  KDCoordinate height = bounds().height();
  KDSize outputSize = m_outputView.minimalSizeForOptimalDisplay();
  if (outputSize.width() + k_digitHorizontalMargin > width) {
    m_outputView.setFrame(KDRect(k_digitHorizontalMargin, 2*k_digitVerticalMargin, width - k_digitHorizontalMargin, height - 3*k_digitVerticalMargin));
  } else {
    m_outputView.setFrame(KDRect(width - outputSize.width() - k_digitHorizontalMargin,/* inputSize.height() +*/ 2*k_digitVerticalMargin, outputSize.width(), height /*- inputSize.height() */- 3*k_digitVerticalMargin));
  }
}

void StackViewCell::setCalculation(Calculation * calculation) {
  App * calculationApp = (App *)app();
  m_outputView.setExpression(calculation->outputLayout(calculationApp->localContext()));
}

void StackViewCell::reloadCell() {
  m_outputView.setBackgroundColor(backgroundColor());
  if (isHighlighted()) {
      m_outputView.setBackgroundColor(Palette::Select);
  }
  layoutSubviews();
  EvenOddCell::reloadCell();
  m_outputView.reloadScroll();
}

void StackViewCell::didBecomeFirstResponder() {
  app()->setFirstResponder(&m_outputView);
}

bool StackViewCell::handleEvent(Ion::Events::Event event) {
  if (event == Ion::Events::Down||
    event == Ion::Events::Up ) {
    CalculationSelectableTableView * tableView = (CalculationSelectableTableView *)parentResponder();
    tableView->scrollToSubviewOfTypeOfCellAtLocation(tableView->selectedColumn(), tableView->selectedRow());
    StackViewCell * selectedCell = (StackViewCell *)(tableView->selectedCell());

    app()->setFirstResponder(selectedCell);
    selectedCell->reloadCell();
    return true;
  }
  return false;
}

}
