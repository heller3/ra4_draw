
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <unistd.h>
#include <getopt.h>

#include "TError.h"
#include "TColor.h"

#include "core/baby.hpp"
#include "core/process.hpp"
#include "core/named_func.hpp"
#include "core/plot_maker.hpp"
#include "core/plot_opt.hpp"
#include "core/palette.hpp"
#include "core/table.hpp"
#include "core/event_scan.hpp"
#include "core/hist1d.hpp"
#include "core/hist2d.hpp"
#include "core/utilities.hpp"
#include "core/functions.hpp"
#include "core/wh_functions.hpp"

using namespace std;
using namespace PlotOptTypes;
using namespace WH_Functions;

namespace{
  bool single_thread = false;
}


int main(){
  gErrorIgnoreLevel = 6000;
  

  /*double lumi2016 = 35.9;
  double lumi2017 = 41.6;
  double lumi2018 = 59.7;*/
  double lumicomb = 137.2;

  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/s16v3/";
  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/f17v2/";
  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/a18v1/";

  string data2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string data2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";

  string signal2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string signal2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string signal2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";

  string mc2018_ttbar_ext_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_06_12/a18v1/";

  /*string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname,"compute-")){
    signal_dir = "/net/cms29";
  }*/
 
  Palette colors("txt/colors.txt", "default");

  //Data
  auto data2016 = Process::MakeShared<Baby_full>("2016 Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto data2017 = Process::MakeShared<Baby_full>("2017 Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto data2018 = Process::MakeShared<Baby_full>("2018 Data", Process::Type::data, colors("data"),{data2018_dir+"slim_data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);
  auto dataComb = Process::MakeShared<Baby_full>("2016-2018 Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*.root",data2017_dir+"slim_data_2017*.root",data2018_dir+"slim_data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);

  //ttbar
  auto tt1l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*.root"},"stitch");
  auto ttbar_2016 = Process::MakeShared<Baby_full>("t#bar{t} 2016", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*",mc2016_dir+"slim*TTJets_2lep_*.root"},"stitch");

  auto tt1l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2017", Process::Type::background, colors("tt_1l"),{mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2017", Process::Type::background, colors("tt_2l"),{mc2017_dir+"slim*TTJets_2lep_*.root"},"stitch");
  auto ttbar_2017 = Process::MakeShared<Baby_full>("t#bar{t} 2017", Process::Type::background, colors("tt_2l"),{mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*",mc2017_dir+"slim*TTJets_2lep_*.root"},"stitch");

  auto tt1l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2018", Process::Type::background, colors("tt_1l"),{mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2018", Process::Type::background, colors("tt_2l"),{mc2018_dir+"slim*TTJets_2lep_a18v1*.root",mc2018_ttbar_ext_dir+"slim*"},"stitch");
  auto ttbar_2018 = Process::MakeShared<Baby_full>("t#bar{t} 2018", Process::Type::background, colors("tt_2l"),{mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_2lep_a18v1*.root",mc2018_ttbar_ext_dir+"slim*"},"stitch");

  auto tt1l_Comb = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016-2018", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*",mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_Comb = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016-2018", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_a18v1*.root"},"stitch");
  auto ttbar_Comb = Process::MakeShared<Baby_full>("t#bar{t} 2016-2018", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*",mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*",mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_a18v1*.root",mc2018_ttbar_ext_dir+"slim*"},"stitch");

  //WJets
  auto wjets_2016 = Process::MakeShared<Baby_full>("W+jets 2016", Process::Type::background, colors("wjets"), {mc2016_dir+"slim*_W*JetsToLNu_s16v3*",mc2016_dir+"slim*W*Jets_NuPt200_s16v*.root"},"stitch");
  auto wjets_2017 = Process::MakeShared<Baby_full>("W+jets 2017", Process::Type::background, colors("wjets"), {mc2017_dir+"slim*_W*JetsToLNu_f17v2*",mc2017_dir+"slim*W*Jets_NuPt200_f17v2*.root"},"stitch");
  auto wjets_2018 = Process::MakeShared<Baby_full>("W+jets 2018", Process::Type::background, colors("wjets"), {mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root"},"stitch");
  auto wjets_Comb = Process::MakeShared<Baby_full>("W+jets 2016-2018", Process::Type::background, colors("wjets"), {mc2016_dir+"slim*_W*JetsToLNu_s16v3*",mc2016_dir+"slim*W*Jets_NuPt200_s16v*.root",mc2017_dir+"slim_W*JetsToLNu_f17v2*",mc2017_dir+"slim*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root"},"stitch");

  //Top Backgrounds
  auto top_2016 = Process::MakeShared<Baby_full>("t#bar{t}/t 2016", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*_ST_*.root",mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*",mc2016_dir+"slim*TTJets_2lep_*.root"},"stitch");
  auto top_2017 = Process::MakeShared<Baby_full>("t#bar{t}/t 2017", Process::Type::background, colors("tt_2l"),{mc2017_dir+"slim*_ST_*.root",mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*",mc2017_dir+"slim*TTJets_2lep_*.root"},"stitch");
  auto top_2018 = Process::MakeShared<Baby_full>("t#bar{t}/t 2018", Process::Type::background, colors("tt_2l"),{mc2018_dir+"slim*_ST_*.root",mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_2lep_a18v1*.root",mc2018_ttbar_ext_dir+"slim*"},"stitch");
  auto top_Comb = Process::MakeShared<Baby_full>("t#bar{t}/t 2016-2018", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root",mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*",mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*",mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_a18v1*.root",mc2018_ttbar_ext_dir+"slim*"},"stitch");

  //Single top
  auto single_t_2016 = Process::MakeShared<Baby_full>("Single t 2016", Process::Type::background, colors("single_t"), {mc2016_dir+"slim*_ST_*.root"});
  auto single_t_2017 = Process::MakeShared<Baby_full>("Single t 2017", Process::Type::background, colors("single_t"), {mc2017_dir+"slim*_ST_*.root"});
  auto single_t_2018 = Process::MakeShared<Baby_full>("Single t 2018", Process::Type::background, colors("single_t"), {mc2018_dir+"slim*_ST_*.root"});
  auto single_t_Comb = Process::MakeShared<Baby_full>("Single t 2016-2018", Process::Type::background, colors("single_t"), {mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"});

  //diboson
  auto diboson_2016 = Process::MakeShared<Baby_full>("Diboson 2016", Process::Type::background, colors("other"),{mc2016_dir+"slim*WW*.root", mc2016_dir+"slim*WZ*.root",mc2016_dir+"slim*ZZ*.root"});
  auto diboson_2017 = Process::MakeShared<Baby_full>("Diboson 2017", Process::Type::background, colors("other"),{mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*WZ*.root",mc2017_dir+"slim*ZZ*.root"});
  auto diboson_2018 = Process::MakeShared<Baby_full>("Diboson 2018", Process::Type::background, colors("other"),{mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"});
  auto diboson_Comb = Process::MakeShared<Baby_full>("Diboson 2016-2018", Process::Type::background, colors("other"),{mc2016_dir+"slim*WW*.root", mc2016_dir+"slim*WZ*.root",mc2016_dir+"slim*ZZ*.root",mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*WZ*.root",mc2017_dir+"slim*ZZ*.root",mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"});

  // ttV
  auto ttV_2016 = Process::MakeShared<Baby_full>("t#bar{t}V 2016", Process::Type::background, colors("ttv"),{mc2016_dir+"slim*_TTWJets*.root", mc2016_dir+"slim*_TTZ*.root"});
  auto ttV_2017 = Process::MakeShared<Baby_full>("t#bar{t}V 2017", Process::Type::background, colors("ttv"), {mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZ*.root"});
  auto ttV_2018 = Process::MakeShared<Baby_full>("t#bar{t}V 2018", Process::Type::background, colors("ttv"), {mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"});
  auto ttV_Comb = Process::MakeShared<Baby_full>("t#bar{t}V 2016-2018", Process::Type::background, colors("ttv"), {mc2016_dir+"slim*_TTWJets*.root", mc2016_dir+"slim*_TTZ*.root",mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZ*.root",mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"});

  // SM WH
  auto smWH_2016 = Process::MakeShared<Baby_full>("SM WH 2016", Process::Type::background, colors("ttv"),{mc2016_dir+"slim_W*H_HToBB_WToLNu_*.root"});
  auto smWH_2017 = Process::MakeShared<Baby_full>("SM WH 2017", Process::Type::background, colors("ttv"), {mc2017_dir+"slim_W*H_HToBB_WToLNu_*.root"});
  auto smWH_2018 = Process::MakeShared<Baby_full>("SM WH 2018", Process::Type::background, colors("ttv"), {mc2018_dir+"slim_W*H_HToBB_WToLNu_*.root"});
  auto smWH_Comb = Process::MakeShared<Baby_full>("SM WH 2016-2018", Process::Type::background, colors("ttv"), {mc2016_dir+"slim_W*H_HToBB_WToLNu_*.root",mc2017_dir+"slim_W*H_HToBB_WToLNu_*.root",mc2018_dir+"slim_W*H_HToBB_WToLNu_*.root"});


  //signals
  auto signal_2016_350_100 = Process::MakeShared<Baby_full>("2016 Signal (350,100)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root"},"pass&&mass_stop==350&&mass_lsp==100");
  auto signal_2017_350_100 = Process::MakeShared<Baby_full>("2017 Signal (350,100)", Process::Type::signal, colors("t1tttt"),{signal2017_dir+"slim_*.root"},"pass&&mass_stop==350&&mass_lsp==100");
  auto signal_2018_350_100 = Process::MakeShared<Baby_full>("2018 Signal (350,100)", Process::Type::signal, colors("t1tttt"),{signal2018_dir+"slim_*.root"},"pass&&mass_stop==350&&mass_lsp==100");
  auto signal_comb_350_100 = Process::MakeShared<Baby_full>("2016-2018 Signal (350,100)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root",signal2017_dir+"slim_*.root",signal2018_dir+"slim_*.root"},"pass&&mass_stop==350&&mass_lsp==100");

  auto signal_2016_700_1 = Process::MakeShared<Baby_full>("2016 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_2017_700_1 = Process::MakeShared<Baby_full>("2017 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2017_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_2018_700_1 = Process::MakeShared<Baby_full>("2018 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2018_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_comb_700_1 = Process::MakeShared<Baby_full>("2016-2018 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root",signal2017_dir+"slim_*.root",signal2018_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");

  vector<shared_ptr<Process> > sample_list_comb = {wjets_Comb,top_Comb,diboson_Comb,ttV_Comb,smWH_Comb,signal_comb_350_100,signal_comb_700_1};
  vector<shared_ptr<Process> > sample_list_2016 = {wjets_2016,top_2016,diboson_2016,ttV_2016,smWH_2016,signal_2016_350_100,signal_2016_700_1};
  vector<shared_ptr<Process> > sample_list_2017 = {wjets_2017,top_2017,diboson_2017,ttV_2017,smWH_2017,signal_2017_350_100,signal_2017_700_1};
  vector<shared_ptr<Process> > sample_list_2018 = {wjets_2018,top_2018,diboson_2018,ttV_2018,smWH_2018,signal_2018_350_100,signal_2018_700_1};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {log_lumi, lin_lumi, log_shapes, lin_shapes,
                                    log_lumi_info, lin_lumi_info, log_shapes_info, lin_shapes_info};
  PlotOpt style2D("txt/plot_styles.txt", "Scatter");
  vector<PlotOpt> bkg_hist = {style2D().Stack(StackType::data_norm).Title(TitleType::preliminary)};
  vector<PlotOpt> bkg_pts = {style2D().Stack(StackType::lumi_shapes).Title(TitleType::info)};
  


  PlotMaker * pmcomb = new PlotMaker();


    Table & signalRegionYields_tot_cutflow_comb = pmcomb->Push<Table>("signalRegionYields_tot_cutflow_comb", vector<TableRow>{
      TableRow("2 jet, resolved, 125 $<$ MET $<$ 200",       WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",   0,0,"weight*trig_eff"*nanoWeight*yearWeight),
      TableRow("2 jet, resolved, 200 $<$ MET $<$ 300",       WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",   0,0,"weight*trig_eff"*nanoWeight*yearWeight),
      TableRow("2 jet, resolved, 300 $<$ MET $<$ 400",       WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",   0,0,"weight*trig_eff"*nanoWeight*yearWeight),
      TableRow("2 jet, resolved, 400 $<$ MET",               WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",              0,0,"weight*trig_eff"*nanoWeight*yearWeight),

      TableRow("2 jet, boosted, 125 $<$ MET $<$ 300",        WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",   0,0,"weight*trig_eff"*nanoWeight*yearWeight),
      TableRow("2 jet, boosted, 300 $<$ MET",                WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",              0,0,"weight*trig_eff"*nanoWeight*yearWeight),

      TableRow("3 jet, resolved, 125 $<$ MET $<$ 200",       WHLeptons==1&&LeadingNonBJetPt_med<300&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",   0,0,"weight*trig_eff"*nanoWeight*yearWeight),
      TableRow("3 jet, resolved, 200 $<$ MET $<$ 300",       WHLeptons==1&&LeadingNonBJetPt_med<300&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",   0,0,"weight*trig_eff"*nanoWeight*yearWeight),
      TableRow("3 jet, resolved, 300 $<$ MET $<$ 400",       WHLeptons==1&&LeadingNonBJetPt_med<300&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",   0,0,"weight*trig_eff"*nanoWeight*yearWeight),
      TableRow("3 jet, resolved, 400 $<$ MET",               WHLeptons==1&&LeadingNonBJetPt_med<300&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",              0,0,"weight*trig_eff"*nanoWeight*yearWeight),

      TableRow("3 jet, boosted, 125 $<$ MET $<$ 300",        WHLeptons==1&&LeadingNonBJetPt_med<300&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",   0,0,"weight*trig_eff"*nanoWeight*yearWeight),
      TableRow("3 jet, boosted, 300 $<$ MET",                WHLeptons==1&&LeadingNonBJetPt_med<300&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",              0,0,"weight*trig_eff"*nanoWeight*yearWeight)

        }, sample_list_comb,true);


if(single_thread) pmcomb->multithreaded_ = false;
  pmcomb->MakePlots(lumicomb);



  vector<GammaParams> yields_signalRegionYields_tot_comb = signalRegionYields_tot_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegionYields_tot_comb){
    cout << yield << endl;
  }

  
}
