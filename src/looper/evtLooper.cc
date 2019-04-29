#define mt2tree_cxx
#include "evtLooper.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>

using namespace std;

/************************************************************************/
bool evtLooper::hasHiggs(Double_t weight, Long64_t jentry) {

  //if (fChain == 0) return;
  Long64_t ientry = LoadTree(jentry);
  Long64_t nb = fChain->GetEntry(jentry);

    vector <TLorentzVector*> vec_Jets;
    vector <Double_t> vec_Jet_CSV;
    for (int i=0; i<njet; i++) {
      if ((jet_btagCSV[i]>=0.8484 && jet_pt[i]>=20) || jet_pt[i]>=30) {
        vec_Jets.push_back(new TLorentzVector);
        vec_Jets.back()->SetPtEtaPhiM(jet_pt[i], jet_eta[i], jet_phi[i], jet_mass[i]);
        vec_Jet_CSV.push_back(jet_btagCSV[i]);
      }
    }
    TLorentzVector HsumJet;
    TLorentzVector HJet1;
    TLorentzVector HJet2;
    Double_t Hscore = 0;
    Double_t HdRbb;
    Double_t lowCSV;
    TLorentzVector lowCSVJet;
    for (int i=0; i<vec_Jets.size()-1; i++) {
      for (int j=i+1; j<vec_Jets.size(); j++) {
        if (vec_Jet_CSV[i]+vec_Jet_CSV[j] > Hscore) {
          Hscore = vec_Jet_CSV[i]+vec_Jet_CSV[j];
          HsumJet = *vec_Jets.at(i) + *vec_Jets.at(j);
	  HJet1 = *vec_Jets.at(i);
	  HJet2 = *vec_Jets.at(j);
      	  HdRbb = pow(vec_Jets.at(i)->Phi()-vec_Jets.at(j)->Phi(), 2) + pow(vec_Jets.at(i)->Eta()-vec_Jets.at(j)->Eta(), 2);
	  if (vec_Jet_CSV[i]<vec_Jet_CSV[j]) lowCSV = vec_Jet_CSV[i]; lowCSVJet = *vec_Jets.at(j);
	  if (vec_Jet_CSV[i]>vec_Jet_CSV[j]) lowCSV = vec_Jet_CSV[j]; lowCSVJet= *vec_Jets.at(i);
        }
      }
    }
    if (nJet30==3 && nBJet20==1) HlowCSV->Fill(lowCSV); CSV_vs_mass->Fill(lowCSV, lowCSVJet.M());
    HJet_pt->Fill(HJet1.Pt(), weight);
    HJet_pt->Fill(HJet2.Pt(), weight);
    //if ((HJet1.M()>90&&HJet1.M()<150) || (HJet2.M()>90&&HJet2.M()<150)) nJets_with_H_mass++;
    HJet_mass->Fill(HJet1.M(), weight);
    HJet_mass->Fill(HJet2.M(), weight);
    H_mass->Fill(HsumJet.M(), weight);
    H_pt->Fill(HsumJet.Pt(), weight);
    if (nJet30==3 && nBJet20==1 && lowCSV < 0.6 && sqrt(HdRbb) < 1) nlow_HdR++;
    if (nJet30==3 && nBJet20==1 && lowCSV < 0.6) H_dRbb_pt->Fill(sqrt(HdRbb), HsumJet.Pt(), weight);

    if (90 < HsumJet.M() &&  HsumJet.M() < 150) {
      return true;
    } else {
      return false;
    }
}

