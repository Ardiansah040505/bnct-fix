#include "ActionInitialization.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "StackingAction.hh"

namespace B3a {

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}
ActionInitialization::~ActionInitialization() = default;

void ActionInitialization::BuildForMaster() const
{
  // In MT mode, master can have RunAction only
  SetUserAction(new RunAction());
}

void ActionInitialization::Build() const
{
  auto runAction = new RunAction();
  SetUserAction(runAction);

  SetUserAction(new EventAction(runAction));
  SetUserAction(new PrimaryGeneratorAction());
  SetUserAction(new StackingAction());
}

} // namespace B3a
