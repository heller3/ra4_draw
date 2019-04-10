#include "core/wh_functions.hpp"

#include "TVector2.h"

#include "core/utilities.hpp"
#include "core/config_parser.hpp"

using namespace std;

namespace WH_Functions{

  const NamedFunc NHighPtNu("NHighPtNu",[](const Baby &b) -> NamedFunc::ScalarType{
      int nnu=0;
        for (unsigned i(0); i<b.gen_pt()->size(); i++){
        if (abs(b.gen_motherid()->at(i))==24 && ( abs(b.gen_id()->at(i)) == 12 || abs(b.gen_id()->at(i)) == 14 || abs(b.gen_id()->at(i)) == 16) && b.gen_pt()->at(i) > 200 ) nnu++;
      }
      return nnu;
    });

  const NamedFunc HasMedLooseCSV("HasMedLooseCSV",[](const Baby &b) -> NamedFunc::ScalarType{
      int nloose=0;
      int nmedium=0;
      for (unsigned i(0); i<b.ak4pfjets_CSV()->size(); i++){
	if (b.ak4pfjets_CSV()->at(i) > 0.5426) nloose++;
	if (b.ak4pfjets_CSV()->at(i) > 0.8484) nmedium++;
      }
      if(nloose>=2 && nmedium>=1) return 1;
      else return 0;
    });

  const NamedFunc HasMedMedDeepCSV("HasMedMedDeepCSV",[](const Baby &b) -> NamedFunc::ScalarType{
      int ndeepmedium=0;
      for(unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
	if (b.ak4pfjets_deepCSV()->at(i) > 0.6324) ndeepmedium++;
      }
      if(ndeepmedium>=2) return 1;
      else return 0;
    });

    const NamedFunc HasLooseLooseDeepCSV("HasLooseLooseDeepCSV",[](const Baby &b) -> NamedFunc::ScalarType{
      int ndeeploose=0;
      for(unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
	if (b.ak4pfjets_deepCSV()->at(i) > 0.2219) ndeeploose++;
      }
      if(ndeeploose>=2) return 1;
      else return 0;
    });

    const NamedFunc HasMedLooseDeepCSV("HasMedLooseDeepCSV",[](const Baby &b) -> NamedFunc::ScalarType{
      int ndeeploose=0;
      int ndeepmedium=0;
      for(unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
	if (b.ak4pfjets_deepCSV()->at(i) > 0.2219) ndeeploose++;
	if (b.ak4pfjets_deepCSV()->at(i) > 0.6324) ndeepmedium++;
      }
      if(ndeeploose>=2 && ndeepmedium>=1) return 1;
      else return 0;
    });

  const NamedFunc HasLooseNoMedDeepCSV("HasLooseNoMedDeepCSV",[](const Baby &b) -> NamedFunc::ScalarType{
      int ndeeploose=0;
      for(unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
	if ((b.ak4pfjets_deepCSV()->at(i) > 0.2219)&&(b.ak4pfjets_deepCSV()->at(i) < 0.6324)) ndeeploose++;
      }
      if(ndeeploose>=2) return 1;
      else return 0;
    });

  const NamedFunc nDeepMedBTagged("nDeepMedBTagged",[](const Baby &b) -> NamedFunc::ScalarType{
      int njets=0;
      for(unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
	if (b.ak4pfjets_deepCSV()->at(i) > 0.6324) njets++;
      }
      return njets;
    });

  const NamedFunc nDeepLooseBTagged("nDeepLooseBTagged",[](const Baby &b) -> NamedFunc::ScalarType{
      int njets=0;
      for(unsigned i(0); i<b.ak4pfjets_deepCSV()->size(); i++){
	if (b.ak4pfjets_deepCSV()->at(i) > 0.2219) njets++;
      }
      return njets;
    });

  const NamedFunc bJetPt("bJetPt",[](const Baby &b) -> NamedFunc::VectorType{
      vector<double> bjetpt;
      for(unsigned i(0); i<b.ak4pfjets_parton_flavor()->size(); i++){
	if(abs(b.ak4pfjets_parton_flavor()->at(i))==5) bjetpt.push_back(b.ak4pfjets_pt()->at(i));
      }
      return bjetpt;
    });
  
  const NamedFunc HasNoBs("HasNoBs",[](const Baby &b) -> NamedFunc::ScalarType{
      int nbjets=0;
      for (unsigned i(0); i<b.ak4pfjets_CSV()->size(); i++){
	if (b.ak4pfjets_CSV()->at(i) > 0.5426) nbjets++;
      }
      if(nbjets==0) return 1;
      else return 0;
    });

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

  const NamedFunc LeadingBJetPt("LeadingBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxpt=0;
      for (unsigned i(0); i<b.ak4pfjets_CSV()->size(); i++){
	if (b.ak4pfjets_CSV()->at(i) > 0.5426 && b.ak4pfjets_pt()->at(i) > maxpt) maxpt =  b.ak4pfjets_pt()->at(i);
      }
      return maxpt;
    });

  const NamedFunc SubLeadingBJetPt("SubLeadingBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
      float maxpt=0;
      float secondmax=0;
      for (unsigned i(0); i<b.ak4pfjets_CSV()->size(); i++){
	if (b.ak4pfjets_CSV()->at(i) > 0.5426 && b.ak4pfjets_pt()->at(i) > secondmax && b.ak4pfjets_pt()->at(i)<=maxpt){
	  secondmax = b.ak4pfjets_pt()->at(i);
	}
	else if (b.ak4pfjets_CSV()->at(i) > 0.5426 && b.ak4pfjets_pt()->at(i) > maxpt){
	  secondmax=maxpt;
	  maxpt =  b.ak4pfjets_pt()->at(i);
	}
      }
      return secondmax;
    });

}
