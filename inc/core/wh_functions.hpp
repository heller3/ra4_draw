#ifndef H_WH_FUNCTIONS
#define H_WH_FUNCTIONS

#include <cstddef>
#include <string>
#include "core/named_func.hpp"
 
namespace WH_Functions{

  // Miscilaneous
  extern const NamedFunc HasMedLooseCSV;
  extern const NamedFunc HasMedMedCSV;
  extern const NamedFunc WHLeptons;
  extern const NamedFunc NBJets;
  extern const NamedFunc NHighPtNu;

  // Basic Jet Pt
  extern const NamedFunc LeadingJetPt;
  extern const NamedFunc SubLeadingJetPt;
  extern const NamedFunc SubSubLeadingJetPt;

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

  // Looking at gen level ISR
  extern const NamedFunc genISRPt;
  extern const NamedFunc genISRgenMETdPhi;
  extern const NamedFunc genISRrecoMETdPhi;
  extern const NamedFunc genISRrecoISRdPhi;
  extern const NamedFunc genISRrecoISRDeltaPt;
  extern const NamedFunc truthOrigin3rdJet;
  
}

#endif
