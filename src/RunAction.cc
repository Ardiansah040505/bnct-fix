#include "RunAction.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"

namespace B3a {

RunAction::RunAction()
 : G4UserRunAction(),
   fSumDose(0.0),
   fSumBoronEdep(0.0)
{
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  // reset accumulators
  fSumDose = 0.0;
  fSumBoronEdep = 0.0;
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  G4cout << "------------------------------------------------------------" << G4endl;
  G4cout << "Run summary: " << nofEvents << " events." << G4endl;
  G4cout << " Total dose in patient : " << G4BestUnit(fSumDose,"Dose") << G4endl;
  G4cout << " Total energy deposited in boron region : " << G4BestUnit(fSumBoronEdep,"Energy") << G4endl;
  G4cout << "------------------------------------------------------------" << G4endl;
}

} // namespace B3a