/************************************************************************/
void evtLooper::hasW(Double_t weight, Long64_t jentry) {
  //if (fChain == 0) return;
  Long64_t ientry = LoadTree(jentry);
  Long64_t nb = fChain->GetEntry(jentry);

  // 1 btagged jet, 4 total jets
  if ( nJet30==4) {
    vector <TLorentzVector*> vec_Jets;
    vector <Double_t> vec_Jet_CSV;
    for (int i=0; i<njet; i++) {
      if ((jet_btagCSV[i]>=0.8484 && jet_pt[i]>=20) || jet_pt[i]>=30) {
      	vec_Jets.push_back(new TLorentzVector);
        vec_Jets.back()->SetPtEtaPhiM(jet_pt[i], jet_eta[i], jet_phi[i], jet_mass[i]);
        vec_Jet_CSV.push_back(jet_btagCSV[i]);
      }
    }
    TLorentzVector WsumJet;
    TLorentzVector WJet1;
    TLorentzVector WJet2;
    Double_t Wscore = 2;
    Double_t WdRbb;
    for (int i=0; i<vec_Jets.size()-1; i++) {
      for (int j=i+1; j<vec_Jets.size(); j++) {
	if (vec_Jet_CSV[i]+vec_Jet_CSV[j] < Wscore) {
          Wscore = vec_Jet_CSV[i]+vec_Jet_CSV[j];
          WsumJet = *vec_Jets.at(i) + *vec_Jets.at(j);
	  WJet1 = *vec_Jets.at(i);
          WJet2 = *vec_Jets.at(j);
	  WdRbb = pow(vec_Jets.at(i)->Phi() - vec_Jets.at(j)->Phi(), 2) + pow(vec_Jets.at(i)->Eta() - vec_Jets.at(j)->Eta(), 2);
        }
      }
    }
    WJet_pt->Fill(WJet1.Pt(), weight);
    WJet_pt->Fill(WJet2.Pt(), weight);
    WJet_mass->Fill(WJet1.M(), weight);
    WJet_mass->Fill(WJet2.M(), weight);
    W_mass->Fill(WsumJet.M(), weight);
    W_pt->Fill(WsumJet.Pt(), weight);
    W_dRbb_pt->Fill(sqrt(WdRbb), WsumJet.Pt(), weight);
    }

  if (nJet30==3 && nBJet20==2) {
    n_3j2b++;
    Int_t blah = 0;
    for (int i=0; i<njet; i++) {
      if (jet_btagCSV[i]>=0.8484 && jet_pt[i]>=20 && jet_pt[i]<30) {
	low_pt_3j2b++;
      }
    }
    if (low_pt_3j2b==0) {
    }
  }

}


