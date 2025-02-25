#if !defined (__CINT__) || defined (__CLING__)
#include "AliAnalysisManager.h"
#include "AliAnalysisTaskSEXicPlusToXi2PifromKFP.h"
#include <TString.h>
#include <TList.h>
#endif

AliAnalysisTaskSEXicPlusToXi2PifromKFP* AddTaskXicPlusToXi2PiFromKFParticle(TString finname="", Bool_t IsMC=kFALSE, TString cuttype="", Bool_t writeQATree = kTRUE)
{
    Bool_t writeXicPlusRecTree = kTRUE;
    Bool_t writeXicPlusMCGenTree = kFALSE;
    if (IsMC) writeXicPlusMCGenTree = kTRUE;

    TString particle = "XicPlus";

    // get the manager via the static access member. since it's static, you don't need
    // an instance of the class to call the function
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    if (!mgr) {
        ::Error("AddMyTask", "No analysis manager to connect to.");
        return NULL;
    }

    // get the input event handler, again via a static method. 
    // this handler is part of the managing system and feeds events
    // to your task
    if (!mgr->GetInputEventHandler()) {
        return NULL;
    }

    // check input cut object
    Bool_t stdCuts = kFALSE;                                                                             
    TFile* fileCuts;
    if ( finname.EqualTo("") ) {
      stdCuts = kTRUE;
    } else {
      fileCuts = TFile::Open(finname.Data());
      if( !fileCuts || (fileCuts && !fileCuts->IsOpen()) ) {
        cout << "Input file not found : check your cut object" << endl;
        return NULL;
      }
    }

    AliRDHFCutsKFP *RDHFCutsKFP = new AliRDHFCutsKFP();
    if (stdCuts) RDHFCutsKFP->SetStandardCutsPP2010();
    else {
      RDHFCutsKFP = (AliRDHFCutsKFP*)fileCuts->Get("XicPlusAnaCuts");
    }
    RDHFCutsKFP->SetName("XicPlusAnaCuts");

    if (!RDHFCutsKFP) {
      cout << "Specific AliRDHFCutsKFP not found" << endl;
      return NULL;
    }

    printf("CREATE TASK\n");

    // by default, a file is open for writing. here, we get the filename
    TString fileName = AliAnalysisManager::GetCommonFileName();
    fileName += ":PWGHF_D2H_KFP_";      // create a subfolder in the file
    fileName += cuttype.Data();
    // now we create an instance of your task
    AliAnalysisTaskSEXicPlusToXi2PifromKFP* task = new AliAnalysisTaskSEXicPlusToXi2PifromKFP("AliAnalysisTaskSEXicPlusToXi2PifromKFP", RDHFCutsKFP);

    if(!task) return NULL;
    task->SetMC(IsMC);
    task->SetDebugLevel(1);
    task->SetWriteXicPlusMCGenTree(writeXicPlusMCGenTree);
    task->SetWriteXicPlusTree(writeXicPlusRecTree);
    task->SetWriteXicPlusQATree(writeQATree);

    // select type of event
//    task->SelectCollisionCandidates(AliVEvent::kAnyINT); // kAnyINT = kMB | kINT7 | kINT5 | kINT8 | kSPI7
    // add your task to the manager
    mgr->AddTask(task);
    // your task needs input: here we connect the manager to your task
    mgr->ConnectInput(task,0,mgr->GetCommonInputContainer());
    // same for the output
    mgr->ConnectOutput(task,1,mgr->CreateContainer(Form("CutsObj_%s", cuttype.Data()), TList::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    mgr->ConnectOutput(task,2,mgr->CreateContainer(Form("Counter_%s", cuttype.Data()), AliNormalizationCounter::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    mgr->ConnectOutput(task,3,mgr->CreateContainer(Form("tree_event_char_%s", cuttype.Data()), TTree::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    mgr->ConnectOutput(task,4,mgr->CreateContainer(Form("tree_%s_%s", particle.Data(),cuttype.Data()), TTree::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    mgr->ConnectOutput(task,5,mgr->CreateContainer(Form("tree_%s_MCGen_%s", particle.Data(),cuttype.Data()), TTree::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    mgr->ConnectOutput(task,6,mgr->CreateContainer(Form("Hist_%s_%s", particle.Data(),cuttype.Data()), TList::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    mgr->ConnectOutput(task,7,mgr->CreateContainer(Form("tree_%s_QA_%s", particle.Data(),cuttype.Data()), TTree::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    mgr->ConnectOutput(task,8,mgr->CreateContainer(Form("tree_%s_QA_%s_woMassConstForLamAndXi", particle.Data(),cuttype.Data()), TTree::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    mgr->ConnectOutput(task,9,mgr->CreateContainer(Form("tree_%s_QA_%s_wMassConstForLam_woMassConstForXi", particle.Data(),cuttype.Data()), TTree::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    mgr->ConnectOutput(task,10,mgr->CreateContainer(Form("tree_%s_QA_%s_woMassConstForLam_wMassConstForXi", particle.Data(),cuttype.Data()), TTree::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    mgr->ConnectOutput(task,11,mgr->CreateContainer(Form("tree_%s_QA_%s_wMassConstForLamAndXi", particle.Data(),cuttype.Data()), TTree::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));

    // in the end, this macro returns a pointer to your task. this will be convenient later on
    // when you will run your analysis in an analysis train on grid
    return task;
}
