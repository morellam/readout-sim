#include "ReadoutSimDetectorConstruction.hh"
// #include "ReadoutSimDetectorMessenger.hh"

#include "G4Element.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"

ReadoutSimDetectorConstruction::ReadoutSimDetectorConstruction()
{
    pmmaMPT = new G4MaterialPropertiesTable();
    penMPT  = new G4MaterialPropertiesTable();
    larMPT  = new G4MaterialPropertiesTable();
    innerCladdingMPT = new G4MaterialPropertiesTable();
    outerCladdingMPT = new G4MaterialPropertiesTable();

    // fDetectorMessenger = new DetectorMessenger(this);
}

ReadoutSimDetectorConstruction::~ReadoutSimDetectorConstruction()
{
    // delete fDetectorMessenger;
}

G4VPhysicalVolume *ReadoutSimDetectorConstruction::Construct() 
{
    DefineMaterials();

    SetOpticalProperties();

    // if (fGeometryName == "baseline")                 {return SetupBaselineDesign();}
    // else if (fGeometryName == "alternative")         {return SetupPanelOnly();}
    // else if (fGeometryName == "baselineCladding")    {return SetupBaselineCladding();}
    // else if (fGeometryName == "alternativeCladding") {return SetupPanelWithCladding();}

    return SetupBaselineDesign();
    // return SetupPanelOnly();
    // return SetupBaselineCladding();
    // return SetupPanelWithCladding();
}

void ReadoutSimDetectorConstruction::DefineMaterials()
{
    auto* H  = new G4Element("Hydrogen", "H", 1., 1.00794 * g / mole);
    auto* C  = new G4Element("Carbon",   "C", 6., 12.011 * g / mole);
    auto* O  = new G4Element("Oxygen",   "O", 8., 16.00 * g / mole);

    G4NistManager *nist = G4NistManager::Instance();
    worldMaterial = nist->FindOrBuildMaterial("G4_lAr");

    // Estimated using the number of elements per chain elements  (C_5 O_2 H_8)
    PMMA = new G4Material("PMMA", 1.18 * g / cm3, 3);
    PMMA->AddElement(H, 0.08);
    PMMA->AddElement(C, 0.60);
    PMMA->AddElement(O, 0.32);

    // Estimated using the number of elements per chain elements  (C_14 H_10 O_4)
    PEN = new G4Material("PEN", 1.36 * g / cm3, 3);
    PEN->AddElement(H, 0.0413);
    PEN->AddElement(C, 0.6942);
    PEN->AddElement(O, 0.2645);

    // Estimated using the number of elements per chain elements  (C_14 H_10 O_4)
    innerCladdingMaterial = new G4Material("innerCladdingMaterial", 1.36 * g / cm3, 3);
    innerCladdingMaterial->AddElement(H, 0.0413);
    innerCladdingMaterial->AddElement(C, 0.6942);
    innerCladdingMaterial->AddElement(O, 0.2645);

    // Estimated using the number of elements per chain elements  (C_14 H_10 O_4)
    outerCladdingMaterial = new G4Material("outerCladdingMaterial", 1.36 * g / cm3, 3);
    outerCladdingMaterial->AddElement(H, 0.0413);
    outerCladdingMaterial->AddElement(C, 0.6942);
    outerCladdingMaterial->AddElement(O, 0.2645);
}

