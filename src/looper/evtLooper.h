#ifndef ttree_h
#define ttree_h

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"

using namespace std;

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class evtLooper {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Declaration of leaf types
Int_t ngoodleps;
Int_t nvetoleps;
Int_t evt;
Float_t mt_met_lep;
Float_t pfmet;
Int_t ngoodjets;
Int_t ngoodbtags;
Float_t ak4_HT;
Bool_t PassTrackVeto;
Bool_t PassTauVeto;
Float_t mbb;
Float_t mct;
Float_t mass_stop;
Float_t mass_lsp;
Float_t weight;
Float_t w_noBtagSF;
Int_t njets;
Float_t ak4pfjets_CSV[100];
Float_t ak4pfjets_deepCSV[100];
Float_t ak4pfjets_pt[100];
Float_t ak4pfjets_eta[100];
Float_t ak4pfjets_phi[100];
Float_t dphi_ak4pfjet_met[100];
Float_t ak4pfjets_m[100];
Int_t ak4pfjets_hadron_flavor[100];
Int_t lep1_pdgid;
Int_t lep2_pdgid;
Float_t lep1_relIso;
Float_t lep2_relIso;
Int_t HLT_SingleEl;
Int_t HLT_SingleMu;
Bool_t pass;
Bool_t goodrun;
Bool_t duplicate;

//## generator info ###
Int_t gen_id[200];
Int_t gen_idx[200];
Float_t gen_pt[200];
Float_t gen_eta[200];
Float_t gen_phi[200];
Float_t gen_m[200];
Int_t gen_motherid[200];
Int_t gen_motheridx[200];
Int_t gen_gmotherid[200];
Int_t gen_gmotheridx[200];

//## isolated tracks ###
Int_t isoTracks_charge[50];
//Double_t isoTracks_absIso[50];
//Double_t isoTracks_dz[50];
Int_t isoTracks_pdgId[50];
//Bool_t isoTracks_isVetoTrack[50];
//Bool_t isoTracks_isVetoTrack_v2[50];
//Bool_t isoTracks_isVetoTrack_v3[50];

//## taus ###
//Double_t tau_passID[50];
Int_t ngoodtaus;
//Double_t tau_againstMuonTight[50];
//Double_t tau_againstElectronLoose[50];
//Bool_t tau_isVetoTau[50];
//Bool_t tau_isVetoTau_v2[50];

//## leptons ###
//Int_t nvleps;
Float_t leps_pt[50];
Float_t leps_eta[50];
Float_t leps_phi[50];
//Float_t leps_id[50];
Float_t els_pt[50];
Float_t els_eta[50];
Float_t els_phi[50];
Float_t els_id[50];
Float_t mus_pt[50];
Float_t mus_eta[50];
Float_t mus_phi[50];
//Float_t mus_id[50];


// Branches
TBranch *b_ngoodleps;
TBranch *b_nvetoleps;
TBranch *b_evt;
TBranch *b_mt_met_lep;
TBranch *b_pfmet;
TBranch *b_ngoodjets;
TBranch *b_ngoodbtags;
TBranch *b_ak4_HT;
TBranch *b_PassTrackVeto;
TBranch *b_PassTauVeto;
TBranch *b_mbb;
TBranch *b_mct;
TBranch *b_mass_stop;
TBranch *b_mass_lsp;
TBranch *b_weight;
TBranch *b_w_noBtagSF;
TBranch *b_njets;
TBranch *b_ak4pfjets_CSV;
TBranch *b_ak4pfjets_deepCSV;
TBranch *b_ak4pfjets_pt;
TBranch *b_ak4pfjets_eta;
TBranch *b_ak4pfjets_phi;
TBranch *b_dphi_ak4pfjet_met;
TBranch *b_ak4pfjets_m;
TBranch *b_ak4pfjets_hadron_flavor;
TBranch *b_lep1_pdgid;
TBranch *b_lep2_pdgid;
TBranch *b_lep1_relIso;
TBranch *b_lep2_relIso;
TBranch *b_HLT_SingleEl;
TBranch *b_HLT_SingleMu;
TBranch *b_pass;
TBranch *b_goodrun;
TBranch *b_duplicate;

//## generator info ###
TBranch *b_gen_id;
TBranch *b_gen_idx;
TBranch *b_gen_pt;
TBranch *b_gen_eta;
TBranch *b_gen_phi;
TBranch *b_gen_m;
TBranch *b_gen_motherid;
TBranch *b_gen_motheridx;
TBranch *b_gen_gmotherid;
TBranch *b_gen_gmotheridx;

//## isolated tracks ###
TBranch *b_isoTracks_charge;
//TBranch *b_isoTracks_absIso;
//TBranch *b_isoTracks_dz;
TBranch *b_isoTracks_pdgId;
//TBranch *b_isoTracks_isVetoTrack;
//TBranch *b_isoTracks_isVetoTrack_v2;
//TBranch *b_isoTracks_isVetoTrack_v3;

//## taus ###
//TBranch *b_tau_passID;
TBranch *b_ngoodtaus;
//TBranch *b_tau_againstMuonTight;
//TBranch *b_tau_againstElectronLoose;
//TBranch *b_tau_isVetoTau;
//TBranch *b_tau_isVetoTau_v2;

//## leptons ###
//TBranch *b_nvleps;
TBranch *b_leps_pt;
TBranch *b_leps_eta;
TBranch *b_leps_phi;
//TBranch *b_leps_id;
TBranch *b_els_pt;
TBranch *b_els_eta;
TBranch *b_els_phi;
TBranch *b_els_id;
TBranch *b_mus_pt;
TBranch *b_mus_eta;
TBranch *b_mus_phi;
//TBranch *b_mus_id;


TH1F *Hpt;
TH2F *genH_dRbb_pt;
TH2F *genW_dRbb_pt;

   evtLooper(TTree *tree=0);
   virtual ~evtLooper();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual int      HasMedLooseDeepCSV(Double_t weight, Long64_t jentry);
   virtual int      HasMedMedDeepCSV(Double_t weight, Long64_t jentry);
   virtual int      WHLeptons(Double_t weight, Long64_t jentry);
   virtual void     sigLoop();
   virtual void     saveHists(const char* input);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ttree_cxx
evtLooper::evtLooper(TTree *tree) : fChain(0) 
{
   Init(tree);
}

evtLooper::~evtLooper()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t evtLooper::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t evtLooper::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void evtLooper::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

fChain->SetBranchAddress("ngoodleps", &ngoodleps, &b_ngoodleps);
fChain->SetBranchAddress("nvetoleps", &ngoodleps, &b_ngoodleps);
fChain->SetBranchAddress("evt", &evt, &b_evt);
fChain->SetBranchAddress("mt_met_lep", &mt_met_lep, &b_mt_met_lep);
fChain->SetBranchAddress("pfmet", &pfmet, &b_pfmet);
fChain->SetBranchAddress("ngoodjets", &ngoodjets, &b_ngoodjets);
fChain->SetBranchAddress("ngoodbtags", &ngoodbtags, &b_ngoodbtags);
fChain->SetBranchAddress("ak4_HT", &ak4_HT, &b_ak4_HT);
fChain->SetBranchAddress("PassTrackVeto", &PassTrackVeto, &b_PassTrackVeto);
fChain->SetBranchAddress("PassTauVeto", &PassTauVeto, &b_PassTauVeto);
fChain->SetBranchAddress("mbb", &mbb, &b_mbb);
fChain->SetBranchAddress("mct", &mct, &b_mct);
fChain->SetBranchAddress("mass_stop", &mass_stop, &b_mass_stop);
fChain->SetBranchAddress("mass_lsp", &mass_lsp, &b_mass_lsp);
fChain->SetBranchAddress("weight", &weight, &b_weight);
fChain->SetBranchAddress("w_noBtagSF", &w_noBtagSF, &b_w_noBtagSF);
fChain->SetBranchAddress("njets", &njets, &b_njets);
fChain->SetBranchAddress("ak4pfjets_CSV", &ak4pfjets_CSV, &b_ak4pfjets_CSV);
fChain->SetBranchAddress("ak4pfjets_deepCSV", &ak4pfjets_deepCSV, &b_ak4pfjets_deepCSV);
fChain->SetBranchAddress("ak4pfjets_pt", &ak4pfjets_pt, &b_ak4pfjets_pt);
fChain->SetBranchAddress("ak4pfjets_eta", &ak4pfjets_eta, &b_ak4pfjets_eta);
fChain->SetBranchAddress("ak4pfjets_phi", &ak4pfjets_phi, &b_ak4pfjets_phi);
fChain->SetBranchAddress("dphi_ak4pfjet_met", &dphi_ak4pfjet_met, &b_dphi_ak4pfjet_met);
fChain->SetBranchAddress("ak4pfjets_m", &ak4pfjets_m, &b_ak4pfjets_m);
fChain->SetBranchAddress("ak4pfjets_hadron_flavor", &ak4pfjets_hadron_flavor, &b_ak4pfjets_hadron_flavor);
fChain->SetBranchAddress("lep1_pdgid", &lep1_pdgid, &b_lep1_pdgid);
fChain->SetBranchAddress("lep2_pdgid", &lep2_pdgid, &b_lep2_pdgid);
fChain->SetBranchAddress("lep1_relIso", &lep1_relIso, &b_lep1_relIso);
fChain->SetBranchAddress("lep2_relIso", &lep2_relIso, &b_lep2_relIso);
fChain->SetBranchAddress("HLT_SingleEl", &HLT_SingleEl, &b_HLT_SingleEl);
fChain->SetBranchAddress("HLT_SingleMu", &HLT_SingleMu, &b_HLT_SingleMu);
fChain->SetBranchAddress("pass", &pass, &b_pass);
fChain->SetBranchAddress("goodrun", &goodrun, &b_goodrun);
fChain->SetBranchAddress("duplicate", &duplicate, &b_duplicate);

//## generator info ###
fChain->SetBranchAddress("gen_id", &gen_id, &b_gen_id);
fChain->SetBranchAddress("gen_idx", &gen_idx, &b_gen_idx);
fChain->SetBranchAddress("gen_pt", &gen_pt, &b_gen_pt);
fChain->SetBranchAddress("gen_eta", &gen_eta, &b_gen_eta);
fChain->SetBranchAddress("gen_phi", &gen_phi, &b_gen_phi);
fChain->SetBranchAddress("gen_m", &gen_m, &b_gen_m);
fChain->SetBranchAddress("gen_motherid", &gen_motherid, &b_gen_motherid);
fChain->SetBranchAddress("gen_motheridx", &gen_motheridx, &b_gen_motheridx);
fChain->SetBranchAddress("gen_gmotherid", &gen_gmotherid, &b_gen_gmotherid);
fChain->SetBranchAddress("gen_gmotheridx", &gen_gmotheridx, &b_gen_gmotheridx);

//## isolated tracks ###
fChain->SetBranchAddress("isoTracks_charge", &isoTracks_charge, &b_isoTracks_charge);
//fChain->SetBranchAddress("isoTracks_absIso", &isoTracks_absIso, &b_isoTracks_absIso);
//fChain->SetBranchAddress("isoTracks_dz", &isoTracks_dz, &b_isoTracks_dz);
fChain->SetBranchAddress("isoTracks_pdgId", &isoTracks_pdgId, &b_isoTracks_pdgId);
//fChain->SetBranchAddress("isoTracks_isVetoTrack", &isoTracks_isVetoTrack, &b_isoTracks_isVetoTrack);
//fChain->SetBranchAddress("isoTracks_isVetoTrack_v2", &isoTracks_isVetoTrack_v2, &b_isoTracks_isVetoTrack_v2);
//fChain->SetBranchAddress("isoTracks_isVetoTrack_v3", &isoTracks_isVetoTrack_v3, &b_isoTracks_isVetoTrack_v3);

//## taus ###
//fChain->SetBranchAddress("tau_passID", &tau_passID, &b_tau_passID);
fChain->SetBranchAddress("ngoodtaus", &ngoodtaus, &b_ngoodtaus);
//fChain->SetBranchAddress("tau_againstMuonTight", &tau_againstMuonTight, &b_tau_againstMuonTight);
//fChain->SetBranchAddress("tau_againstElectronLoose", &tau_againstElectronLoose, &b_tau_againstElectronLoose);
//fChain->SetBranchAddress("tau_isVetoTau", &tau_isVetoTau, &b_tau_isVetoTau);
//fChain->SetBranchAddress("tau_isVetoTau_v2", &tau_isVetoTau_v2, &b_tau_isVetoTau_v2);

//## leptons ###
//fChain->SetBranchAddress("nvleps", &nvleps, &b_nvleps);
fChain->SetBranchAddress("leps_pt", &leps_pt, &b_leps_pt);
fChain->SetBranchAddress("leps_eta", &leps_eta, &b_leps_eta);
fChain->SetBranchAddress("leps_phi", &leps_phi, &b_leps_phi);
//fChain->SetBranchAddress("leps_id", &leps_id, &b_leps_id);
fChain->SetBranchAddress("els_pt", &els_pt, &b_els_pt);
fChain->SetBranchAddress("els_eta", &els_eta, &b_els_eta);
fChain->SetBranchAddress("els_phi", &els_phi, &b_els_phi);
fChain->SetBranchAddress("els_id", &els_id, &b_els_id);
fChain->SetBranchAddress("mus_pt", &mus_pt, &b_mus_pt);
fChain->SetBranchAddress("mus_eta", &mus_eta, &b_mus_eta);
fChain->SetBranchAddress("mus_phi", &mus_phi, &b_mus_phi);
//fChain->SetBranchAddress("mus_id", &mus_id, &b_mus_id);


Hpt = new TH1F("Hpt", "Hpt", 30, 150, 2000);
genH_dRbb_pt = new TH2F("genH_dRbb_pt", "", 50, 0, 4, 50, 0, 1200);
genW_dRbb_pt = new TH2F("genW_dRbb_pt", "", 50, 0, 4, 50, 0, 1200);

   Notify();
}

Bool_t evtLooper::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void evtLooper::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t evtLooper::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ttree_cxx
