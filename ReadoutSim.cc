#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "G4UImanager.hh"

#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "ReadoutSimDetectorConstruction.hh"
#include "ReadoutSimActionInitialization.hh"


int main(int argc,char** argv)
{
    //detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = nullptr;
    if (argc == 1) ui = new G4UIExecutive(argc,argv);

    G4MTRunManager * runManager = new G4MTRunManager;
    G4int nThreads = std::min(G4Threading::G4GetNumberOfCores(), 1);
    runManager->SetNumberOfThreads(nThreads);
    G4cout << "===== ReadoutSim is started with "
            <<  runManager->GetNumberOfThreads() << " threads =====" << G4endl;

    // Set mandatory initialization classes
    //
    // Detector construction
    runManager-> SetUserInitialization(new ReadoutSimDetectorConstruction());
    //
    // Physics list
    G4VModularPhysicsList* physicsList = new FTFP_BERT;
    physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    physicsList->RegisterPhysics(opticalPhysics);
    runManager->SetUserInitialization(physicsList);
    //
    // User action initialization
    runManager->SetUserInitialization(new ReadoutSimActionInitialization());

   //initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    //get the pointer to the User Interface manager 
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui)  {
        //interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else  {
        //batch mode  
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    // job termination
    delete visManager;
    delete runManager;
    return 0;
}