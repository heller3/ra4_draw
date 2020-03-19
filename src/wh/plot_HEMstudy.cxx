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
  
  double lumi2018 = 59.7;

  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_Run2018_v33_4_2019_12_30/";

  Palette colors("txt/colors.txt", "default");

  //Data
  auto data2018 = Process::MakeShared<Baby_full>("2018 Data, HEM events only", Process::Type::data, colors("data"),{data2018_dir+"slim_data_2018*.root"},"pass"&&HasHEMevent>0.);
  auto data2018_noHEM = Process::MakeShared<Baby_full>("2018 Data, no HEM events", Process::Type::background, colors("t1tttt"),{data2018_dir+"slim_data_2018*.root"},"pass"&&HasHEMevent==0.);
  auto data2018_noHEMlep_HEMjet = Process::MakeShared<Baby_full>("2018 Data, HEM jet", Process::Type::data, colors("data"),{data2018_dir+"slim_data_2018*.root"},"pass"&&HasHEMevent==0.&&HasHEMjet>0.);
  auto data2018_noHEMlep_noHEMjet = Process::MakeShared<Baby_full>("2018 Data, no HEM jets", Process::Type::background, colors("t1tttt"),{data2018_dir+"slim_data_2018*.root"},"pass"&&HasHEMevent==0.&&HasHEMjet==0.);
  //auto data2018_noHEM = Process::MakeShared<Baby_full>("2018 Data, no HEM events", Process::Type::background, colors("t1tttt"),{data2018_dir+"*slim_data_2018*.root"},"pass&&nvetoleps>=2&&(((vetoleps_pdgid[1]==11||vetoleps_pdgid[1]==-11)&&(vetoleps_phi[1]<-1.6||vetoleps_phi[1]>-0.8)&&(vetoleps_eta[1]>-1.4))||(vetoleps_pdgid[1]==13||vetoleps_pdgid[1]==-13))");
  //auto data2018_noHEM = Process::MakeShared<Baby_full>("2018 Data, no HEM events", Process::Type::background, colors("t1tttt"),{data2018_dir+"*slim_data_2018*.root"},"pass&&nvetoleps>=2");

  vector<shared_ptr<Process> > sample_list_2018 = {data2018,data2018_noHEM};
  vector<shared_ptr<Process> > sample_list_2018_jets = {data2018_noHEMlep_HEMjet,data2018_noHEMlep_noHEMjet};


  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm)
    .RatioMinimum(0.5).RatioMaximum(1.5);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    //.Bottom(BottomType::off)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info, log_shapes_info, lin_shapes_info};

  //Need different PlotMaker for each luminosity.
    //For some reason, they needed to be pointers to go into vector later on..
  PlotMaker * pm2018 = new PlotMaker();

  //Preselection
  NamedFunc preselection = "run>=319077&&nvetoleps==2&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && WHLeptons==1;
  NamedFunc tt2l = "run>=319077&&nvetoleps==3&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125" && WHLeptons==1;
  NamedFunc tt1l = "run>=319077&&nvetoleps==2&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125" && WHLeptons==1;

 /* NamedFunc ttbar_sel = "pass&&ngoodjets>=4&&pfmet>125&&ngoodbtags>=1" && WHLeptons==1;
  NamedFunc ttbar_sel_full_eff = "leps_pt[0]>50&&pfmet>250&&pass&&ngoodjets>=4&&ngoodbtags>=1" && WHLeptons==1;

  NamedFunc ttbar_1l_sel = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125&&ngoodbtags>=1" && WHLeptons==1;
  NamedFunc ttbar_1l_sel_full_eff = "leps_pt[0]>50&&pfmet>250&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&ngoodbtags>=1" && WHLeptons==1;

  NamedFunc wjets_sel = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50 && ngoodbtags==0" && WHLeptons==1;
  NamedFunc wjets_sel_full_eff= "leps_pt[0]>50&&pfmet>250&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>50 && ngoodbtags==0" && WHLeptons==1;

  NamedFunc ttbar_2l_sel = "pass&&nvetoleps==2&&ngoodjets==2&&pfmet>125&&ngoodbtags>=1" && WHLeptons>=1;
  NamedFunc ttbar_2l_sel_full_eff = "leps_pt[0]>50&&pfmet>250&&pass&&nvetoleps==2&&ngoodjets==2&&ngoodbtags>=1" && WHLeptons>=1;*/

  pm2018->Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "mT [GeV]"),
    preselection, sample_list_2018_jets, all_plot_types).Weight("weight * w_pu").Tag("y2018_HEM_preselection_"); 

  pm2018->Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "mT [GeV]"),
    tt2l, sample_list_2018_jets, all_plot_types).Weight("weight * w_pu").Tag("y2018_HEM_tt2l_"); 

  pm2018->Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "mT [GeV]"),
    tt1l, sample_list_2018_jets, all_plot_types).Weight("weight * w_pu").Tag("y2018_HEM_tt1l_"); 

  /*vector<NamedFunc> sels = {preselection}; 
  vector<PlotMaker *> pms = {pm2018};
  vector<vector<shared_ptr<Process> >> samples_Run2 = {sample_list_2018};
  vector<string> years = {"y2018"};
  vector<string> weights = {"weight","weight * w_pu"};

  for(uint isel=0;isel<sels.size();isel++){
    for(uint iyear=0;iyear<samples_Run2.size();iyear++){
      for(uint iweight=0;iweight<weights.size();iweight++){


        pms[iyear]->Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "mT [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]);  

      }
    }
  }*/

  pm2018->MakePlots(lumi2018);

}