/************************************************************************/
void evtLooper::genAnalysis(Double_t weight, Long64_t jentry ) {
  if (fChain == 0) return;
  Long64_t ientry = LoadTree(jentry);
  Long64_t nb = fChain->GetEntry(jentry);

  vector <Int_t> Jet_indx;
  vector <Int_t> bJet_indx;
  vector <Int_t> HbJet_indx;
  vector <Int_t> antiHbJet_indx;
  vector <Int_t> WJet_indx;
  vector <Int_t> antiWJet_indx;
  vector <TLorentzVector*> vec_genJets;
  vector <TLorentzVector*> vec_genbJets;
  vector <TLorentzVector*> vec_genHbJets;
  vector <TLorentzVector*> vec_genantiHbJets;
  vector <TLorentzVector*> vec_genWJets;
  vector <TLorentzVector*> vec_genantiWJets;
  for (int i=0; i<ngenPart; i++) {
    if (abs(genPart_pdgId[i])<6 && find(Jet_indx.begin(), Jet_indx.end(), genPart_motherIdx[i])==Jet_indx.end()) {
      Jet_indx.push_back(genPart_motherIdx[i]);
      vec_genJets.push_back(new TLorentzVector);
      vec_genJets.back()->SetPtEtaPhiM(genPart_pt[i], genPart_eta[i], genPart_phi[i], genPart_mass[i]);
    }
    if(abs(genPart_pdgId[i])==5 && abs(genPart_motherId[i])!=25 && find(bJet_indx.begin(), bJet_indx.end(), genPart_motherIdx[i])==bJet_indx.end()) {
      bJet_indx.push_back(genPart_motherIdx[i]);
      genbJet_pt->Fill(genPart_pt[i], weight); // Here is where genbJet_pt is filled
      vec_genbJets.push_back(new TLorentzVector);
      vec_genbJets.back()->SetPtEtaPhiM(genPart_pt[i], genPart_eta[i], genPart_phi[i], genPart_mass[i]);
    }
    if(genPart_pdgId[i]==5 && abs(genPart_motherId[i])==25 && find(HbJet_indx.begin(), HbJet_indx.end(), genPart_motherIdx[i])==HbJet_indx.end()) {
      HbJet_indx.push_back(genPart_motherIdx[i]);
      genHJet_pt->Fill(genPart_pt[i], weight); // Here is where genHbJet_pt is filled
      vec_genHbJets.push_back(new TLorentzVector);
      vec_genHbJets.back()->SetPtEtaPhiM(genPart_pt[i], genPart_eta[i], genPart_phi[i], genPart_mass[i]);
    }
    if(genPart_pdgId[i]==-5 && abs(genPart_motherId[i])==25 && find(antiHbJet_indx.begin(), antiHbJet_indx.end(), genPart_motherIdx[i])==antiHbJet_indx.end()) {
      antiHbJet_indx.push_back(genPart_motherIdx[i]);
      genHJet_pt->Fill(genPart_pt[i], weight); // Here is where genHbJet_pt is also filled
      vec_genantiHbJets.push_back(new TLorentzVector);
      vec_genantiHbJets.back()->SetPtEtaPhiM(genPart_pt[i], genPart_eta[i], genPart_phi[i], genPart_mass[i]);
    }
    if(genPart_pdgId[i]>0 && genPart_pdgId[i]<=6 && abs(genPart_motherId[i])==24 && find(WJet_indx.begin(), WJet_indx.end(), genPart_motherIdx[i])==WJet_indx.end()) {
      WJet_indx.push_back(genPart_motherIdx[i]);
      genWJet_pt->Fill(genPart_pt[i], weight); // Here is where genHbJet_pt is filled
      vec_genWJets.push_back(new TLorentzVector);
      vec_genWJets.back()->SetPtEtaPhiM(genPart_pt[i], genPart_eta[i], genPart_phi[i], genPart_mass[i]);
    }
    if(genPart_pdgId[i]<0 && genPart_pdgId[i]>=-6 && abs(genPart_motherId[i])==24 && find(antiWJet_indx.begin(), antiWJet_indx.end(), genPart_motherIdx[i])==antiWJet_indx.end()) {
      antiWJet_indx.push_back(genPart_motherIdx[i]);
      genWJet_pt->Fill(genPart_pt[i], weight); // Here is where genHbJet_pt is also filled
      vec_genantiWJets.push_back(new TLorentzVector);
      vec_genantiWJets.back()->SetPtEtaPhiM(genPart_pt[i], genPart_eta[i], genPart_phi[i], genPart_mass[i]);
    }
  }

  TLorentzVector HsumJet;
  TLorentzVector WsumJet;
  Double_t H_dRbb;
  Double_t W_dRbb;
  for (int i=0; i<vec_genHbJets.size(); i++) {
    for (int j=0; j<vec_genantiHbJets.size(); j++) {
      if (HbJet_indx[i]==antiHbJet_indx[j]) {
	HsumJet = *vec_genHbJets.at(i) + *vec_genantiHbJets.at(j);
	H_dRbb = pow(vec_genHbJets.at(i)->Phi() - vec_genantiHbJets.at(j)->Phi(), 2) + pow(vec_genHbJets.at(i)->Eta() - vec_genantiHbJets.at(j)->Eta(), 2);
	if (sqrt(H_dRbb) < 1) gen_nlow_HdR++;
	genH_dRbb_pt->Fill(sqrt(H_dRbb), HsumJet.Pt(), weight); // Here is where gendRbb_Hpt is filled
	genH_pt->Fill(HsumJet.Pt(), weight);
	genH_mass->Fill(HsumJet.M());
      }
    }
  }
  for (int i=0; i<vec_genWJets.size(); i++) {
    for (int j=0; j<vec_genantiWJets.size(); j++) {
      if (WJet_indx[i]==antiWJet_indx[j]) {
        WsumJet = *vec_genWJets.at(i) + *vec_genantiWJets.at(j);
        W_dRbb = pow(vec_genWJets.at(i)->Phi() - vec_genantiWJets.at(j)->Phi(), 2) + pow(vec_genWJets.at(i)->Eta() - vec_genantiWJets.at(j)->Eta(), 2);
        genW_dRbb_pt->Fill(sqrt(W_dRbb), WsumJet.Pt(), weight); // Here is where gendRbb_Hpt is filled
        genW_pt->Fill(WsumJet.Pt(), weight);
        genW_mass->Fill(WsumJet.M());
      }
    }
  }

}

/*********************************************************************************************************                                                                       
 * Used to loop through SM background events                                                                                                                                       */
