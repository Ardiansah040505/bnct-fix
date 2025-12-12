#include "StackingAction.hh"

#include "G4Track.hh"
#include "G4NeutrinoE.hh"

namespace B3a {

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track)
{
  // keep primary
  if (track->GetParentID() == 0) return fUrgent;

  // kill neutrinos to save CPU
  if (track->GetDefinition() == G4NeutrinoE::NeutrinoE()) return fKill;

  // otherwise keep
  return fUrgent;
}

} // namespace B3a
