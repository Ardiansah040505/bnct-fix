#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4SDManager.hh"

#include "G4GenericMessenger.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"   // for G4BestUnit
#include "G4UIcommand.hh"

#include <iomanip>

namespace B3a
{

// Constructor
DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fWorldLV(nullptr), fWorldPV(nullptr),
   fCheckOverlaps(true),
   fShieldThickness(120.*cm),
   fMessenger(nullptr)
{
  DefineMaterials();

  // Setup generic messenger to allow changing shield thickness from UI/macros
    fMessenger = new G4GenericMessenger(this, "/B3/detector/", "Detector control");
  // thread-safe method with unit support
  fMessenger->DeclareMethodWithUnit("setShieldThickness", "cm",
                                    &DetectorConstruction::SetShieldThickness,
                                    "Set neutron shielding thickness");

}

// Destructor
DetectorConstruction::~DetectorConstruction()
{
  delete fMessenger;
}

void DetectorConstruction::DefineMaterials()
{
  auto nist = G4NistManager::Instance();

  // ensure common materials are available
  nist->FindOrBuildMaterial("G4_AIR");
  nist->FindOrBuildMaterial("G4_WATER");
  nist->FindOrBuildMaterial("G4_Pb");
  nist->FindOrBuildMaterial("G4_Al");
  nist->FindOrBuildMaterial("G4_Bi");

  // define a simple Boron-10 enriched example material (optional)
  G4double a = 10.0129*g/mole;
  G4Isotope* isoB10 = new G4Isotope("B10", 5, 10, a);
  G4Element* elB10 = new G4Element("Boron10", "B10", 1);
  elB10->AddIsotope(isoB10, 100.*perCent);
  G4double density = 2.34*g/cm3;
  G4Material* boronMat = new G4Material("BoronEnriched", density, 1);
  boronMat->AddElement(elB10, 1.0);
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  ConstructVolumes();
  return fWorldPV;
}

void DetectorConstruction::ConstructVolumes()
{
  auto nist = G4NistManager::Instance();

  // --- World ---
  G4double worldX = 600.*cm;
  G4double worldY = 300.*cm;
  G4double worldZ = 700.*cm;

  auto solidWorld = new G4Box("World", 0.5*worldX, 0.5*worldY, 0.5*worldZ);
  auto logicWorld = new G4LogicalVolume(solidWorld, nist->FindOrBuildMaterial("G4_AIR"), "WorldLV");
  fWorldLV = logicWorld;
  fWorldPV = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, fCheckOverlaps);

  // We'll place components along z axis; beam travels in +z
  G4double zpos = -300.*cm;

  // -------------------------
  // 1) Neutron shielding (large box) : thickness controlled by fShieldThickness
  // -------------------------
  G4double shield_thick = fShieldThickness;
  G4double shieldX = 220.*cm;
  G4double shieldY = 220.*cm;

  auto solidShield = new G4Box("NeutronShield", 0.5*shieldX, 0.5*shieldY, 0.5*shield_thick);
  auto shieldMat = nist->FindOrBuildMaterial("G4_Pb"); // placeholder; replace if desired
  auto logicShield = new G4LogicalVolume(solidShield, shieldMat, "NeutronShieldLV");
  new G4PVPlacement(nullptr, G4ThreeVector(0.,0., zpos + 0.5*shield_thick), logicShield, "NeutronShield", logicWorld, false, 0, fCheckOverlaps);

  // advance z beyond shield
  zpos += shield_thick + 2.*cm;

  // -------------------------
  // 2) Reflector (box)
  // -------------------------
  G4double reflector_z = 50.*cm;
  auto solidReflector = new G4Box("Reflector", 0.5*120.*cm, 0.5*120.*cm, 0.5*reflector_z);
  auto logicReflector = new G4LogicalVolume(solidReflector, nist->FindOrBuildMaterial("G4_Al"), "ReflectorLV");
  new G4PVPlacement(nullptr, G4ThreeVector(0.,0., zpos + 0.5*reflector_z), logicReflector, "Reflector", logicWorld, false, 0, fCheckOverlaps);

  // -------------------------
  // 3) Moderator inside reflector (water as placeholder)
  // -------------------------
  G4double moderator_z = 30.*cm;
  auto solidModerator = new G4Box("Moderator", 0.5*60.*cm, 0.5*60.*cm, 0.5*moderator_z);
  auto logicModerator = new G4LogicalVolume(solidModerator, nist->FindOrBuildMaterial("G4_WATER"), "ModeratorLV");
  G4double mod_zpos = zpos + 0.5*reflector_z - 0.5*moderator_z;
  new G4PVPlacement(nullptr, G4ThreeVector(0.,0., mod_zpos), logicModerator, "Moderator", logicWorld, false, 0, fCheckOverlaps);

  // advance z beyond reflector
  zpos += reflector_z + 2.*cm;

  // -------------------------
  // 4) Bismuth filter (disk)
  // -------------------------
  G4double bi_radius = 8.*cm;
  G4double bi_thick = 1.0*cm;
  auto solidBi = new G4Tubs("BiFilter", 0., bi_radius, 0.5*bi_thick, 0., twopi);
  auto logicBi = new G4LogicalVolume(solidBi, nist->FindOrBuildMaterial("G4_Bi"), "BiFilterLV");
  new G4PVPlacement(nullptr, G4ThreeVector(0.,0., zpos + 0.5*bi_thick), logicBi, "BiFilter", logicWorld, false, 0, fCheckOverlaps);