void ReadoutSimDetectorConstruction::SetOpticalProperties()
{
    G4int nEntries = 1;

    // optical properties of materials 
    G4double energy[] = {2.88*eV};    // 430nm, peak emission of PEN

    // PMMA
    G4double pmmaRIndex[] = {1.50};   // for PMMA @ 430nm
    G4double pmmaAbsorption[] = {2.*m};     // for PMMA @ 430nm, try 1.5/2/2.5/3 m
    pmmaMPT->AddProperty("RINDEX", energy, pmmaRIndex, nEntries)->SetSpline(true);
    pmmaMPT->AddProperty("ABSLENGTH", energy, pmmaAbsorption, nEntries)->SetSpline(true);
    PMMA->SetMaterialPropertiesTable(pmmaMPT);

    // PEN
    G4double penAbsorption[] = {0.054*m};   // for PEN @ 440nm, more or less what I need for PEN @ 430nm (the actual wavelength that I'm simulating)
    G4double penRIndex[] = {1.7};     // for PEN @ 430nm
    penMPT->AddProperty("RINDEX", energy, penRIndex, nEntries)->SetSpline(true);
    penMPT->AddProperty("ABSLENGTH", energy, penAbsorption, nEntries)->SetSpline(true); 
    PEN->SetMaterialPropertiesTable(penMPT);

    // LAr
    G4double larAbsorption[] = {0.1*m};     // for lAr @ 430nm, random value to just kill photons when they enter in lAr volume
    G4double larRIndex[] = {1.23};    // for lAr @ 430nm, from Sellmeier formula
    larMPT->AddProperty("RINDEX", energy, larRIndex, nEntries)->SetSpline(true);
    larMPT->AddProperty("ABSLENGTH", energy, larAbsorption, nEntries)->SetSpline(true);
    worldMaterial->SetMaterialPropertiesTable(larMPT);

    // Inner Cladding
    G4double claddingAbsorption[] = {0.054*m};  // for cladding @ 430nm, guessed value, similar to PEN?
    G4double innerCladdingRIndex[] = {1.45};   // for PMMA @ 430nm, guessed value
    innerCladdingMPT->AddProperty("RINDEX", energy, innerCladdingRIndex, nEntries)->SetSpline(true);
    innerCladdingMPT->AddProperty("ABSLENGTH", energy, claddingAbsorption, nEntries)->SetSpline(true); 
    innerCladdingMaterial->SetMaterialPropertiesTable(innerCladdingMPT);

    // Outer Cladding
    G4double outerCladdingRIndex[] = {1.40};   // for PMMA @ 430nm, guessed value
    outerCladdingMPT->AddProperty("RINDEX", energy, outerCladdingRIndex, nEntries)->SetSpline(true);
    outerCladdingMPT->AddProperty("ABSLENGTH", energy, claddingAbsorption, nEntries)->SetSpline(true); 
    outerCladdingMaterial->SetMaterialPropertiesTable(outerCladdingMPT);


}

// auto ReadoutSimDetectorConstruction::SetupPanelOnly() -> G4VPhysicalVolume*
// {
//     //
//     // World
//     //
//     G4double world_hx = 2.5*m;
//     G4double world_hy = 2.5*m;  
//     G4double world_hz = 2.5*m;
//     G4Box* worldSolid = new G4Box("World", world_hx, world_hy, world_hz);
//     auto* fWorldLogical  = new G4LogicalVolume(worldSolid, worldMaterial, "World_log");
//     auto* fWorldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fWorldLogical, "World_phys", nullptr, false, 0);

//     //
//     // PMMA panel volume
//     //
//     G4double panel_hx = 0.05*m;
//     G4double panel_hy = 0.5*m;  
//     G4double panel_hz = 1.5*m;
//     G4Box* panelSolid = new G4Box("Panel", panel_hx, panel_hy, panel_hz);
//     auto fPanelLogical = new G4LogicalVolume(panelSolid, PMMA, "Panel_log");
//     auto* fPanelPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fPanelLogical, "Panel_phys", fWorldLogical, false, 0);

//     //
//     // PEN layers
//     //
//     G4double penLayerThickness = 0.10*cm; //2000 micron
//     G4Box* bigLayerSolid = new G4Box("Layer", penLayerThickness, panel_hy, panel_hz);
//     auto* fBigLayerLogical = new G4LogicalVolume(bigLayerSolid, PEN, "bigLayer_log");
//     auto* fBigLayerPhysical1 = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + penLayerThickness,0.,0.), fBigLayerLogical, "bigLayer_phys", fWorldLogical, false, 0);
//     auto* fBigLayerPhysical2 = new G4PVPlacement(nullptr, G4ThreeVector(-panel_hx - penLayerThickness,0.,0.), fBigLayerLogical, "bigLayer_phys", fWorldLogical, false, 0);

