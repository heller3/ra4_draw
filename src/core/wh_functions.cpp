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

  const NamedFunc nEventsGluonSplit("nEventsGluonSplit",[](const Baby &b) -> NamedFunc::ScalarType{
    int nevent = 0;

    for(unsigned i(0); i<b.gen_id()->size(); i++){
      if(abs(b.gen_id()->at(i))==5&&b.gen_motherid()->at(i)==21){
        for(unsigned j(i+1);j<b.gen_id()->size();j++){
          if(b.gen_id()->at(j)==-b.gen_id()->at(i)&&b.gen_motheridx()->at(j)==b.gen_motheridx()->at(i)&&(b.gen_pt()->at(j)>30||b.gen_pt()->at(i)>30)){
            nevent++;
          }//Close if statement for opposite b, same mother index, pt cut
        }//Close for loop over second half of particles in event
      }//Close if statement that find b with gluon mother
    }//Close for loop over all particles in event
    return nevent;

    });

  const NamedFunc nModEventsGluonSplit("nModEventsGluonSplit",[](const Baby &b) -> NamedFunc::ScalarType{
    int nevent = 0;
    int ngenb = 0;

    for(unsigned i(0); i<b.gen_id()->size(); i++){
      if(abs(b.gen_id()->at(i))==5){
        ngenb++;
      }
      if(abs(b.gen_id()->at(i))==5&&b.gen_motherid()->at(i)==21&&b.gen_pt()->at(i)>20){
        nevent++;
      }//Close if statement that find b with gluon mother
    }//Close for loop over all particles in event

    if(ngenb==0){
      nevent = -1;
    }

    return nevent;

    });

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

  const NamedFunc nDeepMedCTagged("nDeepMedCTagged",[](const Baby &b) -> NamedFunc::ScalarType{
      int njets=0;
      for(unsigned i(0); i<b.ak4pfjets_deepCSVc()->size(); i++){
        if (b.ak4pfjets_deepCSVc()->at(i)/(b.ak4pfjets_deepCSVc()->at(i)+b.ak4pfjets_deepCSVl()->at(i)) > 0.155) njets++;
      }
      return njets;
    });
  const NamedFunc nDeepTightCTagged("nDeepTightCTagged",[](const Baby &b) -> NamedFunc::ScalarType{
      int njets=0;
      for(unsigned i(0); i<b.ak4pfjets_deepCSVc()->size(); i++){
        if (b.ak4pfjets_deepCSVc()->at(i)/(b.ak4pfjets_deepCSVc()->at(i)+b.ak4pfjets_deepCSVl()->at(i)) > 0.59) njets++;
      }
      return njets;
    });

  const NamedFunc nDeepMedCvBTagged("nDeepMedCvBTagged",[](const Baby &b) -> NamedFunc::ScalarType{
      int njets=0;
      for(unsigned i(0); i<b.ak4pfjets_deepCSVc()->size(); i++){
        if (b.ak4pfjets_deepCSVc()->at(i)/(b.ak4pfjets_deepCSVc()->at(i)+b.ak4pfjets_deepCSV()->at(i)) > 0.14) njets++;
      }
      return njets;
    });
  const NamedFunc nDeepTightCvBTagged("nDeepTightCvBTagged",[](const Baby &b) -> NamedFunc::ScalarType{
      int njets=0;
      for(unsigned i(0); i<b.ak4pfjets_deepCSVc()->size(); i++){
        if (b.ak4pfjets_deepCSVc()->at(i)/(b.ak4pfjets_deepCSVc()->at(i)+b.ak4pfjets_deepCSV()->at(i)) > 0.05) njets++;
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

  const NamedFunc nRealBs("nRealBs",[](const Baby &b) -> NamedFunc::ScalarType{
      int nbquarks=0;
      for(unsigned i(0); i<b.ak4pfjets_parton_flavor()->size(); i++){
        if(abs(b.ak4pfjets_parton_flavor()->at(i))==5) nbquarks++;
        cout << b.ak4pfjets_parton_flavor()->at(i) << endl;
      }
      return nbquarks;
    });

  const NamedFunc sortedJetsPt_Leading("sortedJetsPt_Leading",[](const Baby &b) -> NamedFunc::ScalarType{
    vector<float>* v = b.ak4pfjets_pt();

    sort(v->begin(), v->end(), greater<int>());

    return v->at(0);
  });

  const NamedFunc sortedJetsPt_subLeading("sortedJetsPt_subLeading",[](const Baby &b) -> NamedFunc::ScalarType{
    vector<float>* v = b.ak4pfjets_pt();

    sort(v->begin(), v->end(), greater<int>());

    return v->at(1);
  });

  const NamedFunc sortedJetsCSV_Leading("sortedJetsCSV_Leading",[](const Baby &b) -> NamedFunc::ScalarType{
    vector< pair <float,float> > v;
    for(unsigned i(0);i<b.ak4pfjets_deepCSV()->size();i++){
      v.push_back(make_pair(b.ak4pfjets_deepCSV()->at(i),b.ak4pfjets_pt()->at(i)));
    }

    sort(v.begin(),v.end());

    int n = v.size();

    return v[n-1].second;
  });

  const NamedFunc sortedJetsCSV_subLeading("sortedJetsCSV_subLeading",[](const Baby &b) -> NamedFunc::ScalarType{
    vector< pair <float,float> > v;
    for(unsigned i(0);i<b.ak4pfjets_deepCSV()->size();i++){
      v.push_back(make_pair(b.ak4pfjets_deepCSV()->at(i),b.ak4pfjets_pt()->at(i)));
    }

    sort(v.begin(),v.end());

    int n = v.size();

    return v[n-2].second;
  });

  const NamedFunc sortedJetsCSV_deltaR("sortedJetsCSV_deltaR",[](const Baby &b) -> NamedFunc::ScalarType{
    vector<pair <float,float> > v_etaPhi;
    vector<pair <float,pair<float,float> > > v_ptEtaPhi;
    vector<pair <float, pair<float, pair <float,float> > > > v_csvPtEtaPhi;

    for(unsigned i(0);i<b.ak4pfjets_deepCSV()->size();i++){
      v_etaPhi.push_back(make_pair(b.ak4pfjets_eta()->at(i),b.ak4pfjets_phi()->at(i)));
      v_ptEtaPhi.push_back(make_pair(b.ak4pfjets_pt()->at(i),v_etaPhi[i]));
      v_csvPtEtaPhi.push_back(make_pair(b.ak4pfjets_deepCSV()->at(i),v_ptEtaPhi[i]));
    }
    sort(v_csvPtEtaPhi.begin(), v_csvPtEtaPhi.end());

    int n = v_csvPtEtaPhi.size();
    
    float deltaR_leading = deltaR(v_csvPtEtaPhi[n-1].second.second.first,v_csvPtEtaPhi[n-1].second.second.second,v_csvPtEtaPhi[n-2].second.second.first,v_csvPtEtaPhi[n-2].second.second.second);

    return deltaR_leading;
  });


  const NamedFunc deltaRLeadingJets("deltaRLeadingJets",[](const Baby &b) -> NamedFunc::ScalarType{
  
    float maxpt=0; int maxindex=-1;
    float secondmaxpt=0; int secondindex=-1;
    for (unsigned i(0); i<b.ak4pfjets_pt()->size(); i++){
      if (b.ak4pfjets_pt()->at(i) > secondmaxpt && b.ak4pfjets_pt()->at(i)<=maxpt){
        secondmaxpt = b.ak4pfjets_pt()->at(i);
        secondindex=i;
      }
      else if (b.ak4pfjets_pt()->at(i) > maxpt){
        secondmaxpt=maxpt;
        secondindex=maxindex;
        maxpt = b.ak4pfjets_pt()->at(i);
        maxindex=i;
      }
    }
    return deltaR(b.ak4pfjets_eta()->at(maxindex),b.ak4pfjets_phi()->at(maxindex),b.ak4pfjets_eta()->at(secondindex),b.ak4pfjets_phi()->at(secondindex));
  });

    const NamedFunc deltaPhiLeadingJets("deltaPhiLeadingJets",[](const Baby &b) -> NamedFunc::ScalarType{

    float maxpt=0; int maxindex=-1;
    float secondmaxpt=0; int secondindex=-1;
    for (unsigned i(0); i<b.ak4pfjets_pt()->size(); i++){
      if (b.ak4pfjets_pt()->at(i) > secondmaxpt && b.ak4pfjets_pt()->at(i)<=maxpt){
        secondmaxpt = b.ak4pfjets_pt()->at(i);
        secondindex=i;
      }
      else if (b.ak4pfjets_pt()->at(i) > maxpt){
        secondmaxpt=maxpt;
        secondindex=maxindex;
        maxpt = b.ak4pfjets_pt()->at(i);
        maxindex=i;
      }
    }
    return deltaPhi(b.ak4pfjets_phi()->at(maxindex),b.ak4pfjets_phi()->at(secondindex));
  });

  const NamedFunc bbmass("bbmass",[](const Baby &b) -> NamedFunc::ScalarType{
    float mass = -1;

    for(unsigned i(0); i<b.gen_id()->size(); i++){
      if(abs(b.gen_id()->at(i))==5){
        for(unsigned j(i+1);j<b.gen_id()->size();j++){
          if(b.gen_id()->at(j)==-b.gen_id()->at(i)){
            //delR = deltaR(b.gen_eta()->at(i),b.gen_phi()->at(i),b.gen_eta()->at(j),b.gen_phi()->at(j));
            TLorentzVector v1,v2,sum;
            v1.SetPtEtaPhiM(b.gen_pt()->at(i),b.gen_eta()->at(i),b.gen_phi()->at(i),b.gen_m()->at(i));
            v2.SetPtEtaPhiM(b.gen_pt()->at(j),b.gen_eta()->at(j),b.gen_phi()->at(j),b.gen_m()->at(j));
            sum=v1+v2;
            mass = sum.M();
          }//Close if statement for opposite b
        }//Close for loop over second half of particles in event
        if (mass>-1) break;
      }//Close if statement that find b
    }//Close for loop over all particles in event
    return mass;

    });

  const NamedFunc nGenBs("nGenBs",[](const Baby &b) -> NamedFunc::ScalarType{
      int nbquarks=0;
      for(unsigned i(0); i<b.gen_id()->size(); i++){
        if(abs(b.gen_id()->at(i))==5) nbquarks++;
      }
      return nbquarks;
    });

  const NamedFunc nGenBsFromGluons("nGenBsFromGluons",[](const Baby &b) -> NamedFunc::ScalarType{
      int nbquarks=0;
      for(unsigned i(0); i<b.gen_id()->size(); i++){
        if(abs(b.gen_id()->at(i))==5&&abs(b.gen_motherid()->at(i))==21) nbquarks++;
      }
      return nbquarks;
    });

  const NamedFunc nGenBs_ptG15("nGenBs_ptG15",[](const Baby &b) -> NamedFunc::ScalarType{
      int nbquarks=0;
      for(unsigned i(0); i<b.gen_id()->size(); i++){
        if(abs(b.gen_id()->at(i))==5&&b.gen_pt()->at(i)>15) nbquarks++;
      }
      return nbquarks;
    });

  const NamedFunc nGenBsFromGluons_ptG15("nGenBsFromGluons_ptG15",[](const Baby &b) -> NamedFunc::ScalarType{
      int nbquarks=0;
      for(unsigned i(0); i<b.gen_id()->size(); i++){
        if(abs(b.gen_id()->at(i))==5&&abs(b.gen_motherid()->at(i))==21&&b.gen_pt()->at(i)>15) nbquarks++;
      }
      return nbquarks;
    });

  const NamedFunc nGenBs_ptG30("nGenBs_ptG30",[](const Baby &b) -> NamedFunc::ScalarType{
      int nbquarks=0;
      for(unsigned i(0); i<b.gen_id()->size(); i++){
        if(abs(b.gen_id()->at(i))==5&&b.gen_pt()->at(i)>30) nbquarks++;
      }
      return nbquarks;
    });

  const NamedFunc nGenBsFromGluons_ptG30("nGenBsFromGluons_ptG30",[](const Baby &b) -> NamedFunc::ScalarType{
      int nbquarks=0;
      for(unsigned i(0); i<b.gen_id()->size(); i++){
        if(abs(b.gen_id()->at(i))==5&&abs(b.gen_motherid()->at(i))==21&&b.gen_pt()->at(i)>30) nbquarks++;
      }
      return nbquarks;
    });

  const NamedFunc genBpT("genBpT",[](const Baby &b) -> NamedFunc::VectorType{
      vector <double> bquarkpT;
      for(unsigned i(0); i<b.gen_id()->size(); i++){
        if(abs(b.gen_id()->at(i))==5){
          bquarkpT.push_back (b.gen_pt()->at(i));
        }
      }
      return bquarkpT;
    });
  
  const NamedFunc genB_leadingpT("genB_leadingpT",[](const Baby &b) -> NamedFunc::ScalarType{
    vector< pair <float,float> > v;
    for(unsigned i(0);i<b.gen_id()->size();i++){
      if(abs(b.gen_id()->at(i))==5){
        v.push_back(make_pair(b.gen_pt()->at(i),b.gen_id()->at(i)));
      }
    }

    sort(v.begin(),v.end());

    int n = v.size();

    float bquarkpT = v[n-1].first;

    return bquarkpT;
  });

  const NamedFunc genB_subleadingpT("genB_subleadingpT",[](const Baby &b) -> NamedFunc::ScalarType{
    vector< pair <float,float> > v;
    for(unsigned i(0);i<b.gen_id()->size();i++){
      if(abs(b.gen_id()->at(i))==5){
        v.push_back(make_pair(b.gen_pt()->at(i),b.gen_id()->at(i)));
      }
    }

    sort(v.begin(),v.end());

    int n = v.size();

    float bquarkpT = v[n-2].first;

    return bquarkpT;
  });

  const NamedFunc bDeltaRGluonSplit("bDeltaRGluonSplit",[](const Baby &b) -> NamedFunc::ScalarType{
    float delR = 0;

    for(unsigned i(0); i<b.gen_id()->size(); i++){
      if(abs(b.gen_id()->at(i))==5&&b.gen_motherid()->at(i)==21){
        for(unsigned j(i+1);j<b.gen_id()->size();j++){
          if(b.gen_id()->at(j)==-b.gen_id()->at(i)&&b.gen_motheridx()->at(j)==b.gen_motheridx()->at(i)&&(b.gen_pt()->at(j)>30||b.gen_pt()->at(i)>30)){
            delR = deltaR(b.gen_eta()->at(i),b.gen_phi()->at(i),b.gen_eta()->at(j),b.gen_phi()->at(j));
          }//Close if statement for opposite b, same mother index, pt cut
        }//Close for loop over second half of particles in event
      }//Close if statement that find b with gluon mother
    }//Close for loop over all particles in event
    return delR;

    });

  const NamedFunc bDeltaR("bDeltaR",[](const Baby &b) -> NamedFunc::ScalarType{
    float delR = 0;

    for(unsigned i(0); i<b.gen_id()->size(); i++){
      if(abs(b.gen_id()->at(i))==5){
        for(unsigned j(i+1);j<b.gen_id()->size();j++){
          if(b.gen_id()->at(j)==-b.gen_id()->at(i)){
            delR = deltaR(b.gen_eta()->at(i),b.gen_phi()->at(i),b.gen_eta()->at(j),b.gen_phi()->at(j));
          }//Close if statement for opposite b
        }//Close for loop over second half of particles in event
      }//Close if statement that find b
    }//Close for loop over all particles in event
    return delR;

    });

  const NamedFunc bMother("bMother",[](const Baby &b) -> NamedFunc::VectorType{
    vector <double> mother_id;

    for(unsigned i(0); i<b.gen_id()->size(); i++){
      if(abs(b.gen_id()->at(i))==5){
        if(abs(b.gen_motherid()->at(i))==2212){
          mother_id.push_back (30);
        }else{
          mother_id.push_back (b.gen_motherid()->at(i));
        }//Close if/else statement for redefining proton
      }//Close if statement that find b
    }//Close for loop over all particles in event
    return mother_id;

    });

  const NamedFunc bMother_pt15("bMother_pt15",[](const Baby &b) -> NamedFunc::VectorType{
    vector <double> mother_id;

    for(unsigned i(0); i<b.gen_id()->size(); i++){
      if(abs(b.gen_id()->at(i))==5&&b.gen_pt()->at(i)>15){
        if(abs(b.gen_motherid()->at(i))==2212){
          mother_id.push_back (30);
        }else{
          mother_id.push_back (b.gen_motherid()->at(i));
        }//Close if/else statement for redefining proton
      }//Close if statement that find b
    }//Close for loop over all particles in event
    return mother_id;

    });

  const NamedFunc bMother_pt30("bMother_pt30",[](const Baby &b) -> NamedFunc::VectorType{
    vector <double> mother_id;

    for(unsigned i(0); i<b.gen_id()->size(); i++){
      if(abs(b.gen_id()->at(i))==5&&b.gen_pt()->at(i)>30){
        if(abs(b.gen_motherid()->at(i))==2212){
          mother_id.push_back (30);
        }else{
          mother_id.push_back (b.gen_motherid()->at(i));
        }//Close if/else statement for redefining proton
      }//Close if statement that find b
    }//Close for loop over all particles in event
    return mother_id;

    });

  const NamedFunc leadingBMother_pt20("leadingBMother_pt20",[](const Baby &b) -> NamedFunc::ScalarType{
    vector< pair <float,float> > v;
    int ngenb = 0;

    for(unsigned i(0);i<b.gen_id()->size();i++){
      if(abs(b.gen_id()->at(i))==5){
        ngenb++;
      }
      if(abs(b.gen_id()->at(i))==5&&b.gen_pt()->at(i)>20){
        v.push_back(make_pair(b.gen_pt()->at(i),b.gen_motherid()->at(i)));
      }
    }

    sort(v.begin(),v.end());

    int n = v.size();

    float bmom = 0.;
    
    if(n!=0){
      bmom = v[n-1].second;
    }

    if(ngenb==0.){
      bmom = 19;
    }

    return bmom;
  });

  const NamedFunc subleadingBMother_pt20("subleadingBMother_pt20",[](const Baby &b) -> NamedFunc::ScalarType{
    vector< pair <float,float> > v;
    int ngenb = 0;

    for(unsigned i(0);i<b.gen_id()->size();i++){
      if(abs(b.gen_id()->at(i))==5){
        ngenb++;
      }
      if(abs(b.gen_id()->at(i))==5&&b.gen_pt()->at(i)>20){
        v.push_back(make_pair(b.gen_pt()->at(i),b.gen_motherid()->at(i)));
      }
    }

    sort(v.begin(),v.end());

    int n = v.size();

    float bmom = 0.;
    
    if(n>=2){
      bmom = v[n-2].second;
    }

    if(ngenb==0.){
      bmom = 19;
    }

    return bmom;
  });

  const NamedFunc bDeltaPhi("bDeltaPhi",[](const Baby &b) -> NamedFunc::ScalarType{
      float delphi=0;
      vector < pair < float,float > > v_csvPhi;
      
      for(unsigned i(0); i<b.ak4pfjets_parton_flavor()->size(); i++){
        if(abs(b.ak4pfjets_parton_flavor()->at(i))==5){
          v_csvPhi.push_back(make_pair(b.ak4pfjets_deepCSV()->at(i),b.ak4pfjets_phi()->at(i)));
        }
      }

      sort(v_csvPhi.begin(),v_csvPhi.end());

      int n = v_csvPhi.size();

      if(n>=2){
        delphi = deltaPhi(v_csvPhi[n-1].second,v_csvPhi[n-2].second);
        return delphi;
      }else return 0;

    });

  const NamedFunc bmetMinDeltaPhi("bmetMinDeltaPhi",[](const Baby &b) -> NamedFunc::ScalarType{
      float delphi=10;
      
      for(unsigned i(0); i<b.ak4pfjets_parton_flavor()->size(); i++){
        if(abs(b.ak4pfjets_parton_flavor()->at(i))==5&&delphi>b.dphi_ak4pfjet_met()->at(i)){
          delphi = b.dphi_ak4pfjet_met()->at(i);
        }
      }

      return delphi;

    });

  const NamedFunc nHeavy("nHeavy",[](const Baby &b) -> NamedFunc::ScalarType{
      float njets=0;
      
      for(unsigned i(0); i<b.ak4pfjets_parton_flavor()->size(); i++){
        if(abs(b.ak4pfjets_parton_flavor()->at(i))==5||abs(b.ak4pfjets_parton_flavor()->at(i))==4){
          njets++;
        }
      }

      return njets;

    });

  const NamedFunc nLight("nLight",[](const Baby &b) -> NamedFunc::ScalarType{
      float njets=0;
      
      for(unsigned i(0); i<b.ak4pfjets_parton_flavor()->size(); i++){
        if(abs(b.ak4pfjets_parton_flavor()->at(i))==1||abs(b.ak4pfjets_parton_flavor()->at(i))==2||abs(b.ak4pfjets_parton_flavor()->at(i))==3){
          njets++;
        }
      }

      return njets;

    });

  const NamedFunc gluBTagged("gluBTagged",[](const Baby &b) -> NamedFunc::ScalarType{
    vector <pair <float, float> > gen_bs;

    for(unsigned i(0); i<b.gen_id()->size(); i++){
      if(abs(b.gen_id()->at(i))==5&&b.gen_motherid()->at(i)==21){
        for(unsigned j(i+1);j<b.gen_id()->size();j++){
          if(b.gen_id()->at(j)==-b.gen_id()->at(i)&&b.gen_motheridx()->at(j)==b.gen_motheridx()->at(i)&&(b.gen_pt()->at(j)>30||b.gen_pt()->at(i)>30)){
            gen_bs.push_back(make_pair(b.gen_eta()->at(i),b.gen_phi()->at(i)));
            gen_bs.push_back(make_pair(b.gen_eta()->at(j),b.gen_phi()->at(j)));
          }//Close if statement for opposite b
        }//Close for loop over second half of particles in event
      }//Close if statement that find b
    }//Close for loop over all particles in event

    float ntagged = 0;
    float delR = -1;
    float mindelR = 10;
    float dcsv = -1;

    int idx1 = 0;
    int idx2 = 0;

    if (gen_bs.size()==0){
      ntagged = -1;
    }else{
      for(unsigned i(0);i<2;i++){
        for(unsigned j(0);j<b.ak4pfjets_eta()->size();j++){
          delR = deltaR(gen_bs[i].first,gen_bs[i].second,b.ak4pfjets_eta()->at(j),b.ak4pfjets_phi()->at(j));
          if(delR < mindelR){
            mindelR = delR;
            dcsv = b.ak4pfjets_deepCSV()->at(j);
            if(i==0){
              idx1=j;
            }else{
              idx2=j;
            }
          }//Close if statement for smallest dR
        }//Close for loop over reco objects
        if(dcsv > 0.6324) ntagged++;
      }//Close for loop over gen objects

      if(idx1==idx2){
        ntagged = ntagged-1;
      }
    }

    return ntagged;

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
