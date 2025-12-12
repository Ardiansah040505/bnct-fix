#include "RunAction.hh"

#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"

namespace B3 {

RunAction::RunAction()
 : G4UserRunAction()
{
  G4AccumulableManager::Instance()->RegisterAccumulable(fSumDose);
  G4AccumulableManager::Instance()->RegisterAccumulable(fSumBoronEdep);
}

RunAction::~RunAction()
{
  G4AccumulableManager::Instance()->UnRegisterAccumulable(&fSumDose);
  G4AccumulableManager::Instance()->UnRegisterAccumulable(&fSumBoronEdep);
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  // reset accumulables
  G4AccumulableManager::Instance()->Reset();
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // merge accumulables
  G4AccumulableManager::Instance()->Merge();

  G4double totalDose = fSumDose.GetValue();
  G4double totalBoronEdep = fSumBoronEdep.GetValue();

  G4cout << "------------------------------------------------------------" << G4endl;
  G4cout << "Run summary: " << nofEvents << " events." << G4endl;
  G4cout << " Total dose in patient : " << G4BestUnit(totalDose,"Dose") << G4endl;
  G4cout << " Total energy deposited in boron region : " << G4BestUnit(totalBoronEdep,"Energy") << G4endl;
  G4cout << "------------------------------------------------------------" << G4endl;
}

} // namespace B3
