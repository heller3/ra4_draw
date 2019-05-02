#include "evtLooper.cc"
void RUNevtLooper(){

  TChain *data = new TChain("t","");
  TChain *tt1l = new TChain("t","");
  TChain *tt2l = new TChain("t","");
  TChain *wjets_low_nu = new TChain("t","");
  TChain *wjets_high_nu = new TChain("t", "");
  TChain *single_t = new TChain("t", "");
  TChain *ttv = new TChain("t", "");
  TChain *diboson = new TChain("t", "");

  TChain *tchiwh_signals = new TChain("t", "");
  // TChain *tchiwh_225_75 = new TChain("tchiwh_225_75", "");
  // TChain *tchiwh_250_1 = new TChain("tchiwh_250_1", "");
  // TChain *tchiwh_350_100 = new TChain("tchiwh_350_100", "");
  // TChain *tchiwh_500_125 = new TChain("tchiwh_500_125", "");
  // TChain *tchiwh_500_1 = new TChain("tchiwh_500_1", "");
  // TChain *tchiwh_700_1 = new TChain("tchiwh_700_1", "");


  string data_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_04_03/";
  string mc_dir = "/home/users/rheller/wh_babies/babies_v30_9_2019_04_03/";
  string signal_dir = "/home/users/rheller/wh_babies/babies_signal_2019_04_03/";

  data->Add((data_dir+"*data_2016*.root").c_str());
  tt1l->Add((mc_dir+"*TTJets_1lep*.root").c_str());
  tt2l->Add((mc_dir+"*TTJets_2lep*.root").c_str());
  wjets_low_nu->Add((mc_dir+"slim_W*JetsToLNu*.root").c_str());
  wjets_high_nu->Add((mc_dir+"*slim_W*Jets_NuPt200*.root").c_str());
  single_t->Add((mc_dir+"*_ST_*.root").c_str());
  ttv->Add((mc_dir+"*_TTWJets*.root").c_str());
  diboson->Add((mc_dir+"*WW*.root").c_str());
  tchiwh_signals->Add((signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root").c_str());
  


  // tchiwh_225_75->Add(signal_dir+"*data_2016*.root");
  // tchiwh_250_1->Add(signal_dir+"*data_2016*.root");
  // tchiwh_350_100->Add(signal_dir+"*data_2016*.root");
  // tchiwh_500_125->Add(signal_dir+"*data_2016*.root");
  // tchiwh_500_1->Add(signal_dir+"*data_2016*.root");
  // tchiwh_700_1->Add(signal_dir+"*data_2016*.root");



  // evtLooper data_scan(data);
  // evtLooper tt1l_scan(tt1l);
  // evtLooper tt2l_scan(tt2l);
  // evtLooper wjets_low_nu_scan(wjets_low_nu);
  // evtLooper wjets_high_nu_scan(wjets_high_nu);
  // evtLooper single_t_scan(single_t);
  // evtLooper ttv_scan(ttv);
  // evtLooper dibosn_scan(diboson);

  evtLooper tchiwh_signals_scan(tchiwh_signals);
  // evtLooper tchiwh_225_75_scan(tchiwh_225_75);
  // evtLooper tchiwh_250_1_scan(tchiwh_250_1);
  // evtLooper tchiwh_350_100_scan(tchiwh_350_100);
  // evtLooper tchiwh_500_125_scan(tchiwh_500_125);
  // evtLooper tchiwh_500_1_scan(tchiwh_500_1);
  // evtLooper tchiwh_700_1_scan(tchiwh_700_1);


  // data_scan.sigLoop();
  // tt1l_scan.sigLoop();
  // tt2l_scan.sigLoop();
  // wjets_low_nu_scan.sigLoop();
  // wjets_high_nu_scan.sigLoop();
  // single_t_scan.sigLoop();
  // ttv_scan.sigLoop();
  // dibosn_scan.sigLoop();

  tchiwh_signals_scan.sigLoop();
  // tchiwh_225_75_scan.sigLoop();
  // tchiwh_250_1_scan.sigLoop();
  // tchiwh_350_100_scan.sigLoop();
  // tchiwh_500_125_scan.sigLoop();
  // tchiwh_500_1_scan.sigLoop();
  // tchiwh_700_1_scan.sigLoop();


  // data_scan.saveHists("data_hists.root");
  // tt1l_scan.saveHists("tt1l_hists.root");
  // tt2l_scan.saveHists("tt2l_hists.root");
  // wjets_low_nu_scan.saveHists("wjets_low_nu_hists.root");
  // wjets_high_nu_scan.saveHists("wjets_high_nu_hists.root");
  // single_t_scan.saveHists("single_t_hists.root");
  // ttv_scan.saveHists("ttv_hists.root");
  // dibosn_scan.saveHists("diboson_hists.root");

  tchiwh_signals_scan.saveHists("tchiwh_signals_hists.root");
  // tchiwh_225_75_scan.saveHists("tchiwh_225_75_hists.root");
  // tchiwh_250_1_scan.saveHists("tchiwh_250_1_hists.root");
  // tchiwh_350_100_scan.saveHists("tchiwh_350_100_hists.root");
  // tchiwh_500_125_scan.saveHists("tchiwh_500_125_hists.root");
  // tchiwh_500_1_scan.saveHists("tchiwh_500_1_hists.root");
  // tchiwh_700_1_scan.saveHists("tchiwh_700_1_hists.root");

}
