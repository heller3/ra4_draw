#include "core/wh_functions.hpp"

#include "TVector2.h"

#include "core/utilities.hpp"
#include "core/config_parser.hpp"

using namespace std;

namespace WH_Functions{

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
