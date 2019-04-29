#include "evtLooper.cc"
void RUNevtLooper(){

  TChain *data = new TChain("mt2","");
  TChain *tt1l = new TChain("mt2","");
  TChain *tt2l = new TChain("mt2","");
  TChain *wjets_low_nu = new TChain("mt2","");
  TChain *wjets_high_nu = new TChain("mt2", "");
  TChain *single_t = new TChain("mt2", "");
  TChain *ttv = new TChain("mt2", "");
  TChain *diboson = new TChain("mt2", "");

  TChain *tchiwh_225_75 = new TChain("mt2", "");
  TChain *tchiwh_250_1 = new TChain("mt2", "");
  TChain *tchiwh_350_100 = new TChain("mt2", "");
  TChain *tchiwh_500_125 = new TChain("mt2", "");
  TChain *tchiwh_500_1 = new TChain("mt2", "");
  TChain *tchiwh_700_1 = new TChain("mt2", "");


  string data_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_04_03/";
  string mc_dir = "/home/users/rheller/wh_babies/babies_v30_9_2019_04_03/";
  string signal_dir = "/home/users/rheller/wh_babies/babies_signal_2019_04_03/";

  data->Add(data_dir+"*data_2016*.root");
  tt1l->Add(mc_dir+"*TTJets_1lep*.root");
  tt2l->Add(mc_dir+"*TTJets_2lep*.root");
  wjets_low_nu->Add(mc_dir+"slim_W*JetsToLNu*.root");
  wjets_high_nu->Add(mc_dir+"*slim_W*Jets_NuPt200*.root");
  single_t->Add(mc_dir+"*_ST_*.root");
  ttv->Add(mc_dir+"*_TTWJets*.root");
  diboson->Add(mc_dir+"*WW*.root");

  tchiwh_225_75->Add(signal_dir+"*data_2016*.root");
  tchiwh_250_1->Add(signal_dir+"*data_2016*.root");
  tchiwh_350_100->Add(signal_dir+"*data_2016*.root");
  tchiwh_500_125->Add(signal_dir+"*data_2016*.root");
  tchiwh_500_1->Add(signal_dir+"*data_2016*.root");
  tchiwh_700_1->Add(signal_dir+"*data_2016*.root");



  evtLooper WH_scan(WH_chain);
  evtLooper tt_scan(tt_chain);
  evtLooper wjets_scan(wjets_chain);
  evtLooper zinv_scan(zinv_chain);
  //evtLooper T2bb_scan(T2bb_chain);
  //evtLooper T1bbbb_scan(T1bbbb_chain);
  //evtLooper T2qq_scan(T2qq_chain);

  WH_scan.sigLoop();
  tt_scan.bgLoop();
  wjets_scan.bgLoop();
  zinv_scan.bgLoop();
  //T2bb_scan.sigLoop();
  //T1bbbb_scan.sigLoop();
  //T2qq_scan.sigLoop();
  
  //WH_scan.findMassWindow();

  WH_scan.saveHists("signal.root");
  tt_scan.saveHists("tt_background.root");
  wjets_scan.saveHists("wjets_background.root");
  zinv_scan.saveHists("zinv_background.root");
  //T2bb_scan.saveHists("T2bb_background.root");
  //T1bbbb_scan.saveHists("T1bbbb_background.root");
  //T2qq_scan.saveHists("T2qq_background.root");
}
