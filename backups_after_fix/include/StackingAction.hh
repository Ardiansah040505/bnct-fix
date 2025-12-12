#ifndef STACKINGACTION_H
#define STACKINGACTION_H

#include "G4UserStackingAction.hh"

class G4Track;

namespace B3 {

class StackingAction : public G4UserStackingAction
{
public:
  StackingAction() = default;
  ~StackingAction() override = default;

  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* track) override;
};

} // namespace B3

#endif // STACKINGACTION_H