void evtLooper::bgLoop() {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;

    // Applying selections
    if((nMuons10==0 && nElectrons10==0 && nPFLep5LowMT == 0 && nPFHad10LowMT == 0) && // lepveto cut                                                                           
       (HLT_PFHT900 || HLT_PFJet450 || HLT_PFHT300_PFMET110 || HLT_PFMET120_PFMHT120 || HLT_PFMETNoMu120_PFMHTNoMu120) && // trigs cut                                            
       ((nVert > 0) && (nJet30 > 1) && (deltaPhiMin > 0.3) && (diffMetMht/met_pt < 0.5) && (mt2 > 200)
	&& ( (ht > 250 && met_pt > 250) || (ht > 1000 && met_pt > 30) )) && // srbase cut                                                                                         
       (Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_globalTightHalo2016Filter
        && Flag_badMuonFilter && Flag_goodVertices && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_badChargedHadronFilter
	&& nJet30FailId == 0 && met_miniaodPt / met_caloPt < 5.0 && nJet200MuFrac50DphiMet == 0) && 
       (nJet30 > 2 && nJet30 < 6 && this->hasHiggs(evt_scale1fb, jentry) && ht>450 && ht<1000)) { // filters cut
        // Fill the histograms
        if(!std::isnan(ht)) htH->Fill(ht, evt_scale1fb);
        if(!std::isnan(met_pt)) met_ptH->Fill(met_pt, evt_scale1fb);
        if(!std::isnan(mt2)) mt2H->Fill(mt2, evt_scale1fb);
        if(!std::isnan(nJet30)) nJet30H->Fill(nJet30, evt_scale1fb);

	// bins to compare to Mt2 analysis results
	if(!std::isnan(mt2) && (nJet30==3 && nBJet20==1) && (ht>=575 && ht<=1000)) mt2_23j1b_high->Fill(mt2, evt_scale1fb);
	if(!std::isnan(mt2) && (nJet30==3 && nBJet20==2) && (ht>=575 && ht<=1000)) mt2_23j2b_high->Fill(mt2, evt_scale1fb);
	if(!std::isnan(mt2) && ((nJet30==4||nJet30==5) && nBJet20==1) && (ht>=575 &&ht<=1000)) mt2_46j1b_high->Fill(mt2, evt_scale1fb);
	if(!std::isnan(mt2) && ((nJet30==4||nJet30==5) && nBJet20==2) && (ht>=575 &&ht<=1000)) mt2_46j2b_high->Fill(mt2, evt_scale1fb);
	if(!std::isnan(mt2) && (nJet30==3 && nBJet20==1) && (ht>=450 && ht<=575)) mt2_23j1b_med->Fill(mt2, evt_scale1fb);
        if(!std::isnan(mt2) && (nJet30==3 && nBJet20==2) && (ht>=450 && ht<=575)) mt2_23j2b_med->Fill(mt2, evt_scale1fb);
        if(!std::isnan(mt2) && ((nJet30==4||nJet30==5) && nBJet20==1) && (ht>=450 &&ht<=575)) mt2_46j1b_med->Fill(mt2, evt_scale1fb);
        if(!std::isnan(mt2) && ((nJet30==4||nJet30==5) && nBJet20==2) && (ht>=450 &&ht<=575)) mt2_46j2b_med->Fill(mt2, evt_scale1fb);

	this->hasW(evt_scale1fb, jentry);
	      	
    }
  }
  cout << "Finished current TChain" << endl;
 }

/*********************************************************************************************************
 * Used to loop through WH signal events
 */
