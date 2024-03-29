#include "ReadoutSimDetectorMessenger.hh"
#include "ReadoutSimDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"

DetectorMessenger::DetectorMessenger(ReadoutSimDetectorConstruction* Det)
{
    fReadoutSimDir = new G4UIdirectory("/readoutsim/");
    fReadoutSimDir->SetGuidance("Parameters for optical simulation.");

    fGeometryTypeCmd = new G4UIcmdWithAString("/readoutsim/geometryType", this);
    fGeometryTypeCmd->SetGuidance("Geometry type.");
    fGeometryTypeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryTypeCmd->SetToBeBroadcasted(false);
}

DetectorMessenger::~DetectorMessenger()
{
    delete fGeometryTypeCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if(command == fGeometryTypeCmd)
    {
        if (newValue != "baseline" && newValue != "baselineCladding") G4cerr << "No geometry with that name.";
        else fDetector->SetGeometry(newValue);
    }
}