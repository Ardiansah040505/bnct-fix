#ifndef RUNACTION_H
#define RUNACTION_H

#include "G4UserRunAction.hh"
#include "G4Types.hh"

class G4Run;

namespace B3a {

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  ~RunAction() override;

  void BeginOfRunAction(const G4Run* run) override;
  void EndOfRunAction(const G4Run* run) override;

  // called from EventAction
  void SumDose(G4double dose) { fSumDose += dose; }
  void CountBoronEdep(G4double e) { fSumBoronEdep += e; }

private:
  G4double fSumDose;
  G4double fSumBoronEdep;
};

} // namespace B3a

#endif // RUNACTION_H
