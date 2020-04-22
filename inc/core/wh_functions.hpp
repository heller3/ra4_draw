#ifndef H_WH_FUNCTIONS
#define H_WH_FUNCTIONS

#include <cstddef>
#include <string>
#include "core/named_func.hpp"
 
namespace WH_Functions{

  // Miscellaneous

  extern const NamedFunc ST_up;
  extern const NamedFunc ST_off;
  extern const NamedFunc ST_down;
  extern const NamedFunc fake_up;
  extern const NamedFunc fake_down;
  extern const NamedFunc ttbar_genmet_fix;
  extern const NamedFunc ttbar_genmet_antifix;
  extern const NamedFunc failTauVetos;
  extern const NamedFunc yearWeight;
  extern const NamedFunc HasMedLooseCSV;
  extern const NamedFunc nBTagInFat;
  extern const NamedFunc OneDoubleBFat;
  extern const NamedFunc HasOnHiggsJet;
  extern const NamedFunc HasBoostedHiggs;
  extern const NamedFunc HasBoostedHiggsNoLepton;
  extern const NamedFunc HasReallyBoostedHiggs;
  extern const NamedFunc HasLooseBoostedHiggs;
  extern const NamedFunc higgsMistagProb;
  extern const NamedFunc nBoostedFatJet;
  extern const NamedFunc nFatJet250;
  extern const NamedFunc nBInFatJet;
  extern const NamedFunc nLooseHiggsTags;
  extern const NamedFunc nVLooseHiggsTags;
  extern const NamedFunc nJetsGood;
  extern const NamedFunc NHighPtNu;
  extern const NamedFunc HighNuPt;
  extern const NamedFunc zpt;
  extern const NamedFunc wpt;
  extern const NamedFunc wpt_lnu;
  extern const NamedFunc higgs_pt;
  extern const NamedFunc bDeltaRHiggs;
  extern const NamedFunc LeadingToppT;
  extern const NamedFunc LeadingWpT;
  extern const NamedFunc SubLeadingWpT;
  extern const NamedFunc HasHadronicTau;
  extern const NamedFunc n_true_emu;
  extern const NamedFunc PassThirdJetHighpTVeto;
  //extern const NamedFunc wpt_reco;
  extern const NamedFunc HasBFailedTag;
  extern const NamedFunc HasMedLooseCSV;
  extern const NamedFunc HasMedMedDeepCSV;
  extern const NamedFunc HasExactMedMedDeepCSV;
  extern const NamedFunc HasLooseLooseDeepCSV;
  extern const NamedFunc HasMedLooseDeepCSV;
  extern const NamedFunc HasLooseNoMedDeepCSV;
  extern const NamedFunc nDeepMedBTagged;
  extern const NamedFunc nDeepLooseBTagged;
  extern const NamedFunc nDeepLooseCvBTagged;
  extern const NamedFunc nDeepMedCTagged;
  extern const NamedFunc nDeepMedCvBTagged;
  extern const NamedFunc nDeepTightCTagged;
  extern const NamedFunc nDeepTightCvBTagged;
  extern const NamedFunc nAK8jets;
  extern const NamedFunc nHiggsTag;
  extern const NamedFunc max_ak8pfjets_deepdisc_hbb;
  extern const NamedFunc bJetPt;
  extern const NamedFunc HasNoBs;
  extern const NamedFunc WHLeptons;
  extern const NamedFunc NBJets;
  extern const NamedFunc nRealBs;
  extern const NamedFunc nRealBs2;
  extern const NamedFunc nRealBtags;
  extern const NamedFunc nGenLightLeps;
  extern const NamedFunc LostHadTaus;
  extern const NamedFunc FatJet_HighestHScore;
  extern const NamedFunc FatJet_ClosestMSD;
  extern const NamedFunc sortedJetsPt_Leading;
  extern const NamedFunc sortedJetsPt_subLeading;
  extern const NamedFunc sortedJetsCSV_Leading;
  extern const NamedFunc sortedJetsCSV_subLeading;
  extern const NamedFunc sortedJetsCSV_deltaR;
  extern const NamedFunc nEventsGluonSplit;
  extern const NamedFunc nGenBs;
  extern const NamedFunc nGenBsFromGluons;
  extern const NamedFunc nGenBs_ptG15;
  extern const NamedFunc nGenBsFromGluons_ptG15;
  extern const NamedFunc nGenBs_ptG30;
  extern const NamedFunc nGenBsFromGluons_ptG30;
  extern const NamedFunc genBpT;
  extern const NamedFunc genBeta;
  extern const NamedFunc genBeta_mostForward;
  extern const NamedFunc bDeltaRGluonSplit;
  extern const NamedFunc bDeltaR;
  extern const NamedFunc bDeltaRfromtop;
  extern const NamedFunc minDR_lep_bquark;
  extern const NamedFunc bMother;
  extern const NamedFunc bMother_pt15;
  extern const NamedFunc bMother_pt30;
  extern const NamedFunc genB_leadingpT;
  extern const NamedFunc genB_subleadingpT;
  extern const NamedFunc bDeltaPhi;
  extern const NamedFunc bmetMinDeltaPhi;
  extern const NamedFunc nHeavy;
  extern const NamedFunc nLight;
  extern const NamedFunc gluBTagged;
  extern const NamedFunc nModEventsGluonSplit;
  extern const NamedFunc leadingBMother_pt20;
  extern const NamedFunc subleadingBMother_pt20;
  extern const NamedFunc outsideHiggsWindow;
  extern const NamedFunc passTriggers;
  extern const NamedFunc mht;
  extern const NamedFunc mht_phi;
  extern const NamedFunc W_pt_lep_met;
  extern const NamedFunc W_pt_lep_mht;
  extern const NamedFunc mt_lep_mht;
  extern const NamedFunc mt_lep_met_rec;
  extern const NamedFunc dilepton_mass;
  extern const NamedFunc dilepton_pt;
  extern const NamedFunc elPt;
  extern const NamedFunc muPt;
  extern const NamedFunc HasHEMevent;
  extern const NamedFunc HasHEMjet;
  extern const NamedFunc mcHEMWeight;
  extern const NamedFunc noPrefireWeight;
  extern const NamedFunc mt_met_lep_uncorr;
  extern const NamedFunc pfmet_uncorr_func;




