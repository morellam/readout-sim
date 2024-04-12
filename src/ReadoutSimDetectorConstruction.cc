#include "ReadoutSimDetectorConstruction.hh"
// #include "ReadoutSimDetectorMessenger.hh"

#include "G4Element.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
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

    DefineCommands();
}

ReadoutSimDetectorConstruction::~ReadoutSimDetectorConstruction()
{
}

G4VPhysicalVolume *ReadoutSimDetectorConstruction::Construct() 
{
    DefineMaterials();
    SetOpticalProperties();

    return SetupBaselineDesign();
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
    // G4double energy[] = {2.88*eV};    // 430nm, peak emission of PEN
    G4double energy[2] = {2*eV, 12*eV};    // 430nm, peak emission of PEN

    // PMMA
    G4double pmmaRIndex[] = {1.50, 1.50};   // for PMMA @ 430nm
    G4double pmmaAbsorption[] = {2.*m, 2.*m};     // for PMMA @ 430nm, try 1.5/2/2.5/3 m
    pmmaMPT->AddProperty("RINDEX", energy, pmmaRIndex, nEntries)->SetSpline(true);
    pmmaMPT->AddProperty("ABSLENGTH", energy, pmmaAbsorption, nEntries)->SetSpline(true);
    PMMA->SetMaterialPropertiesTable(pmmaMPT);

    // PEN
    G4int absorption_entries = 136;
    G4double absorption_energy[136] = {12.*eV, 3.25814*eV,3.23948*eV,3.22315*eV,3.20819*eV,3.19096*eV,3.17511*eV,3.15942*eV,3.14388*eV,3.12612*eV,3.10698*eV,3.09271*eV,3.07916*eV,3.06507*eV,3.05309*eV,3.04186*eV,3.02973*eV,3.02013*eV,3.00802*eV,2.99648*eV,2.98503*eV,2.97493*eV,2.96239*eV,2.95244*eV,2.94243*eV,2.93218*eV,2.92149*eV,2.90902*eV,2.90005*eV,2.89053*eV,2.88579*eV,2.87611*eV,2.86416*eV,2.85268*eV,2.84109*eV,2.82832*eV,2.82095*eV,2.80855*eV,2.79673*eV,2.78961*eV,2.78390*eV,2.77279*eV,2.76498*eV,2.75155*eV,2.74112*eV,2.72878*eV,2.71710*eV,2.70530*eV,2.69350*eV,2.66995*eV,2.65884*eV,2.64937*eV,2.61333*eV,2.60607*eV,2.58887*eV,2.57971*eV,2.56882*eV,2.55787*eV,2.53466*eV,2.51224*eV,2.50293*eV,2.49215*eV,2.48426*eV,2.47415*eV,2.46551*eV,2.45629*eV,2.43998*eV,2.42426*eV,2.38991*eV,2.36889*eV,2.35666*eV,2.34899*eV,2.34214*eV,2.32433*eV,2.30188*eV,2.29362*eV,2.28542*eV,2.27912*eV,2.25971*eV,2.23884*eV,2.23103*eV,2.22327*eV,2.21801*eV,2.20065*eV,2.18052*eV,2.17310*eV,2.16574*eV,2.16041*eV,2.14329*eV,2.12515*eV,2.11811*eV,2.11111*eV,2.10574*eV,2.08947*eV,2.07314*eV,2.06644*eV,2.05972*eV,2.05466*eV,2.03887*eV,2.02186*eV,2.01549*eV,2.00915*eV,2.00486*eV,1.99038*eV,1.97695*eV,1.97085*eV,1.96480*eV,1.96123*eV,1.94927*eV,1.93665*eV,1.93004*eV,1.92578*eV,1.91555*eV,1.90491*eV,1.90002*eV,1.89541*eV,1.88600*eV,1.87689*eV,1.87244*eV,1.87020*eV,1.86030*eV,1.85376*eV,1.84856*eV,1.84033*eV,1.83314*eV,1.82837*eV,1.82401*eV,1.81985*eV,1.81248*eV,1.80920*eV,1.80189*eV,1.79320*eV,1.78871*eV,1.77977*eV,1.77483*eV, 1.77147*eV};
    G4double absorption_values[136] = {0.3758, 0.3758,0.4457,0.5604,0.2549,0.2937,0.4058,0.7564,1.172,1.7744,2.4753,3.9466,5.0731,6.8639,8.2917,10.1109,12.2749,14.3209,16.8414,19.0424,21.3319,23.6221,25.7956,28.0331,30.1711,32.3501,34.6198,37.2724,39.1275,41.383,43.0375,44.6318,46.6015,48.5223,50.7679,53.0692,55.1642,57.0244,58.3387,58.7343,60.8373,61.3766,63.1698,63.6815,64.6267,65.4004,66.1446,67.7824,68.2573,69.166,69.421,69.0529,69.7186,69.5965,70.9755,71.1224,71.3394,71.5693,71.2311,71.1341,70.3446,69.7107,69.2227,67.7364,67.5254,67.4859,67.6422,67.7565,67.0881,67.1796,67.4574,67.6737,68.077,69.1077,69.8746,70.3309,70.3277,70.4992,70.2329,69.1941,68.8777,68.3743,68.0007,66.5712,65.4022,65.2242,64.7695,64.5717,64.538,65.1323,65.8164,66.3866,66.4781,67.5016,68.6281,68.8893,69.1024,69.3924,70.5099,71.3434,72.3067,72.736,72.948,74.0237,76.4575,76.9708,78.4357,78.9934,80.8292,84.1252,86.2386,87.2857,90.7117,94.4017,96.5345,98.0538,101.6405,104.496,106.4628,108.5769,111.8888,115.6327,116.6733,116.8666,120.3332,122.1505,122.0227,120.9319,125.004,127.1816,128.182,130.8079,132.2029,134.2453,132.8147,133.6449};
    G4int emission_entries = 200;
    G4double emission_energy[200] = {4.121*eV, 4.076*eV,4.034*eV,3.995*eV,3.956*eV,3.918*eV,3.882*eV,3.845*eV,3.806*eV,3.773*eV,3.738*eV,3.704*eV,3.671*eV,3.639*eV,3.606*eV,3.574*eV,3.543*eV,3.513*eV,3.483*eV,3.453*eV,3.424*eV,3.411*eV,3.387*eV,3.373*eV,3.364*eV,3.355*eV,3.350*eV,3.346*eV,3.338*eV,3.334*eV,3.331*eV,3.322*eV,3.319*eV,3.317*eV,3.310*eV,3.301*eV,3.295*eV,3.294*eV,3.288*eV,3.282*eV,3.280*eV,3.278*eV,3.271*eV,3.266*eV,3.265*eV,3.258*eV,3.252*eV,3.246*eV,3.246*eV,3.242*eV,3.234*eV,3.232*eV,3.228*eV,3.220*eV,3.217*eV,3.213*eV,3.205*eV,3.197*eV,3.192*eV,3.184*eV,3.177*eV,3.177*eV,3.169*eV,3.165*eV,3.163*eV,3.156*eV,3.151*eV,3.145*eV,3.145*eV,3.137*eV,3.132*eV,3.130*eV,3.122*eV,3.118*eV,3.118*eV,3.110*eV,3.105*eV,3.104*eV,3.094*eV,3.085*eV,3.081*eV,3.073*eV,3.066*eV,3.051*eV,3.035*eV,3.013*eV,2.991*eV,2.969*eV,2.948*eV,2.929*eV,2.918*eV,2.905*eV,2.897*eV,2.890*eV,2.881*eV,2.874*eV,2.869*eV,2.861*eV,2.859*eV,2.850*eV,2.846*eV,2.839*eV,2.837*eV,2.830*eV,2.821*eV,2.814*eV,2.813*eV,2.806*eV,2.799*eV,2.795*eV,2.789*eV,2.781*eV,2.775*eV,2.771*eV,2.761*eV,2.761*eV,2.755*eV,2.744*eV,2.736*eV,2.729*eV,2.725*eV,2.723*eV,2.717*eV,2.708*eV,2.700*eV,2.691*eV,2.681*eV,2.676*eV,2.667*eV,2.664*eV,2.653*eV,2.649*eV,2.640*eV,2.631*eV,2.627*eV,2.619*eV,2.610*eV,2.601*eV,2.589*eV,2.586*eV,2.577*eV,2.566*eV,2.554*eV,2.545*eV,2.535*eV,2.525*eV,2.516*eV,2.501*eV,2.484*eV,2.471*eV,2.459*eV,2.450*eV,2.440*eV,2.425*eV,2.414*eV,2.400*eV,2.387*eV,2.373*eV,2.362*eV,2.349*eV,2.336*eV,2.323*eV,2.310*eV,2.296*eV,2.284*eV,2.271*eV,2.258*eV,2.245*eV,2.233*eV,2.222*eV,2.210*eV,2.197*eV,2.185*eV,2.174*eV,2.163*eV,2.152*eV,2.140*eV,2.129*eV,2.118*eV,2.107*eV,2.096*eV,2.085*eV,2.075*eV,2.064*eV,2.054*eV,2.043*eV,2.033*eV,2.023*eV,2.013*eV,2.003*eV,1.994*eV,1.985*eV,1.974*eV,1.965*eV,1.956*eV,1.946*eV,1.936*eV,1.928*eV,1.918*eV,1.911*eV};
    G4double emission_values[200] = {7.0293,16.9415,10.7047,45.726,50.7554,55.2072,77.4307,61.2781,45.8031,16.3452,55.7023,14.6248,-13.1988,6.555,2.6498,-11.4939,-4.7904,25.487,49.7759,96.5219,209.3136,339.5284,512.1626,725.255,927.58,1143.9247,1351.3291,1644.9531,1925.6427,2196.7536,2442.0504,2618.048,2776.4256,2996.688,3263.9944,3500.9443,3742.1178,3983.4344,4230.3848,4398.3571,4600.1356,4827.7794,5078.4118,5300.7009,5571.4976,5856.4642,6061.7348,6266.8834,6405.6897,6594.1269,6841.5772,6978.9624,7170.03,7420.6595,7568.734,7797.5579,8031.0384,8247.0488,8535.363,8787.8291,8922.142,9141.1737,9406.8964,9567.665,9782.9677,10063.6821,10283.1786,10456.5366,10688.7506,10967.5929,11178.9257,11444.6893,11714.6674,11897.2054,12072.3493,12339.7591,12495.3968,12730.9516,13001.2756,13216.637,13448.1597,13627.8594,13827.8985,14053.2262,14269.0933,14441.0115,14501.9804,14524.0235,14458.484,14325.3404,14134.0736,13963.8366,13768.8243,13593.031,13393.594,13145.8641,12952.8068,12782.246,12560.672,12346.54,12124.4686,11936.6808,11725.3635,11529.9927,11302.2122,11015.0504,10797.0964,10562.8642,10434.8026,10173.7304,10030.1194,9757.0008,9550.5153,9328.0247,9179.752,8931.5451,8733.5726,8491.1374,8272.8068,7956.0463,7868.4239,7649.2608,7485.9592,7262.1554,7048.2165,6675.112,6386.5461,6167.6057,5951.6386,5694.8763,5468.461,5314.9438,5024.5664,4838.6677,4640.787,4426.4052,4231.3355,4044.6662,3855.5094,3652.9909,3430.2043,3196.1524,3011.6655,2784.6268,2605.7246,2452.9249,2302.4445,2118.9833,1899.8405,1748.9089,1623.6297,1489.8054,1371.2659,1239.6131,1171.9962,1046.581,938.7413,868.631,772.7616,701.2099,625.1904,579.8622,545.0141,494.4586,503.6137,472.2251,452.3345,494.5934,508.9393,542.4361,608.6249,707.8472,777.5688,870.1675,877.0437,869.6932,812.2685,734.182,622.9797,513.2745,419.066,359.4056,291.2013,278.2788,237.0143,213.6086,207.9547,159.0263,171.1955,158.1363,175.3452,142.3859,135.165,137.5661,119.57,134.6123,105.92,93.3617,89.2049,47.6809}; 
    G4double penRIndex[] = {1.7};     // for PEN @ 430nm

    penMPT->AddProperty("RINDEX", energy, penRIndex, nEntries);
    penMPT->AddProperty("WLSABSLENGTH", absorption_energy, absorption_values, absorption_entries); 
    penMPT->AddProperty("WLSCOMPONENT", emission_energy, emission_values, emission_entries);
    penMPT->AddConstProperty("WLSTIMECONSTANT", 25.3*ns); // from L. Manzanillas et al. “Optical properties of low background PEN structural components for the LEGEND-200 experiment”. In: JINST 17 P09007 (2022), https://doi.org/10.1088/1748-0221/17/09/P09007
    penMPT->AddConstProperty("WLSMEANNUMBERPHOTONS", 0.69); // from [Araujo2022] at LAr temperature
    PEN->SetMaterialPropertiesTable(penMPT);

    // LAr
    G4double larAbsorption[] = {0.1*m, 0.1*m};     // for lAr @ 430nm, random value to just kill photons when they enter in lAr volume
    G4double larRIndex[] = {1.23, 1.23};    // for lAr @ 430nm, from Sellmeier formula
    larMPT->AddProperty("RINDEX", energy, larRIndex, nEntries)->SetSpline(true);
    larMPT->AddProperty("ABSLENGTH", energy, larAbsorption, nEntries)->SetSpline(true);
    worldMaterial->SetMaterialPropertiesTable(larMPT);

    // Inner Cladding
    // G4double claddingAbsorption[] = {0.054*m};  // for cladding @ 430nm, guessed value, similar to PEN?
    // G4double innerCladdingRIndex[] = {1.45};   // for PMMA @ 430nm, guessed value
    // innerCladdingMPT->AddProperty("RINDEX", energy, innerCladdingRIndex, nEntries)->SetSpline(true);
    // innerCladdingMPT->AddProperty("ABSLENGTH", energy, claddingAbsorption, nEntries)->SetSpline(true); 
    // innerCladdingMaterial->SetMaterialPropertiesTable(innerCladdingMPT);

    // Outer Cladding
    // G4double outerCladdingRIndex[] = {1.40};   // for PMMA @ 430nm, guessed value
    // outerCladdingMPT->AddProperty("RINDEX", energy, outerCladdingRIndex, nEntries)->SetSpline(true);
    // outerCladdingMPT->AddProperty("ABSLENGTH", energy, claddingAbsorption, nEntries)->SetSpline(true); 
    // outerCladdingMaterial->SetMaterialPropertiesTable(outerCladdingMPT);


}