//     G4Box* smallLayerSolid = new G4Box("Layer", panel_hx, penLayerThickness, panel_hz);
//     auto* fSmallLayerLogical = new G4LogicalVolume(smallLayerSolid, PEN, "smallLayer_log");
//     auto* fSmallLayerPhysical1 = new G4PVPlacement(nullptr, G4ThreeVector(0., panel_hy + penLayerThickness, 0.), fSmallLayerLogical, "smallLayer_phys", fWorldLogical, false, 0);
//     auto* fSmallLayerPhysical2 = new G4PVPlacement(nullptr, G4ThreeVector(0., -panel_hy - penLayerThickness, 0.), fSmallLayerLogical, "smallLayer_phys", fWorldLogical, false, 0);

//     //
//     // Detector Volume
//     //
//     G4double detThickness = 0.1*cm; // 2 mm
//     G4Box* detSolid = new G4Box("Detector", panel_hx, panel_hy, detThickness);
//     auto *fDetLogical = new G4LogicalVolume(detSolid, PMMA, "Detector_log");
//     auto* fDetPhysical = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., - panel_hz - detThickness), fDetLogical, "Detector_phys", fWorldLogical, false, 0);
//     auto* fDetPhysical2 = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., panel_hz + detThickness), fDetLogical, "Detector_phys", fWorldLogical, false, 0);

//     auto* yellowVisAtt = new G4VisAttributes(G4Colour::Yellow());
//     yellowVisAtt->SetVisibility(true);
//     auto* greyVisAtt = new G4VisAttributes(G4Colour::Grey());
//     greyVisAtt->SetVisibility(true);
//     auto* blueVisAtt = new G4VisAttributes(G4Colour::Blue());
//     blueVisAtt->SetVisibility(true);
//     auto* greenVisAtt = new G4VisAttributes(G4Colour::Green());
//     greenVisAtt->SetVisibility(true);
//     auto* magentaVisAtt = new G4VisAttributes(G4Colour::Magenta());
//     magentaVisAtt->SetVisibility(true);

//     fWorldLogical->SetVisAttributes(yellowVisAtt);
//     fPanelLogical->SetVisAttributes(greyVisAtt);
//     fBigLayerLogical->SetVisAttributes(blueVisAtt);
//     fSmallLayerLogical->SetVisAttributes(blueVisAtt);
//     fDetLogical->SetVisAttributes(greenVisAtt);

//     return fWorldPhysical;
// }

// auto ReadoutSimDetectorConstruction::SetupPanelWithCladding() -> G4VPhysicalVolume*
// {
//     //
//     // World
//     //
//     G4double world_hx = 2.5*m;
//     G4double world_hy = 2.5*m;  
//     G4double world_hz = 2.5*m;
//     G4Box* worldSolid = new G4Box("World", world_hx, world_hy, world_hz);
//     auto* fWorldLogical  = new G4LogicalVolume(worldSolid, worldMaterial, "World_log");
//     auto* fWorldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fWorldLogical, "World_phys", nullptr, false, 0);

//     //
//     // PMMA panel volume
//     //
//     G4double panel_hx = 0.05*m;
//     G4double panel_hy = 0.5*m;  
//     G4double panel_hz = 1.5*m;
//     G4Box* panelSolid = new G4Box("Panel", panel_hx, panel_hy, panel_hz);
//     auto* fPanelLogical = new G4LogicalVolume(panelSolid, PMMA, "Panel_log");
//     auto* fPanelPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fPanelLogical, "Panel_phys", fWorldLogical, false, 0);
    
//     //
//     // Inner Cladding
//     //
//     G4double claddingLayerThickness = 0.0005*cm; //10 micron
//     G4Box* bigInnCladSolid = new G4Box("smallInnClad", claddingLayerThickness, panel_hy, panel_hz);
//     auto* fBigInnCladLogical = new G4LogicalVolume(bigInnCladSolid, innerCladdingMaterial, "innClad_log");
//     auto* fBigInnCladPhysical1 = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + claddingLayerThickness,0.,0.), fBigInnCladLogical, "innerCladdingLayer_phys", fWorldLogical, false, 0);
//     auto* fBigInnCladPhysical2 = new G4PVPlacement(nullptr, G4ThreeVector(-panel_hx - claddingLayerThickness,0.,0.), fBigInnCladLogical, "innerCladdingLayer_phys", fWorldLogical, false, 0);