  // Basic Jet Pt
  extern const NamedFunc LeadingJetPt;
  extern const NamedFunc SubLeadingJetPt;
  extern const NamedFunc SubSubLeadingJetPt;
  extern const NamedFunc deltaRLeadingJets;
  extern const NamedFunc deltaPhiLeadingJets;

  // Funcs to look at number of 3rd jets which are actually B's
  extern const NamedFunc LeadingNonBJetPt;
  extern const NamedFunc LeadingFakeNonBJetPt;
  extern const NamedFunc LeadingRealNonBJetPt;
  extern const NamedFunc LeadingNonBJetPt_med;
  extern const NamedFunc LeadingFakeNonBJetPt_med;
  extern const NamedFunc LeadingRealNonBJetPt_med;

  // Funcs to look at number of B-tags which are fakes
  extern const NamedFunc LeadingBJetPt;
  extern const NamedFunc LeadingRealBJetPt;
  extern const NamedFunc LeadingFakeBJetPt;
  extern const NamedFunc LeadingBJetPt_med;
  extern const NamedFunc LeadingRealBJetPt_med;
  extern const NamedFunc LeadingFakeBJetPt_med;
  extern const NamedFunc SubLeadingBJetPt;
  extern const NamedFunc SubLeadingRealBJetPt;
  extern const NamedFunc SubLeadingFakeBJetPt;

  extern const NamedFunc nNonBTagJets;
  extern const NamedFunc signature_pT;

  // Looking at gen level ISR
  extern const NamedFunc genISRPt;
  extern const NamedFunc genISRgenMETdPhi;
  extern const NamedFunc genISRrecoMETdPhi;
  extern const NamedFunc genISRrecoISRdPhi;
  extern const NamedFunc genISRrecoISRDeltaPt;
  extern const NamedFunc truthOrigin3rdJet;

  extern const NamedFunc bbmass;
  extern const NamedFunc nGenBs_ptG15;
  extern const NamedFunc nGenBs_ptG30;
  extern const NamedFunc nGenBs_forward;
  extern const NamedFunc nGenBs_central;
  extern const NamedFunc nGenBs_ptG30_central;

}

#endif