void evtLooper::sigLoop() {
  if (fChain == 0) return;
  
  Double_t luminosity = 1;
  Int_t nEvents = 0;
  Double_t HbbBR = 0.5824;
  Long64_t nentries = fChain->GetEntriesFast();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;                                                                                                                                             

    if(!std::isnan(evt_nEvts)) nEvents += 1;

    // Applying Cuts
    if((nMuons10==0 && nElectrons10==0 && nPFLep5LowMT == 0 && nPFHad10LowMT == 0) && // lepveto cut                                                                              
       (HLT_PFHT900 || HLT_PFJet450 || HLT_PFHT300_PFMET110 || HLT_PFMET120_PFMHT120 || HLT_PFMETNoMu120_PFMHTNoMu120) && // trigs cut                                            
       ((nVert > 0) && (nJet30 > 1) && (deltaPhiMin > 0.3) && (diffMetMht/met_pt < 0.5) && (mt2 > 200)
        && ( (ht > 250 && met_pt > 250) || (ht > 1000 && met_pt > 30) )) && // srbase cut                                                                                         
       (Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_globalTightHalo2016Filter
        && Flag_badMuonFilter && Flag_goodVertices && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_badChargedHadronFilter
        && nJet30FailId == 0 && met_miniaodPt / met_caloPt < 5.0 && nJet200MuFrac50DphiMet == 0) && 
      (nJet30>2 && nJet30<6 && this->hasHiggs(1, jentry) && ht>450 && ht<1000)) { // filters cut
      // Fill histograms
        if(!std::isnan(ht)) htH->Fill(ht);
        if(!std::isnan(met_pt)) met_ptH->Fill(met_pt);
        if(!std::isnan(mt2)) mt2H->Fill(mt2);
	if(!std::isnan(nJet30)) nJet30H->Fill(nJet30);
	
	if(!std::isnan(mt2) && (nJet30==3 && nBJet20==1) && (ht>=575 && ht<=1000)) mt2_23j1b_high->Fill(mt2);
	if(!std::isnan(mt2) && (nJet30==3 && nBJet20==2) && (ht>=575 && ht<=1000)) mt2_23j2b_high->Fill(mt2);
	if(!std::isnan(mt2) && ((nJet30==4||nJet30==5) && nBJet20==1) && (ht>=575 &&ht<=1000)) mt2_46j1b_high->Fill(mt2);
	if(!std::isnan(mt2) && ((nJet30==4||nJet30==5) && nBJet20==2) && (ht>=575 &&ht<=1000)) mt2_46j2b_high->Fill(mt2);
	if(!std::isnan(mt2) && (nJet30==3 && nBJet20==1) && (ht>=450 && ht<=575)) mt2_23j1b_med->Fill(mt2);
	if(!std::isnan(mt2) && (nJet30==3 && nBJet20==2) && (ht>=450 && ht<=575)) mt2_23j2b_med->Fill(mt2);
	if(!std::isnan(mt2) && ((nJet30==4||nJet30==5) && nBJet20==1) && (ht>=450 &&ht<=575)) mt2_46j1b_med->Fill(mt2);
	if(!std::isnan(mt2) && ((nJet30==4||nJet30==5) && nBJet20==2) && (ht>=450 &&ht<=575)) mt2_46j2b_med->Fill(mt2);

	this->hasW(1, jentry);
	this->genAnalysis(1, jentry);
	        
    }
  }

  // Calculateing weights and cross section                                                                                                                                       
  TFile *f_xsec = new TFile("xsec_susy_13tev.root");
  if (f_xsec->IsOpen()) printf("File opened successfully\n");
  TH1F *h_xsec_c1n2 = (TH1F*)f_xsec->Get("h_xsec_c1n2");
  TH1F *h_xsec_squark = (TH1F*)f_xsec->Get("h_xsec_squark");
  TH1F *h_xsec_stop = (TH1F*)f_xsec->Get("h_xsec_stop");
  TH1F *h_xsec_gluino = (TH1F*)f_xsec->Get("h_xsec_gluino");
  TAxis *c1n2_x = h_xsec_c1n2->GetXaxis();
  TAxis *squark_x = h_xsec_squark->GetXaxis();
  TAxis *stop_x = h_xsec_stop->GetXaxis();
  TAxis *gluino_x = h_xsec_gluino->GetXaxis();
  Int_t c1n2_bin = c1n2_x->FindBin(700);
  Int_t squark_bin = squark_x->FindBin(600);
  Int_t stop_bin = stop_x->FindBin(500);
  Int_t gluino_bin = gluino_x->FindBin(1000);
  Double_t weight = h_xsec_c1n2->GetBinContent(c1n2_bin)*1000/nEvents;

  htH->Scale(weight);
  met_ptH->Scale(weight);
  mt2H->Scale(weight);
  nJet30H->Scale(weight);

  HJet_pt->Scale(weight);
  H_mass->Scale(weight);
  H_pt->Scale(weight);
  H_dRbb_pt->Scale(weight);

  WJet_pt->Scale(weight);
  W_mass->Scale(weight);
  W_pt->Scale(weight);
  W_dRbb_pt->Scale(weight);

  genHJet_pt->Scale(weight);
  genH_mass->Scale(weight);
  genH_pt->Scale(weight);
  genH_dRbb_pt->Scale(weight);

  mt2_23j1b_high->Scale(weight);                                                                                                                                   
  mt2_23j2b_high->Scale(weight);                                                                                                                                    
  mt2_46j1b_high->Scale(weight);                                                                                                                                    
  mt2_46j2b_high->Scale(weight);                                                                                                                                    
  mt2_23j1b_med->Scale(weight);                                                                                                                                      
  mt2_23j2b_med->Scale(weight);                                                                                                                                      
  mt2_46j1b_med->Scale(weight);                                                                                                                                      
  mt2_46j2b_med->Scale(weight);
  
  cout << n_3j2b << endl;
  cout << low_pt_3j2b << endl;
  cout << nlow_HdR << endl;
  cout << gen_nlow_HdR << endl;
  cout << nJets_with_H_mass << endl;
  cout << "Finished current TChain" << endl;
}