//     G4Box* smallInnCladSolid = new G4Box("smallInnClad", panel_hx, claddingLayerThickness, panel_hz);
//     auto* fSmallInnCladLogical = new G4LogicalVolume(smallInnCladSolid, innerCladdingMaterial, "innClad_log");
//     auto* fSmallInnCladdPhysical1 = new G4PVPlacement(nullptr, G4ThreeVector(0., panel_hy + claddingLayerThickness, 0.), fSmallInnCladLogical, "innerCladdingLayer_phys", fWorldLogical, false, 0);
//     auto* fSmallInnCladdPhysical2 = new G4PVPlacement(nullptr, G4ThreeVector(0., -panel_hy - claddingLayerThickness, 0.), fSmallInnCladLogical, "innerCladdingLayer_phys", fWorldLogical, false, 0);

//     //
//     // Outer Cladding
//     //

//     G4Box* bigOutCladSolid = new G4Box("smallOutClad", claddingLayerThickness, panel_hy, panel_hz);
//     auto* fBigOutCladLogical = new G4LogicalVolume(bigOutCladSolid, outerCladdingMaterial, "OutClad_log");
//     auto* fBigOutCladPhysical1 = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 3*claddingLayerThickness,0.,0.), fBigOutCladLogical, "OuterCladdingLayer_phys", fWorldLogical, false, 0);
//     auto* fBigOutCladPhysical2 = new G4PVPlacement(nullptr, G4ThreeVector(-panel_hx - 3*claddingLayerThickness,0.,0.), fBigOutCladLogical, "OuterCladdingLayer_phys", fWorldLogical, false, 0);

//     G4Box* smallOutCladSolid = new G4Box("smallOutClad", panel_hx, claddingLayerThickness, panel_hz);
//     auto* fSmallOutCladLogical = new G4LogicalVolume(smallOutCladSolid, outerCladdingMaterial, "OutClad_log");
//     auto* fSmallOutCladdPhysical1 = new G4PVPlacement(nullptr, G4ThreeVector(0., panel_hy + 3*claddingLayerThickness, 0.), fSmallOutCladLogical, "OuterCladdingLayer_phys", fWorldLogical, false, 0);
//     auto* fSmallOutCladdPhysical2 = new G4PVPlacement(nullptr, G4ThreeVector(0., -panel_hy - 3*claddingLayerThickness, 0.), fSmallOutCladLogical, "OuterCladdingLayer_phys", fWorldLogical, false, 0);

//     //
//     // PEN layers
//     //
//     G4int spacing = 4;
//     // G4double penLayerThickness = 0.025*cm; //500 micron
//     G4double penLayerThickness = 0.1*cm; //1000 micron
//     G4Box* bigLayerSolid = new G4Box("Layer", penLayerThickness, panel_hy, panel_hz);
//     auto* fBigLayerLogical = new G4LogicalVolume(bigLayerSolid, PEN, "Layer_log");
//     auto* fBigLayerPhysical1 = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + penLayerThickness + spacing*claddingLayerThickness,0.,0.), fBigLayerLogical, "Layer_phys", fWorldLogical, false, 0);
//     auto* fBigLayerPhysical2 = new G4PVPlacement(nullptr, G4ThreeVector(-panel_hx - penLayerThickness - spacing*claddingLayerThickness,0.,0.), fBigLayerLogical, "Layer_phys", fWorldLogical, false, 0);

//     G4Box* smallLayerSolid = new G4Box("Layer", panel_hx, penLayerThickness, panel_hz);
//     auto* fSmallLayerLogical = new G4LogicalVolume(smallLayerSolid, PEN, "Layer_log");
//     auto* fSmallLayerPhysical1 = new G4PVPlacement(nullptr, G4ThreeVector(0., panel_hy + penLayerThickness + spacing*claddingLayerThickness, 0.), fSmallLayerLogical, "Layer_phys", fWorldLogical, false, 0);
//     auto* fSmallLayerPhysical2 = new G4PVPlacement(nullptr, G4ThreeVector(0., -panel_hy - penLayerThickness - spacing*claddingLayerThickness, 0.), fSmallLayerLogical, "Layer_phys", fWorldLogical, false, 0);

