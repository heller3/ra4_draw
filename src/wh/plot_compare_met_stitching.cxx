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
const NamedFunc NHighPtNu("NHighPtNu",[](const Baby &b) -> NamedFunc::ScalarType{
      int nnu=0;
        for (unsigned i(0); i<b.gen_pt()->size(); i++){
        if (abs(b.gen_motherid()->at(i))==24 && ( abs(b.gen_id()->at(i)) == 12 || abs(b.gen_id()->at(i)) == 14 || abs(b.gen_id()->at(i)) == 16) && b.gen_pt()->at(i) > 200 ) nnu++;
      }
      return nnu;
    });


int main(){
  gErrorIgnoreLevel = 6000;

  double lumi = 1.;

  //string data_dir = "/home/users/rheller/wh_babies/babies_2019_02_27/";
  //string mc_dir = "/home/users/rheller/wh_babies/babies_2019_02_27/";
  // string data_dir_old = "/home/users/rheller/wh_babies/babies_2019_02_12/";
  // string data_dir = "/home/users/rheller/wh_babies/babies_2019_02_27/";

  string data2016_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_08_09/";
  
  string mc2016_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_08_13/s16v3/";

  string mc2016_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_08_09/s16v3/";

  Palette colors("txt/colors.txt", "default");


  auto data2016 = Process::MakeShared<Baby_full>("2016 Data, new", Process::Type::data, colors("data"),{data2016_dir+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1)");

 
  auto tt1l_2016_inclu = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016, inclusive", Process::Type::data, colors("data"),
    {mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*"});

  auto tt1l_2016_inclu_stitch = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016, inclusive,stiched", Process::Type::background, colors("tt_1l"),
    {mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*"},"stitch");

  auto tt1l_2016_met_stitch = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016, gen met > 150", Process::Type::background, colors("wjets"),
    {mc2016_dir_ttmet+"*TTJets_1lep_*met150*.root"},"stitch");


  auto tt2l_2016_inclu = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016, inclusive", Process::Type::data, colors("data"),
    {mc2016_dir+"*TTJets_2lep_s16v3*.root"});

  auto tt2l_2016_inclu_stitch = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016, inclusive, stiched", Process::Type::background, colors("tt_2l"),
    {mc2016_dir+"*TTJets_2lep_s16v3*.root"},"stitch");

  auto tt2l_2016_met_stitch = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016, gen met > 150", Process::Type::background, colors("wjets"),
    {mc2016_dir_ttmet+"*TTJets_2lep_met150*.root"},"stitch");


   auto wjets_2016_inclu = Process::MakeShared<Baby_full>("W+Jets 2016, inclusive", Process::Type::data, colors("data"),
    {mc2016_dir+"*slim_W*JetsToLNu_s16v3*.root"});

  auto wjets_2016_inclu_stitch = Process::MakeShared<Baby_full>("W+Jets 2016, inclusive, stiched", Process::Type::background, colors("wjets"),
    {mc2016_dir+"*slim_W*JetsToLNu_s16v3*.root"},"stitch");

  auto wjets_2016_met_stitch = Process::MakeShared<Baby_full>("W+Jets 2016, nu pT > 200", Process::Type::background, colors("dy"),
    {mc2016_dir+"*slim_W*Jets_NuPt200_s16v*.root"},"stitch");  


  vector<vector<shared_ptr<Process> >> sample_list_list ={{tt1l_2016_inclu,tt1l_2016_inclu_stitch,tt1l_2016_met_stitch},{tt2l_2016_inclu,tt2l_2016_inclu_stitch,tt2l_2016_met_stitch},{wjets_2016_inclu,wjets_2016_inclu_stitch,wjets_2016_met_stitch}}; 
  vector<string> tag_list = {"tt1l","tt2l","wjets"};

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


  PlotMaker pm;

  //Preselection
  NamedFunc preselection = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>150&&mt_met_lep>50 && ngoodbtags==2" && WHLeptons==1;
  NamedFunc sig = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150 && ngoodbtags==2"&& WHLeptons==1;

  vector<NamedFunc> sels = {preselection,sig}; 

  for(uint i=0;i<sample_list_list.size();i++){
 
    pm.Push<Hist1D>(Axis(25, 300, 800., "pfmet", "E_{T}^{miss} [GeV]"),
        preselection && "pfmet>300", sample_list_list[i], all_plot_types).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(28, 0, 700., "pfmet", "E_{T}^{miss} [GeV]"),
        preselection, sample_list_list[i], all_plot_types).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(28, 0, 700., "pfmet", "E_{T}^{miss} [GeV]"),
       "1", sample_list_list[i], all_plot_types).Tag(tag_list[i]);  



    pm.Push<Hist1D>(Axis(25, 300, 800., "genmet", "gen E_{T}^{miss} [GeV]"),
        preselection && "genmet>300", sample_list_list[i], all_plot_types).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(28, 0, 700., "genmet", "gen E_{T}^{miss} [GeV]"),
        preselection, sample_list_list[i], all_plot_types).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(28, 0, 700., "genmet", "gen E_{T}^{miss} [GeV]"),
       "1", sample_list_list[i], all_plot_types).Tag(tag_list[i]);  


    pm.Push<Hist1D>(Axis(25, 300, 800., HighNuPt, "Leading neutrino pT [GeV]"),
        preselection && HighNuPt>300., sample_list_list[i], all_plot_types).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(28, 0, 700., HighNuPt, "Leading neutrino pT [GeV]"),
        preselection, sample_list_list[i], all_plot_types).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(28, 0, 700., HighNuPt, "Leading neutrino pT [GeV]"),
       "1", sample_list_list[i], all_plot_types).Tag(tag_list[i]);  



    pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "mT [GeV]"),
        preselection , sample_list_list[i], all_plot_types).Tag(tag_list[i]); 

    pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "mT [GeV]"),
       "1" , sample_list_list[i], all_plot_types).Tag(tag_list[i]); 



}

  pm.MakePlots(lumi);

}
