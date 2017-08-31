#include "app.h"
#include "../apps_container.h"
#include "calculation_icon.h"
#include "../i18n.h"

using namespace Poincare;

using namespace Shared;

namespace RPNCalculation {

I18n::Message App::Descriptor::name() {
  return I18n::Message::CalculRpnApp;
}

I18n::Message App::Descriptor::upperName() {
  return I18n::Message::CalculRpnAppCapital;
}

const Image * App::Descriptor::icon() {
  return ImageStore::CalculationIcon;
}

App * App::Snapshot::unpack(Container * container) {
  return new App(container, this);
}

void App::Snapshot::reset() {
  m_calculationStack.deleteAll();
}

App::Descriptor * App::Snapshot::descriptor() {
  static Descriptor descriptor;
  return &descriptor;
}

CalculationStore * App::Snapshot::calculationStore() {
  return &m_calculationStack;
}

void App::Snapshot::tidy() {
  m_calculationStack.tidy();
}

App::App(Container * container, Snapshot * snapshot) :
  TextFieldDelegateApp(container, snapshot, &m_editExpressionController),
  m_localContext((GlobalContext *)((AppsContainer *)container)->globalContext(), snapshot->calculationStore()),
  m_historyController(&m_editExpressionController, snapshot->calculationStore()),
  m_editExpressionController(&m_modalViewController, &m_historyController, snapshot->calculationStore())
{
}

Context * App::localContext() {
  return &m_localContext;
}

}
