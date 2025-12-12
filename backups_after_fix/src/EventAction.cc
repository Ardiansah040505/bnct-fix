#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

namespace B3 {

EventAction::EventAction(RunAction* runAction)
 : G4UserEventAction(),
   fRunAction(runAction),
   fCollID_patient(-1),
   fCollID_boron(-1)
{}

EventAction::~EventAction() = default;

void EventAction::BeginOfEventAction(const G4Event*) {}

void EventAction::EndOfEventAction(const G4Event* event)
{
  auto HCE = event->GetHCofThisEvent();
  if (!HCE) return;

  // get collection IDs once
  if (fCollID_patient < 0) {
    G4SDManager* SDMan = G4SDManager::GetSDMpointer();
    fCollID_patient = SDMan->GetCollectionID("patient/dose");
    fCollID_boron = SDMan->GetCollectionID("boron/edep");
  }

  // patient dose
  if (fCollID_patient >= 0) {
    auto evtMap = (G4THitsMap<G4double>*)(HCE->GetHC(fCollID_patient));
    if (evtMap) {
      G4double dose = 0.;
      for (auto itr = evtMap->GetMap()->begin(); itr != evtMap->GetMap()->end(); ++itr) {
        dose += *(itr->second);
      }
      if (dose > 0.) fRunAction->SumDose(dose);
    }
  }

  // boron edep (as proxy)
  if (fCollID_boron >= 0) {
    auto evtMapB = (G4THitsMap<G4double>*)(HCE->GetHC(fCollID_boron));
    if (evtMapB) {
      G4double edepB = 0.;
      for (auto itr = evtMapB->GetMap()->begin(); itr != evtMapB->GetMap()->end(); ++itr) {
        edepB += *(itr->second);
      }
      if (edepB > 0.) fRunAction->CountBoronEdep(edepB);
    }
  }
}

} // namespace B3
