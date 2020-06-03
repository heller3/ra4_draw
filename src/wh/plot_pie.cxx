#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "TError.h"
#include "TColor.h"

#include "core/baby.hpp"
#include "core/process.hpp"
#include "core/named_func.hpp"
#include "core/plot_maker.hpp"
#include "core/table.hpp"
#include "core/plot_opt.hpp"
#include "core/palette.hpp"
#include "core/hist1d.hpp"
#include "core/wh_functions.hpp"

using namespace std;
using namespace PlotOptTypes;
using namespace WH_Functions;

int main(){
  gErrorIgnoreLevel = 6000;

  double lumi = 137.;

  Palette colors("txt/colors.txt", "default");

  // Cuts in baseline speed up the yield finding
  string baseline         = "";
  NamedFunc baselinef     = "pass&&nvetoleps>=1&&ngoodjets>=2 && ngoodjets<=3 &&pfmet>125&&mt_met_lep>150 && ngoodbtags==2 && nWHLeptons>=1";


  string mc2016_dir       = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/s16v3/";
  string mc2016_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/s16v3/";

  string mc2017_dir       = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/f17v2/";
  string mc2017_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/f17v2/";

  string mc2018_dir       = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/a18v1/";
  string mc2018_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_07/a18v1/";

  string signal_dir2016   = "/home/users/rheller/wh_babies/babies_signal_s16v3_v32_2019_10_07/";
  string signal_dir2017   = "/home/users/rheller/wh_babies/babies_signal_f17v2_v32_0_2019_10_07/";
  string signal_dir2018   = "/home/users/rheller/wh_babies/babies_signal_a18v1_v32_0_2019_10_07/";

  auto all_tt2l = {mc2016_dir+"*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2017_dir+"*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2018_dir+"*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"*TTJets_2lep_*met80*.root"};

  auto all_tt1l = {mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2017_dir+"*TTJets_1lep_top_f17v2*.root",mc2017_dir+"*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2018_dir+"*TTJets_1lep_top_a18v1*.root",mc2018_dir+"*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"*TTJets_1lep_*met80*.root"};

  auto all_tt = {mc2016_dir+"*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2017_dir+"*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2018_dir+"*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"*TTJets_2lep_*met80*.root",mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2017_dir+"*TTJets_1lep_top_f17v2*.root",mc2017_dir+"*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2018_dir+"*TTJets_1lep_top_a18v1*.root",mc2018_dir+"*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"*TTJets_1lep_*met80*.root"};

  auto all_ST = {mc2016_dir+"*_ST_*.root",mc2017_dir+"*_ST_*.root",mc2018_dir+"*_ST_*.root"};

  auto all_top = {mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2017_dir+"*TTJets_1lep_top_f17v2*.root",mc2017_dir+"*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2018_dir+"*TTJets_1lep_top_a18v1*.root",mc2018_dir+"*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"*TTJets_1lep_*met80*.root",mc2016_dir+"*_ST_*.root",mc2017_dir+"*_ST_*.root",mc2018_dir+"*_ST_*.root",mc2016_dir+"*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2017_dir+"*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2018_dir+"*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"*TTJets_2lep_*met80*.root"};


  auto all_other = {mc2016_dir+"*WW*.root", mc2016_dir+"*WZ*.root",mc2016_dir+"*ZZ*.root",mc2017_dir+"*WW*.root", mc2017_dir+"*WZ*.root",mc2017_dir+"*ZZ*.root",mc2018_dir+"*WW*.root", mc2018_dir+"*WZ*.root",mc2018_dir+"*ZZ*.root",mc2016_dir+"*_TTWJets*.root", mc2016_dir+"*_TTZ*.root",mc2017_dir+"*_TTWJets*.root", mc2017_dir+"*_TTZ*.root",mc2018_dir+"*_TTWJets*.root", mc2018_dir+"*_TTZ*.root"};

  //// Contributions
  auto proc_wjets = Process::MakeShared<Baby_full>("W+jets 2016-2018", Process::Type::background, kCyan-3, {mc2016_dir+"*slim_W*JetsToLNu_s16v3*",mc2016_dir+"*W*Jets_NuPt200_s16v*.root",mc2017_dir+"*slim_W*JetsToLNu_f17v2*",mc2017_dir+"*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"*slim_W*JetsToLNu_a18v1*",mc2018_dir+"*W*Jets_NuPt200_a18v1*.root"},"stitch&&evt!=74125994"&&baselinef);
  auto proc_top = Process::MakeShared<Baby_full>("top 2016-2018", Process::Type::background, kRed+1,all_top,"stitch"&&baselinef);
  auto proc_tt2l = Process::MakeShared<Baby_full>("tt2l 2016-2018", Process::Type::background, kRed+1,all_tt2l,"stitch"&&baselinef);
  auto proc_tt1l = Process::MakeShared<Baby_full>("tt1l 2016-2018", Process::Type::background, kRed+1,all_tt1l,"stitch"&&baselinef);
  auto proc_ST = Process::MakeShared<Baby_full>("ST 2016-2018", Process::Type::background, kRed+1,all_ST,"stitch"&&baselinef);
  auto proc_other = Process::MakeShared<Baby_full>("TTV and VV 2016-2018", Process::Type::background, kRed+1,all_other,baselinef);
 
  auto proc_top_2realb = Process::MakeShared<Baby_full>("top, 2 real b-tags 2016-2018", Process::Type::background, kGreen-3,all_top,"stitch"&&baselinef&&nRealBtags==2.);
  auto proc_top_2realb_tt = Process::MakeShared<Baby_full>("ttbar, 2 real b-tags 2016-2018", Process::Type::background, kGreen-3,all_tt,"stitch"&&baselinef&&nRealBtags==2.);
  auto proc_top_2realb_st = Process::MakeShared<Baby_full>("ST, 2 real b-tags 2016-2018", Process::Type::background,  kRed+1,all_ST,"stitch"&&baselinef&&nRealBtags==2.);
  auto proc_top_1realb = Process::MakeShared<Baby_full>("top, 1 real b-tag 2016-2018", Process::Type::background, kBlue+1,all_top,"stitch"&&baselinef&&nRealBtags==1.);
  auto proc_top_0realb = Process::MakeShared<Baby_full>("top, 0 real b-tags 2016-2018", Process::Type::background, kOrange+7,all_top,"stitch"&&baselinef&&nRealBtags==0.);
  // auto proc_top_2realb = Process::MakeShared<Baby_full>("top, 2 real b-tags 2016-2018", Process::Type::background, kGreen-3,all_ST,"stitch"&&baselinef&&nRealBtags==2.);
  // auto proc_top_1realb = Process::MakeShared<Baby_full>("top, 1 real b-tag 2016-2018", Process::Type::background, kCyan-3,all_ST,"stitch"&&baselinef&&nRealBtags==1.);
  // auto proc_top_0realb = Process::MakeShared<Baby_full>("top, 0 real b-tags 2016-2018", Process::Type::background, kRed+1,all_ST,"stitch"&&baselinef&&nRealBtags==0.);
  auto proc_top_1realb_not_tagged = Process::MakeShared<Baby_full>("top, 1 real b-tag, 1b failed tag", Process::Type::background, kMagenta+3,all_top,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag>0.);
  //jet with hflavor 5, but not tagged.
  // auto proc_top_1realb_missing_genb = Process::MakeShared<Baby_full>("top, 1 real b-tag, only 1 b in gen record", Process::Type::background,kPink-2,all_top,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs<2.); //never happens

  auto proc_top_1realb_merged = Process::MakeShared<Baby_full>("top, 1 real b-tag, merged bb", Process::Type::background, kCyan-3,all_top,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>0.&&bDeltaRfromtop<0.5);
    //drbb < 0.5
  auto proc_top_1realb_forward = Process::MakeShared<Baby_full>("top, 1 real b-tag, forward b", Process::Type::background, kRed+1,all_top,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central<2.);
  //forward: gen b with eta  > 2.4
  auto proc_top_1realb_soft = Process::MakeShared<Baby_full>("top, 1 real b-tag, soft b", Process::Type::background,kGreen-3,all_top,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2.&&nGenBs_ptG30_central<2.);
  auto proc_top_1realb_lepcleaning = Process::MakeShared<Baby_full>("top, 1 real b-tag, lepton cleaning", Process::Type::background, kBlue+1,all_top,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2.&&nGenBs_ptG30_central>=2. && minDR_lep_bquark < 0.5);
  //Soft: gen b with pt < 30
  auto proc_top_1realb_extrab = Process::MakeShared<Baby_full>("top, 1 real b-tag, more than 2b", Process::Type::background, kPink-2, all_top,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2.&&nGenBs_ptG30_central>=2. && minDR_lep_bquark >= 0.5);
    auto proc_top_1realb_other = Process::MakeShared<Baby_full>("top, 1 real b-tag, other reason", Process::Type::background, kOrange+7,all_top,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs==2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2.&&nGenBs_ptG30_central>=2. && minDR_lep_bquark >= 0.5);

  auto proc_ttbar_1realb_not_tagged = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, 1b failed tag", Process::Type::background, kMagenta+3,all_ST,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag>0.);
  //jet with hflavor 5, but not tagged.
  auto proc_ttbar_1realb_missing_genb = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, only 1 b in gen record", Process::Type::background, kPink-2,all_ST,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs<2.);

  auto proc_ttbar_1realb_merged = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, merged bb", Process::Type::background, kCyan-3,all_ST,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>0.&&bDeltaRfromtop<0.5);
    //drbb < 0.5
  auto proc_ttbar_1realb_forward = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, forward b", Process::Type::background, kRed+1,all_ST,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central<2.);
  //forward: gen b with eta  > 2.4
  auto proc_ttbar_1realb_soft = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, soft b", Process::Type::background,kGreen-3,all_ST,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2&&nGenBs_ptG30_central<2.);
  //Soft: gen b with pt < 30
  auto proc_ttbar_1realb_other = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, other reason", Process::Type::background, kOrange+7,all_ST,"stitch"&&baselinef&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2&&nGenBs_ptG30_central>=2.);


  auto proc_top_lost_lep = Process::MakeShared<Baby_full>("top, lost e/mu 2016-2018", Process::Type::background, kGreen-3,all_top,"stitch"&&baselinef&&n_true_emu>=2.&& HasHadronicTau==0.);
  auto proc_top_lost_htau = Process::MakeShared<Baby_full>("top, lost htau 2016-2018", Process::Type::background, kCyan-3,all_top,"stitch"&&baselinef&&HasHadronicTau>0.);
  auto proc_top_1l = Process::MakeShared<Baby_full>("top, 1l 2016-2018", Process::Type::background, kRed+1,all_top,"stitch"&&baselinef&&n_true_emu<2.&&HasHadronicTau==0.);


  // vector<shared_ptr<Process> > sample_list ={tt2l,tt1l,wjets_low_nu,wjets_high_nu,single_t,other};
  vector<shared_ptr<Process> > real_bs_sample_list ={proc_top_2realb_tt,proc_top_2realb_st,proc_top_1realb,proc_top_0realb};
  vector<shared_ptr<Process> > real_bs_sample_list_short ={proc_top_2realb_tt,proc_top_2realb_st, proc_top_1realb};
  vector<shared_ptr<Process> > lost_b_sample_list ={proc_top_1realb_not_tagged,proc_top_1realb_merged,proc_top_1realb_forward,proc_top_1realb_soft,proc_top_1realb_lepcleaning,proc_top_1realb_extrab,proc_top_1realb_other};
  vector<shared_ptr<Process> > lost_b_sample_list_ttbar ={proc_ttbar_1realb_not_tagged,proc_ttbar_1realb_missing_genb,proc_ttbar_1realb_merged,proc_ttbar_1realb_forward,proc_ttbar_1realb_soft,proc_ttbar_1realb_other};
  vector<shared_ptr<Process> > lost_b_sample_list_simplified ={proc_top_1realb_forward,proc_top_1realb_soft,proc_top_1realb_other};
  vector<shared_ptr<Process> > lepton_sample_list ={proc_top_lost_lep,proc_top_lost_htau,proc_top_1l};

//  vector<string> tag_list = {"tt1l","tt2l"};
//  vector<shared_ptr<Process> > sample_list = {data, /*tchiwh_250_1, tchiwh_225_75,*/ tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::off)
    .YAxis(YAxisType::log)
    .Stack(StackType::lumi_shapes);
    // .RatioMinimum(0.5).RatioMaximum(1.5);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .Bottom(BottomType::off)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {lin_shapes_info,log_lumi_info};


  PlotMaker pm;

  //Selection Types
  // string basic_cuts =            "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2";
  // string basic_cuts_mct =        "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200";
  // string basic_cuts_mct_mt =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150";
  // string basic_cuts_mct_mbb =    "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150";
  // string basic_cuts_mt_mbb =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>150&&mbb>90&&mbb<150";
  // string basic_cuts_mct_mt_mbb = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";
  // string dilep_basic_cuts =      "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2";
  // string dilep_mct_mt_mbb =      "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";


  // NamedFunc signalRegion =        "pfmet>200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc mbbside =             "pfmet>200&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc dilep =               "pfmet>125" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc lowMCT =              "pfmet>200&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;

  // NamedFunc signalRegion_lowMET =        "pfmet>125&&pfmet<200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc mbbside_lowMET =             "pfmet>125&&pfmet<200&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc dilep_lowMET =               "pfmet>125&&pfmet<200" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc lowMCT_lowMET =              "pfmet>125&&pfmet<200&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;

  // NamedFunc signalRegion_medMET =        "pfmet>200&&pfmet<300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc mbbside_medMET =             "pfmet>200&&pfmet<300&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc dilep_medMET =               "pfmet>200&&pfmet<300" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc lowMCT_medMET =              "pfmet>200&&pfmet<300&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;

  // NamedFunc signalRegion_highMET =        "pfmet>300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc mbbside_highMET =             "pfmet>300&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc dilep_highMET =               "pfmet>300" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc lowMCT_highMET =              "pfmet>300&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;

  TString single_lep      = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&nWHLeptons==1";
  TString dilep           = "nvetoleps==2";
  vector<NamedFunc> metbins = {"pfmet>125&&pfmet<=200","pfmet>200&&pfmet<=300","pfmet>300&&pfmet<400","pfmet>400","pfmet>200"};
  vector<TString> metNames = {"low MET","medium MET","high MET","vhigh MET"};
  // vector<NamedFunc> boosted_metbins = {"pfmet>125&&pfmet<=300","pfmet>300"};
  vector<NamedFunc> boosted_metbins = {"pfmet>125"};
  vector<TString> boosted_metNames = {"low MET","high MET"};

  vector<NamedFunc> njetbins = {"ngoodjets==2","ngoodjets==3"&&LeadingNonBJetPt_med<100.}; 
  vector<TString> njetNames = {"2 jets","3 jets"};

  vector<NamedFunc> deepAK8bins = {max_ak8pfjets_deepdisc_hbb<=0.8,max_ak8pfjets_deepdisc_hbb>0.8};
  TString signal_region = single_lep+"&&mct>200&&mbb>90&&mbb<150";
  TString mct_control_region = single_lep+"&&mct>150&&mct<=200&&mbb>90&&mbb<150";    
  // if(dilep_mode) mct_control_region = dilep+"&&mct>200&&mbb>90&&mbb<150";    

  TString signal_region_boost = single_lep+"&&mct>200&&mbb>90&&mbb<150";
  TString mct_control_region_boost = single_lep+"&&mct<=200&&mbb>90&&mbb<150";
  // if(dilep_mode) mct_control_region_boost = dilep+"&&mct>200&&mbb>90&&mbb<150";


  // map<string, vector<shared_ptr<Process> > > categories;
  map<string, vector<shared_ptr<Process> > > categories_Bs;
  map<string, vector<shared_ptr<Process> > > categories_lost_Bs;
  map<string, vector<shared_ptr<Process> > > categories_lost_Bs_tt;
  // map<string, vector<shared_ptr<Process> > > categories_split_Bs;

  // categories["process"] = vector<shared_ptr<Process> >();
  // categories["process"].push_back(tt2l);
  // categories["process"].push_back(tt1l);
  // categories["process"].push_back(wjets_low_nu);
  // categories["process"].push_back(wjets_high_nu);
  // categories["process"].push_back(single_t);
  // categories["process"].push_back(other);

  categories_Bs["RealBs"] = vector<shared_ptr<Process> >();
  categories_Bs["RealBs"].push_back(proc_top_2realb_tt);
  categories_Bs["RealBs"].push_back(proc_top_2realb_st);
  categories_Bs["RealBs"].push_back(proc_top_1realb);
  categories_Bs["RealBs"].push_back(proc_top_0realb);

  categories_lost_Bs["LostBs"] = vector<shared_ptr<Process> >();
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_not_tagged);
  // categories_lost_Bs["LostBs"].push_back(proc_top_1realb_missing_genb);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_merged);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_forward);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_soft);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_lepcleaning);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_extrab);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_other);

  categories_lost_Bs_tt["LostBs_tt"] = vector<shared_ptr<Process> >();
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_not_tagged);
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_missing_genb);
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_merged);
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_forward);
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_soft);
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_other);

  //What happened to missing real b's in events with 1 fake?


  // categories_split_Bs["fullRealBs"] = vector<shared_ptr<Process> >();
  // categories_split_Bs["fullRealBs"].push_back(tt2l_2b);
  // categories_split_Bs["fullRealBs"].push_back(tt2l_1b);
  // categories_split_Bs["fullRealBs"].push_back(wjets_low_nu);
  // categories_split_Bs["fullRealBs"].push_back(wjets_high_nu_1b);
  // categories_split_Bs["fullRealBs"].push_back(wjets_high_nu_0b);
  // categories_split_Bs["fullRealBs"].push_back(single_t);
  // categories_split_Bs["fullRealBs"].push_back(other);


  vector<TableRow> signal_table_cuts;
  vector<TableRow> mbbside_table_cuts;
  vector<TableRow> dilep_table_cuts;
  vector<TableRow> lowMCT_table_cuts;
  for(uint ijet=0;ijet<njetbins.size();ijet++){
    for(uint imet=0;imet<metbins.size();imet++){
      signal_table_cuts.push_back(TableRow(Form("SR, %s %s",njetNames[ijet].Data(),metNames[imet].Data()), signal_region && njetbins[ijet] && metbins[imet] && deepAK8bins[0],0,0, "weight * w_pu" * yearWeight)); 
      lowMCT_table_cuts.push_back(TableRow(Form("low mCT CR %s %s",njetNames[ijet].Data(),metNames[imet].Data()), mct_control_region && njetbins[ijet] && metbins[imet] && deepAK8bins[0] ,0,0, "weight * w_pu" * yearWeight)); 
   
    }
    for(uint imet_boos=0;imet_boos<boosted_metbins.size();imet_boos++){
     signal_table_cuts.push_back(TableRow(Form("SR, %s %s",njetNames[ijet].Data(),boosted_metNames[imet_boos].Data()), signal_region_boost && njetbins[ijet] && boosted_metbins[imet_boos] && deepAK8bins[1],0,0, "weight * w_pu" * yearWeight)); 
     lowMCT_table_cuts.push_back(TableRow(Form("low mCT CR %s %s",njetNames[ijet].Data(),boosted_metNames[imet_boos].Data()), mct_control_region_boost && njetbins[ijet] && boosted_metbins[imet_boos] && deepAK8bins[1],0,0, "weight * w_pu" * yearWeight)); 
    }  
  

     // pm.Push<Hist1D>(Axis(10, 0, 100., genBpT, "b-quark gen pT [GeV]"), signal_region && njetbins[ijet] && "pfmet>125 && pfmet<200" , lost_b_sample_list_simplified, all_plot_types).Weight("weight * w_pu" * yearWeight);
      pm.Push<Hist1D>(Axis(10, 0, 100., genB_subleadingpT, "b-quark subleading gen pT [GeV]"), signal_region && njetbins[ijet] && "pfmet>125 && pfmet<200" , lost_b_sample_list_simplified, all_plot_types).Weight("weight * w_pu" * yearWeight);
      pm.Push<Hist1D>(Axis(10, -5, 5., genBeta_mostForward, "b-quark most forward gen eta"), signal_region && njetbins[ijet] && "pfmet>125 && pfmet<200" , lost_b_sample_list_simplified, all_plot_types).Weight("weight * w_pu" * yearWeight);
      pm.Push<Hist1D>(Axis(10, 0, 4., bDeltaR, "b-quark deltaR"), signal_region && njetbins[ijet] && "pfmet>125 && pfmet<200" , lost_b_sample_list_simplified, all_plot_types).Weight("weight * w_pu" * yearWeight);
      pm.Push<Hist1D>(Axis(10, 0, 4., minDR_lep_bquark, "min dR(bquark, reco lep)"), signal_region && njetbins[ijet] && "pfmet>125 && pfmet<200" , lost_b_sample_list_simplified, all_plot_types).Weight("weight * w_pu" * yearWeight);


      // pm.Push<Hist1D>(Axis(10, 0, 100., genBpT, "b-quark gen pT [GeV]"), signal_region && njetbins[ijet] && "pfmet>200" , lost_b_sample_list_simplified, all_plot_types).Weight("weight * w_pu" * yearWeight);
      pm.Push<Hist1D>(Axis(10, 0, 100., genB_subleadingpT, "b-quark subleading gen pT [GeV]"), signal_region && njetbins[ijet] && "pfmet>200" , lost_b_sample_list_simplified, all_plot_types).Weight("weight * w_pu" * yearWeight);
      pm.Push<Hist1D>(Axis(10, -5, 5., genBeta_mostForward, "b-quark most forward gen eta"), signal_region && njetbins[ijet] && "pfmet>200" , lost_b_sample_list_simplified, all_plot_types).Weight("weight * w_pu" * yearWeight);
      pm.Push<Hist1D>(Axis(10, 0, 4., bDeltaR, "b-quark deltaR"), signal_region && njetbins[ijet] && "pfmet>200" , lost_b_sample_list_simplified, all_plot_types).Weight("weight * w_pu" * yearWeight);
      pm.Push<Hist1D>(Axis(10, 0, 4., minDR_lep_bquark, "min dR(bquark, reco lep)"), signal_region && njetbins[ijet] && "pfmet>200" , lost_b_sample_list_simplified, all_plot_types).Weight("weight * w_pu" * yearWeight);
 
      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), single_lep+"&&mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , real_bs_sample_list_short, all_plot_types).Weight("weight * w_pu" * yearWeight);
      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), single_lep+"&&mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , real_bs_sample_list_short, all_plot_types).Weight("weight * w_pu" * yearWeight);


  }

  // signal_table_cuts.push_back(TableRow("Signal Region, low MET", signalRegion_lowMET,0,0, "weight * w_pu" * yearWeight));
  // signal_table_cuts.push_back(TableRow("Signal Region, med MET", signalRegion_medMET,0,0, "weight * w_pu" * yearWeight));
  // signal_table_cuts.push_back(TableRow("Signal Region, high MET", signalRegion_highMET,0,0, "weight * w_pu" * yearWeight)); 
  
  // mbbside_table_cuts.push_back(TableRow("mbb Sideband", mbbside,0,0, "weight * w_pu" * yearWeight));
  // mbbside_table_cuts.push_back(TableRow("mbb Sideband, low MET", mbbside_lowMET,0,0, "weight * w_pu" * yearWeight));
  // mbbside_table_cuts.push_back(TableRow("mbb Sideband, med MET", mbbside_medMET,0,0, "weight * w_pu" * yearWeight));
  // mbbside_table_cuts.push_back(TableRow("mbb Sideband, high MET", mbbside_highMET,0,0, "weight * w_pu" * yearWeight));
  
  // dilep_table_cuts.push_back(TableRow("dilep", dilep,0,0, "weight * w_pu" * yearWeight));
  // dilep_table_cuts.push_back(TableRow("dilep, low MET", dilep_lowMET,0,0, "weight * w_pu" * yearWeight));
  // dilep_table_cuts.push_back(TableRow("dilep, med MET", dilep_medMET,0,0, "weight * w_pu" * yearWeight));
  // dilep_table_cuts.push_back(TableRow("dilep, high MET", dilep_highMET,0,0, "weight * w_pu" * yearWeight));

  // lowMCT_table_cuts.push_back(TableRow("low mCT", lowMCT,0,0, "weight * w_pu" * yearWeight));
  // lowMCT_table_cuts.push_back(TableRow("low mCT, low MET", lowMCT_lowMET,0,0, "weight * w_pu" * yearWeight));
  // lowMCT_table_cuts.push_back(TableRow("low mCT, med MET", lowMCT_medMET,0,0, "weight * w_pu" * yearWeight));
  // lowMCT_table_cuts.push_back(TableRow("low mCT, high MET", lowMCT_highMET,0,0, "weight * w_pu" * yearWeight));


  // for(auto &ipr: categories) pm.Push<Table>("chart_PRESELECTION_"+ipr.first,  preselection_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_PRESELECTION_HIGH_MT_"+ipr.first,  preselection_highmT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_SIGNAL_"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_MBBSIDE_"+ipr.first,  mbbside_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_0B_"+ipr.first,  no_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_1B_"+ipr.first,  one_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_DILEP_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_LOW_MT_"+ipr.first,  lowMT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_LOW_MCT_"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_PRESELECTION_MEDMED_"+ipr.first,  preselection_medmed_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_SIGNAL_MEDMED_"+ipr.first,  signal_medmed_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_0B_MEDMED_"+ipr.first,  no_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_1B_MEDMED_"+ipr.first,  one_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_DILEP_MEDMED_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);

  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_SIGNAL_"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_Bs) pm.Push<Table>("chart_MBBSIDE_"+ipr.first,  mbbside_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_Bs) pm.Push<Table>("chart_DILEP_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_Bs) pm.Push<Table>("chart_LOW_MCT_"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);

 for(auto &ipr: categories_lost_Bs) pm.Push<Table>("chart_SIGNAL_lostb"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_lost_Bs) pm.Push<Table>("chart_LOW_MCT_lostb"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);

  // for(auto &ipr: categories_lost_Bs_tt) pm.Push<Table>("chart_SIGNAL_lostb_tt"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_lost_Bs_tt) pm.Push<Table>("chart_LOW_MCT_lostb_tt"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);


  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_PRESELECTION_"+ipr.first,  preselection_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_PRESELECTION_HIGH_MT_"+ipr.first,  preselection_highmT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_SIGNAL_"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_MBBSIDE_"+ipr.first,  mbbside_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_0B_"+ipr.first,  no_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_1B_"+ipr.first,  one_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_DILEP_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_LOW_MT_"+ipr.first,  lowMT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_LOW_MCT_"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_PRESELECTION_MEDMED_"+ipr.first,  preselection_medmed_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_SIGNAL_MEDMED_"+ipr.first,  signal_medmed_table_cuts, ipr.second, true, true, true, false);

  Table & signal_chart = pm.Push<Table>("signal_chart", signal_table_cuts, real_bs_sample_list, false);
  Table & signal_chart_lostb = pm.Push<Table>("signal_chart_lostb", signal_table_cuts, categories_lost_Bs["LostBs"], false);
  // Table & signal_chart_lostb = pm.Push<Table>("signal_chart_lostb", signal_table_cuts, lost_b_sample_list, false);
  // Table & signal_chart_lostb_tt = pm.Push<Table>("signal_chart_lostb_tt", signal_table_cuts, categories_lost_Bs_tt["LostBs_tt"], false);
  // Table & signal_chart_lostb_tt = pm.Push<Table>("signal_chart_lostb_tt", signal_table_cuts, lost_b_sample_list_ttbar, false);
  // Table & mbbside_chart = pm.Push<Table>("mbbside_chart", mbbside_table_cuts, sample_list, false);
  // Table & dilep_chart = pm.Push<Table>("dilep_chart", dilep_table_cuts, sample_list, false);
  // Table & lowmCT_chart = pm.Push<Table>("lowmCT_chart", lowMCT_table_cuts, real_bs_sample_list, false);
  // Table & lowmCT_chart_lostb = pm.Push<Table>("lowmCT_chart_lostb", lowMCT_table_cuts, lost_b_sample_list, false);
  // Table & lowmCT_chart_lostb_tt = pm.Push<Table>("lowmCT_chart_lostb_tt", lowMCT_table_cuts, lost_b_sample_list_ttbar, false);

  // Table & real_b_preselection_chart = pm.Push<Table>("real_b_preselection_chart", preselection_table_cuts, real_bs_sample_list, false);
  // Table & real_b_preselection_highmT_chart = pm.Push<Table>("real_b_preselection_highmT_chart", preselection_highmT_table_cuts, real_bs_sample_list, false);
  // Table & real_b_signal_chart = pm.Push<Table>("real_b_signal_chart", signal_table_cuts, real_bs_sample_list, false);
  // Table & real_b_mbbside_chart = pm.Push<Table>("real_b_mbbside_chart", mbbside_table_cuts, real_bs_sample_list, false);
  // Table & real_b_no_b_chart = pm.Push<Table>("real_b_no_b_chart", no_b_table_cuts, real_bs_sample_list, false);
  // Table & real_b_one_b_chart = pm.Push<Table>("real_b_one_b_chart", one_b_table_cuts, real_bs_sample_list, false);
  // Table & real_b_dilep_chart = pm.Push<Table>("real_b_dilep_chart", dilep_table_cuts, real_bs_sample_list, false);
  // Table & real_b_lowmT_chart = pm.Push<Table>("real_b_lowmT_chart", lowMT_table_cuts, real_bs_sample_list, false);
  // Table & real_b_lowmCT_chart = pm.Push<Table>("real_b_lowmCT_chart", lowMCT_table_cuts, real_bs_sample_list, false);
  // Table & real_b_preselection_medmed_chart = pm.Push<Table>("real_b_preselection_medmed_chart", preselection_medmed_table_cuts, real_bs_sample_list, false);
  // Table & real_b_signal_medmed_chart = pm.Push<Table>("real_b_signal_medmed_chart", signal_medmed_table_cuts, real_bs_sample_list, false);

  // Table & split_b_preselection_chart = pm.Push<Table>("split_b_preselection_chart", preselection_table_cuts, b_full_sample_list, false);
  // Table & split_b_preselection_highmT_chart = pm.Push<Table>("split_b_preselection_highmT_chart", preselection_highmT_table_cuts, b_full_sample_list, false);
  // Table & split_b_signal_chart = pm.Push<Table>("split_b_signal_chart", signal_table_cuts, b_full_sample_list, false);
  // Table & split_b_mbbside_chart = pm.Push<Table>("split_b_mbbside_chart", mbbside_table_cuts, b_full_sample_list, false);
  // Table & split_b_no_b_chart = pm.Push<Table>("split_b_no_b_chart", no_b_table_cuts, b_full_sample_list, false);
  // Table & split_b_one_b_chart = pm.Push<Table>("split_b_one_b_chart", one_b_table_cuts, b_full_sample_list, false);
  // Table & split_b_dilep_chart = pm.Push<Table>("split_b_dilep_chart", dilep_table_cuts, b_full_sample_list, false);
  // Table & split_b_lowmT_chart = pm.Push<Table>("split_b_lowmT_chart", lowMT_table_cuts, b_full_sample_list, false);
  // Table & split_b_lowmCT_chart = pm.Push<Table>("split_b_lowmCT_chart", lowMCT_table_cuts, b_full_sample_list, false);
  // Table & split_b_preselection_medmed_chart = pm.Push<Table>("split_b_preselection_medmed_chart", preselection_medmed_table_cuts, b_full_sample_list, false);
  // Table & split_b_signal_medmed_chart = pm.Push<Table>("split_b_signal_medmed_chart", signal_medmed_table_cuts, b_full_sample_list, false);

  pm.min_print_ = true;
  pm.MakePlots(lumi);

  cout<<"Finished making plots."<<endl;
  vector<GammaParams> yields_signal = signal_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_signal){
    cout << yield << endl;
  }

    vector<GammaParams> yields_signal_lb = signal_chart_lostb.BackgroundYield(lumi);
  for(const auto &yield: yields_signal_lb){
    cout << yield << endl;
  }

  // vector<GammaParams> yields_signal_lb_tt = signal_chart_lostb_tt.BackgroundYield(lumi);
  // for(const auto &yield: yields_signal_lb_tt){
  //   cout << yield << endl;
  // }
  // vector<GammaParams> yields_mbbside = mbbside_chart.BackgroundYield(lumi);
  // for(const auto &yield: yields_mbbside){
  //   cout << yield << endl;
  // }


  // vector<GammaParams> yields_dilep = dilep_chart.BackgroundYield(lumi);
  // for(const auto &yield: yields_dilep){
  //   cout << yield << endl;
  // }


  // vector<GammaParams> yields_lowmCT = lowmCT_chart.BackgroundYield(lumi);
  // for(const auto &yield: yields_lowmCT){
  //   cout << yield << endl;
  // }

  //   vector<GammaParams> yields_lowmCT_lb = lowmCT_chart_lostb.BackgroundYield(lumi);
  // for(const auto &yield: yields_lowmCT_lb){
  //   cout << yield << endl;
  // }


  //   vector<GammaParams> yields_lowmCT_lb_tt = lowmCT_chart_lostb_tt.BackgroundYield(lumi);
  // for(const auto &yield: yields_lowmCT_lb_tt){
  //   cout << yield << endl;
  // }


}
