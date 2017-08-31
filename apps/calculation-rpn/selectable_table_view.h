#ifndef RPNCALCULATION_SELECTABLE_TABLE_VIEW_H
#define RPNCALCULATION_SELECTABLE_TABLE_VIEW_H

#include <escher.h>
#include "stack_view_cell.h"
namespace RPNCalculation {

class CalculationSelectableTableView : public ::SelectableTableView {
public:
  CalculationSelectableTableView(Responder * parentResponder, TableViewDataSource * dataSource,
    SelectableTableViewDataSource * selectionDataSource, SelectableTableViewDelegate * delegate = nullptr);
  void scrollToCell(int i, int j) override;
  void scrollToSubviewOfTypeOfCellAtLocation(int i, int j);
};

}

#endif
