#include <poincare/expression.h>
#include "expression_parser.hpp"
#include "expression_lexer.hpp"

int poincare_expression_yyparse(yyscan_t scanner, Expression ** expressionOutput);

void CreateFromString(char * string) {
  yyscan_t scanner;
  YY_BUFFER_STATE buf;
  poincare_expression_yylex_init(&scanner);
  buf = poincare_expression_yy_scan_string(string, scanner);
  void * selector = 0;
  Expression * expression = 0;
  poincare_expression_yyparse(scanner, &expression);


  expression->recursiveLayout();
  expression->m_frame.origin = KDPOINT(0, 100);
  expression->recursiveDraw();

  poincare_expression_yy_delete_buffer(buf, scanner);
  poincare_expression_yylex_destroy(scanner);
}


void Expression::forEachChild(ExpressionAction action) {
  Expression ** childPointer = children();
  if (childPointer == NULL) {
    return;
  }
  while(*childPointer != NULL) {
    (*childPointer->*action)();
    childPointer++;
  }
}

void Expression::recursiveLayout() {
  forEachChild(&Expression::recursiveLayout);
  layout();
}

void Expression::recursiveDraw() {
  KDPoint origin = KDGetOrigin();
  KDSetOrigin(KDPOINT(origin.x + m_frame.origin.x,
        origin.y + m_frame.origin.y));

  forEachChild(&Expression::recursiveDraw);

  draw();

  KDSetOrigin(origin);
}

void Expression::draw() {
  // No-op by default
}

void Expression::layout() {
  // No-op by default
}