//     //
//     // Detector Volume
//     //
//     G4double detThickness = 0.1*cm; // 2 mm
//     G4Box* detSolid = new G4Box("Detector", panel_hx, panel_hy, detThickness);
//     auto* fDetLogical = new G4LogicalVolume(detSolid, PMMA, "Detector_log");
//     auto* fDetPhysical = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., - panel_hz - detThickness), fDetLogical, "Detector_phys", fWorldLogical, false, 0);
//     auto* fDetPhysical2 = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., panel_hz + detThickness), fDetLogical, "Detector_phys", fWorldLogical, false, 0);

//     auto* yellowVisAtt = new G4VisAttributes(G4Colour::Yellow());
//     yellowVisAtt->SetVisibility(true);
//     auto* greyVisAtt = new G4VisAttributes(G4Colour::Grey());
//     greyVisAtt->SetVisibility(true);
//     auto* blueVisAtt = new G4VisAttributes(G4Colour::Blue());
//     blueVisAtt->SetVisibility(true);
//     auto* greenVisAtt = new G4VisAttributes(G4Colour::Green());
//     greenVisAtt->SetVisibility(true);
//     auto* magentaVisAtt = new G4VisAttributes(G4Colour::Magenta());
//     magentaVisAtt->SetVisibility(true);

//     fWorldLogical->SetVisAttributes(yellowVisAtt);
//     fPanelLogical->SetVisAttributes(greyVisAtt);
//     fBigLayerLogical->SetVisAttributes(blueVisAtt);
//     fSmallLayerLogical->SetVisAttributes(blueVisAtt);
//     fBigInnCladLogical->SetVisAttributes(magentaVisAtt);
//     fSmallInnCladLogical->SetVisAttributes(magentaVisAtt);
//     fBigOutCladLogical->SetVisAttributes(magentaVisAtt);
//     fSmallOutCladLogical->SetVisAttributes(magentaVisAtt);
//     fDetLogical->SetVisAttributes(greenVisAtt);

//     return fWorldPhysical;
// }

auto ReadoutSimDetectorConstruction::SetupBaselineDesign() -> G4VPhysicalVolume*
{
    //
    // World
    //
    G4double world_hx = 2.5*m;
    G4double world_hy = 2.5*m;  
    G4double world_hz = 2.5*m;
    G4Box* worldSolid = new G4Box("World", world_hx, world_hy, world_hz);
    auto* fWorldLogical  = new G4LogicalVolume(worldSolid, worldMaterial, "World_log");
    auto* fWorldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fWorldLogical, "World_phys", nullptr, false, 0);

    //
    // PMMA panel volume
    //
    G4double panel_x = 0.5*m;  // 1m
    G4double panel_y = 0.05*m; // 10cm
    G4double panel_z = 1.5*m;  // 3m
    G4Box* panelSolid = new G4Box("Panel", panel_x, panel_y, panel_z);
    auto fPanelLogical = new G4LogicalVolume(panelSolid, PMMA, "Panel_log");
    auto* fPanelPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fPanelLogical, "Panel_phys", fWorldLogical, false, 0);


    // PEN layer thickness, defined here because it changes the position of the small light guide
    G4double layerThickness = 0.005*cm; //100 micron

    //
    // PEN layers around light guide
    //
    G4double pen_x = panel_x; // 1m 
    G4double pen_y = 0.01*m + layerThickness;   // 2cm (guide) + PEN foil thickness
    G4double pen_z = 0.05*m + layerThickness*2; // 10cm (guide) + PEN foil thickness on top and bottom of the guide
    G4Box* penSolid = new G4Box("PENFoil", pen_x, pen_y, pen_z);
    auto* fPENLogical = new G4LogicalVolume(penSolid, PEN, "PEN_log");
    auto* fPENPhysical = new G4PVPlacement(nullptr, G4ThreeVector(0., panel_y + pen_y, 0.), fPENLogical, "PEN_phys", fWorldLogical, false, 0);
    
    //
    // PMMA light guide
    // 
    G4double guide_x = panel_x;  // 1m 
    G4double guide_y = 0.01*m;    // 2cm
    G4double guide_z = 0.05*m;    // 10cm
    G4Box* guideSolid = new G4Box("Guide", guide_x, guide_y, guide_z);
    auto* fGuideLogical = new G4LogicalVolume(guideSolid, PMMA, "Guide_log");
    auto* fGuidePhysical = new G4PVPlacement(nullptr, G4ThreeVector(0., -layerThickness, 0.), fGuideLogical, "Guide_phys", fPENLogical, false, 0);

    auto* yellowVisAtt = new G4VisAttributes(G4Colour::Yellow());
    yellowVisAtt->SetVisibility(true);
    auto* greyVisAtt = new G4VisAttributes(G4Colour::Grey());
    greyVisAtt->SetVisibility(true);
    auto* blueVisAtt = new G4VisAttributes(G4Colour::Blue());
    blueVisAtt->SetVisibility(true);
    // auto* greenVisAtt = new G4VisAttributes(G4Colour::Green());
    // greenVisAtt->SetVisibility(true);
    auto* magentaVisAtt = new G4VisAttributes(G4Colour::Magenta());
    magentaVisAtt->SetVisibility(true);

    fWorldLogical->SetVisAttributes(yellowVisAtt);
    fPanelLogical->SetVisAttributes(greyVisAtt);
    fGuideLogical->SetVisAttributes(greyVisAtt);
    fPENLogical->SetVisAttributes(blueVisAtt);

    return fWorldPhysical;
}

