#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Neutron.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"

namespace B3a {

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr)
{
  G4int npar = 1;
  fParticleGun = new G4ParticleGun(npar);

  auto ptable = G4ParticleTable::GetParticleTable();
  auto neutron = G4Neutron::Definition();
  fParticleGun->SetParticleDefinition(neutron);
  fParticleGun->SetParticleEnergy(1.*MeV);   // default energy - adjust for BNCT as needed
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -300.*cm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

} // namespace B3a
