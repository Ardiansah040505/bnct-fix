#ifndef EVENTACTION_H
#define EVENTACTION_H

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4HCofThisEvent;
class RunAction;

namespace B3 {

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

} // namespace B3

#endif // EVENTACTION_H
