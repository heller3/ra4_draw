
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

  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_mc_s16v3_v33_4_2019_12_30/";
  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_mc_f17v2_v33_4_2019_12_30/";
  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_mc_a18v1_v33_4_2019_12_30/";

  string data2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2019_12_30/";
  string data2017_dir = "/home/users/dspitzba/wh_babies/babies_Run2017_v33_4_2019_12_30/";
  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_Run2018_v33_4_2019_12_30/";

  string signal2016_dir = "/home/users/dspitzba/wh_babies/babies_signal_s16v3_v33_4_2019_12_30/";
  string signal2017_dir = "/home/users/dspitzba/wh_babies/babies_signal_f17v2_v33_4_2019_12_30/";
  string signal2018_dir = "/home/users/dspitzba/wh_babies/babies_signal_a18v1_v33_4_2019_12_30/";

  /*string mc2016_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/s16v3/";
  string mc2017_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/f17v2/";
  string mc2018_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/a18v1/";

  string data2016_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/";
  string data2017_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/";
  string data2018_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/";

  string signal2016_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/";
  string signal2017_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/";
  string signal2018_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/";*/

  /*string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname,"compute-")){
    signal_dir = "/net/cms29";
  }*/
 
  Palette colors("txt/colors.txt", "default");

  //top background
  auto top_comb = Process::MakeShared<Baby_full>("top backgrounds", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",
                                                                                                                mc2016_dir+"slim*TTJets_1lep_tbar_*",
                                                                                                                mc2017_dir+"slim*TTJets_1lep_top_*.root",
                                                                                                                mc2017_dir+"slim*TTJets_1lep_tbar_*",
                                                                                                                mc2018_dir+"slim*TTJets_1lep_top_*.root",
                                                                                                                mc2018_dir+"slim*TTJets_1lep_tbar_*",
                                                                                                                mc2016_dir+"slim*TTJets_2lep_*.root",
                                                                                                                mc2017_dir+"slim*TTJets_2lep_*.root",
                                                                                                                mc2018_dir+"slim*TTJets_2lep_*.root",
                                                                                                                mc2016_dir+"slim*_ST_*.root",
                                                                                                                mc2017_dir+"slim*_ST_*.root",
                                                                                                                mc2018_dir+"slim*_ST_*.root",
                                                                                                                mc2016_dir+"slim*_TTWJets*.root",
                                                                                                                mc2016_dir+"slim*_TTZ*.root",
                                                                                                                mc2017_dir+"slim*_TTWJets*.root",
                                                                                                                mc2017_dir+"slim*_TTZ*.root",
                                                                                                                mc2018_dir+"slim*_TTWJets*.root",
                                                                                                                mc2018_dir+"slim*_TTZ*.root"},"stitch");

  //W background
  auto W_comb = Process::MakeShared<Baby_full>("W+jets background", Process::Type::background, colors("wjets"),{mc2016_dir+"slim*_W*JetsToLNu_s16v3*",
                                                                                                                mc2016_dir+"slim*W*Jets_NuPt200_s16v*.root",
                                                                                                                mc2017_dir+"slim*_W*JetsToLNu_f17v2*",
                                                                                                                mc2017_dir+"slim*W*Jets_NuPt200_f17v2*.root",
                                                                                                                mc2018_dir+"slim_W*JetsToLNu_a18v1*",
                                                                                                                mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root",
                                                                                                                mc2016_dir+"slim*WW*.root",
                                                                                                                mc2016_dir+"slim*WZ*.root",
                                                                                                                mc2016_dir+"slim*ZZ*.root",
                                                                                                                mc2017_dir+"slim*WW*.root",
                                                                                                                mc2017_dir+"slim*WZ*.root",
                                                                                                                mc2017_dir+"slim*ZZ*.root",
                                                                                                                mc2018_dir+"slim*WW*.root",
                                                                                                                mc2018_dir+"slim*WZ*.root",
                                                                                                                mc2018_dir+"slim*ZZ*.root"},"stitch");
  
  //signals

  auto signal_comb_650_300 = Process::MakeShared<Baby_full>("2016-2018 Signal (650,300)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root",signal2017_dir+"slim_*.root",signal2018_dir+"slim_*.root"},"pass&&mass_stop==650&&mass_lsp==300");
    
  auto signal_comb_700_1 = Process::MakeShared<Baby_full>("Combined Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root",signal2017_dir+"slim_*.root",signal2018_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");

  vector<shared_ptr<Process> > sample_list_comb = {top_comb,W_comb,signal_comb_700_1,signal_comb_650_300};

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


    Table & signalRegion_resolved_cutflow_comb = pmcomb->Push<Table>("signalRegion_resolved_cutflow_comb", vector<TableRow>{
      TableRow("125$<$MET$<$200",WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2"&&max_ak8pfjets_deepdisc_hbb<=0.8,0,0,"weight*w_pu"*yearWeight),
      TableRow("200$<$MET$<$300",WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2"&&max_ak8pfjets_deepdisc_hbb<=0.8,0,0,"weight*w_pu"*yearWeight),
      TableRow("300$<$MET$<$400",WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2"&&max_ak8pfjets_deepdisc_hbb<=0.8,0,0,"weight*w_pu"*yearWeight),
      TableRow("400$<$MET",WHLeptons==1&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2"&&max_ak8pfjets_deepdisc_hbb<=0.8,0,0,"weight*w_pu"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_loose3jet_resolved_cutflow_comb = pmcomb->Push<Table>("signalRegion_loose3jet_resolved_cutflow_comb", vector<TableRow>{
      TableRow("125$<$MET$<$200",WHLeptons==1&&LeadingNonBJetPt_med<300&&max_ak8pfjets_deepdisc_hbb<=0.8&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2",0,0,"weight*w_pu"*yearWeight),
      TableRow("200$<$MET$<$300",WHLeptons==1&&LeadingNonBJetPt_med<300&&max_ak8pfjets_deepdisc_hbb<=0.8&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2",0,0,"weight*w_pu"*yearWeight),
      TableRow("300$<$MET$<$400",WHLeptons==1&&LeadingNonBJetPt_med<300&&max_ak8pfjets_deepdisc_hbb<=0.8&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2",0,0,"weight*w_pu"*yearWeight),
      TableRow("400$<$MET",WHLeptons==1&&LeadingNonBJetPt_med<300&&max_ak8pfjets_deepdisc_hbb<=0.8&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2",0,0,"weight*w_pu"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_boosted_cutflow_comb = pmcomb->Push<Table>("signalRegion_boosted_cutflow_comb", vector<TableRow>{
      TableRow("125$<$MET$<$300",WHLeptons==1&&max_ak8pfjets_deepdisc_hbb>0.8&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2",0,0,"weight*w_pu"*yearWeight),
      TableRow("300$<$MET",WHLeptons==1&&max_ak8pfjets_deepdisc_hbb>0.8&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2",0,0,"weight*w_pu"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_3jet_boosted_cutflow_comb = pmcomb->Push<Table>("signalRegion_3jet_boosted_cutflow_comb", vector<TableRow>{
      TableRow("125$<$MET$<$300",WHLeptons==1&&max_ak8pfjets_deepdisc_hbb>0.8&&LeadingNonBJetPt_med<300&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2",0,0,"weight*w_pu"*yearWeight),
      TableRow("300$<$MET",WHLeptons==1&&max_ak8pfjets_deepdisc_hbb>0.8&&LeadingNonBJetPt_med<300&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2",0,0,"weight*w_pu"*yearWeight)

        }, sample_list_comb,true);


if(single_thread) pmcomb->multithreaded_ = false;
  pmcomb->MakePlots(lumicomb);



  vector<GammaParams> yields_signalRegion_resolved_comb = signalRegion_resolved_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_resolved_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signalRegion_loose3jet_resolved_comb = signalRegion_loose3jet_resolved_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_loose3jet_resolved_comb){
    cout << yield << endl;
  }
  
  vector<GammaParams> yields_signalRegion_boosted_comb = signalRegion_boosted_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_boosted_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signalRegion_3jet_boosted_comb = signalRegion_3jet_boosted_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_3jet_boosted_comb){
    cout << yield << endl;
  }

  
}
