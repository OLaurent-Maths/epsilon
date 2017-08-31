#include "history_controller.h"
#include "app.h"
#include "../apps_container.h"
#include <assert.h>

namespace RPNCalculation {

HistoryController::HistoryController(Responder * parentResponder, CalculationStore * calculationStore) :
  DynamicViewController(parentResponder),
  m_calculationHistory{},
  m_calculationStore(calculationStore)
{
}

void HistoryController::reload() {
  selectableTableView()->reloadData();
}

void HistoryController::didBecomeFirstResponder() {
  selectCellAtLocation(0, numberOfRows()-1);
  app()->setFirstResponder(selectableTableView());
}

void HistoryController::willExitResponderChain(Responder * nextFirstResponder) {
  if (nextFirstResponder == parentResponder()) {
    selectableTableView()->deselectTable();
  }
}

bool HistoryController::handleEvent(Ion::Events::Event event) {
  if (event == Ion::Events::Down) {
    selectableTableView()->deselectTable();
    app()->setFirstResponder(parentResponder());
    return true;
  }
  if (event == Ion::Events::Up) {
    return true;
  }
  if (event == Ion::Events::OK || event == Ion::Events::EXE) {
    int focusRow = selectedRow();
    StackViewCell * selectedCell = (StackViewCell *)selectableTableView()->selectedCell();
    EditExpressionController * editController = (EditExpressionController *)parentResponder();
    selectableTableView()->deselectTable();
    app()->setFirstResponder(editController);
    Calculation * calculation = m_calculationStore->calculationAtIndex(focusRow);
    editController->insertTextBody(calculation->outputText());
    return true;
  }
  if (event == Ion::Events::Backspace) {
    int focusRow = selectedRow();
    StackViewCell * selectedCell = (StackViewCell *)selectableTableView()->selectedCell();
    selectableTableView()->deselectTable();
    EditExpressionController * editController = (EditExpressionController *)parentResponder();
    m_calculationStore->deleteCalculationAtIndex(focusRow);
    reload();
    if (numberOfRows()== 0) {
      app()->setFirstResponder(editController);
      return true;
    }
    if (focusRow > 0) {
      selectableTableView()->selectCellAtLocation(0, focusRow-1);
    } else {
      selectableTableView()->selectCellAtLocation(0, 0);
    }

    tableViewDidChangeSelection(selectableTableView(), 0, -1);
    selectableTableView()->scrollToCell(0, selectedRow());
    return true;
  }
  if (event == Ion::Events::Clear) {
    selectableTableView()->deselectTable();
    m_calculationStore->deleteAll();
    reload();
    app()->setFirstResponder(parentResponder());
    return true;
  }
  if (event == Ion::Events::Back) {
    EditExpressionController * editController = (EditExpressionController *)parentResponder();
    selectableTableView()->deselectTable();
    app()->setFirstResponder(editController);
    return true;
  }
  if (event == Ion::Events::Copy) {
    StackViewCell * selectedCell = (StackViewCell *)selectableTableView()->selectedCell();
    int focusRow = selectedRow();
    Calculation * calculation = m_calculationStore->calculationAtIndex(focusRow);
    Clipboard::sharedClipboard()->store(calculation->outputText());
    return true;
  }
  return false;
}

void HistoryController::tableViewDidChangeSelection(SelectableTableView * t, int previousSelectedCellX, int previousSelectedCellY) {
  StackViewCell * selectedCell = (StackViewCell *)(t->selectedCell());
  if (selectedCell == nullptr) {
    return;
  }
  app()->setFirstResponder(selectedCell);
  selectedCell->reloadCell();
}

int HistoryController::numberOfRows() {
  return m_calculationStore->numberOfCalculations();
};

HighlightCell * HistoryController::reusableCell(int index, int type) {
  assert(type == 0);
  assert(index >= 0);
  assert(index < k_maxNumberOfDisplayedRows);
  return m_calculationHistory[index];
}

int HistoryController::reusableCellCount(int type) {
  assert(type == 0);
  return k_maxNumberOfDisplayedRows;
}

void HistoryController::willDisplayCellForIndex(HighlightCell * cell, int index) {
  StackViewCell * myCell = (StackViewCell *)cell;
  myCell->setCalculation(m_calculationStore->calculationAtIndex(index));
  myCell->setEven(index%2 == 0);
}

KDCoordinate HistoryController::rowHeight(int j) {
  if (j >= m_calculationStore->numberOfCalculations()) {
    return 0;
  }
  Calculation * calculation = m_calculationStore->calculationAtIndex(j);
  App * calculationApp = (App *)app();
  KDCoordinate outputHeight = calculation->outputLayout(calculationApp->localContext())->size().height();
  return outputHeight + 2*StackViewCell::k_digitVerticalMargin;
}

KDCoordinate HistoryController::cumulatedHeightFromIndex(int j) {
  int result = 0;
  for (int k = 0; k < j; k++) {
    result += rowHeight(k);
  }
  return result;
}

int HistoryController::indexFromCumulatedHeight(KDCoordinate offsetY) {
  int result = 0;
  int j = 0;
  while (result < offsetY && j < numberOfRows()) {
    result += rowHeight(j++);
  }
  return (result < offsetY || offsetY == 0) ? j : j - 1;
}

int HistoryController::typeAtLocation(int i, int j) {
  return 0;
}

void HistoryController::scrollToCell(int i, int j) {
  selectableTableView()->scrollToCell(i, j);
}

CalculationSelectableTableView * HistoryController::selectableTableView() {
  return (CalculationSelectableTableView *)view();
}

View * HistoryController::loadView() {
  CalculationSelectableTableView * tableView = new CalculationSelectableTableView(this, this, this, this);
for (int i = 0; i < k_maxNumberOfDisplayedRows; i++) {
    m_calculationHistory[i] = new StackViewCell(tableView);
  }
  return tableView;
}

void HistoryController::unloadView(View * view) {
  for (int i = 0; i < k_maxNumberOfDisplayedRows; i++) {
    delete m_calculationHistory[i];
    m_calculationHistory[i] = nullptr;
  }
  delete view;
}

}
