#ifndef ReadoutSimDetectorConstruction_h
#define ReadoutSimDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4GenericMessenger.hh"

class DetectorMessenger;

class ReadoutSimDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        ReadoutSimDetectorConstruction();
        ~ReadoutSimDetectorConstruction();

        virtual G4VPhysicalVolume *Construct(); 

        // void SetGeometry(G4String name) {fGeometryName = name;};
    
    private:
        void DefineMaterials();
        void DefineCommands();
        void SetOpticalProperties();
        void SetSpace(G4int);
        void SetWLSBack(G4int);

        G4VPhysicalVolume* SetupPanelOnly();
        G4VPhysicalVolume* SetupPanelWithCladding();
        G4VPhysicalVolume* SetupBaselineDesign();
        G4VPhysicalVolume* SetupBaselineCladding();

        // G4String fGeometryName;

        G4LogicalVolume *fRightPENLayerLogical, *fLeftPENLayerLogical;
        G4LogicalVolume *fTopPENLayerLogical, *fBotPENLayerLogical;
        G4Material *worldMaterial;
        G4Material *PMMA, *PEN;
        G4Material *innerCladdingMaterial, *outerCladdingMaterial;

        G4double space;
        G4int WLS_y = 1;
        G4int centerGuide = 1;

        G4MaterialPropertiesTable *pmmaMPT, *penMPT, *larMPT, *innerCladdingMPT, *outerCladdingMPT;
};

#endif