// auto ReadoutSimDetectorConstruction::SetupBaselineCladding() -> G4VPhysicalVolume*
// {
//     //
//     // World
//     //
//     G4double world_hx = 2.5*m;
//     G4double world_hy = 2.5*m;  
//     G4double world_hz = 2.5*m;
//     G4Box* worldSolid = new G4Box("World", world_hx, world_hy, world_hz);
//     auto* fWorldLogical  = new G4LogicalVolume(worldSolid, worldMaterial, "World_log");
//     auto* fWorldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fWorldLogical, "World_phys", nullptr, false, 0);

//     //
//     // PMMA panel volume
//     //
//     G4double panel_hx = 0.05*m; // 10cm
//     G4double panel_hy = 0.5*m;  // 1m
//     G4double panel_hz = 1.5*m;  // 3m
//     G4Box* panelSolid = new G4Box("Panel", panel_hx, panel_hy, panel_hz);
//     auto fPanelLogical = new G4LogicalVolume(panelSolid, PMMA, "Panel_log");
//     auto* fPanelPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fPanelLogical, "Panel_phys", fWorldLogical, false, 0);


//     // PEN layer thickness, defined here because it changes the position of the small light guide
//     G4double layerThickness = 0.005*cm; //100 micron
//     G4double claddingThickness = 0.0005*cm; //10 micron
    
//     //
//     // PMMA small light guide
//     //
//     G4double guide_x = 0.01*m;   // 2cm
//     G4double guide_y = panel_hy; // 1m 
//     G4double guide_z = 0.05*m;   // 10cm
//     G4Box* guideSolid = new G4Box("Guide", guide_x, guide_y, guide_z);
//     auto* fGuideLogical = new G4LogicalVolume(guideSolid, PMMA, "Guide_log");
//     auto* fGuidePhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 4*claddingThickness + 2*layerThickness + guide_x, 0., 0.), fGuideLogical, "Guide_phys", fWorldLogical, false, 0);

//     //
//     // Cladding layers between light guide and PEN layer
//     //

