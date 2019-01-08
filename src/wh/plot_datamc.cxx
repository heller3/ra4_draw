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

using namespace std;
using namespace PlotOptTypes;

NamedFunc HasMedLooseCSV("HasMedLooseCSV",[](const Baby &b) -> NamedFunc::ScalarType{
    int nloose=0;
    int nmedium=0;
    for (unsigned i(0); i<b.ak4pfjets_CSV()->size(); i++){
      if (b.ak4pfjets_CSV()->at(i) > 0.5426) nloose++;
      if (b.ak4pfjets_CSV()->at(i) > 0.8484) nmedium++;
    }
    if(nloose>=2 && nmedium>=1) return 1;
    else return 0;
  });

NamedFunc LeadingBJetPt("LeadingBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
    float maxpt=0;
    for (unsigned i(0); i<b.ak4pfjets_CSV()->size(); i++){
      if (b.ak4pfjets_CSV()->at(i) > 0.5426 && b.ak4pfjets_pt()->at(i) > maxpt) maxpt =  b.ak4pfjets_pt()->at(i);
    }
    return maxpt;
  });

NamedFunc SubLeadingBJetPt("SubLeadingBJetPt",[](const Baby &b) -> NamedFunc::ScalarType{
    float maxpt=0;
    float secondmax=0;
    for (unsigned i(0); i<b.ak4pfjets_CSV()->size(); i++){
      if (b.ak4pfjets_CSV()->at(i) > 0.5426 && b.ak4pfjets_pt()->at(i) > secondmax && b.ak4pfjets_pt()->at(i)<=maxpt){
        secondmax = b.ak4pfjets_pt()->at(i);
      }
      else if (b.ak4pfjets_CSV()->at(i) > 0.5426 && b.ak4pfjets_pt()->at(i) > maxpt){
        secondmax=maxpt;
        maxpt =  b.ak4pfjets_pt()->at(i);
      }
    }
    return secondmax;
  });


int main(){
  gErrorIgnoreLevel = 6000;

  double lumi = 35.9;

  string mc_dir = "/home/users/rheller/wh_babies/babies_2019_01_07/";
  string signal_dir = "/home/users/rheller/wh_babies/babies_2019_01_06/";
  Palette colors("txt/colors.txt", "default");

  auto tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_1lep*ext1*.root"});
  auto tt2l = Process::MakeShared<Baby_full>("t#bar{t} (2l)", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*TTJets_2lep*ext1*.root"});
  auto wjets = Process::MakeShared<Baby_full>("W+jets", Process::Type::background, colors("wjets"),
    {mc_dir+"*W*JetsToLNu*.root"});
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


  vector<shared_ptr<Process> > sample_list = {tchiwh_250_1, tchiwh_225_75, tt1l, tt2l, wjets, single_t, ttv, diboson};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    //.Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .Bottom(BottomType::off)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info};


//"ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>150"
  NamedFunc preselection = "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV;
  NamedFunc cr2l_1lep = "ngoodleps==1&&!PassTrackVeto&&!PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV;
  NamedFunc cr2l_2lep = "ngoodleps==1&&nvetoleps==2&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV;
  NamedFunc cr2l = "((ngoodleps==1&&!PassTrackVeto&&!PassTauVeto)||(ngoodleps==1&&nvetoleps==2))&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV;
  NamedFunc signalRegion = "mbb>90&&mbb<150&&mct>170&&mt_met_lep>150";


  PlotMaker pm;

  //Preselection
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "leps_pt[0]", "Lepton p_{T} [GeV]"),
                  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., LeadingBJetPt, "Leading b-jet p_{T} [GeV]"),
                  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., SubLeadingBJetPt, "Subleading b-jet p_{T} [GeV]"),
                  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  preselection, sample_list, all_plot_types);

  pm.Push<Hist1D>(Axis(7, 0, 7, "ngoodjets", "N_{jets}"),
                  "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV,
		  sample_list, all_plot_types);

  pm.Push<Hist1D>(Axis(25, 0, 500., "leps_pt[0]", "lep p_{T} [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., LeadingBJetPt, "leading b jet p_{T} [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., SubLeadingBJetPt, "subleading b jet p_{T} [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T}(l;#nu) [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
                  cr2l, sample_list, all_plot_types);
  
  pm.Push<Hist1D>(Axis(25, 0, 500., "leps_pt[0]", "lep p_{T} [GeV]"),
                  cr2l_1lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., LeadingBJetPt, "leading b jet p_{T} [GeV]"),
                  cr2l_1lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., SubLeadingBJetPt, "subleading b jet p_{T} [GeV]"),
                  cr2l_1lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  cr2l_1lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T}(l;#nu) [GeV]"),
                  cr2l_1lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  cr2l_1lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
                  cr2l_1lep, sample_list, all_plot_types);

  pm.Push<Hist1D>(Axis(25, 0, 500., "leps_pt[0]", "lep p_{T} [GeV]"),
                  cr2l_2lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., LeadingBJetPt, "leading b jet p_{T} [GeV]"),
                  cr2l_2lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., SubLeadingBJetPt, "subleading b jet p_{T} [GeV]"),
                  cr2l_2lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  cr2l_2lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T}(l;#nu) [GeV]"),
                  cr2l_2lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  cr2l_2lep, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
                  cr2l_2lep, sample_list, all_plot_types);

  //Signal region
 /* pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  preselection&&signalRegion, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  preselection&&signalRegion, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "leps_pt[0]", "Lepton p_{T} [GeV]"),
                  preselection&&signalRegion, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., LeadingBJetPt, "Leading b-jet p_{T} [GeV]"),
                  preselection&&signalRegion, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., SubLeadingBJetPt, "Subleading b-jet p_{T} [GeV]"),
                  preselection&&signalRegion, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  preselection&&signalRegion, sample_list, all_plot_types);*/

 

  pm.MakePlots(lumi);

}
