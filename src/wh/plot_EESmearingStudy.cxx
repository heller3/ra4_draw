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
  
  double lumi2017 = 41.6;
  double lumi2018 = 59.7;

  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_Run2018_v33_4_2019_12_30/";
  string data2017_dir = "/home/users/dspitzba/wh_babies/babies_Run2017_v33_4_2019_12_30/";

  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_mc_a18v1_v33_4_2019_12_30/";
  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_mc_f17v2_v33_4_2019_12_30/";

  Palette colors("txt/colors.txt", "default");

  //Data
  auto data2018 = Process::MakeShared<Baby_full>("2018 Data", Process::Type::data, colors("data"),{data2018_dir+"slim_data_2018*.root"},"pass");
  auto data2017 = Process::MakeShared<Baby_full>("2017 Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017*.root"},"pass");
  
  auto data2017B = Process::MakeShared<Baby_full>("2017B Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017B*.root"},"pass");
  auto data2017C = Process::MakeShared<Baby_full>("2017C Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017C*.root"},"pass");
  auto data2017D = Process::MakeShared<Baby_full>("2017D Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017D*.root"},"pass");
  auto data2017E = Process::MakeShared<Baby_full>("2017E Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017E*.root"},"pass");
  auto data2017F = Process::MakeShared<Baby_full>("2017F Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017F*.root"},"pass");

  auto mc_2017 = Process::MakeShared<Baby_full>("MC 2017", Process::Type::background, colors("t1tttt"),{mc2017_dir+"slim_*.root","pass"});

  /*auto tt1l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2017", Process::Type::background, colors("tt_1l"),
    {mc2017_dir+"slim*TTJets_1lep_top_f17v2*.root",mc2017_dir+"slim*TTJets_1lep_tbar_f17v2*.root"},"pass");
  auto tt2l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2017", Process::Type::background, colors("tt_2l"),
    {mc2017_dir+"slim*TTJets_2lep_f17v2*.root"},"pass&&stitch");
  auto tt1l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2018", Process::Type::background, colors("tt_1l"),
    {mc2018_dir+"slim*TTJets_1lep_top_a18v1*.root",mc2018_dir+"slim*TTJets_1lep_tbar_a18v1*.root"},"pass");
  auto tt2l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2018", Process::Type::background, colors("tt_2l"),
    {mc2018_dir+"slim*TTJets_2lep_a18v1*.root"},"pass&&stitch");

  auto wjets_2017 = Process::MakeShared<Baby_full>("W+jets 2017", Process::Type::background, colors("wjets"), {mc2017_dir+"slim_W*JetsToLNu_f17v2*.root"},"pass&&stitch");
  auto wjets_2018 = Process::MakeShared<Baby_full>("W+jets 2018", Process::Type::background, colors("wjets"), {mc2018_dir+"slim_W*JetsToLNu_a18v1*.root"},"pass&&stitch");

  auto single_t_2017 = Process::MakeShared<Baby_full>("Single t 2017", Process::Type::background, colors("single_t"), {mc2017_dir+"slim*_ST_*.root"},"pass");
  auto single_t_2018 = Process::MakeShared<Baby_full>("Single t 2018", Process::Type::background, colors("single_t"), {mc2018_dir+"slim*_ST_*.root"},"pass");
 
  auto diboson_2017 = Process::MakeShared<Baby_full>("Diboson 2017", Process::Type::background, colors("other"),{mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*WZ*.root",mc2017_dir+"slim*ZZ*.root"},"pass");
  auto diboson_2018 = Process::MakeShared<Baby_full>("Diboson 2018", Process::Type::background, colors("other"),{mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"},"pass");

  auto ttV_2017 = Process::MakeShared<Baby_full>("t#bar{t}V 2017", Process::Type::background, colors("ttv"), {mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZ*.root"},"pass");
  auto ttV_2018 = Process::MakeShared<Baby_full>("t#bar{t}V 2018", Process::Type::background, colors("ttv"), {mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"},"pass");
*/

  //ttbar
  auto tt1l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2017", Process::Type::background, colors("tt_1l"),{mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2017", Process::Type::background, colors("tt_2l"),{mc2017_dir+"slim*TTJets_2lep_*.root"},"stitch");

  auto tt1l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2018", Process::Type::background, colors("tt_1l"),{mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2018", Process::Type::background, colors("tt_2l"),{mc2018_dir+"slim*TTJets_2lep_*.root"},"stitch");

  //WJets
  auto wjets_2017 = Process::MakeShared<Baby_full>("W+jets 2017", Process::Type::background, colors("wjets"), {mc2017_dir+"slim*_W*JetsToLNu_f17v2*",mc2017_dir+"slim*W*Jets_NuPt200_f17v2*.root"},"stitch");
  auto wjets_2018 = Process::MakeShared<Baby_full>("W+jets 2018", Process::Type::background, colors("wjets"), {mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root"},"stitch");

  //Single top
  auto single_t_2017 = Process::MakeShared<Baby_full>("Single t 2017", Process::Type::background, colors("single_t"), {mc2017_dir+"slim*_ST_*.root"});
  auto single_t_2018 = Process::MakeShared<Baby_full>("Single t 2018", Process::Type::background, colors("single_t"), {mc2018_dir+"slim*_ST_*.root"});

  //diboson
  auto diboson_2017 = Process::MakeShared<Baby_full>("Diboson 2017", Process::Type::background, colors("other"),{mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*WZ*.root",mc2017_dir+"slim*ZZ*.root"});
  auto diboson_2018 = Process::MakeShared<Baby_full>("Diboson 2018", Process::Type::background, colors("other"),{mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"});

  // ttV
  auto ttV_2017 = Process::MakeShared<Baby_full>("t#bar{t}V 2017", Process::Type::background, colors("ttv"), {mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZ*.root"});
  auto ttV_2018 = Process::MakeShared<Baby_full>("t#bar{t}V 2018", Process::Type::background, colors("ttv"), {mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"});


  vector<shared_ptr<Process> > sample_list_2018 = {data2018,tt1l_2018,tt2l_2018,wjets_2018,single_t_2018,diboson_2018,ttV_2018};
  vector<shared_ptr<Process> > sample_list_2017 = {data2017,tt1l_2017,tt2l_2017,wjets_2017,single_t_2017,diboson_2017,ttV_2017};
  vector<shared_ptr<Process> > sample_list_2017B = {data2017B,mc_2017};
  vector<shared_ptr<Process> > sample_list_2017C = {data2017C,mc_2017};
  vector<shared_ptr<Process> > sample_list_2017D = {data2017D,mc_2017};
  vector<shared_ptr<Process> > sample_list_2017E = {data2017E,mc_2017};
  vector<shared_ptr<Process> > sample_list_2017F = {data2017F,mc_2017};


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
  PlotMaker * pm2017 = new PlotMaker();
  PlotMaker * pm2017B = new PlotMaker();
  PlotMaker * pm2017C = new PlotMaker();
  PlotMaker * pm2017D = new PlotMaker();
  PlotMaker * pm2017E = new PlotMaker();
  PlotMaker * pm2017F = new PlotMaker();
  PlotMaker * pm2018 = new PlotMaker();

  //Preselection
  NamedFunc preselection = "pass&&nvetoleps==2&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && WHLeptons==1;
  NamedFunc tt2l = "pass&&nvetoleps==3&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125" && WHLeptons==1;
  NamedFunc tt1l = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&ngoodbtags>=1&&pfmet>125" && WHLeptons==1;
  NamedFunc tt1l_noMET = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&ngoodbtags>=1" && WHLeptons==1;

 /* NamedFunc ttbar_sel = "pass&&ngoodjets>=4&&pfmet>125&&ngoodbtags>=1" && WHLeptons==1;
  NamedFunc ttbar_sel_full_eff = "leps_pt[0]>50&&pfmet>250&&pass&&ngoodjets>=4&&ngoodbtags>=1" && WHLeptons==1;

  NamedFunc ttbar_1l_sel = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125&&ngoodbtags>=1" && WHLeptons==1;
  NamedFunc ttbar_1l_sel_full_eff = "leps_pt[0]>50&&pfmet>250&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&ngoodbtags>=1" && WHLeptons==1;

  NamedFunc wjets_sel = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50 && ngoodbtags==0" && WHLeptons==1;
  NamedFunc wjets_sel_full_eff= "leps_pt[0]>50&&pfmet>250&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>50 && ngoodbtags==0" && WHLeptons==1;

  NamedFunc ttbar_2l_sel = "pass&&nvetoleps==2&&ngoodjets==2&&pfmet>125&&ngoodbtags>=1" && WHLeptons>=1;
  NamedFunc ttbar_2l_sel_full_eff = "leps_pt[0]>50&&pfmet>250&&pass&&nvetoleps==2&&ngoodjets==2&&ngoodbtags>=1" && WHLeptons>=1;*/

  vector<NamedFunc> sels = {preselection,tt2l,tt1l,tt1l_noMET}; 
  vector<PlotMaker *> pms = {pm2018,pm2017,pm2017B,pm2017C,pm2017D,pm2017E,pm2017F};
  vector<vector<shared_ptr<Process> >> samples_Run2 = {sample_list_2018,sample_list_2017,sample_list_2017B,sample_list_2017C,sample_list_2017D,sample_list_2017E,sample_list_2017F};
  vector<string> years = {"y2018","y2017","y2017B","y2017C","y2017D","y2017E","y2017F"};
  vector<string> selectionTags = {"_preselection_","_tt2l_","_tt1l_","_tt1l_noMET_"};
  vector<NamedFunc> weights = {"weight","weight * w_pu","weight * w_pu"*yearWeight};
  vector<string> weightnames = {"weight","weight * w_pu","weight * w_pu*yearWeight"};

  for(uint isel=0;isel<sels.size();isel++){
    for(uint iyear=0;iyear<pms.size();iyear++){
        for(uint iweight=0;iweight<weights.size();iweight++){

        pms[iyear]->Push<Hist1D>(Axis(40, 0, 400., mt_met_lep_uncorr, "mT (uncorr) [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]+"_EESmearing_"+selectionTags[isel]);
        
        pms[iyear]->Push<Hist1D>(Axis(40, 0, 400., "mt_met_lep", "mT [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]+"_EESmearing_"+selectionTags[isel]);  
        
        pms[iyear]->Push<Hist1D>(Axis(20, 0, 400., "pfmet", "MET [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]+"_EESmearing_"+selectionTags[isel]);

        pms[iyear]->Push<Hist1D>(Axis(20, 0, 400., pfmet_uncorr_func, "MET (uncorr) [GeV]"),
          sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]+"_EESmearing_"+selectionTags[isel]);
      }
    }
  }

  pm2017->MakePlots(lumi2017);
  pm2017B->MakePlots(lumi2017);
  pm2017C->MakePlots(lumi2017);
  pm2017D->MakePlots(lumi2017);
  pm2017E->MakePlots(lumi2017);
  pm2017F->MakePlots(lumi2017);
  pm2018->MakePlots(lumi2018);

}
