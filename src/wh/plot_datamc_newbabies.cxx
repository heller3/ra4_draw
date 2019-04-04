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

  double lumi = 35.9;

  //string data_dir = "/home/users/rheller/wh_babies/babies_2019_02_27/";
  //string mc_dir = "/home/users/rheller/wh_babies/babies_2019_02_27/";
  string data_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_04_03/";
  string mc_dir = "/home/users/rheller/wh_babies/babies_v30_9_2019_04_03/";
  string signal_dir = "/home/users/rheller/wh_babies/babies_signal_2019_04_03/";
  
  string babytag = "v30_9";

  Palette colors("txt/colors.txt", "default");

  auto data = Process::MakeShared<Baby_full>("Data", Process::Type::data, colors("data"),{data_dir+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1)");
  auto tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_1lep*.root"});
  auto tt2l = Process::MakeShared<Baby_full>("t#bar{t} (2l)", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*TTJets_2lep*.root"});
  auto wjets_low_nu = Process::MakeShared<Baby_full>("W+jets, nu pT < 200", Process::Type::background, colors("qcd"),
  {mc_dir+"slim_W*JetsToLNu*.root"},NHighPtNu==0.);
 //  {mc_dir+"slim_W*JetsToLNu_s16v3*.root"},NHighPtNu==0.);
  auto wjets_high_nu = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"});
   // {mc_dir+"*W*Jets_NuPt200_s16v*.root"});
  // auto wjets = Process::MakeShared<Baby_full>("W+jets", Process::Type::background, colors("wjets"),
  // {mc_dir+"slim_W*JetsToLNu*.root"});

  auto single_t = Process::MakeShared<Baby_full>("Single t", Process::Type::background, colors("single_t"),
    {mc_dir+"*_ST_*.root"});
  auto ttv = Process::MakeShared<Baby_full>("t#bar{t}V", Process::Type::background, colors("ttv"),
    {mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root"});
  auto diboson = Process::MakeShared<Baby_full>("Diboson", Process::Type::background, colors("other"),
    {mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"});

    // {mc_dir+"*DYJetsToLL*.root", mc_dir+"*QCD_HT*0_Tune*.root", mc_dir+"*QCD_HT*Inf_Tune*.root",
    //     mc_dir+"*_ZJet*.root", mc_dir+"*_ttHTobb_M125_*.root",
    //     mc_dir+"*_TTGJets*.root", mc_dir+"*_TTTT_*.root",
    //     mc_dir+"*_WH_HToBB*.root", mc_dir+"*_ZH_HToBB*.root", 
    //     mc_dir+"*_WWTo*.root", mc_dir+"*_WZ*.root",
    //     mc_dir+"_ZZ_*.root"}, "stitch_met");


   auto tchiwh_225_75 = Process::MakeShared<Baby_full>("TChiWH(225,75)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==225&&mass_lsp==75");
    tchiwh_225_75->SetLineStyle(2);
  auto tchiwh_250_1 = Process::MakeShared<Baby_full>("TChiWH(250,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==250&&mass_lsp==1");


  vector<shared_ptr<Process> > sample_list = {data, /*tchiwh_250_1, tchiwh_225_75,*/ tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson};

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
  NamedFunc preselection = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=2&&pfmet>125&&mt_met_lep>50&&ngoodbtags>=1&&nloosebtags>=2"&& WHLeptons==1;
  NamedFunc preselection_true = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50&&ngoodbtags>=1&&nloosebtags>=2"&& WHLeptons==1;
  NamedFunc ttbar_sel = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125&&ngoodbtags>=1&&nloosebtags>=2" && WHLeptons==1;
  vector<string> weights = {"w_noBtagSF","w_noBtagSF*w_BtagSF_WH"};
  vector<string> tag_list = {"no_btagSF","whBtagSF"};

  vector<NamedFunc> sels = {preselection,preselection_true,ttbar_sel}; 

    for(uint j=0;j<weights.size();j++){


      //Preselection, N-1
    pm.Push<Hist1D>(Axis(30, 0, 300., "mt_met_lep", "mT [GeV]"),
        "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&ngoodbtags>=1&&nloosebtags>=2"&& WHLeptons==1, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    pm.Push<Hist1D>(Axis(30, 0, 300., "mct", "m_{CT} [GeV]"),
        preselection_true, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    pm.Push<Hist1D>(Axis(30, 0, 300., "mbb", "m_{bb} [GeV]"),
      preselection_true, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    pm.Push<Hist1D>(Axis(25, 125, 500., "pfmet", " E_{T}^{miss}[GeV]"),
      preselection_true, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);   

    pm.Push<Hist1D>(Axis(25, 0, 100., "leps_pt[0]", "Lepton p_{T} [GeV]"),
      preselection_true, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);  

    pm.Push<Hist1D>(Axis(5, 2, 7., "ngoodjets", "N_{jets}"),
        "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=2&&pfmet>125&&mt_met_lep>50&&ngoodbtags>=1&&nloosebtags>=2"&& WHLeptons==1, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);

    pm.Push<Hist1D>(Axis(4, 0, 4., "ngoodbtags", "N_{med deepCSV btags}"),
          "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50"&& WHLeptons==1, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);

    pm.Push<Hist1D>(Axis(4, 0, 4., "ngoodbtags", "N_{med deepCSV btags}"),
          "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50"&& WHLeptons==1, sample_list, all_plot_types).Weight("w_noBtagSF*w_BtagSF_medmed").Tag("medmed");


    pm.Push<Hist1D>(Axis(4, 0, 4., "nloosebtags", "N_{loose deepCSV btags}"),
          "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50"&& WHLeptons==1, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);
  

    pm.Push<Hist1D>(Axis(4, 0, 4., "nloosebtags", "N_{loose deepCSV btags}"),
          "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags>=1&&pfmet>125&&mt_met_lep>50"&& WHLeptons==1, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);
    

    pm.Push<Hist1D>(Axis(4, 0, 4., WHLeptons, "N_{leptons}"),
          "nvetoleps>=1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50", sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);


    //ttbar selection
    pm.Push<Hist1D>(Axis(30, 0, 300., "mt_met_lep", "mT [GeV]"),
        "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125&&ngoodbtags>=1&&nloosebtags>=2"&& WHLeptons==1, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    pm.Push<Hist1D>(Axis(30, 0, 300., "mct", "m_{CT} [GeV]"),
        ttbar_sel, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    pm.Push<Hist1D>(Axis(30, 0, 300., "mbb", "m_{bb} [GeV]"),
      ttbar_sel, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    pm.Push<Hist1D>(Axis(25, 125, 500., "pfmet", " E_{T}^{miss}[GeV]"),
      ttbar_sel, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);   

    pm.Push<Hist1D>(Axis(25, 20, 120., "leps_pt[0]", "Lepton p_{T} [GeV]"),
      ttbar_sel, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);  

    pm.Push<Hist1D>(Axis(5, 2, 7., "ngoodjets", "N_{jets}"),
        "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=2&&pfmet>125&&ngoodbtags>=1&&nloosebtags>=2"&& WHLeptons==1, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);

    pm.Push<Hist1D>(Axis(4, 0, 4., "ngoodbtags", "N_{med deepCSV btags}"),
          "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125"&& WHLeptons==1, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);

    pm.Push<Hist1D>(Axis(4, 0, 4., "nloosebtags", "N_{loose deepCSV btags}"),
          "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125"&& WHLeptons==1, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);
    
    pm.Push<Hist1D>(Axis(4, 0, 4., "ngoodbtags", "N_{med deepCSV btags}"),
          "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125"&& WHLeptons==1, sample_list, all_plot_types).Weight("w_noBtagSF*w_BtagSF_medmed").Tag("medmed");


    pm.Push<Hist1D>(Axis(4, 0, 4., "nloosebtags", "N_{loose deepCSV btags}"),
          "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125&&ngoodbtags>=1"&& WHLeptons==1, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);
    

    pm.Push<Hist1D>(Axis(4, 0, 4., WHLeptons, "N_{leptons}"),
          "nvetoleps>=1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125", sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);
}

  pm.MakePlots(lumi);

}
