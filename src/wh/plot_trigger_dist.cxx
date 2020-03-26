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
  
  double lumi = 35.9;

  string jetHT2016_dir = "/home/users/rheller/wh_babies/babies_v30_3_2019_09_13/slim_data*2016*jetht*.root";
  string jetHT2017_dir = "/home/users/rheller/wh_babies/babies_v30_3_2019_09_13/slim_data*2017*jetht*.root";
  string jetHT2018_dir = "/home/users/rheller/wh_babies/babies_v30_3_2019_09_13/slim_data*2018*jetht*.root";


  Palette colors("txt/colors.txt", "default");

  //Define processes
  auto jetHT2016 = Process::MakeShared<Baby_full>("2016 JetHT", Process::Type::background, colors("wjets"),{jetHT2016_dir},"pass");
  auto jetHT2017 = Process::MakeShared<Baby_full>("2017 JetHT", Process::Type::background, colors("single_t"),{jetHT2017_dir},"pass");
  auto jetHT2018 = Process::MakeShared<Baby_full>("2018 JetHT", Process::Type::background, colors("other"),{jetHT2018_dir},"pass");


  //List of processes
  vector<shared_ptr<Process> > sample_list_jetHT= {jetHT2016,jetHT2017,jetHT2018};


  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm)
    .RatioMinimum(0.5).RatioMaximum(1.5);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .Bottom(BottomType::off)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info/*lin_shapes_info*/};

  //Need different PlotMaker for each luminosity.
    //For some reason, they needed to be pointers to go into vector later on..
  PlotMaker * pm = new PlotMaker();

  //Selection
  TString basecuts= "(ngoodjets==2||ngoodjets==3)&&ngoodleps==1&&PassTrackVeto&&PassTauVeto&&pass==1";
  TString ElorMu= "(abs(leps_pdgid[0])==11)||(abs(leps_pdgid[0])==13&&abs(leps_eta[0])<2.1)";

  NamedFunc unprescaled = basecuts+"&&"+ElorMu+"&&(HLT_PFHT_unprescaled==1)";
  NamedFunc prescaled = basecuts+"&&"+ElorMu+"&&(HLT_PFHT_prescaled==1)";

  vector<NamedFunc> sels = {unprescaled, prescaled}; 
  vector<string> weights = {"weight","weight * w_pu"};

  for(uint isel=0;isel<sels.size();isel++){
    for(uint iweight=0;iweight<weights.size();iweight++){

        pm->Push<Hist1D>(Axis(15, 125, 500., elPt, "electron p_{T} [GeV]"),
          sels[isel], sample_list_jetHT, all_plot_types).Weight(weights[iweight]); 
        
        pm->Push<Hist1D>(Axis(15, 125, 500., muPt, "muon p_{T} [GeV]"),
          sels[isel], sample_list_jetHT, all_plot_types).Weight(weights[iweight]);

        pm->Push<Hist1D>(Axis(15, 125, 500., "pfmet", "MET [GeV]"),
          sels[isel], sample_list_jetHT, all_plot_types).Weight(weights[iweight]);

        pm->Push<Hist1D>(Axis(15, 125, 500., mht, "mht [GeV]"),
          sels[isel], sample_list_jetHT, all_plot_types).Weight(weights[iweight]);  

    
    } //loop over weights
  } //loop over selections

  pm->MakePlots(lumi);

}
