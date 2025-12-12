#ifndef EVENTACTION_H
#define EVENTACTION_H

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "RunAction.hh"   // ensure RunAction is visible

class G4HCofThisEvent;

namespace B3a {

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction* runAction);
  ~EventAction() override;

  void BeginOfEventAction(const G4Event*) override;
  void EndOfEventAction(const G4Event*) override;

private:
  RunAction* fRunAction;

  G4int fCollID_patient;
  G4int fCollID_boron;
};

} // namespace B3a

#endif // EVENTACTION_H
