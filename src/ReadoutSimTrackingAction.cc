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

    // const G4Step* step = aTrack->GetStep();
    G4String volume_name = aTrack->GetVolume()->GetName();

    analysisMan->FillNtupleDColumn(0, aTrack->GetVertexPosition().getX() / cm);
    analysisMan->FillNtupleDColumn(1, aTrack->GetVertexPosition().getY() / cm);
    analysisMan->FillNtupleDColumn(2, aTrack->GetVertexPosition().getZ() / cm);
    analysisMan->FillNtupleDColumn(3, aTrack->GetMomentumDirection().getX() / cm);
    analysisMan->FillNtupleDColumn(4, aTrack->GetMomentumDirection().getY() / cm);
    analysisMan->FillNtupleDColumn(5, aTrack->GetMomentumDirection().getZ() / cm);
    analysisMan->FillNtupleSColumn(6, volume_name);
    // analysisMan->AddNtupleRow(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ReadoutSimTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();

    G4String volume_name = aTrack->GetVolume()->GetName();
    analysisMan->FillNtupleSColumn(7, volume_name);
    analysisMan->FillNtupleDColumn(8, aTrack->GetPosition().getX() / cm);
    analysisMan->FillNtupleDColumn(9, aTrack->GetPosition().getY() / cm);
    analysisMan->FillNtupleDColumn(10, aTrack->GetPosition().getZ() / cm);
    analysisMan->FillNtupleDColumn(11, aTrack->GetKineticEnergy() / eV);

    // track_length_g4 = aTrack->GetTrackLength() / cm;

    // analysisMan->FillNtupleDColumn(9, track_length_g4);
    analysisMan->AddNtupleRow(0);
}