//     //
//     // Outer Cladding
//     //
//     // Front
//     G4Box* frontOuterCladdingSolid = new G4Box("frontOuterCladding", claddingThickness, guide_y, guide_z);
//     auto* fFrontOuterCladdingLogical = new G4LogicalVolume(frontOuterCladdingSolid, outerCladdingMaterial, "frontOuterCladding_log");
//     auto* fFrontOuterCladdingPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 2*layerThickness + 7*claddingThickness  + 2*guide_x, 0., 0.), fFrontOuterCladdingLogical, "fFrontOuterCladding_phys", fWorldLogical, false, 0);
//     // Back
//     auto* fBackOuterCladdingLogical = new G4LogicalVolume(frontOuterCladdingSolid, outerCladdingMaterial, "backOuterCladding_log");
//     auto* fBackOuterCladdingPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 2*layerThickness + claddingThickness, 0., 0.), fBackOuterCladdingLogical, "fBackOuterCladding_phys", fWorldLogical, false, 0);
//     // Top
//     G4Box* topOuterCladdingSolid = new G4Box("topOuterCladding", guide_x, guide_y, claddingThickness);
//     auto* fTopOuterCladdingLogical = new G4LogicalVolume(topOuterCladdingSolid, outerCladdingMaterial, "topOuterCladding_log");
//     auto* fTopOuterCladdingPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 2*layerThickness + 4*claddingThickness + guide_x, 0., guide_z + 3*claddingThickness), fTopOuterCladdingLogical, "topOuterCladding_phys", fWorldLogical, false, 0);
//     // Bot
//     auto* fBotOuterCladdingLogical = new G4LogicalVolume(topOuterCladdingSolid, outerCladdingMaterial, "botOuterCladding_log");
//     auto* fBotOuterCladdingPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 2*layerThickness + 4*claddingThickness + guide_x, 0., - guide_z - 3*claddingThickness), fBotOuterCladdingLogical, "botOuterCladding_phys", fWorldLogical, false, 0);

//     //
//     // Inner Cladding
//     //
//     // Front
//     G4Box* frontInnerCladdingSolid = new G4Box("frontInnerCladding", claddingThickness, guide_y, guide_z);
//     auto* fFrontInnerCladdingLogical = new G4LogicalVolume(frontInnerCladdingSolid, innerCladdingMaterial, "frontInnerCladding_log");
//     auto* fFrontInnerCladdingPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 2*layerThickness + 5*claddingThickness  + 2*guide_x, 0., 0.), fFrontInnerCladdingLogical, "fFrontInnerCladding_phys", fWorldLogical, false, 0);
//     // Back
//     auto* fBackInnerCladdingLogical = new G4LogicalVolume(frontInnerCladdingSolid, innerCladdingMaterial, "backInnerCladding_log");
//     auto* fBackInnerCladdingPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 2*layerThickness + 3*claddingThickness, 0., 0.), fBackInnerCladdingLogical, "fBackInnerCladding_phys", fWorldLogical, false, 0);
//     // Top
//     G4Box* topInnerCladdingSolid = new G4Box("topInnerCladding", guide_x, guide_y, claddingThickness);
//     auto* fTopInnerCladdingLogical = new G4LogicalVolume(topInnerCladdingSolid, innerCladdingMaterial, "topInnerCladding_log");
//     auto* fTopInnerCladdingPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 2*layerThickness + 4*claddingThickness + guide_x, 0., guide_z + claddingThickness), fTopInnerCladdingLogical, "topInnerCladding_phys", fWorldLogical, false, 0);
//     // Bot
//     auto* fBotInnerCladdingLogical = new G4LogicalVolume(topInnerCladdingSolid, outerCladdingMaterial, "botInnerCladding_log");
//     auto* fBotInnerCladdingPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 2*layerThickness + 4*claddingThickness + guide_x, 0., - guide_z - claddingThickness), fBotInnerCladdingLogical, "botInnerCladding_phys", fWorldLogical, false, 0);


//     //
//     // PEN layers around light guide
//     //
//     // Front
//     G4Box* frontPENSolid = new G4Box("frontPENLayer", layerThickness, guide_y, guide_z);
//     auto* fFrontPENLayerLogical = new G4LogicalVolume(frontPENSolid, PEN, "frontPENLayer_log");
//     auto* fFrontPENLayerPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 2*guide_x + 8*claddingThickness + 3*layerThickness, 0., 0.), fFrontPENLayerLogical, "frontPENLayer_phys", fWorldLogical, false, 0);
//     // Back
//     auto* fBackPENLayerLogical = new G4LogicalVolume(frontPENSolid, PEN, "backPENLayer_log");
//     auto* fBackPENLayerPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + layerThickness, 0., 0.), fBackPENLayerLogical, "backPENLayer_phys", fWorldLogical, false, 0);
    