void ReadoutSimDetectorConstruction::SetSpace(G4int val)
{
    if(val == 1) space = 2.*cm; 
    else space = 0.*cm;
}

void ReadoutSimDetectorConstruction::SetWLSBack(G4int val)
{
    if(val == 1){
        WLS_y = 2;
        centerGuide = 0;
    }
}

void ReadoutSimDetectorConstruction::setWLSWrap(G4int val)
{
    if(val == 0){
        WLS_material = PMMA;
        layerThickness = 0.*cm;
    }
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
    layerThickness = 0.005*cm; //100 micron

    //
    // PEN layers around light guide
    //
    G4double pen_x = panel_x; // 1m 
    G4double pen_y = 0.005*m + layerThickness * WLS_y;  // 1cm (guide) + PEN foil thickness (WLS_y is 1 for only front, 2 for front and back of guide covered with WLS)
    G4double pen_z = 0.05*m + layerThickness * 2; // 10cm (guide) + PEN foil thickness on top and bottom of the guide
    G4Box* penSolid = new G4Box("PENFoil", pen_x, pen_y, pen_z);
    auto* fPENLogical = new G4LogicalVolume(penSolid, PEN, "PEN_log");
    auto* fPENPhysical = new G4PVPlacement(nullptr, G4ThreeVector(0., panel_y + pen_y + space, 0.), fPENLogical, "PEN_phys", fWorldLogical, false, 0);
    
    //
    // PMMA light guide
    // 
    G4double guide_x = panel_x;  // 1m 
    G4double guide_y = 0.005*m;  // 1cm
    G4double guide_z = 0.05*m;   // 10cm
    G4Box* guideSolid = new G4Box("Guide", guide_x, guide_y, guide_z);
    auto* fGuideLogical = new G4LogicalVolume(guideSolid, PMMA, "Guide_log");
    auto* fGuidePhysical = new G4PVPlacement(nullptr, G4ThreeVector(0., -layerThickness * centerGuide, 0.), fGuideLogical, "Guide_phys", fPENLogical, false, 0);

    //
    // PMMA "detector"
    // this is just a volume to be placed at the end of the guide to counts photons
    // 
    G4double detector_x = .5*cm;  // 1cm 
    G4double detector_y = .5*cm;  // 1cm
    G4double detector_z = 0.05*m;   // 10cm
    G4Box* detectorSolid    = new G4Box("Detector", detector_x, detector_y, detector_z);
    auto* fDetectorLogical  = new G4LogicalVolume(detectorSolid, PMMA, "Detector_log");
    auto* fRightDetPhysical = new G4PVPlacement(nullptr, G4ThreeVector(panel_x + detector_x, panel_y + pen_y + space, 0.), fDetectorLogical, "RightDetector_phys", fWorldLogical, false, 0);
    auto* fLeftDetPhysical  = new G4PVPlacement(nullptr, G4ThreeVector(- panel_x - detector_x, panel_y + pen_y + space, 0.), fDetectorLogical, "LeftDetector_phys", fWorldLogical, false, 0);

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


void ReadoutSimDetectorConstruction::DefineCommands()
{
    // Define geometry command directory using generic messenger class
    auto fDetectorMessenger = new G4GenericMessenger(this, "/RS/guide/", "Commands for controlling detector setup");

    // switch command
    fDetectorMessenger->DeclareMethod("setSpaceGuide", &ReadoutSimDetectorConstruction::SetSpace)
    .SetGuidance("Decide whether you want some space between light guide and moderator")
    .SetGuidance("0 = Light guide is attached to PMMA panel")
    .SetGuidance("1 = There is some space between light guide and PMMA panel")
    .SetCandidates("0 1")
    .SetDefaultValue("0");

    fDetectorMessenger->DeclareMethod("setWLSWrap", &ReadoutSimDetectorConstruction::setWLSWrap)
    .SetGuidance("Decide whether you want to wrap the light guide with WLS")
    .SetGuidance("0 = without WLS wrapping")
    .SetGuidance("1 = with WLS wrapping")
    .SetCandidates("0 1")
    .SetDefaultValue("1");

    fDetectorMessenger->DeclareMethod("setWLSBack", &ReadoutSimDetectorConstruction::SetWLSBack)
    .SetGuidance("Decide whether you want WLS also on the back of the light guide (i.e. between light guide and PMMA panel)")
    .SetGuidance("0 = without WLS on light guide's back")
    .SetGuidance("1 = with WLS on light guide's back")
    .SetCandidates("0 1")
    .SetDefaultValue("0");

}