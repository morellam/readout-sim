#include "g4root.hh"
#include "ReadoutSimRunAction.hh"


ReadoutSimRunAction::ReadoutSimRunAction()
{
    fRun = nullptr;
}

ReadoutSimRunAction::~ReadoutSimRunAction()
{}

G4Run* ReadoutSimRunAction::GenerateRun()
{
  fRun = new Run();
  return fRun;
}

void ReadoutSimRunAction::BeginOfRunAction(const G4Run *aRun)
{
    // G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->OpenFile("readout.root"); 
    
    man->CreateNtuple("Score", "Score");
    // man->CreateNtupleDColumn("left-right");
    // man->CreateNtupleDColumn("bot");
    // man->CreateNtupleDColumn("top");
    // man->CreateNtupleDColumn("left");
    // man->CreateNtupleDColumn("right");
    // man->CreateNtupleDColumn("xHit");
    // man->CreateNtupleDColumn("yHit");
    // man->CreateNtupleDColumn("zHit");
    man->CreateNtupleDColumn("xPhotonVertex");
    man->CreateNtupleDColumn("yPhotonVertex");
    man->CreateNtupleDColumn("zPhotonVertex");
    man->CreateNtupleDColumn("xPhotonDirection");
    man->CreateNtupleDColumn("yPhotonDirection");
    man->CreateNtupleDColumn("zPhotonDirection");
    man->CreateNtupleIColumn("photonTrackID");
    man->CreateNtupleSColumn("finalVolume"); 
    man->CreateNtupleDColumn("stepLength");
    man->CreateNtupleDColumn("G4TrackLength");

    man->FinishNtuple(0);

    // G4cout << "Ho creato la Ntupla" << G4endl;
}

void ReadoutSimRunAction::EndOfRunAction(const G4Run *aRun)
{
    // if (isMaster) fRun->EndOfRun();

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile("readout.root"); 
}