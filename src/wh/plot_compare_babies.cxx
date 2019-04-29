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
  // string data_dir_new = "/home/users/rheller/wh_babies/babies_2019_02_27/";

  string data_dir_old = "/home/users/rheller/wh_babies/babies_2019_02_12/";
  string data_dir_new = "/home/users/rheller/wh_babies/babies_v31_1_2019_04_03/";
  
 // string mc_dir_old_tt1l = "/home/users/rheller/wh_babies/babies_2019_02_11/";
  string mc_dir_old = "/home/users/rheller/wh_babies/babies_2019_02_11/";
  string mc_dir_new = "/home/users/rheller/wh_babies/babies_v30_9_2019_04_03/";

  string signal_dir_old = "/home/users/rheller/wh_babies/babies_2019_02_12/";
  string signal_dir_new = "/home/users/rheller/wh_babies/babies_signal_2019_04_03/";

  Palette colors("txt/colors.txt", "default");

  auto data_old = Process::MakeShared<Baby_full>("Data, v29_14", Process::Type::background, kBlack,{data_dir_old+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1)");
  data_old->SetFillColor(kWhite);
  data_old->SetLineColor(kAzure-2);
  data_old->SetLineWidth(3);
  auto data_new = Process::MakeShared<Baby_full>("Data, v31_1", Process::Type::data, colors("data"),{data_dir_new+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1)");


  auto tt1l_old = Process::MakeShared<Baby_full>("t#bar{t} (1l), v29_11", Process::Type::background, colors("tt_1l"),
    {mc_dir_old+"*TTJets_1lep*.root"});
  tt1l_old->SetFillColor(kWhite);
  tt1l_old->SetLineColor(kAzure-2);
  tt1l_old->SetLineWidth(3);
  auto tt1l_new = Process::MakeShared<Baby_full>("t#bar{t} (1l), v30_9", Process::Type::data, colors("data"),
    {mc_dir_new+"*TTJets_1lep*.root"});


  auto tt2l_old = Process::MakeShared<Baby_full>("t#bar{t} (2l), v29_11", Process::Type::background, colors("tt_2l"),
    {mc_dir_old+"*TTJets_2lep*.root"});
  tt2l_old->SetFillColor(kWhite);
  tt2l_old->SetLineColor(kAzure-2);
  tt2l_old->SetLineWidth(3);
  auto tt2l_new = Process::MakeShared<Baby_full>("t#bar{t} (2l), v30_9", Process::Type::data, colors("data"),
    {mc_dir_new+"*TTJets_2lep*.root"});
 


  auto wjets_low_nu_old = Process::MakeShared<Baby_full>("W+jets, nu pT < 200, v29_11", Process::Type::background, colors("wjets"),
    {mc_dir_old+"*slim_W*JetsToLNu_*.root"});
  wjets_low_nu_old->SetFillColor(kWhite);
  wjets_low_nu_old->SetLineColor(kAzure-2);
  wjets_low_nu_old->SetLineWidth(3);
  auto wjets_low_nu_new = Process::MakeShared<Baby_full>("W+jets, nu pT < 200, v30_9", Process::Type::data, colors("data"),
    {mc_dir_new+"*slim_W*JetsToLNu_s16v3*"});


  auto wjets_high_nu_old = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200, v29_11", Process::Type::background, colors("wjets"),
    {mc_dir_old+"*W*Jets_NuPt200_*.root"});
  wjets_high_nu_old->SetFillColor(kWhite);
  wjets_high_nu_old->SetLineColor(kAzure-2);
  wjets_high_nu_old->SetLineWidth(3);
  auto wjets_high_nu_new = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200, v30_9", Process::Type::data, colors("data"),
    {mc_dir_new+"*W*Jets_NuPt200_s16v*.root"});

  auto single_t_old = Process::MakeShared<Baby_full>("Single t, v29_11", Process::Type::background, colors("single_t"),
    {mc_dir_old+"*_ST_*.root"});
  single_t_old->SetFillColor(kWhite);
  single_t_old->SetLineColor(kAzure-2);
  single_t_old->SetLineWidth(3);
  auto single_t_new = Process::MakeShared<Baby_full>("Single t, v30_9", Process::Type::data, colors("data"),
    {mc_dir_new+"*_ST_*.root"});


  auto other_old = Process::MakeShared<Baby_full>("Other, v29_11", Process::Type::background, colors("other"),
    {mc_dir_old+"*_TTWJets*.root", mc_dir_old+"*_TTZ*.root", mc_dir_old+"*WW*.root", mc_dir_old+"*WZ*.root",mc_dir_old+"*ZZ*.root"});
  other_old->SetFillColor(kWhite);
  other_old->SetLineColor(kAzure-2);
  other_old->SetLineWidth(3);
  auto other_new = Process::MakeShared<Baby_full>("Other, v30_9", Process::Type::data, colors("data"),
   {mc_dir_new+"*_TTWJets*.root", mc_dir_new+"*_TTZ*.root", mc_dir_new+"*WW*.root", mc_dir_new+"*WZ*.root",mc_dir_new+"*ZZ*.root"});


  auto signal_old = Process::MakeShared<Baby_full>("Signal (500,1), v29_11", Process::Type::background, colors("t1tttt"),
    {signal_dir_old+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==500&&mass_lsp==1");
  signal_old->SetFillColor(kWhite);
  signal_old->SetLineColor(kAzure-2);
  signal_old->SetLineWidth(3);
  auto signal_new = Process::MakeShared<Baby_full>("Signal (500,1), v30_9", Process::Type::data, colors("data"),
    {signal_dir_new+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==500&&mass_lsp==1");



  vector<vector<shared_ptr<Process> >> sample_list_list ={{data_new,data_old},{tt1l_new,tt1l_old},{tt2l_new,tt2l_old},
                                                        {wjets_high_nu_new,wjets_high_nu_old},{wjets_low_nu_new,wjets_low_nu_old},
                                                         {single_t_new,single_t_old},{other_new,other_old},{signal_new,signal_old}};
  vector<string> tag_list = {"data","tt1l","tt2l","wjets_high_nu","wjets_low_nu","single_t","other","signal"};
//  vector<shared_ptr<Process> > sample_list = {data, /*tchiwh_250_1, tchiwh_225_75,*/ tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson};

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
  NamedFunc preselection = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=2&&pfmet>150&&mt_met_lep>50"&& WHLeptons==1;
  NamedFunc ttbar_sel = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>150" && WHLeptons==1;
  vector<NamedFunc> sels = {preselection,ttbar_sel}; 
  vector<string> weights = {"1","w_noBtagSF"};


  for(uint i=0;i<sample_list_list.size();i++){

    for(uint j=0;j<weights.size();j++){
    pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
        preselection && "ngoodbtags>=1 && nloosebtags==2 && ngoodjets>=2", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  
    pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
       "1", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(30, 0, 300., "mt_met_lep", "mT [GeV]"),
        preselection && "ngoodbtags>=1 && nloosebtags==2 && ngoodjets>=2", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  
    pm.Push<Hist1D>(Axis(30, 0, 300., "mt_met_lep", "mT [GeV]"),
       "pfmet>150", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(7,0,7, "ngoodjets", "N_{jets}"),
        preselection && "ngoodbtags>=1 && nloosebtags==2", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  
    pm.Push<Hist1D>(Axis(7,0,7, "ngoodjets", "N_{jets}"),
       "pfmet>150", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(4,0,4, "ngoodbtags", "N_{medium btags}"),
        preselection && " nloosebtags>=2 && ngoodjets>=2", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  
    pm.Push<Hist1D>(Axis(4,0,4, "ngoodbtags", "N_{medium btags}"),
       "pfmet>150", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(4,0,4, "nloosebtags", "N_{loose btags}"),
        preselection && "ngoodbtags>=1 && ngoodjets>=2", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  
    pm.Push<Hist1D>(Axis(4,0,4, "nloosebtags", "N_{loose btags}"),
       "pfmet>150", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(4,0,4, WHLeptons, "N_{leptons}"),
        "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=2&&pfmet>150&&mt_met_lep>50 &&ngoodbtags>=1 && nloosebtags==2 && ngoodjets>=2", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  
    pm.Push<Hist1D>(Axis(4,0,4, WHLeptons, "N_{leptons}"),
       "pfmet>150", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  

    pm.Push<Hist1D>(Axis(4,0,4, "nvetoleps", "N_{veto leptons}"),
        "PassTrackVeto&&PassTauVeto&&ngoodjets>=2&&pfmet>150&&mt_met_lep>50 && ngoodbtags>=1 && nloosebtags==2 && ngoodjets>=2" && WHLeptons==1, sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  
    pm.Push<Hist1D>(Axis(4,0,4, "nvetoleps", "N_{veto leptons}"),
       "pfmet>150", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);

    pm.Push<Hist1D>(Axis(4,0,4, "PassTauVeto", "PassTauVeto"),
        "nvetoleps==1&&PassTrackVeto&&ngoodjets>=2&&pfmet>150&&mt_met_lep>50 && ngoodbtags>=1 && nloosebtags==2 && ngoodjets>=2" && WHLeptons==1, sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  
    pm.Push<Hist1D>(Axis(4,0,4, "PassTauVeto", "PassTauVeto"),
       "pfmet>150", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);

    pm.Push<Hist1D>(Axis(4,0,4, "PassTrackVeto", "PassTrackVeto"),
        "nvetoleps==1&&PassTauVeto&&ngoodjets>=2&&pfmet>150&&mt_met_lep>50 && ngoodbtags>=1 && nloosebtags==2 && ngoodjets>=2" && WHLeptons==1, sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);  
    pm.Push<Hist1D>(Axis(4,0,4, "PassTrackVeto", "PassTrackVeto"),
       "pfmet>150", sample_list_list[i], all_plot_types).Weight(weights[j]).Tag(tag_list[i]);
}
}

  pm.MakePlots(lumi);

}
