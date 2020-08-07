
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
  
  string mc2018_ttbar_ext_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_06_12/a18v1/";
  
  string signal_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_07_09/";

  /*string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname,"compute-")){
    signal_dir = "/net/cms29";
  }*/
 
  Palette colors("txt/colors.txt", "default");

  //Top backgrounds only
  auto top_Comb = Process::MakeShared<Baby_full>("top 2016-2018", Process::Type::background, colors("ttv"), {mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*",mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*",mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_a18v1*.root",mc2018_ttbar_ext_dir+"slim*",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root",mc2016_dir+"slim*_TTWJets*.root", mc2016_dir+"slim*_TTZ*.root",mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZ*.root",mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"},"stitch");

  //non-top backgrounds only
  auto nontop_Comb = Process::MakeShared<Baby_full>("non-top 2016-2018", Process::Type::background, colors("ttv"), {mc2016_dir+"slim*WW*.root", mc2016_dir+"slim*WZ*.root",mc2016_dir+"slim*ZZ*.root",mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*WZ*.root",mc2017_dir+"slim*ZZ*.root",mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root",mc2016_dir+"slim*_W*JetsToLNu_s16v3*",mc2016_dir+"slim*W*Jets_NuPt200_s16v*.root",mc2017_dir+"slim_W*JetsToLNu_f17v2*",mc2017_dir+"slim*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root"},"stitch");

  //signals

  auto signal_comb_800_1 = Process::MakeShared<Baby_full>("2016-2018 Signal (800,1)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_SMS_TChiWH*.root"},"pass&&mass_stop==800&&mass_lsp==1");
  auto signal_comb_400_150 = Process::MakeShared<Baby_full>("2016-2018 Signal (450,150)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_SMS_TChiWH*.root"},"pass&&mass_stop==400&&mass_lsp==150");
  auto signal_comb_225_75 = Process::MakeShared<Baby_full>("2016-2018 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_SMS_TChiWH*.root"},"pass&&mass_stop==225&&mass_lsp==75");

  vector<shared_ptr<Process> > signal_list_comb = {signal_comb_800_1,signal_comb_400_150,signal_comb_225_75};
  vector<shared_ptr<Process> > sample_list_comb = {top_Comb,nontop_Comb,signal_comb_800_1,signal_comb_400_150,signal_comb_225_75};

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


    Table & signalRegion_muon_eff_cutflow_comb = pmcomb->Push<Table>("signalRegion_muon_eff_cutflow_comb", vector<TableRow>{
      TableRow("|eta| $<=$ 1.442",           WHMuonSigEff[1.]==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("2.1 $<$ |eta|",              WHMuonSigEff[2.]==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),

        }, sample_list_comb,true);


    Table & signalRegion_ele_eff_cutflow_comb = pmcomb->Push<Table>("signalRegion_ele_eff_cutflow_comb", vector<TableRow>{
      TableRow("|eta| $<=$ 1.442",           WHElSigEff[1.]==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("2.1 $<$ |eta|",              WHElSigEff[2.]==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),

        }, sample_list_comb,true);


    /*Table & lowMCTControlRegion_2jet_resolved_contamination_cutflow_comb = pmcomb->Push<Table>("lowMCTControlRegion_2jet_resolved_contamination_cutflow_comb", vector<TableRow>{
      TableRow("125-200",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("200-300",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("300-400",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<300&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("over400",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<300&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),

        }, sample_list_comb,true);


    Table & lowMCTControlRegion_3jet_resolved_contamination_cutflow_comb = pmcomb->Push<Table>("lowMCTControlRegion_3jet_resolved_contamination_cutflow_comb", vector<TableRow>{
      TableRow("125-200",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("200-300",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("300-400",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<300&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("over400",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<300&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),

        }, sample_list_comb,true);


    Table & lowMCTControlRegion_2jet_boosted_contamination_cutflow_comb = pmcomb->Push<Table>("lowMCTControlRegion_2jet_boosted_contamination_cutflow_comb", vector<TableRow>{
      TableRow("125-300",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("over300",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<300&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),

        }, sample_list_comb,true);


    Table & lowMCTControlRegion_3jet_boosted_contamination_cutflow_comb = pmcomb->Push<Table>("lowMCTControlRegion_3jet_boosted_contamination_cutflow_comb", vector<TableRow>{
      TableRow("125-300",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("over300",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>100&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<300&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),

        }, sample_list_comb,true);*/



if(single_thread) pmcomb->multithreaded_ = false;
  pmcomb->MakePlots(lumicomb);



  vector<GammaParams> yields_signalRegion_muon_eff_comb = signalRegion_muon_eff_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_muon_eff_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signalRegion_ele_eff_comb = signalRegion_ele_eff_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_ele_eff_comb){
    cout << yield << endl;
  }

  /*vector<GammaParams> yields_lowMCTControlRegion_2jet_resolved_contamination_comb = lowMCTControlRegion_2jet_resolved_contamination_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_lowMCTControlRegion_2jet_resolved_contamination_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_lowMCTControlRegion_3jet_resolved_contamination_comb = lowMCTControlRegion_3jet_resolved_contamination_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_lowMCTControlRegion_3jet_resolved_contamination_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_lowMCTControlRegion_2jet_boosted_contamination_comb = lowMCTControlRegion_2jet_boosted_contamination_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_lowMCTControlRegion_2jet_boosted_contamination_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_lowMCTControlRegion_3jet_boosted_contamination_comb = lowMCTControlRegion_3jet_boosted_contamination_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_lowMCTControlRegion_3jet_boosted_contamination_comb){
    cout << yield << endl;
  }*/
  
}
