#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

// Use Hadron elastic HP if available
#include "G4HadronElasticPhysicsHP.hh"

#include "G4SystemOfUnits.hh"

namespace B3a {

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  RegisterPhysics(new G4EmStandardPhysics());
  RegisterPhysics(new G4DecayPhysics());

  // Hadronic / neutron HP (elastic HP). This may require NDL/G4NDL data.
  RegisterPhysics(new G4HadronElasticPhysicsHP());

  RegisterPhysics(new G4RadioactiveDecayPhysics());
}

void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}

} // namespace B3a
