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
#include "core/plot_opt.hpp"
#include "core/palette.hpp"
#include "core/hist1d.hpp"
#include "core/wh_functions.hpp"

using namespace std;
using namespace PlotOptTypes;
using namespace WH_Functions;


int main(){
  gErrorIgnoreLevel = 6000;
  
  double lumi2016 = 35.9;
  double lumi2017 = 41.6;
  double lumi2018 = 59.7;
  double lumicomb = 137.2;

  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_mc_s16v3_v33_4_2019_12_30/";
  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_mc_f17v2_v33_4_2019_12_30/";
  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_mc_a18v1_v33_4_2019_12_30/";

  string data2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2019_12_30/";
  string data2017_dir = "/home/users/dspitzba/wh_babies/babies_Run2017_v33_4_2019_12_30/";
  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_Run2018_v33_4_2019_12_30/";

  string signal2016_dir = "/home/users/dspitzba/wh_babies/babies_signal_s16v3_v33_4_2019_12_30/";
  string signal2017_dir = "/home/users/dspitzba/wh_babies/babies_signal_f17v2_v33_4_2019_12_30/";
  string signal2018_dir = "/home/users/dspitzba/wh_babies/babies_signal_a18v1_v33_4_2019_12_30/";

  Palette colors("txt/colors.txt", "default");

  //Data
  auto data2016 = Process::MakeShared<Baby_full>("2016 Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto data2017 = Process::MakeShared<Baby_full>("2017 Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto data2018 = Process::MakeShared<Baby_full>("2018 Data", Process::Type::data, colors("data"),{data2018_dir+"slim_data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto dataComb = Process::MakeShared<Baby_full>("Combined Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*.root",data2017_dir+"slim_data_2017*.root",data2018_dir+"slim_data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");

  //ttbar
  auto tt1l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016", Process::Type::background, colors("tt_1l"),{mc2016_dir+"*TTJets_1lep_top_*.root",mc2016_dir+"*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016", Process::Type::background, colors("tt_2l"),{mc2016_dir+"*TTJets_2lep_*.root"},"stitch");

  auto tt1l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2017", Process::Type::background, colors("tt_1l"),{mc2017_dir+"*TTJets_1lep_top_*.root",mc2017_dir+"*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2017", Process::Type::background, colors("tt_2l"),{mc2017_dir+"*TTJets_2lep_*.root"},"stitch");

  auto tt1l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2018", Process::Type::background, colors("tt_1l"),{mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2018", Process::Type::background, colors("tt_2l"),{mc2018_dir+"slim*TTJets_2lep_*.root"},"stitch");

  auto tt1l_Comb = Process::MakeShared<Baby_full>("t#bar{t} (1l) Combined", Process::Type::background, colors("tt_1l"),{mc2016_dir+"*TTJets_1lep_top_*.root",mc2016_dir+"*TTJets_1lep_tbar_*",mc2017_dir+"*TTJets_1lep_top_*.root",mc2017_dir+"*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_Comb = Process::MakeShared<Baby_full>("t#bar{t} (2l) Combined", Process::Type::background, colors("tt_2l"),{mc2016_dir+"*TTJets_2lep_*.root",mc2017_dir+"*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root"},"stitch");

  //WJets
  auto wjets_2016 = Process::MakeShared<Baby_full>("W+jets 2016", Process::Type::background, colors("wjets"), {mc2016_dir+"*slim_W*JetsToLNu_s16v3*",mc2016_dir+"*W*Jets_NuPt200_s16v*.root"},"stitch");
  auto wjets_2017 = Process::MakeShared<Baby_full>("W+jets 2017", Process::Type::background, colors("wjets"), {mc2017_dir+"*slim_W*JetsToLNu_f17v2*",mc2017_dir+"*W*Jets_NuPt200_f17v2*.root"},"stitch");
  auto wjets_2018 = Process::MakeShared<Baby_full>("W+jets 2018", Process::Type::background, colors("wjets"), {mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root"},"stitch");
  auto wjets_Comb = Process::MakeShared<Baby_full>("W+jets Combined", Process::Type::background, colors("wjets"), {mc2016_dir+"*slim_W*JetsToLNu_s16v3*",mc2016_dir+"*W*Jets_NuPt200_s16v*.root",mc2017_dir+"*slim_W*JetsToLNu_f17v2*",mc2017_dir+"*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root"},"stitch");

  //Single top
  auto single_t_2016 = Process::MakeShared<Baby_full>("Single t 2016", Process::Type::background, colors("single_t"), {mc2016_dir+"*_ST_*.root"});
  auto single_t_2017 = Process::MakeShared<Baby_full>("Single t 2017", Process::Type::background, colors("single_t"), {mc2017_dir+"*_ST_*.root"});
  auto single_t_2018 = Process::MakeShared<Baby_full>("Single t 2018", Process::Type::background, colors("single_t"), {mc2018_dir+"slim*_ST_*.root"});
  auto single_t_Comb = Process::MakeShared<Baby_full>("Single t Combined", Process::Type::background, colors("single_t"), {mc2016_dir+"*_ST_*.root",mc2017_dir+"*_ST_*.root",mc2018_dir+"slim*_ST_*.root"});

  //diboson
  auto diboson_2016 = Process::MakeShared<Baby_full>("Diboson 2016", Process::Type::background, colors("other"),{mc2016_dir+"*WW*.root", mc2016_dir+"*WZ*.root",mc2016_dir+"*ZZ*.root"});
  auto diboson_2017 = Process::MakeShared<Baby_full>("Diboson 2017", Process::Type::background, colors("other"),{mc2017_dir+"*WW*.root", mc2017_dir+"*WZ*.root",mc2017_dir+"*ZZ*.root"});
  auto diboson_2018 = Process::MakeShared<Baby_full>("Diboson 2018", Process::Type::background, colors("other"),{mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"});
  auto diboson_Comb = Process::MakeShared<Baby_full>("Diboson Combined", Process::Type::background, colors("other"),{mc2016_dir+"*WW*.root", mc2016_dir+"*WZ*.root",mc2016_dir+"*ZZ*.root",mc2017_dir+"*WW*.root", mc2017_dir+"*WZ*.root",mc2017_dir+"*ZZ*.root",mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"});

  // ttV
  auto ttV_2016 = Process::MakeShared<Baby_full>("t#bar{t}V 2016", Process::Type::background, colors("ttv"),{mc2016_dir+"*_TTWJets*.root", mc2016_dir+"*_TTZ*.root"});
  auto ttV_2017 = Process::MakeShared<Baby_full>("t#bar{t}V 2017", Process::Type::background, colors("ttv"), {mc2017_dir+"*_TTWJets*.root", mc2017_dir+"*_TTZ*.root"});
  auto ttV_2018 = Process::MakeShared<Baby_full>("t#bar{t}V 2018", Process::Type::background, colors("ttv"), {mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"});
  auto ttV_Comb = Process::MakeShared<Baby_full>("t#bar{t}V Combined", Process::Type::background, colors("ttv"), {mc2016_dir+"*_TTWJets*.root", mc2016_dir+"*_TTZ*.root",mc2017_dir+"*_TTWJets*.root", mc2017_dir+"*_TTZ*.root",mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"});


  //signals
  auto signal_2016_225_75 = Process::MakeShared<Baby_full>("2016 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
    signal_2016_225_75->SetLineStyle(2);
  auto signal_2017_225_75 = Process::MakeShared<Baby_full>("2017 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2017_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
    signal_2017_225_75->SetLineStyle(2);
  auto signal_2018_225_75 = Process::MakeShared<Baby_full>("2018 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2018_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
    signal_2018_225_75->SetLineStyle(2);
  auto signal_comb_225_75 = Process::MakeShared<Baby_full>("Combined Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root",signal2017_dir+"slim_*.root",signal2018_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
    signal_comb_225_75->SetLineStyle(2);


  auto tt2l_medmed_Comb = Process::MakeShared<Baby_full>("t#bar{t} (2l) med/med SR", Process::Type::background, colors("tt_2l"),{mc2016_dir+"*TTJets_2lep_*.root",mc2017_dir+"*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root"},WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&stitch");
  auto single_t_medmed_Comb = Process::MakeShared<Baby_full>("Single t med/med SR", Process::Type::background, colors("single_t"), {mc2016_dir+"*_ST_*.root",mc2017_dir+"*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2");

  auto tt2l_medloose_Comb = Process::MakeShared<Baby_full>("t#bar{t} (2l) med/loose CR", Process::Type::background, colors("tt_1l"),{mc2016_dir+"*TTJets_2lep_*.root",mc2017_dir+"*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root"},WHLeptons==1&&HasMedLooseCSV&&"pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150&&stitch");
  auto single_t_medloose_Comb = Process::MakeShared<Baby_full>("Single med/loose CR", Process::Type::background, colors("wjets"), {mc2016_dir+"*_ST_*.root",mc2017_dir+"*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},WHLeptons==1&&HasMedLooseCSV&&"pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150");

  auto signal_2016_700_1 = Process::MakeShared<Baby_full>("2016 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_2017_700_1 = Process::MakeShared<Baby_full>("2017 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2017_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_2018_700_1 = Process::MakeShared<Baby_full>("2018 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2018_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_comb_700_1 = Process::MakeShared<Baby_full>("Combined Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root",signal2017_dir+"slim_*.root",signal2018_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");

  //List of processes
  vector<shared_ptr<Process> > sample_list_2016 = {data2016,wjets_2016,tt2l_2016,tt1l_2016,single_t_2016,diboson_2016,ttV_2016};
  vector<shared_ptr<Process> > sample_list_2017 = {data2017,wjets_2017,tt2l_2017,tt1l_2017,single_t_2017,diboson_2017,ttV_2017};
  vector<shared_ptr<Process> > sample_list_2018 = {data2018,wjets_2018,tt2l_2018,tt1l_2018,single_t_2018,diboson_2018,ttV_2018};
  vector<shared_ptr<Process> > sample_list_Comb = {dataComb,wjets_Comb,tt2l_Comb,tt1l_Comb,single_t_Comb,diboson_Comb,ttV_Comb};

  vector<shared_ptr<Process> > sample_list_signal_2016 = {data2016,wjets_2016,tt2l_2016,tt1l_2016,single_t_2016,diboson_2016,ttV_2016,signal_2016_225_75,signal_2016_700_1};
  vector<shared_ptr<Process> > sample_list_signal_2017 = {data2017,wjets_2017,tt2l_2017,tt1l_2017,single_t_2017,diboson_2017,ttV_2017,signal_2017_225_75,signal_2017_700_1};
  vector<shared_ptr<Process> > sample_list_signal_2018 = {data2018,wjets_2018,tt2l_2018,tt1l_2018,single_t_2018,diboson_2018,ttV_2018,signal_2018_225_75,signal_2018_700_1};
  vector<shared_ptr<Process> > sample_list_signal_Comb = {dataComb,wjets_Comb,tt2l_Comb,tt1l_Comb,single_t_Comb,diboson_Comb,ttV_Comb,signal_comb_225_75,signal_comb_700_1};

  vector<shared_ptr<Process> > sample_list_nodata_2016 = {wjets_2016,tt2l_2016,tt1l_2016,single_t_2016,diboson_2016,ttV_2016};
  vector<shared_ptr<Process> > sample_list_nodata_2017 = {wjets_2017,tt2l_2017,tt1l_2017,single_t_2017,diboson_2017,ttV_2017};
  vector<shared_ptr<Process> > sample_list_nodata_2018 = {wjets_2018,tt2l_2018,tt1l_2018,single_t_2018,diboson_2018,ttV_2018};
  vector<shared_ptr<Process> > sample_list_nodata_Comb = {wjets_Comb,tt2l_Comb,tt1l_Comb,single_t_Comb,diboson_Comb,ttV_Comb};

  vector<shared_ptr<Process> > sample_list_tt2l = {tt2l_medmed_Comb,tt2l_medloose_Comb};
  vector<shared_ptr<Process> > sample_list_single_t = {single_t_medmed_Comb,single_t_medloose_Comb};


  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm)
    .RatioMinimum(0).RatioMaximum(2.0);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .Bottom(BottomType::ratio)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info, log_shapes_info};

  //Need different PlotMaker for each luminosity.
    //For some reason, they needed to be pointers to go into vector later on..
  PlotMaker * pm2016 = new PlotMaker();
  PlotMaker * pm2017 = new PlotMaker();
  PlotMaker * pm2018 = new PlotMaker();
  PlotMaker * pmComb = new PlotMaker();

  PlotMaker * pm2016_signal = new PlotMaker();
  PlotMaker * pm2017_signal = new PlotMaker();
  PlotMaker * pm2018_signal = new PlotMaker();
  PlotMaker * pmComb_signal = new PlotMaker();

  PlotMaker * pmetc = new PlotMaker();

  pmetc->Push<Hist1D>(Axis(5, 0, 500., "mct", "mct [GeV]"),
      "pass", sample_list_tt2l, all_plot_types).Weight("weight*w_pu"*yearWeight).Tag("coarse_yAll_tt2l_");
  pmetc->Push<Hist1D>(Axis(6, 200, 500., "mct", "mct [GeV]"),
      "pass", sample_list_tt2l, all_plot_types).Weight("weight*w_pu"*yearWeight).Tag("fine_yAll_tt2l_");

  pmetc->Push<Hist1D>(Axis(5, 0, 500., "mct", "mct [GeV]"),
      "pass", sample_list_single_t, all_plot_types).Weight("weight*w_pu"*yearWeight).Tag("coarse_yAll_single_t_");

  //Selections
  NamedFunc dilepton_lowMET_lowMT_looseLep = "pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>50&&ngoodbtags==2" && WHLeptons==1;
  NamedFunc dilepton_lowMET_lowMT_tightLep = "pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>50&&ngoodbtags==2" && WHLeptons==2;
  NamedFunc dilepton_lowMET_highMT_looseLep = "pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&ngoodbtags==2" && WHLeptons==1;
  NamedFunc dilepton_lowMET_highMT_tightLep = "pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&ngoodbtags==2" && WHLeptons==2;
  NamedFunc dilepton_looseLep = WHLeptons==1&&"pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&ngoodbtags==2";
  NamedFunc dilepton_tightLep = "pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&ngoodbtags==2" && WHLeptons==2;

  NamedFunc dilepton_looseLep_noMET = WHLeptons==1&&"pass&&nvetoleps>=2&&ngoodjets==2&&ngoodbtags==2&&pfmet>50&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150";
  NamedFunc dilepton_looseLep_noMbb = WHLeptons==1&&"pass&&nvetoleps>=2&&ngoodjets==2&&ngoodbtags==2&&pfmet>200&&mct>200&&mt_met_lep>150";
  NamedFunc dilepton_looseLep_noMCT = WHLeptons==1&&"pass&&nvetoleps>=2&&ngoodjets==2&&ngoodbtags==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150";
  NamedFunc dilepton_looseLep_noMT  = WHLeptons==1&&"pass&&nvetoleps>=2&&ngoodjets==2&&ngoodbtags==2&&pfmet>200&&mbb>90&&mbb<150&&mct>200";

  NamedFunc dilepton_looseLep_lowMET       = WHLeptons==1&&"pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&ngoodbtags==2";
  NamedFunc dilepton_looseLep_lowMET_noMbb = WHLeptons==1&&"pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mct>200&&mt_met_lep>150&&ngoodbtags==2";
  NamedFunc dilepton_looseLep_lowMET_noMCT = WHLeptons==1&&"pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mt_met_lep>150&&ngoodbtags==2";
  NamedFunc dilepton_looseLep_lowMET_noMT  = WHLeptons==1&&"pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&ngoodbtags==2";

  NamedFunc dilepton_looseLep_medloose_lowMET       = WHLeptons==1&&HasMedLooseCSV&&"pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150";
  NamedFunc dilepton_looseLep_medloose_lowMET_noMbb = WHLeptons==1&&HasMedLooseCSV&&"pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mct>200&&mt_met_lep>150";
  NamedFunc dilepton_looseLep_medloose_lowMET_noMCT = WHLeptons==1&&HasMedLooseCSV&&"pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mt_met_lep>150";
  NamedFunc dilepton_looseLep_medloose_lowMET_noMT  = WHLeptons==1&&HasMedLooseCSV&&"pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mct>200";

  NamedFunc dilepton_looseLep_3jet_lowMET       = WHLeptons==1&&LeadingNonBJetPt_med>100&&"pass&&nvetoleps>=2&&ngoodjets==3&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&ngoodbtags==2";
  NamedFunc dilepton_looseLep_3jet_lowMET_noMbb = WHLeptons==1&&LeadingNonBJetPt_med>100&&"pass&&nvetoleps>=2&&ngoodjets==3&&pfmet>125&&mct>200&&mt_met_lep>150&&ngoodbtags==2";
  NamedFunc dilepton_looseLep_3jet_lowMET_noMCT = WHLeptons==1&&LeadingNonBJetPt_med>100&&"pass&&nvetoleps>=2&&ngoodjets==3&&pfmet>125&&mbb>90&&mbb<150&&mt_met_lep>150&&ngoodbtags==2";
  NamedFunc dilepton_looseLep_3jet_lowMET_noMT  = WHLeptons==1&&LeadingNonBJetPt_med>100&&"pass&&nvetoleps>=2&&ngoodjets==3&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&ngoodbtags==2";

  //vector<NamedFunc> sels = {/*dilepton_lowMET_lowMT_tightLep,dilepton_lowMET_lowMT_looseLep,dilepton_lowMET_highMT_tightLep,dilepton_lowMET_highMT_looseLep,dilepton_tightLep,*/dilepton_looseLep}; 
  //vector<string> sels_tags = {/*"_dilepton_lowMET_lowMT_tightLep","_dilepton_lowMET_lowMT_looseLep","_dilepton_lowMET_highMT_tightLep","_dilepton_lowMET_highMT_looseLep","_dilepton_tightLep",*/"_dilepton_looseLep"}; 
  vector<NamedFunc> sels   = {dilepton_looseLep_lowMET,dilepton_looseLep_lowMET_noMbb,dilepton_looseLep_lowMET_noMCT,dilepton_looseLep_lowMET_noMT,dilepton_looseLep_3jet_lowMET,dilepton_looseLep_3jet_lowMET_noMbb,dilepton_looseLep_3jet_lowMET_noMCT,dilepton_looseLep_3jet_lowMET_noMT}; 
  vector<string> sels_tags = {"_dilepton_looseLep_lowMET","_dilepton_looseLep_lowMET_noMbb","_dilepton_looseLep_lowMET_noMCT","_dilepton_looseLep_lowMET_noMT","_dilepton_looseLep_3jet_lowMET","_dilepton_looseLep_3jet_lowMET_noMbb","_dilepton_looseLep_3jet_lowMET_noMCT","_dilepton_looseLep_3jet_lowMET_noMT"}; 
  vector<PlotMaker *> pms = {pm2016,pm2017,pm2018,pmComb,pm2016_signal,pm2017_signal,pm2018_signal,pmComb_signal};
  vector<vector<shared_ptr<Process> > > samples_Run2 = {sample_list_2016,sample_list_2017,sample_list_2018,sample_list_Comb,sample_list_signal_2016,sample_list_signal_2017,sample_list_signal_2018,sample_list_signal_Comb};
  vector<string> years = {"y2016","y2017","y2018","yAll","y2016_signal","y2017_signal","y2018_signal","yAll_signal"};
  vector<NamedFunc> weights = {"weight * w_pu"*yearWeight};

  for(uint isel=0;isel<sels.size();isel++){
    for(uint iyear=0;iyear<samples_Run2.size();iyear++){
      for(uint iweight=0;iweight<weights.size();iweight++){

        pms[iyear]->Push<Hist1D>(Axis(10, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]+sels_tags[isel]);  

        pms[iyear]->Push<Hist1D>(Axis(10, 0, 500., "mct", "mct [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]+sels_tags[isel]);  

        pms[iyear]->Push<Hist1D>(Axis(10, 0, 300., "mbb", "mbb [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]+sels_tags[isel]);

        pms[iyear]->Push<Hist1D>(Axis(10, 0, 500., "mt_met_lep", "mt [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]+sels_tags[isel]);

        pms[iyear]->Push<Hist1D>(Axis(10, 0, 300., "leps_pt[0]", "Leading lepton p_{T} [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]+sels_tags[isel]);  
        
        pms[iyear]->Push<Hist1D>(Axis(10, 0, 10., "ngoodjets", "Njets"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]+sels_tags[isel]); 


        pms[iyear]->Push<Hist1D>(Axis(5, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("coarse_"+years[iyear]+sels_tags[isel]);  

        pms[iyear]->Push<Hist1D>(Axis(5, 0, 500., "mct", "mct [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("coarse_"+years[iyear]+sels_tags[isel]);

        pms[iyear]->Push<Hist1D>(Axis(5, 30, 330., "mbb", "mbb [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("coarse_"+years[iyear]+sels_tags[isel]);

        pms[iyear]->Push<Hist1D>(Axis(5, 0, 500., "mt_met_lep", "mt [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("coarse_"+years[iyear]+sels_tags[isel]);

        pms[iyear]->Push<Hist1D>(Axis(5, 0, 300., "leps_pt[0]", "Leading lepton p_{T} [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("coarse_"+years[iyear]+sels_tags[isel]);
        
        pms[iyear]->Push<Hist1D>(Axis(10, -2.5, 2.5, "leps_eta[0]", "Leading lepton #eta [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("coarse_"+years[iyear]+sels_tags[isel]);  
      }
    }
  }

  PlotMaker * pmSR2016 = new PlotMaker();
  PlotMaker * pmSR2017 = new PlotMaker();
  PlotMaker * pmSR2018 = new PlotMaker();
  PlotMaker * pmSRComb = new PlotMaker();

   //Selections
  NamedFunc signalRegion = "pass&&nvetoleps>=2&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>50&&ngoodbtags==2" && WHLeptons==1;

  NamedFunc signalRegion_3jet = "pass&&nvetoleps>=2&&ngoodjets==3&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>50&&ngoodbtags==2" && WHLeptons==1 && LeadingNonBJetPt_med>100;

  vector<NamedFunc> SR_sels   = {signalRegion, signalRegion_3jet}; 
  vector<string> SR_sels_tags = {"_signalRegion","_signalRegion_3jet"}; 
  vector<PlotMaker *> SR_pms = {pmSR2016,pmSR2017,pmSR2018,pmSRComb};
  vector<vector<shared_ptr<Process> > > SR_samples_Run2 = {sample_list_nodata_2016,sample_list_nodata_2017,sample_list_nodata_2018,sample_list_nodata_Comb};
  vector<string> SR_years = {"y2016_nodata","y2017_nodata","y2018_nodata","yAll_nodata"};
  vector<NamedFunc> SR_weights = {"weight * w_pu"*yearWeight};

  for(uint isel=0;isel<SR_sels.size();isel++){
    for(uint iyear=0;iyear<SR_samples_Run2.size();iyear++){
      for(uint iweight=0;iweight<SR_weights.size();iweight++){

        SR_pms[iyear]->Push<Hist1D>(Axis(10, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag(SR_years[iyear]+SR_sels_tags[isel]);  

        SR_pms[iyear]->Push<Hist1D>(Axis(10, 0, 500., "mct", "mct [GeV]"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag(SR_years[iyear]+SR_sels_tags[isel]);  

        SR_pms[iyear]->Push<Hist1D>(Axis(10, 0, 300., "mbb", "mbb [GeV]"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag(SR_years[iyear]+SR_sels_tags[isel]);

        SR_pms[iyear]->Push<Hist1D>(Axis(10, 0, 500., "mt_met_lep", "mt [GeV]"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag(SR_years[iyear]+SR_sels_tags[isel]);

        SR_pms[iyear]->Push<Hist1D>(Axis(10, 0, 300., "leps_pt[0]", "Leading lepton p_{T} [GeV]"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag(SR_years[iyear]+SR_sels_tags[isel]);  
        
        SR_pms[iyear]->Push<Hist1D>(Axis(10, 0, 10., "ngoodjets", "Njets"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag(SR_years[iyear]+SR_sels_tags[isel]); 


        SR_pms[iyear]->Push<Hist1D>(Axis(5, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag("coarse_"+SR_years[iyear]+SR_sels_tags[isel]);  

        SR_pms[iyear]->Push<Hist1D>(Axis(5, 0, 500., "mct", "mct [GeV]"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag("coarse_"+SR_years[iyear]+SR_sels_tags[isel]);  

        SR_pms[iyear]->Push<Hist1D>(Axis(5, 0, 300., "mbb", "mbb [GeV]"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag("coarse_"+SR_years[iyear]+SR_sels_tags[isel]);

        SR_pms[iyear]->Push<Hist1D>(Axis(5, 0, 500., "mt_met_lep", "mt [GeV]"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag("coarse_"+SR_years[iyear]+SR_sels_tags[isel]);

        SR_pms[iyear]->Push<Hist1D>(Axis(5, 0, 300., "leps_pt[0]", "Leading lepton p_{T} [GeV]"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag("coarse_"+SR_years[iyear]+SR_sels_tags[isel]);
        
        SR_pms[iyear]->Push<Hist1D>(Axis(5, 0, 2.5, "leps_eta[0]", "Leading lepton #eta [GeV]"),
          SR_sels[isel], SR_samples_Run2[iyear], all_plot_types).Weight(SR_weights[iweight]).Tag("coarse_"+SR_years[iyear]+SR_sels_tags[isel]);  
      }
    }
  }

  pm2016->MakePlots(lumi2016);
  pm2017->MakePlots(lumi2017);
  pm2018->MakePlots(lumi2018);
  pmComb->MakePlots(lumicomb);

  pm2016_signal->MakePlots(lumi2016);
  pm2017_signal->MakePlots(lumi2017);
  pm2018_signal->MakePlots(lumi2018);
  pmComb_signal->MakePlots(lumicomb);

  pmSR2016->MakePlots(lumi2016);
  pmSR2017->MakePlots(lumi2017);
  pmSR2018->MakePlots(lumi2018);
  pmSRComb->MakePlots(lumicomb);

  pmetc->MakePlots(lumicomb);

}
