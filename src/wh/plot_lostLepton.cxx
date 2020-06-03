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
  
  /*double lumi2016 = 35.9;
  double lumi2017 = 41.6;
  double lumi2018 = 59.7;*/
  double lumicomb = 137.2;

  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_mc_s16v3_v33_4_2019_12_30/";
  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_mc_f17v2_v33_4_2019_12_30/";
  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_mc_a18v1_v33_4_2019_12_30/";

  Palette colors("txt/colors.txt", "default");

  //ttbar
  auto tt2l_Comb_lostLight = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016-2018, lost e or mu", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root"},nGenLightLeps==2&&"stitch");
  auto tt2l_Comb_lostHadTau = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016-2018, lost hadronic tau", Process::Type::background, colors("single_t"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root"},LostHadTaus==1&&"stitch");
  auto tt2l_Comb_lostLep = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016-2018, lost lepton", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root"},(nGenLightLeps==2||LostHadTaus==1)&&"stitch");

  //Single top
  auto single_t_Comb_lostLight = Process::MakeShared<Baby_full>("Single t 2016-2018, lost e or mu", Process::Type::background, colors("wjets"), {mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2);
  auto single_t_Comb_lostHadTau = Process::MakeShared<Baby_full>("Single t 2016-2018, lost hadronic tau", Process::Type::background, colors("dy"), {mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},LostHadTaus==1);
  auto single_t_Comb_lostLep = Process::MakeShared<Baby_full>("Single t 2016-2018, lost lepton", Process::Type::background, colors("wjets"), {mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},(nGenLightLeps==2||LostHadTaus==1));


  auto Comb_lostLight = Process::MakeShared<Baby_full>("t#bar{t} (2l) & ST 2016-2018, lost e or mu", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&"stitch");
  auto Comb_lostHadTau = Process::MakeShared<Baby_full>("t#bar{t} (2l) & ST 2016-2018, lost hadronic tau", Process::Type::background, colors("wjets"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},LostHadTaus==1&&"stitch");

  //List of processes

  vector<shared_ptr<Process> > sample_list_Comb = {tt2l_Comb_lostLight,tt2l_Comb_lostHadTau,single_t_Comb_lostLight,single_t_Comb_lostHadTau};
  vector<shared_ptr<Process> > sample_list_lightortau = {Comb_lostLight,Comb_lostHadTau};
  vector<shared_ptr<Process> > sample_list_tt2lorst = {tt2l_Comb_lostLep,single_t_Comb_lostLep};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    //.Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm)
    .RatioMinimum(0).RatioMaximum(2.0);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    //.Bottom(BottomType::ratio)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info, log_shapes_info};

  //Need different PlotMaker for each luminosity.
    //For some reason, they needed to be pointers to go into vector later on..
  /*PlotMaker * pm2016 = new PlotMaker();
  PlotMaker * pm2017 = new PlotMaker();
  PlotMaker * pm2018 = new PlotMaker();*/
  PlotMaker * pmComb = new PlotMaker();
  PlotMaker * pmlightortau = new PlotMaker();
  PlotMaker * pmtt2lorst = new PlotMaker();

  //Selections
  NamedFunc signalRegion = "pass&&nvetoleps==1&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&ngoodbtags==2&&PassTrackVeto&&PassTauVeto" && WHLeptons==1;

  vector<NamedFunc> sels   = {signalRegion}; 
  vector<string> sels_tags = {"_signalRegion"}; 
  vector<PlotMaker *> pms = {/*pm2016,pm2017,pm2018,*/pmComb,pmlightortau,pmtt2lorst};
  vector<vector<shared_ptr<Process> > > samples_Run2 = {/*sample_list_2016,sample_list_2017,sample_list_2018,*/sample_list_Comb,sample_list_lightortau,sample_list_tt2lorst};
  vector<string> years = {/*"y2016","y2017","y2018",*/"yAll","yAll_lightortau","yAll_tt2lorst"};
  vector<NamedFunc> weights = {"weight * w_pu"*yearWeight*mcHEMWeight};

  for(uint isel=0;isel<sels.size();isel++){
    for(uint iyear=0;iyear<samples_Run2.size();iyear++){
      for(uint iweight=0;iweight<weights.size();iweight++){

          pms[iyear]->Push<Hist1D>(Axis(10, 0, 50., ptLostLeps, "Lost lepton p_{T} [GeV]"),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("coarse_"+years[iyear]+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(16, -4.0, 4.0, etaLostLeps, "Lost lepton #eta [GeV]"),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("coarse_"+years[iyear]+sels_tags[isel]);

          pms[iyear]->Push<Hist1D>(Axis(40, 0, 200., ptLostLeps, "Lost lepton p_{T} [GeV]"),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("coarse_extended_"+years[iyear]+sels_tags[isel]);
          

      }
    }
  }

  /*pm2016->MakePlots(lumi2016);
  pm2017->MakePlots(lumi2017);
  pm2018->MakePlots(lumi2018);*/
  pmComb->MakePlots(lumicomb);
  pmlightortau->MakePlots(lumicomb);
  pmtt2lorst->MakePlots(lumicomb);


}