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

  string data2016_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_10_05/";
  string data2017_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_05/";
  string data2018_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_05/";

  string mc2016_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/s16v3/";
  string mc2016_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/s16v3/";

  string mc2017_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/f17v2/";
  string mc2017_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/f17v2/";

  string mc2018_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/a18v1/";
  string mc2018_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_07/a18v1/";  


  Palette colors("txt/colors.txt", "default");

  //Data
  auto data =  Process::MakeShared<Baby_full>("2016-2018 Data", Process::Type::data, colors("data"),{data2016_dir+"*data_2016*.root",data2017_dir+"*data_2017*.root",data2018_dir+"*data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto data2016 = Process::MakeShared<Baby_full>("2016 Data", Process::Type::data, colors("data"),{data2016_dir+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto data2017 = Process::MakeShared<Baby_full>("2017 Data", Process::Type::data, colors("data"),{data2017_dir+"*data_2017*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto data2018 = Process::MakeShared<Baby_full>("2018 Data", Process::Type::data, colors("data"),{data2018_dir+"*data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");

  auto tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016-2018", Process::Type::background, colors("tt_1l"),
    {mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2017_dir+"*TTJets_1lep_top_f17v2*.root",mc2017_dir+"*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2018_dir+"*TTJets_1lep_top_a18v1*.root",mc2018_dir+"*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"*TTJets_1lep_*met80*.root"},"stitch");
  auto tt2l = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016-2018", Process::Type::background, colors("tt_2l"),
    {mc2016_dir+"*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2017_dir+"*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2018_dir+"*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"*TTJets_2lep_*met80*.root"},"stitch");

  auto tt1l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016", Process::Type::background, colors("tt_1l"),
    {mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"*TTJets_1lep_*met150*.root"},"stitch");
  auto tt2l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016", Process::Type::background, colors("tt_2l"),
    {mc2016_dir+"*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"*TTJets_2lep_*met150*.root"},"stitch");
  auto tt1l_2016_inclu = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016", Process::Type::background, colors("tt_1l"),
    {mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*"});
  auto tt2l_2016_inclu = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016", Process::Type::background, colors("tt_2l"),
    {mc2016_dir+"*TTJets_2lep_s16v3*.root"});


  auto tt1l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2017", Process::Type::background, colors("tt_1l"),
    {mc2017_dir+"*TTJets_1lep_top_f17v2*.root",mc2017_dir+"*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"*TTJets_1lep_*met150*.root"},"stitch");
  auto tt2l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2017", Process::Type::background, colors("tt_2l"),
    {mc2017_dir+"*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"*TTJets_2lep_*met150*.root"},"stitch");
  auto tt1l_2017_inclu = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2017", Process::Type::background, colors("tt_1l"),
    {mc2017_dir+"*TTJets_1lep_top_f17v2*.root",mc2017_dir+"*TTJets_1lep_tbar_f17v2*"});
  auto tt2l_2017_inclu = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2017", Process::Type::background, colors("tt_2l"),
    {mc2017_dir+"*TTJets_2lep_f17v2*.root"});

  auto tt1l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2018", Process::Type::background, colors("tt_1l"),
    {mc2018_dir+"*TTJets_1lep_top_a18v1*.root",mc2018_dir+"*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"*TTJets_1lep_*met80*.root"},"stitch");
  auto tt2l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2018", Process::Type::background, colors("tt_2l"),
    {mc2018_dir+"*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"*TTJets_2lep_*met80*.root"},"stitch");

  //WJets
  auto wjets = Process::MakeShared<Baby_full>("W+jets 2016-2018", Process::Type::background, colors("wjets"), {mc2016_dir+"*slim_W*JetsToLNu_s16v3*",mc2016_dir+"*W*Jets_NuPt200_s16v*.root",mc2017_dir+"*slim_W*JetsToLNu_f17v2*",mc2017_dir+"*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"*slim_W*JetsToLNu_a18v1*",mc2018_dir+"*W*Jets_NuPt200_a18v1*.root"},"stitch");

  auto wjets_2016 = Process::MakeShared<Baby_full>("W+jets 2016", Process::Type::background, colors("wjets"), {mc2016_dir+"*slim_W*JetsToLNu_s16v3*",mc2016_dir+"*W*Jets_NuPt200_s16v*.root"},"stitch");
  auto wjets_2017 = Process::MakeShared<Baby_full>("W+jets 2017", Process::Type::background, colors("wjets"), {mc2017_dir+"*slim_W*JetsToLNu_f17v2*",mc2017_dir+"*W*Jets_NuPt200_f17v2*.root"},"stitch");
  auto wjets_2017_inclu = Process::MakeShared<Baby_full>("W+jets 2017", Process::Type::background, colors("wjets"), {mc2017_dir+"*slim_W*JetsToLNu_f17v2*"});
  auto wjets_2018 = Process::MakeShared<Baby_full>("W+jets 2018", Process::Type::background, colors("wjets"), {mc2018_dir+"*slim_W*JetsToLNu_a18v1*",mc2018_dir+"*W*Jets_NuPt200_a18v1*.root"},"stitch");
  auto wjets_2018_inclu = Process::MakeShared<Baby_full>("W+jets 2018", Process::Type::background, colors("wjets"), {mc2018_dir+"*slim_W*JetsToLNu_a18v1*"});


  //Single top
  auto single_t = Process::MakeShared<Baby_full>("Single t 2016-2018", Process::Type::background, colors("single_t"), {mc2016_dir+"*_ST_*.root",mc2017_dir+"*_ST_*.root",mc2018_dir+"*_ST_*.root"});

  auto single_t_2016 = Process::MakeShared<Baby_full>("Single t 2016", Process::Type::background, colors("single_t"), {mc2016_dir+"*_ST_*.root"});
  auto single_t_2017 = Process::MakeShared<Baby_full>("Single t 2017", Process::Type::background, colors("single_t"), {mc2017_dir+"*_ST_*.root"});
  auto single_t_2018 = Process::MakeShared<Baby_full>("Single t 2018", Process::Type::background, colors("single_t"), {mc2018_dir+"*_ST_*.root"});


  //diboson
  auto diboson = Process::MakeShared<Baby_full>("Diboson 2016-2018", Process::Type::background, colors("other"),
  {mc2016_dir+"*WW*.root", mc2016_dir+"*WZ*.root",mc2016_dir+"*ZZ*.root",mc2017_dir+"*WW*.root", mc2017_dir+"*WZ*.root",mc2017_dir+"*ZZ*.root",mc2018_dir+"*WW*.root", mc2018_dir+"*WZ*.root",mc2018_dir+"*ZZ*.root"});

  auto diboson_2016 = Process::MakeShared<Baby_full>("Diboson 2016", Process::Type::background, colors("other"),
   {mc2016_dir+"*WW*.root", mc2016_dir+"*WZ*.root",mc2016_dir+"*ZZ*.root"});
  auto diboson_2017 = Process::MakeShared<Baby_full>("Diboson 2017", Process::Type::background, colors("other"),{mc2017_dir+"*WW*.root", mc2017_dir+"*WZ*.root",mc2017_dir+"*ZZ*.root"});
  auto diboson_2018 = Process::MakeShared<Baby_full>("Diboson 2018", Process::Type::background, colors("other"),{mc2018_dir+"*WW*.root", mc2018_dir+"*WZ*.root",mc2018_dir+"*ZZ*.root"});

  // ttV
  auto ttV = Process::MakeShared<Baby_full>("t#bar{t}V 2016-2018", Process::Type::background, colors("ttv"),{mc2016_dir+"*_TTWJets*.root", mc2016_dir+"*_TTZ*.root",mc2017_dir+"*_TTWJets*.root", mc2017_dir+"*_TTZ*.root",mc2018_dir+"*_TTWJets*.root", mc2018_dir+"*_TTZ*.root"});
  
  auto ttV_2016 = Process::MakeShared<Baby_full>("t#bar{t}V 2016", Process::Type::background, colors("ttv"),{mc2016_dir+"*_TTWJets*.root", mc2016_dir+"*_TTZ*.root"});
  auto ttV_2017 = Process::MakeShared<Baby_full>("t#bar{t}V 2017", Process::Type::background, colors("ttv"), {mc2017_dir+"*_TTWJets*.root", mc2017_dir+"*_TTZ*.root"});
  auto ttV_2018 = Process::MakeShared<Baby_full>("t#bar{t}V 2018", Process::Type::background, colors("ttv"), {mc2018_dir+"*_TTWJets*.root", mc2018_dir+"*_TTZ*.root"});



  //List of processes
  vector<shared_ptr<Process> > sample_list_2016 = {data2016,wjets_2016,tt2l_2016,tt1l_2016,single_t_2016,diboson_2016,ttV_2016};
  vector<shared_ptr<Process> > sample_list_2017 = {data2017,wjets_2017,tt2l_2017,tt1l_2017,single_t_2017,diboson_2017,ttV_2017};
  vector<shared_ptr<Process> > sample_list_2018 = {data2018,wjets_2018,tt2l_2018,tt1l_2018,single_t_2018,diboson_2018,ttV_2018};

  vector<shared_ptr<Process> > sample_list = {data,wjets,tt2l,tt1l,single_t,diboson,ttV};


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
  PlotMaker * pm2016 = new PlotMaker();
  PlotMaker * pm2017 = new PlotMaker();
  PlotMaker * pm2018 = new PlotMaker();
  PlotMaker * pm = new PlotMaker();

  //Preselection
  NamedFunc preselection = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50 && ngoodbtags==2 && nWHLeptons==1";
  NamedFunc preselection_full_eff = "leps_pt[0]>50&&pfmet>250&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>50 && ngoodbtags==2 && nWHLeptons==1";

  NamedFunc ttbar_sel = "pass&&ngoodjets>=4&&pfmet>125&&ngoodbtags>=1 && nWHLeptons==1";
  NamedFunc ttbar_sel_full_eff = "leps_pt[0]>50&&pfmet>250&&pass&&ngoodjets>=4&&ngoodbtags>=1 && nWHLeptons==1";

  NamedFunc ttbar_1l_sel = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125&&ngoodbtags>=1 && nWHLeptons==1";
  NamedFunc ttbar_1l_sel_full_eff = "leps_pt[0]>50&&pfmet>250&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&ngoodbtags>=1 && nWHLeptons==1";

  NamedFunc wjets_sel = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50 && ngoodbtags==0 && nWHLeptons==1";
  NamedFunc wjets_sel_full_eff= "leps_pt[0]>50&&pfmet>250&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>50 && ngoodbtags==0 && nWHLeptons==1";

  NamedFunc wjets_CR = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150 && mct>200 && ngoodbtags==0 && nWHLeptons==1";

  NamedFunc ttbar_2l_sel = "pass&&nvetoleps==2&&ngoodjets==2&&pfmet>125&&ngoodbtags>=1 && nWHLeptons>=1";
  NamedFunc ttbar_2l_sel_full_eff = "leps_pt[0]>50&&pfmet>250&&pass&&nvetoleps==2&&ngoodjets==2&&ngoodbtags>=1 && nWHLeptons>=1";


  NamedFunc highmT = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&ngoodbtags==2 && mt_met_lep > 150 && nWHLeptons==1";

  NamedFunc signal_looser = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>125 && mct > 175 && ngoodbtags==2 && nWHLeptons==1" && outsideHiggsWindow==1.;

  NamedFunc signal_region_blind = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150&&ngoodbtags==2&&mct>200 && nWHLeptons==1" && outsideHiggsWindow==1.;
  NamedFunc signal_region = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150&&ngoodbtags==2&&mct>200 && nWHLeptons==1";

  NamedFunc CR_lower_mT = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>90&&mt_met_lep<140&&ngoodbtags==2&&mct>200 && nWHLeptons==1";
  NamedFunc CR_lower_mcT = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150&&ngoodbtags==2&&mct>150&&mct<200 && nWHLeptons==1";


  //vector<NamedFunc> sels = {preselection,ttbar_2l_sel,wjets_sel,CR_lower_mcT,CR_lower_mT/*,highmT*/}; 
  vector<NamedFunc> sels = {wjets_CR}; 
  //vector<NamedFunc> sels = {preselection,ttbar_sel,wjets_sel,ttbar_1l_sel,ttbar_2l_sel}; 
  vector<NamedFunc> sels_full_eff = {preselection_full_eff,ttbar_sel_full_eff,wjets_sel_full_eff,ttbar_1l_sel_full_eff,ttbar_2l_sel_full_eff}; 
  vector<PlotMaker *> pms = {pm2016,pm2017,pm2018,pm};
  vector<vector<shared_ptr<Process> >> samples_Run2 = {sample_list_2016,sample_list_2017,sample_list_2018,sample_list};
  vector<string> years = {"y2016","y2017","y2018","yLegacy"};
  vector<NamedFunc> weights = {"weight","weight * w_pu"};
  vector<NamedFunc> legacyWeights = {"weight"*yearWeight,"weight * w_pu" * yearWeight};

   vector<string> metbins = {"1","pfmet>200"};
  for(uint isel=0;isel<sels.size();isel++){
    for(uint iyear=0;iyear<samples_Run2.size();iyear++){
       for(uint imet=0;imet<metbins.size();imet++){
        for(uint iweight=0;iweight<weights.size();iweight++){
        
        if(iyear<3){
        pms[iyear]->Push<Hist1D>(Axis(15, 125, 500., "pfmet", "E_{T}^{miss} [GeV]"),
          sels[isel] && metbins[imet], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]);  

        pms[iyear]->Push<Hist1D>(Axis(10, 0, 300., "mbb", "m_{bb} [GeV]"),
          sels[isel] && metbins[imet], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]);  

        pms[iyear]->Push<Hist1D>(Axis(10, 0, 400., "mct", "m_{CT} [GeV]"),
          sels[isel] && metbins[imet], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]);  

        pms[iyear]->Push<Hist1D>(Axis(10, 0, 400., "mt_met_lep", "m_{T} [GeV]"),
          sels[isel] && metbins[imet], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]);  
      }
      else{
        pms[iyear]->Push<Hist1D>(Axis(15, 125, 500., "pfmet", "E_{T}^{miss} [GeV]"),
          sels[isel] && metbins[imet], samples_Run2[iyear], all_plot_types).Weight(legacyWeights[iweight]).Tag(years[iyear]);  

        pms[iyear]->Push<Hist1D>(Axis(10, 0, 300., "mbb", "m_{bb} [GeV]"),
          sels[isel] && metbins[imet], samples_Run2[iyear], all_plot_types).Weight(legacyWeights[iweight]).Tag(years[iyear]);  

        pms[iyear]->Push<Hist1D>(Axis(10, 0, 400., "mct", "m_{CT} [GeV]"),
          sels[isel] && metbins[imet], samples_Run2[iyear], all_plot_types).Weight(legacyWeights[iweight]).Tag(years[iyear]);  

        pms[iyear]->Push<Hist1D>(Axis(10, 0, 400., "mt_met_lep", "m_{T} [GeV]"),
          sels[isel] && metbins[imet], samples_Run2[iyear], all_plot_types).Weight(legacyWeights[iweight]).Tag(years[iyear]);  

      }
        // if(isel>0)continue;
        // pms[iyear]->Push<Hist1D>(Axis(10, 0, 400., "mct", "m_{CT} [GeV]"),
        //   "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>90&&mt_met_lep<140&&ngoodbtags==2" && WHLeptons==1 && metbins[imet], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]);  

        // pms[iyear]->Push<Hist1D>(Axis(10, 0, 400., "mt_met_lep", "m_{T} [GeV]"),
        //   "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&ngoodbtags==2&&mct>150&&mct<200" && WHLeptons==1 && metbins[imet], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag(years[iyear]);  

        }
      }
    }
  }

   pm2016->MakePlots(lumi2016);
   pm2017->MakePlots(lumi2017);
 cout<<lumi2016<<lumi2017<<lumi2018<<endl;
   pm2018->MakePlots(lumi2018);
  pm->MakePlots(lumi2016+lumi2017+lumi2018);

}