//     // Right
//     G4Box* rightPENSolid = new G4Box("rightPENLayer", guide_x, layerThickness, guide_z);
//     fRightPENLayerLogical = new G4LogicalVolume(rightPENSolid, PMMA, "rightPENLayer_log");
//     auto* fRightPENPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 4*claddingThickness + 2*layerThickness + guide_x, guide_y + layerThickness, 0.),fRightPENLayerLogical , "rightPENLayer_phys", fWorldLogical, false, 0);
//     // Left
//     fLeftPENLayerLogical = new G4LogicalVolume(rightPENSolid, PMMA, "leftPENLayer_log");
//     auto* fLeftPENPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 4*claddingThickness + 2*layerThickness + guide_x, - guide_y - layerThickness, 0.), fLeftPENLayerLogical, "leftPENLayer_phys", fWorldLogical, false, 0);

//     // Top
//     G4Box* topPENSolid = new G4Box("topPENLayer", guide_x, guide_y, layerThickness);
//     fTopPENLayerLogical = new G4LogicalVolume(topPENSolid, PEN, "topPENLayer_log");
//     auto* fTopPENPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 2*layerThickness + 4*claddingThickness + guide_x, 0., guide_z + 4*claddingThickness + layerThickness), fTopPENLayerLogical, "topPENLayer_phys", fWorldLogical, false, 0);
//     // Bot
//     fBotPENLayerLogical = new G4LogicalVolume(topPENSolid, PEN, "botPENLayer_log");
//     auto* fBotPENPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_hx + 2*layerThickness + 4*claddingThickness + guide_x, 0., - guide_z - 4*claddingThickness - layerThickness), fBotPENLayerLogical, "botPENLayer_phys", fWorldLogical, false, 0);


//     auto* yellowVisAtt = new G4VisAttributes(G4Colour::Yellow());
//     yellowVisAtt->SetVisibility(true);
//     auto* greyVisAtt = new G4VisAttributes(G4Colour::Grey());
//     greyVisAtt->SetVisibility(true);
//     auto* blueVisAtt = new G4VisAttributes(G4Colour::Blue());
//     blueVisAtt->SetVisibility(true);
//     auto* greenVisAtt = new G4VisAttributes(G4Colour::Green());
//     greenVisAtt->SetVisibility(true);
//     auto* magentaVisAtt = new G4VisAttributes(G4Colour::Magenta());
//     magentaVisAtt->SetVisibility(true);

//     fWorldLogical->SetVisAttributes(yellowVisAtt);
//     fPanelLogical->SetVisAttributes(greyVisAtt);
//     fGuideLogical->SetVisAttributes(greyVisAtt);
//     // PEN
//     fFrontPENLayerLogical->SetVisAttributes(blueVisAtt);
//     fBackPENLayerLogical->SetVisAttributes(blueVisAtt);
//     fTopPENLayerLogical->SetVisAttributes(blueVisAtt);
//     fBotPENLayerLogical->SetVisAttributes(blueVisAtt);
//     // Cladding 
//     fFrontOuterCladdingLogical->SetVisAttributes(magentaVisAtt);
//     fFrontInnerCladdingLogical->SetVisAttributes(magentaVisAtt);
//     fBackOuterCladdingLogical->SetVisAttributes(magentaVisAtt);
//     fBackInnerCladdingLogical->SetVisAttributes(magentaVisAtt);
//     fTopOuterCladdingLogical->SetVisAttributes(magentaVisAtt);
//     fTopInnerCladdingLogical->SetVisAttributes(magentaVisAtt);
//     fBotOuterCladdingLogical->SetVisAttributes(magentaVisAtt);
//     fBotInnerCladdingLogical->SetVisAttributes(magentaVisAtt);
//     // Detector
//     fRightPENLayerLogical->SetVisAttributes(greenVisAtt);
//     fLeftPENLayerLogical->SetVisAttributes(greenVisAtt);

//     return fWorldPhysical;
// }