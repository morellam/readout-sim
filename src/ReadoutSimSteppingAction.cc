#include "ReadoutSimSteppingAction.hh"
#include "Run.hh"

#include "G4OpBoundaryProcess.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SteppingManager.hh"
#include "G4RunManager.hh"
#include "G4ProcessManager.hh"

#include "G4SystemOfUnits.hh"

#include "g4root.hh"

ReadoutSimSteppingAction::ReadoutSimSteppingAction()
: G4UserSteppingAction()
{
    trackLength = 0.;
}

ReadoutSimSteppingAction::~ReadoutSimSteppingAction()
{}

void ReadoutSimSteppingAction::UserSteppingAction(const G4Step* step)
{
    // static G4ParticleDefinition* opticalphoton = 
    //             G4OpticalPhoton::OpticalPhotonDefinition();
    // G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();

    // Run* run = static_cast<Run*>(
    //            G4RunManager::GetRunManager()->GetNonConstCurrentRun());

    // G4Track* track = step->GetTrack();
    // G4StepPoint* endPoint   = step->GetPostStepPoint();
    // G4StepPoint* startPoint = step->GetPreStepPoint();

    // G4String particleName = track->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();

    // G4String startVolumeName = startPoint->GetTouchable()->GetVolume() -> GetLogicalVolume() -> GetName();
    // G4String endVolumeName = endPoint->GetTouchable()->GetVolume() -> GetLogicalVolume() -> GetName();

    // trackLength = trackLength + track->GetStepLength() / m;

    // uncomment for Big Panel Design
    // if(endVolumeName == "Detector_log" && startVolumeName == "Panel_log")
    // {
    //     track->SetTrackStatus(fStopAndKill);
    //     run->AddDetection();
    //     analysisMan->FillNtupleIColumn(7, 1);
    //     analysisMan->FillNtupleDColumn(8, trackLength);
    //     trackLength = 0.*m;
    // }
    // // If photon inside LAr, kill track
    // if(endVolumeName == "World_log") 
    // {
    //     track->SetTrackStatus(fStopAndKill);
    //     trackLength = 0.*m;
    //     run->AddLArAbsorption();
    //     // if(startVolumeName == "Panel_log") run->AddPanelTowardLAr();
    //     // else if (startVolumeName == "frontPENLayer_log" ||
    //     //          startVolumeName == "backPENLayer_log"  ||
    //     //          startVolumeName == "topPENLayer_log"   ||
    //     //          startVolumeName == "botPENLayer_log"
    //     //         ) run->AddPENTowardLAr();
    //     // else if (startVolumeName == "Guide_log") run->AddLightGuideTowardLAr();
    // }

    ///////////////////////////////////////////////
    // Uncomment for Baseline (12 strips) Design //
    ///////////////////////////////////////////////

    // If photon inside Left or Right volume, is detected
    // if(endVolumeName == "leftPENLayer_log" && startVolumeName == "Guide_log")
    // {
    //     track->SetTrackStatus(fStopAndKill);
    //     run->AddDetection();
    //     analysisMan->FillNtupleIColumn(7, 1);
    //     analysisMan->FillNtupleDColumn(8, trackLength);
    //     trackLength = 0.;
    // }

    // if(endVolumeName == "rightPENLayer_log" && startVolumeName == "Guide_log")
    // {
    //     track->SetTrackStatus(fStopAndKill);
    //     run->AddDetection();
    //     analysisMan->FillNtupleIColumn(7, 2);
    //     analysisMan->FillNtupleDColumn(8, trackLength);
    //     trackLength = 0.;
    // }

    // If photon inside LAr, kill track
    // if(endVolumeName == "World_log") 
    // {
    //     track->SetTrackStatus(fStopAndKill);
    //     trackLength = 0.;
    //     run->AddLArAbsorption();
    //     if(startVolumeName == "Panel_log") run->AddPanelTowardLAr();
    //     else if (startVolumeName == "frontPENLayer_log" ||
    //              startVolumeName == "backPENLayer_log"  ||
    //              startVolumeName == "topPENLayer_log"   ||
    //              startVolumeName == "botPENLayer_log"
    //             ) run->AddPENTowardLAr();
    //     else if (startVolumeName == "Guide_log") run->AddLightGuideTowardLAr();

    // }

    // const G4VProcess* pds = endPoint->GetProcessDefinedStep(); 

    // G4cout << "Process: \t" << pds->GetProcessName() << G4endl;
    // G4cout << "Currently in \t" << startVolumeName << G4endl;
    // G4cout << "Going into \t" << endVolumeName << G4endl; 
    // G4cout << "Step length \t" << track->GetStepLength()/m << G4endl;
    // G4cout << "Track length: \t" << trackLength << "\n" << G4endl;

    // if photon is absorbed, store volume information
    // if(pds->GetProcessName() == "OpAbsorption")
    // {
    //     analysisMan->FillNtupleIColumn(7, 0);
    //     trackLength = 0.;
    //     if(endVolumeName == "Panel_log") run->AddPanelAbsorption();
    //     else if(endVolumeName == "Guide_log") run->AddLightGuideAbsorption();
    //     else if (endVolumeName == "frontPENLayer_log" ||
    //              endVolumeName == "backPENLayer_log"  ||
    //              endVolumeName == "topPENLayer_log"   ||
    //              endVolumeName == "botPENLayer_log"
    //             ) run->AddPenAbsorption();
    //     else if (endVolumeName == "frontOuterCladding_log" ||
    //              endVolumeName == "backOuterCladding_log"  ||
    //              endVolumeName == "topOuterCladding_log"   ||
    //              endVolumeName == "botOuterCladding_log"
    //             ) run->AddOuterCladdingAbsorption();
    //     else if (endVolumeName == "frontInnerCladding_log" ||
    //              endVolumeName == "backInnerCladding_log"  ||
    //              endVolumeName == "topInnerCladding_log"   ||
    //              endVolumeName == "botInnerCladding_log"
    //             ) run->AddInnerCladdingAbsorption();
    // }

    return;

}
