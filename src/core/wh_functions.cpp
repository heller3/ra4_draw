#include "core/wh_functions.hpp"
#include <algorithm> //std::min
#include <math.h>

#include "TVector2.h"
#include "TLorentzVector.h"

#include "core/utilities.hpp"
#include "core/config_parser.hpp"


using namespace std;

namespace WH_Functions{

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Miscilaneous

  const NamedFunc NHighPtNu("NHighPtNu",[](const Baby &b) -> NamedFunc::ScalarType{
      int nnu=0;
        for (unsigned i(0); i<b.gen_pt()->size(); i++){
        if (abs(b.gen_motherid()->at(i))==24 && ( abs(b.gen_id()->at(i)) == 12 || abs(b.gen_id()->at(i)) == 14 || abs(b.gen_id()->at(i)) == 16) && b.gen_pt()->at(i) > 200 ) nnu++;
      }
      return nnu;
      });

  /*
   * returns 1 if there are two b jets with at least one Medium and one Loose score
   * returns 0 otherwise
   */
  const NamedFunc HasMedLooseCSV("HasMedLooseCSV",[](const Baby &b) -> NamedFunc::ScalarType{
      int nloose=0;
      int nmedium=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
      	if (b.ak4pfjets_deepCSV()->at(i) > 0.2217) nloose++;
      	if (b.ak4pfjets_deepCSV()->at(i) > 0.6321) nmedium++;
      }
      if(nloose>=2 && nmedium>=1) return 1;
      else return 0;
    });

  /*
   * returns 1 if there are two b jets with at least a Medium score
   * returns 0 otherwise
   */
  const NamedFunc HasMedMedCSV("HasMedMedCSV",[](const Baby &b) -> NamedFunc::ScalarType{
      int nmedium=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_deepCSV()->at(i) > 0.6321) nmedium++;
      }
      if(nmedium>=2) return 1;
      else return 0;
    });

  /*
  * Returns number of >loose bjets
  */
  const NamedFunc NBJets("NBJets",[](const Baby &b) -> NamedFunc::ScalarType{
      int nloose=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_deepCSV()->at(i) > 0.2217) nloose++;
      }
      return nloose;
    });

  /*
   * returns number of good, isolated leptons in event
   */
  const NamedFunc WHLeptons("WHLeptons",[](const Baby &b) -> NamedFunc::ScalarType{
      int nwhleptons=0;
      if (abs(b.lep1_pdgid())==11&&b.leps_pt()->at(0)>30&&b.lep1_relIso()*b.leps_pt()->at(0)<5) nwhleptons++;
      if (abs(b.lep1_pdgid())==13&&b.leps_pt()->at(0)>25&&b.lep1_relIso()*b.leps_pt()->at(0)<5&&abs(b.leps_eta()->at(0))<2.1) nwhleptons++;

      if (b.leps_pt()->size()==2){
	if (abs(b.lep2_pdgid())==11&&b.leps_pt()->at(1)>30&&b.lep2_relIso()*b.leps_pt()->at(1)<5) nwhleptons++;
	if (abs(b.lep2_pdgid())==13&&b.leps_pt()->at(1)>25&&b.lep2_relIso()*b.leps_pt()->at(1)<5&&abs(b.leps_eta()->at(1))<2.1) nwhleptons++;
      }

      return nwhleptons;
    });


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Basic Jet Pt

  /*
   * returns pt of highest pt jet
   */
  const NamedFunc LeadingJetPt("LeadingJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_pt()->at(i) > maxpt) maxpt =  b.ak4pfjets_pt()->at(i);
      }
      return maxpt;
    });
  

  /*
   * returns pt of second highest pt jet
   */
  const NamedFunc SubLeadingJetPt("SubLeadingJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxpt=0;
      float secondmaxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_pt()->at(i) > secondmaxpt && b.ak4pfjets_pt()->at(i)<=maxpt){
          secondmaxpt = b.ak4pfjets_pt()->at(i);
        }
        else if (b.ak4pfjets_pt()->at(i) > maxpt){
          secondmaxpt=maxpt;
          maxpt =  b.ak4pfjets_pt()->at(i);
        }
      }
      return secondmaxpt;
    });

  /*
   * returns pt of the third highest pt jet
   */
  const NamedFunc SubSubLeadingJetPt("SubSubLeadingJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxpt=0;
      float secondmaxpt=0;
      float thirdmaxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_pt()->at(i) > thirdmaxpt && b.ak4pfjets_pt()->at(i)<=secondmaxpt){
          thirdmaxpt = b.ak4pfjets_pt()->at(i);
        }
        else if (b.ak4pfjets_pt()->at(i) > maxpt){
          thirdmaxpt=secondmaxpt;
          secondmaxpt=maxpt;
          maxpt=b.ak4pfjets_pt()->at(i);
        }
        else if (b.ak4pfjets_pt()->at(i) <= maxpt && b.ak4pfjets_pt()->at(i) > secondmaxpt){
          thirdmaxpt = secondmaxpt;
          secondmaxpt = b.ak4pfjets_pt()->at(i);
        }
      }
      return thirdmaxpt;
    });


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Funcs to look at number of 3rd jets which are actually B's

  /*
   * Pt of the non-bjet with the highest pt
   */
  const NamedFunc LeadingNonBJetPt("LeadingNonBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_pt()->at(i) > maxpt && b.ak4pfjets_deepCSV()->at(i) < 0.2217){
           maxpt = b.ak4pfjets_pt()->at(i);
        }
      }
      return maxpt;
    });

  const NamedFunc LeadingFakeNonBJetPt("LeadingFakeNonBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_pt()->at(i) > maxpt && abs(b.ak4pfjets_hadron_flavor()->at(i))==5 && b.ak4pfjets_deepCSV()->at(i) < 0.2217){
           maxpt = b.ak4pfjets_pt()->at(i);
        }
      }
      return maxpt;
    });

   const NamedFunc LeadingRealNonBJetPt("LeadingRealNonBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_pt()->at(i) > maxpt && abs(b.ak4pfjets_hadron_flavor()->at(i))!=5 && b.ak4pfjets_deepCSV()->at(i) < 0.2217){
           maxpt = b.ak4pfjets_pt()->at(i);
        }
      }
      return maxpt;
    });

   const NamedFunc LeadingNonBJetPt_med("LeadingNonBJetPt_med",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_pt()->at(i) > maxpt && b.ak4pfjets_deepCSV()->at(i) < 0.6321){
           maxpt = b.ak4pfjets_pt()->at(i);
        }
      }
      return maxpt;
    });

  const NamedFunc LeadingFakeNonBJetPt_med("LeadingFakeNonBJetPt_med",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_pt()->at(i) > maxpt && abs(b.ak4pfjets_hadron_flavor()->at(i))==5 && b.ak4pfjets_deepCSV()->at(i) < 0.6321){
           maxpt = b.ak4pfjets_pt()->at(i);
        }
      }
      return maxpt;
    });

   const NamedFunc LeadingRealNonBJetPt_med("LeadingRealNonBJetPt_med",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_pt()->at(i) > maxpt && abs(b.ak4pfjets_hadron_flavor()->at(i))!=5 && b.ak4pfjets_deepCSV()->at(i) < 0.6321){
           maxpt = b.ak4pfjets_pt()->at(i);
        }
      }
      return maxpt;
    });


   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Funcs to look at number of B-tags which are fakes

   const NamedFunc LeadingBJetPt("LeadingBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxCSV=0;
      float maxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_deepCSV()->at(i) > maxCSV && b.ak4pfjets_deepCSV()->at(i) > 0.2217){
           maxCSV = b.ak4pfjets_deepCSV()->at(i);
           maxpt = b.ak4pfjets_pt()->at(i);

        }
      }
      return maxpt;
    });

  const NamedFunc LeadingRealBJetPt("LeadingRealBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxCSV=0;
      float maxpt=0;
      float output=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_deepCSV()->at(i) > maxCSV && b.ak4pfjets_deepCSV()->at(i) > 0.2217){
           maxCSV = b.ak4pfjets_deepCSV()->at(i);
           maxpt = b.ak4pfjets_pt()->at(i);
           if (abs(b.ak4pfjets_hadron_flavor()->at(i))==5) output=maxpt;
           else if (abs(b.ak4pfjets_hadron_flavor()->at(i))!=5) output=0;
        }
      }
      return output;
    });

   const NamedFunc LeadingFakeBJetPt("LeadingFakeBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxCSV=0;
      float maxpt=0;
      float output=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_deepCSV()->at(i) > maxCSV && b.ak4pfjets_deepCSV()->at(i) > 0.2217){
           maxCSV = b.ak4pfjets_deepCSV()->at(i);
           maxpt = b.ak4pfjets_pt()->at(i);
           if (abs(b.ak4pfjets_hadron_flavor()->at(i))!=5) output=maxpt;
           else if (abs(b.ak4pfjets_hadron_flavor()->at(i))==5) output=0;
        }
      }
      return output;
    });

    /*
   * returns sub-leading b jet pt
   */
  const NamedFunc SubLeadingBJetPt("SubLeadingBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxCSV=0;
      float maxpt=0;
      float secondmaxCSV=0;
      float secondmaxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_deepCSV()->at(i) > 0.2217 && b.ak4pfjets_deepCSV()->at(i) > secondmaxCSV && b.ak4pfjets_deepCSV()->at(i)<=maxCSV){
            secondmaxCSV = b.ak4pfjets_deepCSV()->at(i);
            secondmaxpt = b.ak4pfjets_pt()->at(i);
        }
        else if (b.ak4pfjets_deepCSV()->at(i) > 0.2217 && b.ak4pfjets_deepCSV()->at(i) > maxCSV){
            secondmaxCSV=maxCSV;
            maxCSV =  b.ak4pfjets_deepCSV()->at(i);
            secondmaxpt=maxpt;
            maxpt =  b.ak4pfjets_pt()->at(i);
        }
      }
      return secondmaxpt;
    });

  const NamedFunc SubLeadingRealBJetPt("SubLeadingRealBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxCSV=0;
      float maxpt=0;
      bool maxisb=false;
      float secondmaxCSV=0;
      float secondmaxpt=0;
      float output=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_deepCSV()->at(i) > 0.2217 && b.ak4pfjets_deepCSV()->at(i) > secondmaxCSV && b.ak4pfjets_deepCSV()->at(i)<=maxCSV){
            secondmaxCSV = b.ak4pfjets_deepCSV()->at(i);
            secondmaxpt = b.ak4pfjets_pt()->at(i);
            if (abs(b.ak4pfjets_hadron_flavor()->at(i))==5) output=secondmaxpt;
            else if (abs(b.ak4pfjets_hadron_flavor()->at(i))!=5) output=0;
        }
        else if (b.ak4pfjets_deepCSV()->at(i) > 0.2217 && b.ak4pfjets_deepCSV()->at(i) > maxCSV){
            secondmaxCSV=maxCSV;
            maxCSV =  b.ak4pfjets_deepCSV()->at(i);
            secondmaxpt=maxpt;
            maxpt =  b.ak4pfjets_pt()->at(i);
            if (maxisb) output=secondmaxpt;
            if (!maxisb) output=0;
            if (abs(b.ak4pfjets_hadron_flavor()->at(i))==5) maxisb=true;
            if (abs(b.ak4pfjets_hadron_flavor()->at(i))!=5) maxisb=false;
        }
      }
      return output;
    });

  const NamedFunc SubLeadingFakeBJetPt("SubLeadingFakeBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxCSV=0;
      float maxpt=0;
      bool maxisb=false;
      float secondmaxCSV=0;
      float secondmaxpt=0;
      float output=0;
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_deepCSV()->at(i) > 0.2217 && b.ak4pfjets_deepCSV()->at(i) > secondmaxCSV && b.ak4pfjets_deepCSV()->at(i)<=maxCSV){
            secondmaxCSV = b.ak4pfjets_deepCSV()->at(i);
            secondmaxpt = b.ak4pfjets_pt()->at(i);
            if (abs(b.ak4pfjets_hadron_flavor()->at(i))!=5) output=secondmaxpt;
            else if (abs(b.ak4pfjets_hadron_flavor()->at(i))==5) output=0;
         }
        else if (b.ak4pfjets_deepCSV()->at(i) > 0.2217 && b.ak4pfjets_deepCSV()->at(i) > maxCSV){
            secondmaxCSV=maxCSV;
            maxCSV =  b.ak4pfjets_deepCSV()->at(i);
            secondmaxpt=maxpt;
            maxpt =  b.ak4pfjets_pt()->at(i);
            if (!maxisb) output=secondmaxpt;
            if (maxisb) output=0;
            if (abs(b.ak4pfjets_hadron_flavor()->at(i))==5) maxisb=true;
            if (abs(b.ak4pfjets_hadron_flavor()->at(i))!=5) maxisb=false;
        }
      }
      return output;
    });


   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Looking at gen level ISR

    /*
    * Pt ISR using truth info
    */
    const NamedFunc genISRPt("genISRPt",[](const Baby &b) -> NamedFunc::ScalarType{
      TLorentzVector *obj1 = new TLorentzVector();
      TLorentzVector *obj2 = new TLorentzVector();
      TLorentzVector ISR;
      int obj1_idx=1000;
      int obj2_idx=1000;
      float ISRpt=-1;
      for (unsigned i(0); i<b.gen_id()->size(); i++){
        if (abs(b.gen_id()->at(i))==1000024) {obj1_idx=i;}
        if (abs(b.gen_id()->at(i))==1000023) {obj2_idx=i;}
        if (b.gen_id()->at(i)==6) {obj1_idx=i;}
        if (b.gen_id()->at(i)==-6) {obj2_idx=i;}
      }
      if (obj1_idx!=1000 && obj2_idx!=1000) {
        obj1->SetPtEtaPhiM(b.gen_pt()->at(obj1_idx), b.gen_eta()->at(obj1_idx), b.gen_phi()->at(obj1_idx), b.gen_m()->at(obj1_idx));
        obj2->SetPtEtaPhiM(b.gen_pt()->at(obj2_idx), b.gen_eta()->at(obj2_idx), b.gen_phi()->at(obj2_idx), b.gen_m()->at(obj2_idx));
        ISR = -(*obj1+*obj2);
        ISRpt = abs(ISR.Pt());
      }
      delete obj1;
      delete obj2;
      return ISRpt;
    });

    const NamedFunc genISRgenMETdPhi("genISRMETdPhi",[](const Baby &b) -> NamedFunc::ScalarType{
      TLorentzVector *obj1 = new TLorentzVector();
      TLorentzVector *obj2 = new TLorentzVector();
      TLorentzVector ISR;
      TLorentzVector MET;
      int obj1_idx=1000;
      int obj2_idx=1000;
      int nmet_sources=0;
      float dR_ISR_MET=-1;
      for (unsigned i(0); i<b.gen_id()->size(); i++){
        if (abs(b.gen_id()->at(i))==1000024) {obj1_idx=i;}
        if (abs(b.gen_id()->at(i))==1000023) {obj2_idx=i;}
        if (b.gen_id()->at(i)==6) {obj1_idx=i;}
        if (b.gen_id()->at(i)==-6) {obj2_idx=i;}
        if (abs(b.gen_id()->at(i))==12||abs(b.gen_id()->at(i))==14||abs(b.gen_id()->at(i))==16
            ||abs(b.gen_id()->at(i))==1000022) {
          TLorentzVector *nu = new TLorentzVector();
          nu->SetPtEtaPhiM(b.gen_pt()->at(i), b.gen_eta()->at(i), b.gen_phi()->at(i), b.gen_m()->at(i));
          MET = MET + *nu;
          nmet_sources++;
          delete nu;
        }
      }
      if (obj1_idx!=1000 && obj2_idx!=1000) {
        obj1->SetPtEtaPhiM(b.gen_pt()->at(obj1_idx), b.gen_eta()->at(obj1_idx), b.gen_phi()->at(obj1_idx), b.gen_m()->at(obj1_idx));
        obj2->SetPtEtaPhiM(b.gen_pt()->at(obj2_idx), b.gen_eta()->at(obj2_idx), b.gen_phi()->at(obj2_idx), b.gen_m()->at(obj2_idx));
        ISR = -(*obj1+*obj2);
      }
      if (nmet_sources!=0) dR_ISR_MET = ISR.DeltaPhi(MET);
      delete obj1;
      delete obj2;
      return dR_ISR_MET;
    });

    const NamedFunc genISRrecoMETdPhi("genISRrecoMETdPhi",[](const Baby &b) -> NamedFunc::ScalarType{
      TLorentzVector *obj1 = new TLorentzVector();
      TLorentzVector *obj2 = new TLorentzVector();
      TLorentzVector ISR;
      TLorentzVector MET;
      int obj1_idx=1000;
      int obj2_idx=1000;
      int nmet_sources=0;
      float dR_ISR_MET=-1;
      for (unsigned i(0); i<b.gen_id()->size(); i++){
        if (abs(b.gen_id()->at(i))==1000024) {obj1_idx=i;}
        if (abs(b.gen_id()->at(i))==1000023) {obj2_idx=i;}
        if (b.gen_id()->at(i)==6) {obj1_idx=i;}
        if (b.gen_id()->at(i)==-6) {obj2_idx=i;}
        if (abs(b.gen_id()->at(i))==12||abs(b.gen_id()->at(i))==14||abs(b.gen_id()->at(i))==16
            ||abs(b.gen_id()->at(i))==1000022) {
          TLorentzVector *nu = new TLorentzVector();
          nu->SetPtEtaPhiM(b.gen_pt()->at(i), b.gen_eta()->at(i), b.gen_phi()->at(i), b.gen_m()->at(i));
          MET = MET + *nu;
          nmet_sources++;
          delete nu;
        }
      }
      if (obj1_idx!=1000 && obj2_idx!=1000) {
        obj1->SetPtEtaPhiM(b.gen_pt()->at(obj1_idx), b.gen_eta()->at(obj1_idx), b.gen_phi()->at(obj1_idx), b.gen_m()->at(obj1_idx));
        obj2->SetPtEtaPhiM(b.gen_pt()->at(obj2_idx), b.gen_eta()->at(obj2_idx), b.gen_phi()->at(obj2_idx), b.gen_m()->at(obj2_idx));
        ISR = -(*obj1+*obj2);
      }
      if (nmet_sources!=0) dR_ISR_MET = ISR.DeltaPhi(MET);
      delete obj1;
      delete obj2;
      return dR_ISR_MET;
    });


    const NamedFunc genISRrecoISRdPhi("genISRrecoISRdPhi",[](const Baby &b) -> NamedFunc::ScalarType{
      TLorentzVector *obj1 = new TLorentzVector();
      TLorentzVector *obj2 = new TLorentzVector();
      TLorentzVector gISR;
      TLorentzVector rISR;
      int obj1_idx=1000;
      int obj2_idx=1000;
      int rISR_idx=1000;
      float rISRpt=0;
      float gISR_rISR_dPhi=-1;
      for (unsigned i(0); i<b.gen_id()->size(); i++){
        if (abs(b.gen_id()->at(i))==1000024) {obj1_idx=i;}
        if (abs(b.gen_id()->at(i))==1000023) {obj2_idx=i;}
        if (b.gen_id()->at(i)==6) {obj1_idx=i;}
        if (b.gen_id()->at(i)==-6) {obj2_idx=i;}
      }
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_pt()->at(i) > rISRpt && b.ak4pfjets_deepCSV()->at(i) < 0.2217){
           rISRpt = b.ak4pfjets_pt()->at(i);
           rISR_idx = i;
        }
      }
      if (obj1_idx!=1000 && obj2_idx!=1000 && rISR_idx!=1000) {
        obj1->SetPtEtaPhiM(b.gen_pt()->at(obj1_idx), b.gen_eta()->at(obj1_idx), 
                            b.gen_phi()->at(obj1_idx), b.gen_m()->at(obj1_idx));
        obj2->SetPtEtaPhiM(b.gen_pt()->at(obj2_idx), b.gen_eta()->at(obj2_idx), 
                            b.gen_phi()->at(obj2_idx), b.gen_m()->at(obj2_idx));
        rISR.SetPtEtaPhiM(b.ak4pfjets_pt()->at(rISR_idx), b.ak4pfjets_eta()->at(rISR_idx), 
                            b.ak4pfjets_phi()->at(rISR_idx), b.ak4pfjets_m()->at(rISR_idx));
        gISR = -(*obj1+*obj2);
        gISR_rISR_dPhi = rISR.DeltaPhi(gISR);
      }
      delete obj1;
      delete obj2;
      return gISR_rISR_dPhi;
    });


    const NamedFunc genISRrecoISRDeltaPt("genISRrecoISRDeltaPt",[](const Baby &b) -> NamedFunc::ScalarType{
      TLorentzVector *obj1 = new TLorentzVector();
      TLorentzVector *obj2 = new TLorentzVector();
      TLorentzVector gISR;
      TLorentzVector rISR;
      int obj1_idx=1000;
      int obj2_idx=1000;
      int rISR_idx=1000;
      float rISRpt=0;
      float gISR_rISR_DeltaPt=1000;
      for (unsigned i(0); i<b.gen_id()->size(); i++){
        if (abs(b.gen_id()->at(i))==1000024) {obj1_idx=i;}
        if (abs(b.gen_id()->at(i))==1000023) {obj2_idx=i;}
        if (b.gen_id()->at(i)==6) {obj1_idx=i;}
        if (b.gen_id()->at(i)==-6) {obj2_idx=i;}
      }
      for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
        if (b.ak4pfjets_pt()->at(i) > rISRpt && b.ak4pfjets_deepCSV()->at(i) < 0.2217){
           rISRpt = b.ak4pfjets_pt()->at(i);
           rISR_idx = i;
        }
      }
      if (obj1_idx!=1000 && obj2_idx!=1000 && rISR_idx!=1000) {
        obj1->SetPtEtaPhiM(b.gen_pt()->at(obj1_idx), b.gen_eta()->at(obj1_idx), 
                            b.gen_phi()->at(obj1_idx), b.gen_m()->at(obj1_idx));
        obj2->SetPtEtaPhiM(b.gen_pt()->at(obj2_idx), b.gen_eta()->at(obj2_idx), 
                            b.gen_phi()->at(obj2_idx), b.gen_m()->at(obj2_idx));
        rISR.SetPtEtaPhiM(b.ak4pfjets_pt()->at(rISR_idx), b.ak4pfjets_eta()->at(rISR_idx), 
                            b.ak4pfjets_phi()->at(rISR_idx), b.ak4pfjets_m()->at(rISR_idx));
        gISR = -(*obj1+*obj2);
        gISR_rISR_DeltaPt = rISR.Pt()-gISR.Pt();
      }
      delete obj1;
      delete obj2;
      return gISR_rISR_DeltaPt;
    });

    const NamedFunc truthOrigin3rdJet("truthOrigin3rdJet",[](const Baby &b) -> NamedFunc::ScalarType{
        TLorentzVector *gOBJ = new TLorentzVector();
        TLorentzVector *rISR = new TLorentzVector();
        int jet_origin=0;
        float dR_rISR_gOBJ = 0.5;
        int gOBJ_idx=1000;
        int rISR_idx=1000;
        bool gOBJ_parent_of_leptons = false;
        float rISRpt=0;
        for (unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++) {
          if (b.ak4pfjets_pt()->at(i) > rISRpt && b.ak4pfjets_deepCSV()->at(i) < 0.2217){
            rISRpt = b.ak4pfjets_pt()->at(i);
            rISR_idx = i;
          }
        }
        if (rISR_idx!=1000) { 
          rISR->SetPtEtaPhiM(b.ak4pfjets_pt()->at(rISR_idx), b.ak4pfjets_eta()->at(rISR_idx), 
                            b.ak4pfjets_phi()->at(rISR_idx), b.ak4pfjets_m()->at(rISR_idx));
        }
        for (unsigned i(0); i<b.gen_id()->size(); i++) {
          gOBJ->SetPtEtaPhiM(b.gen_pt()->at(i), b.gen_eta()->at(i), 
                            b.gen_phi()->at(i), b.gen_m()->at(i));
          if (gOBJ->DeltaR(*rISR)<dR_rISR_gOBJ && abs(b.gen_id()->at(i))!=24 && abs(b.gen_id()->at(i))!=6 
              && abs(b.gen_id()->at(i))!=12 && abs(b.gen_id()->at(i))!=14 && abs(b.gen_id()->at(i))!=16
              && abs(b.gen_id()->at(i))!=1000024 && abs(b.gen_id()->at(i))!=1000023 && abs(b.gen_id()->at(i))!=1000022
              && abs(b.gen_id()->at(i))!=22) {
                gOBJ_idx = i;
                dR_rISR_gOBJ = gOBJ->DeltaR(*rISR);
          }
        }
        for (unsigned i(0); i<b.gen_id()->size(); i++) {
          if ((abs(b.gen_id()->at(i))==11||abs(b.gen_id()->at(i))==13||
              abs(b.gen_id()->at(i))==15)&& b.gen_motheridx()->at(i)==gOBJ_idx) gOBJ_parent_of_leptons = true;
        }
        if (gOBJ_idx!=1000) {
          if (abs(b.gen_id()->at(gOBJ_idx))==5 && abs(b.gen_motherid()->at(gOBJ_idx))==6) jet_origin=1;
          else if (abs(b.gen_id()->at(gOBJ_idx))<=6 && abs(b.gen_motherid()->at(gOBJ_idx))==24 && abs(b.gen_gmotherid()->at(gOBJ_idx))==6) jet_origin=2;
          else if (abs(b.gen_id()->at(gOBJ_idx))==15 && abs(b.gen_motherid()->at(gOBJ_idx))==24 && abs(b.gen_gmotherid()->at(gOBJ_idx))==6 && gOBJ_parent_of_leptons) jet_origin=3;
          else if (abs(b.gen_id()->at(gOBJ_idx))==15 && abs(b.gen_motherid()->at(gOBJ_idx))==24 && abs(b.gen_gmotherid()->at(gOBJ_idx))==6) jet_origin=4;
          else jet_origin=5;
        }
        delete gOBJ;
        delete rISR;

        return jet_origin;
    });


 
}
