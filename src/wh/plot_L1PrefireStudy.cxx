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

  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_mc_s16v3_v33_4_2019_12_30/";
  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_mc_f17v2_v33_4_2019_12_30/";
  string signal2016_dir = "/home/users/dspitzba/wh_babies/babies_signal_s16v3_v33_4_2019_12_30/";
  string signal2017_dir = "/home/users/dspitzba/wh_babies/babies_signal_f17v2_v33_4_2019_12_30/";

  Palette colors("txt/colors.txt", "default");

  //Samples
  auto mc_noweight_2016 = Process::MakeShared<Baby_full>("2016 MC w/o weights", Process::Type::background, colors("data"),{mc2016_dir+"slim_*.root"},"pass");
  auto mc_noweight_2017 = Process::MakeShared<Baby_full>("2017 MC w/o weights", Process::Type::background, colors("data"),{mc2017_dir+"slim_*.root"},"pass");
  auto mc_weight_2016 = Process::MakeShared<Baby_full>("2016 MC w/ weights", Process::Type::background, colors("t1tttt"),{mc2016_dir+"slim_*.root"},"pass");
  auto mc_weight_2017 = Process::MakeShared<Baby_full>("2017 MC w/ weights", Process::Type::background, colors("t1tttt"),{mc2017_dir+"slim_*.root"},"pass");

  auto signal_noweight_2016_225_75 = Process::MakeShared<Baby_full>("2016 Signal (225,75) w/o weights", Process::Type::background, colors("data"),{signal2016_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
  auto signal_noweight_2017_225_75 = Process::MakeShared<Baby_full>("2017 Signal (225,75) w/o weights", Process::Type::background, colors("data"),{signal2017_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
  auto signal_weight_2016_225_75 = Process::MakeShared<Baby_full>("2016 Signal (225,75) w/ weights", Process::Type::background, colors("t1tttt"),{signal2016_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
  auto signal_weight_2017_225_75 = Process::MakeShared<Baby_full>("2017 Signal (225,75) w/ weights", Process::Type::background, colors("t1tttt"),{signal2017_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");

  auto signal_noweight_2016_700_1 = Process::MakeShared<Baby_full>("2016 Signal (700,1) w/o weights", Process::Type::background, colors("data"),{signal2016_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_noweight_2017_700_1 = Process::MakeShared<Baby_full>("2017 Signal (700,1) w/o weights", Process::Type::background, colors("data"),{signal2017_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_weight_2016_700_1 = Process::MakeShared<Baby_full>("2016 Signal (700,1) w/ weights", Process::Type::background, colors("t1tttt"),{signal2016_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_weight_2017_700_1 = Process::MakeShared<Baby_full>("2017 Signal (700,1) w/ weights", Process::Type::background, colors("t1tttt"),{signal2017_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");

  vector<shared_ptr<Process> > sample_list_2016_weight   = {mc_weight_2016};
  vector<shared_ptr<Process> > sample_list_2016_noweight = {mc_noweight_2016};
  vector<shared_ptr<Process> > sample_list_2017_weight   = {mc_weight_2017};
  vector<shared_ptr<Process> > sample_list_2017_noweight = {mc_noweight_2017};

  vector<shared_ptr<Process> > sample_list_2016_weight_225_75   = {signal_weight_2016_225_75};
  vector<shared_ptr<Process> > sample_list_2016_noweight_225_75 = {signal_noweight_2016_225_75};
  vector<shared_ptr<Process> > sample_list_2017_weight_225_75   = {signal_weight_2017_225_75};
  vector<shared_ptr<Process> > sample_list_2017_noweight_225_75 = {signal_noweight_2017_225_75};

  vector<shared_ptr<Process> > sample_list_2016_weight_700_1   = {signal_weight_2016_700_1};
  vector<shared_ptr<Process> > sample_list_2016_noweight_700_1 = {signal_noweight_2016_700_1};
  vector<shared_ptr<Process> > sample_list_2017_weight_700_1   = {signal_weight_2017_700_1};
  vector<shared_ptr<Process> > sample_list_2017_noweight_700_1 = {signal_noweight_2017_700_1};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    //.Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm)
    .RatioMinimum(0.5).RatioMaximum(1.5)
    .FileExtensions({"pdf","root"});
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear).FileExtensions({"pdf","root"});
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    //.Bottom(BottomType::off)
    .ShowBackgroundError(false)
    .FileExtensions({"pdf","root"});
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear).FileExtensions({"pdf","root"});
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info).FileExtensions({"pdf","root"});
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info).FileExtensions({"pdf","root"});
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info).FileExtensions({"pdf","root"});
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info).FileExtensions({"pdf","root"});
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info, log_shapes_info, lin_shapes_info};

  //Need different PlotMaker for each luminosity.
    //For some reason, they needed to be pointers to go into vector later on..
  PlotMaker * pm2016 = new PlotMaker();
  PlotMaker * pm2017 = new PlotMaker();

  //Preselection
  NamedFunc preselection = "nvetoleps==2&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && WHLeptons==1;
  NamedFunc tt2l         = "nvetoleps==3&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125" && WHLeptons==1;
  NamedFunc tt1l         = "nvetoleps==2&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125" && WHLeptons==1;


  /*pm2018->Push<Hist1D>(Axis(25, 0, 500., "pfmet", "MET [GeV]"),
    preselection, sample_list_2016, all_plot_types).Weight("weight * w_pu").Tag("y2016_L1Study_preselection_"); 

  pm2018->Push<Hist1D>(Axis(25, 0, 500., "pfmet", "MET [GeV]"),
    tt2l, sample_list_2016, all_plot_types).Weight("weight * w_pu").Tag("y2016_L1Study_tt2l_"); 

  pm2018->Push<Hist1D>(Axis(25, 0, 500., "pfmet", "MET [GeV]"),
    tt1l, sample_list_2016, all_plot_types).Weight("weight * w_pu").Tag("y2016_L1Study_tt1l_"); 

  pm2018->Push<Hist1D>(Axis(25, 0, 500., "pfmet", "MET [GeV]"),
    preselection, sample_list_2017, all_plot_types).Weight("weight * w_pu").Tag("y2017_L1Study_preselection_"); 

  pm2018->Push<Hist1D>(Axis(25, 0, 500., "pfmet", "MET [GeV]"),
    tt2l, sample_list_2017, all_plot_types).Weight("weight * w_pu").Tag("y2017_L1Study_tt2l_"); 

  pm2018->Push<Hist1D>(Axis(25, 0, 500., "pfmet", "MET [GeV]"),
    tt1l, sample_list_2017, all_plot_types).Weight("weight * w_pu").Tag("y2017_L1Study_tt1l_"); */

  vector<NamedFunc> sels = {preselection,tt1l,tt2l}; 
  vector<PlotMaker *> pms = {pm2016,pm2017};
  vector<vector<shared_ptr<Process> >> samples_Run2 = {sample_list_2016_weight,
                                                       sample_list_2016_noweight,
                                                       sample_list_2017_weight,
                                                       sample_list_2017_noweight,
                                                       sample_list_2016_weight_225_75,
                                                       sample_list_2016_noweight_225_75,
                                                       sample_list_2017_weight_225_75,
                                                       sample_list_2017_noweight_225_75,
                                                       sample_list_2016_weight_700_1,
                                                       sample_list_2016_noweight_700_1,
                                                       sample_list_2017_weight_700_1,
                                                       sample_list_2017_noweight_700_1};
  vector<string> years = {"y2016_","y2017_"};
  vector<string> selectionTags = {"preselection_","tt1l_","tt2l_"};
  vector<string> weights = {"weight","weight * w_pu"};
  vector<string> processWeights = {"1","1/w_L1","1","1/w_L1","1","1/w_L1","1","1/w_L1","1","1/w_L1","1","1/w_L1"};
  vector<string> processTags = {"","","","","signal_225_75_","signal_225_75_","signal_225_75_","signal_225_75_","signal_700_1_","signal_700_1_","signal_700_1_","signal_700_1_"};

  for(uint isel=0;isel<sels.size();isel++){
    for(uint iyear=0;iyear<pms.size();iyear++){
      for(uint iproc=0;iproc<samples_Run2.size();iproc++){
        for(uint iweight=0;iweight<weights.size();iweight++){


          pms[iyear]->Push<Hist1D>(Axis(25, 0, 500., "pfmet", "MET [GeV]"),
            sels[isel], samples_Run2[iproc], all_plot_types).Weight(weights[iweight]*processWeights[iproc]).Tag(years[iyear]+"L1Study_"+processTags[iproc]+selectionTags[isel]);

          pms[iyear]->Push<Hist1D>(Axis(25, 0, 500., "mct", "MCT [GeV]"),
            sels[isel], samples_Run2[iproc], all_plot_types).Weight(weights[iweight]*processWeights[iproc]).Tag(years[iyear]+"L1Study_"+processTags[iproc]+selectionTags[isel]);

          pms[iyear]->Push<Hist1D>(Axis(25, 0, 500., "mbb", "Mbb [GeV]"),
            sels[isel], samples_Run2[iproc], all_plot_types).Weight(weights[iweight]*processWeights[iproc]).Tag(years[iyear]+"L1Study_"+processTags[iproc]+selectionTags[isel]);

          pms[iyear]->Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "MT [GeV]"),
            sels[isel], samples_Run2[iproc], all_plot_types).Weight(weights[iweight]*processWeights[iproc]).Tag(years[iyear]+"L1Study_"+processTags[iproc]+selectionTags[isel]);  
        }
      }
    }
  }

  pm2016->MakePlots(lumi2016);
  pm2017->MakePlots(lumi2017);

}