  zpos += bi_thick + 1.*cm;

  // -------------------------
  // 5) Fast neutron filter (thin disk; Al placeholder)
  // -------------------------
  G4double ffilter_radius = 8.*cm;
  G4double ffilter_thick = 1.0*cm;
  auto solidFF = new G4Tubs("FastFilter", 0., ffilter_radius, 0.5*ffilter_thick, 0., twopi);
  auto logicFF = new G4LogicalVolume(solidFF, nist->FindOrBuildMaterial("G4_Al"), "FastFilterLV");
  new G4PVPlacement(nullptr, G4ThreeVector(0.,0., zpos + 0.5*ffilter_thick), logicFF, "FastFilter", logicWorld, false, 0, fCheckOverlaps);

  zpos += ffilter_thick + 1.*cm;

  // -------------------------
  // 6) Aperture / Collimator (Pb annulus approximated by a full tube)
  // -------------------------
  G4double aperture_outer = 10.*cm;
  G4double aperture_inner = 0.*cm;
  G4double aperture_thick = 2.*cm;
  auto solidAperture = new G4Tubs("Aperture", aperture_inner, aperture_outer, 0.5*aperture_thick, 0., twopi);
  auto apertureMat = nist->FindOrBuildMaterial("G4_Pb");
  auto logicAperture = new G4LogicalVolume(solidAperture, apertureMat, "ApertureLV");
  new G4PVPlacement(nullptr, G4ThreeVector(0.,0., zpos + 0.5*aperture_thick), logicAperture, "Aperture", logicWorld, false, 0, fCheckOverlaps);

  zpos += aperture_thick + 2.*cm;

  // -------------------------
  // 7) Air gap (beam exit)
  // -------------------------
  G4double air_gap_z = 30.*cm;
  auto solidAirGap = new G4Box("AirGap", 0.5*50.*cm, 0.5*50.*cm, 0.5*air_gap_z);
  auto logicAirGap = new G4LogicalVolume(solidAirGap, nist->FindOrBuildMaterial("G4_AIR"), "AirGapLV");
  new G4PVPlacement(nullptr, G4ThreeVector(0.,0., zpos + 0.5*air_gap_z), logicAirGap, "AirGap", logicWorld, false, 0, fCheckOverlaps);

  zpos += air_gap_z + 2.*cm;

  // -------------------------
  // 8) Patient (cylinder)
  // -------------------------
  G4double patient_radius = 8.*cm;
  G4double patient_dz = 10.*cm;
  auto solidPatient = new G4Tubs("Patient", 0., patient_radius, 0.5*patient_dz, 0., twopi);
  auto patientMat = nist->FindOrBuildMaterial("G4_BRAIN_ICRP");
  auto logicPatient = new G4LogicalVolume(solidPatient, patientMat, "PatientLV");
  new G4PVPlacement(nullptr, G4ThreeVector(0.,0., zpos + 0.5*patient_dz), logicPatient, "Patient", logicWorld, false, 0, fCheckOverlaps);

  // -------------------------
  // Visualization attributes
  // -------------------------
  logicShield->SetVisAttributes(new G4VisAttributes(G4Colour(0.9,0.6,0.6)));      // reddish shielding
  logicReflector->SetVisAttributes(new G4VisAttributes(G4Colour(0.9,0.9,0.6)));    // yellowish reflector
  logicModerator->SetVisAttributes(new G4VisAttributes(G4Colour(0.9,0.4,0.7)));    // magenta moderator
  logicBi->SetVisAttributes(new G4VisAttributes(G4Colour(0.1,0.6,0.1)));          // dark green bismuth
  logicFF->SetVisAttributes(new G4VisAttributes(G4Colour(0.9,0.9,0.2)));          // bright yellow fast filter
  logicAperture->SetVisAttributes(new G4VisAttributes(G4Colour(0.6,0.3,0.0)));     // brown aperture
  logicAirGap->SetVisAttributes(new G4VisAttributes(G4Colour(0.6,0.9,0.95)));     // cyan air/room
  logicPatient->SetVisAttributes(new G4VisAttributes(G4Colour(0.2,0.2,0.9)));     // blue patient

  // -------------------------
  // Sensitive detectors / scoring
  // -------------------------
  // Patient: dose
  auto patientSD = new G4MultiFunctionalDetector("patient");
  G4SDManager::GetSDMpointer()->AddNewDetector(patientSD);
  G4VPrimitiveScorer* primDose = new G4PSDoseDeposit("dose");
  patientSD->RegisterPrimitive(primDose);
  logicPatient->SetSensitiveDetector(patientSD);

  // FastFilter (example "boron-like" region) scoring of edep
  auto boronSD = new G4MultiFunctionalDetector("boron");
  G4SDManager::GetSDMpointer()->AddNewDetector(boronSD);
  G4VPrimitiveScorer* primB = new G4PSEnergyDeposit("edep");
  boronSD->RegisterPrimitive(primB);
  logicFF->SetSensitiveDetector(boronSD);

  // done
}

void DetectorConstruction::SetShieldThickness(G4double val)
{
  fShieldThickness = val;

  G4cout << ">>> DetectorConstruction: new shield thickness = "
         << G4BestUnit(fShieldThickness, "Length") << G4endl;

  auto rm = G4RunManager::GetRunManager();
  if (rm) {
    rm->ReinitializeGeometry();
  }
}

} // namespace B3a
