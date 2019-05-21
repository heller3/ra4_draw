#define ttree_cxx
#include "evtLooper.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>

using namespace std;

int evtLooper::HasMedLooseDeepCSV(Double_t weight, Long64_t jentry) {
  //if (fChain == 0) return;                                                                                                                                                       
  Long64_t ientry = LoadTree(jentry);
  Long64_t nb = fChain->GetEntry(jentry);

  int ndeeploose=0;
  int ndeepmedium=0;
  for(unsigned i(0); i<ngoodjets; i++){
    if (ak4pfjets_deepCSV[i] > 0.2219) ndeeploose++;
    if (ak4pfjets_deepCSV[i] > 0.6324) ndeepmedium++;
  }
  if(ndeeploose>=2 && ndeepmedium>=1) return 1;
  else return 0;
}

int evtLooper::HasMedMedDeepCSV(Double_t weight, Long64_t jentry) {
  //if (fChain == 0) return;                                                                                                                                                       
  Long64_t ientry = LoadTree(jentry);
  Long64_t nb = fChain->GetEntry(jentry);

  int ndeepmedium=0;
  for(unsigned i(0); i<ngoodjets; i++){
    if (ak4pfjets_deepCSV[i] > 0.6324) ndeepmedium++;
  }
  if(ndeepmedium>=2) return 1;
  else return 0;
}

int evtLooper::WHLeptons(Double_t weight, Long64_t jentry) {
  //if (fChain == 0) return;                                                                                                                                                       
  Long64_t ientry = LoadTree(jentry);
  Long64_t nb = fChain->GetEntry(jentry);

  int nwhleptons=0;
  if (abs(lep1_pdgid)==11&&leps_pt[0]>30&&lep1_relIso*leps_pt[0]<5) nwhleptons++;
  if (abs(lep1_pdgid)==13&&leps_pt[0]>25&&lep1_relIso*leps_pt[0]<5&&abs(leps_eta[0])<2.1) nwhleptons++;

  if (ngoodleps==2){
     if (abs(lep2_pdgid)==11&&leps_pt[1]>30&&lep2_relIso*leps_pt[1]<5) nwhleptons++;
     if (abs(lep2_pdgid)==13&&leps_pt[1]>25&&lep2_relIso*leps_pt[1]<5&&abs(leps_eta[1])<2.1) nwhleptons++;
  }
  return nwhleptons;
}

/*********************************************************************************************************
 * Used to loop through WH signal events
 */
void evtLooper::sigLoop() {
  if (fChain == 0) return;

  Int_t nEvents = 0;
  Long64_t nentries = fChain->GetEntriesFast();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;                                                                                                                                             


    // defining cuts
    bool preselection = nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50 && this->HasMedLooseDeepCSV(1,jentry)==1 && this->WHLeptons(1,jentry)==1;
    bool signalRegion = mct>170&&mt_met_lep>150; //mbb>90&&mbb<150&&

    // Applying Cuts
    if(preselection && signalRegion && mass_stop==700 && mass_lsp==1) { // filters cut

      vector <Int_t> HbJet_indx;
      vector <Int_t> antiHbJet_indx;
      vector <TLorentzVector*> vec_genHbJets;
      vector <TLorentzVector*> vec_genantiHbJets;
      cout << "this" << endl;

      for (int i=0; i<200; i++) {
        if(gen_id[i]==5 && abs(gen_motherid[i])==25 && find(HbJet_indx.begin(), HbJet_indx.end(), gen_motheridx[i])==HbJet_indx.end()) {
          HbJet_indx.push_back(gen_motheridx[i]);                                                                                        
          vec_genHbJets.push_back(new TLorentzVector);
          vec_genHbJets.back()->SetPtEtaPhiM(gen_pt[i], gen_eta[i], gen_phi[i], gen_m[i]);
        }
        if(gen_id[i]==-5 && abs(gen_motherid[i])==25 && find(antiHbJet_indx.begin(), antiHbJet_indx.end(), gen_motheridx[i])==antiHbJet_indx.end()) {
          antiHbJet_indx.push_back(gen_motheridx[i]);                                                                                
          vec_genantiHbJets.push_back(new TLorentzVector);
          vec_genantiHbJets.back()->SetPtEtaPhiM(gen_pt[i], gen_eta[i], gen_phi[i], gen_m[i]);
        }      
    }
      // Fill histograms
      TLorentzVector HsumJet;
      Double_t H_dRbb;
      for (int i=0; i<vec_genHbJets.size(); i++) {
        for (int j=0; j<vec_genantiHbJets.size(); j++) {
          if (HbJet_indx[i]==antiHbJet_indx[j]) {
            HsumJet = *vec_genHbJets.at(i) + *vec_genantiHbJets.at(j);
            H_dRbb = pow(vec_genHbJets.at(i)->Phi() - vec_genantiHbJets.at(j)->Phi(), 2) + pow(vec_genHbJets.at(i)->Eta() - vec_genantiHbJets.at(j)->Eta(), 2);
            genH_dRbb_pt->Fill(sqrt(H_dRbb), HsumJet.Pt(), weight); // Here is where gendRbb_Hpt is filled                                                                             
          }
        }
      }   
    }
  }

  // Calculateing weights and cross section                                                                                                                                       
  // TFile *f_xsec = new TFile("xsec_susy_13tev.root");
  // if (f_xsec->IsOpen()) printf("File opened successfully\n");
  // TH1F *h_xsec_c1n2 = (TH1F*)f_xsec->Get("h_xsec_c1n2");
  // TAxis *c1n2_x = h_xsec_c1n2->GetXaxis();
  // Int_t c1n2_bin = c1n2_x->FindBin(700);
  Double_t weight = 1.0;//h_xsec_c1n2->GetBinContent(c1n2_bin)*1000/nEvents;

  genH_dRbb_pt->Scale(weight);
}

/*********************************************************************************************************                                                                       
 * Saves filled histograms
 */
void evtLooper::saveHists(const char* fileName) {
  TFile *f = new TFile(fileName, "RECREATE");
  if ( f->IsOpen() ) printf("File opened successfully\n");

  genH_dRbb_pt->Write("genH_dRbb_pt");
  
  f->Write();
  f->Close();
  cout << "Created " << fileName << endl;
}