/*********************************************************************************************************                                                                          * find the minimum mass windoes                                                                                                                                                
*/
void evtLooper::findMassWindow() {
  TAxis *H_x = H_mass->GetXaxis();
  TAxis *W_x = W_mass->GetXaxis();
  Double_t H_range = 300;
  Double_t W_range = 300;
  int H_low;
  int H_high;
  int W_low;
  int W_high;
  for (int i=0; i<125; i+=1) {
    for (int j=125; j<300; j+=1) {
      Int_t H_lowbin = H_x->FindBin(i);
      Int_t H_highbin = H_x->FindBin(j);
      Double_t H_percent = (Double_t) H_mass->Integral(H_lowbin, H_highbin)/H_mass->GetEntries();
      cout << H_percent << endl;
      if ( H_percent>=0.7 && j-i < H_range ) {
	H_range = j-i;
        H_low = i;
        H_high = j;
      }
    }
  }
  for (int i=0; i<82; i+=1) {
    for (int j=82; j<350; j+=1) {
      Int_t W_lowbin = W_x->FindBin(i);
      Int_t W_highbin = W_x->FindBin(j);
      Double_t W_percent = (Double_t) W_mass->Integral(W_lowbin, W_highbin)/W_mass->GetEntries();
      if ( W_percent>=0.6 && j-i < W_range ) {
        W_range = j-i;
        W_low = i;
        W_high = j;
      }
    }
  }
  cout << "Higgs from "  << H_low << " to " <<  H_high << endl;
  cout << "W from "  << W_low << " to " <<  W_high << endl;
}

/*********************************************************************************************************                                                                       
 * Saves filled histograms
 */
void evtLooper::saveHists(const char* fileName) {
  TFile *f = new TFile(fileName, "RECREATE");
  if ( f->IsOpen() ) printf("File opened successfully\n");

  htH->Write("ht");
  met_ptH->Write("met_pt");
  mt2H->Write("mt2");
  nJet30H->Write("nJet30");

  HlowCSV->Write("HlowCSV");
  CSV_vs_mass->Write("CSV_vs_mass");

  HJet_pt->Write("HJet_pt");
  HJet_mass->Write("HJet_mass");
  H_mass->Write("H_mass");
  H_mass_dRbb->Write("H_mass_dRbb");
  H_pt->Write("H_pt");
  H_dRbb_pt->Write("H_dRbb_pt");

  WJet_pt->Write("WJet_pt");
  WJet_mass->Write("WJet_mass");
  W_mass->Write("W_mass");
  W_mass_dRbb->Write("W_mass_dRbb");
  W_pt->Write("W_pt");
  W_dRbb_pt->Write("W_dRbb_pt");

  genbJet_pt->Write("genbJet_pt");
  genHJet_pt->Write("genHJet_pt");
  genH_mass->Write("genH_mass");
  genH_pt->Write("genH_pt");
  genH_dRbb_pt->Write("genH_dRbb_pt");

  genWJet_pt->Write("genWJet_pt");
  genW_mass->Write("genW_mass");
  genW_pt->Write("genW_pt");
  genW_dRbb_pt->Write("genW_dRbb_pt");

  mt2_23j1b_high->Write("mt2_23j1b_high");
  mt2_23j2b_high->Write("mt2_23j2b_high");
  mt2_46j1b_high->Write("mt2_46j1b_high");
  mt2_46j2b_high->Write("mt2_46j2b_high");
  mt2_23j1b_med->Write("mt2_23j1b_med");
  mt2_23j2b_med->Write("mt2_23j2b_med");
  mt2_46j1b_med->Write("mt2_46j1b_med");
  mt2_46j2b_med->Write("mt2_46j2b_med");
 

  
  f->Write();
  f->Close();
  cout << "Created " << fileName << endl;
}
