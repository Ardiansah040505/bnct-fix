#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

namespace B3a {

class G4GenericMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  ~DetectorConstruction() override;

  G4VPhysicalVolume* Construct() override;

  // setter used by messenger
  void SetShieldThickness(G4double val);

  // getter
  G4double GetShieldThickness() const { return fShieldThickness; }

private:
  void DefineMaterials();
  void ConstructVolumes();

private:
  G4LogicalVolume* fWorldLV;
  G4VPhysicalVolume* fWorldPV;

  G4bool fCheckOverlaps;

  // parameter that user can change
  G4double fShieldThickness; // in internal units

  // messenger for UI commands
  G4GenericMessenger* fMessenger;
};

} // namespace B3a

#endif // DETECTORCONSTRUCTION_H
