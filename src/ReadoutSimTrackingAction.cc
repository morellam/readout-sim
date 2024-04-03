#include "ReadoutSimTrackingAction.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ReadoutSimTrackingAction::ReadoutSimTrackingAction()
:G4UserTrackingAction()
{
}

void ReadoutSimTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();

    track_length_g4 = 0.;

    analysisMan->FillNtupleDColumn(0, aTrack->GetVertexPosition().getX() / cm);
    analysisMan->FillNtupleDColumn(1, aTrack->GetVertexPosition().getY() / cm);
    analysisMan->FillNtupleDColumn(2, aTrack->GetVertexPosition().getZ() / cm);
    analysisMan->FillNtupleDColumn(3, aTrack->GetMomentumDirection().getX() / cm);
    analysisMan->FillNtupleDColumn(4, aTrack->GetMomentumDirection().getY() / cm);
    analysisMan->FillNtupleDColumn(5, aTrack->GetMomentumDirection().getZ() / cm);
    analysisMan->FillNtupleIColumn(6, aTrack->GetTrackID());
    // analysisMan->AddNtupleRow(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ReadoutSimTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();

    const G4Step* step = aTrack->GetStep();
    G4String volume_name = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
    analysisMan->FillNtupleSColumn(7, volume_name);

    // track_length_g4 = aTrack->GetTrackLength() / cm;

    // analysisMan->FillNtupleDColumn(9, track_length_g4);
    analysisMan->AddNtupleRow(0);
}
