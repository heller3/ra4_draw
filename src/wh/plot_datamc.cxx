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

  string data_dir = "/home/users/rheller/wh_babies/babies_2019_01_08/";
  string mc_dir = "/home/users/rheller/wh_babies/babies_2019_01_07/";
  string signal_dir = "/home/users/rheller/wh_babies/babies_2019_01_09/";
  
  Palette colors("txt/colors.txt", "default");

  auto data = Process::MakeShared<Baby_full>("Data", Process::Type::data, colors("data"),{data_dir+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1)");
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


  vector<shared_ptr<Process> > sample_list = {data, tchiwh_250_1, tchiwh_225_75, tt1l, tt2l, wjets, single_t, ttv, diboson};
  vector<shared_ptr<Process> > short_sample_list = {tchiwh_250_1, tchiwh_225_75, tt1l, tt2l, wjets};

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
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info,lin_shapes_info};


//"ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>150"
  NamedFunc preselection = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV && WHLeptons==1;
  NamedFunc preselectionNoBTagging = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && WHLeptons==1;
  NamedFunc preselectionNoB3Jet = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&mt_met_lep>50" && WHLeptons==1;
  //NamedFunc cr2l_1lep = "!PassTrackVeto&&!PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1;
  //NamedFunc cr2l_2lep = "nvetoleps==2&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1;
  NamedFunc cr2l = "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1;
  NamedFunc signalRegion = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mct>150&&mt_met_lep>150"&& WHLeptons==1;
  NamedFunc signalRegionNoMct = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150"&& WHLeptons==1;
  NamedFunc signalRegionMedMedDeepCSV = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150"&& WHLeptons==1&&HasMedMedDeepCSV;
  NamedFunc signalRegionMedLooseDeepCSV = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150"&& WHLeptons==1&&HasMedLooseDeepCSV;
  NamedFunc signalRegionLooseLooseDeepCSV = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150"&& WHLeptons==1&&HasLooseLooseDeepCSV;
  NamedFunc noCuts = true;


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
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "els_pt", "Electron p_{T}"),
		  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mus_pt", "Muon p_{T}"),
		  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "ak4pfjets_pt", "Jet p_{T}"),
		  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(12, -3, 3, "els_eta", "Electron #eta"),
		  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(12, -3, 3, "mus_eta", "Muon #eta"),
		  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(12, -3, 3, "ak4pfjets_eta", "Jet #eta"),
		  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(16, -4, 4, "els_phi", "Electron #phi"),
		  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(16, -4, 4, "mus_phi", "Muon #phi"),
		  preselection, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(16, -4, 4, "ak4pfjets_phi", "Jet #phi"),
		  preselection, sample_list, all_plot_types);
  //pm.Push<Hist1D>(Axis(3, 0, 3, WHLeptons,"1 good lep"), "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV && WHLeptons>=1, sample_list, all_plot_types);
  //pm.Push<Hist1D>(Axis(3, 0, 3, WHLeptons,"2nd lep veto"), "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(2, 0, 2, "PassTrackVeto","Track Veto"),
		  "nvetoleps==1&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(2, 0, 2, "PassTauVeto","Tau Veto"),
		  "nvetoleps==1&&PassTrackVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(10, 0, 10, "ngoodjets","nGoodJets"),
		  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(2, 0, 2, HasMedLooseCSV,"MedLooseCSV"),
		  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet","E_{T}^{miss} [GeV]"),
		  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>50" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep","m_{T} [GeV]"),
		  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);


  pm.Push<Hist1D>(Axis(10, 0, 1, "ak4pfjets_deepCSV","deepCSV"),
		  noCuts, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(10, 0, 1, "ak4pfjets_deepCSV","deepCSV"),
		  preselectionNoBTagging, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(10, 0, 1, "ak4pfjets_deepCSV","deepCSV"),
		  signalRegion, short_sample_list, all_plot_types);

  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepMedBTagged, "N_{medium jets}"),
		  noCuts, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepLooseBTagged, "N_{loose jets}"),
		  noCuts, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepMedBTagged, "N_{medium jets}"),
		  preselectionNoBTagging, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepLooseBTagged, "N_{loose jets}"),
		  preselectionNoBTagging, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepMedBTagged, "N_{medium jets}"),
		  preselectionNoB3Jet, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepLooseBTagged, "N_{loose jets}"),
		  preselectionNoB3Jet, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepMedBTagged, "N_{medium jets}"),
		  signalRegion, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepLooseBTagged, "N_{loose jets}"),
		  signalRegion, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepMedBTagged, "N_{medium jets}"),
		  signalRegionNoMct, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepLooseBTagged, "N_{loose jets}"),
		  signalRegionNoMct, short_sample_list, all_plot_types);

  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
		  signalRegionMedMedDeepCSV, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
		  signalRegionMedLooseDeepCSV, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
		  signalRegionLooseLooseDeepCSV, sample_list, all_plot_types);
  
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "p_{T}"),
		  WHLeptons==1&&"nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>170&&mt_met_lep>150&&mbb>=90&&mbb<=150"&&HasMedMedDeepCSV, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "p_{T}"),
		  WHLeptons==1&&"nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>170&&mt_met_lep>150&&mbb>=90&&mbb<=150"&&HasMedLooseDeepCSV, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "p_{T}"),
		  WHLeptons==1&&"nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>170&&mt_met_lep>150&&mbb>=90&&mbb<=150"&&HasLooseLooseDeepCSV, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "p_{T}"),
		  signalRegion&&HasMedMedDeepCSV, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "p_{T}"),
		  signalRegion&&HasMedLooseDeepCSV, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "p_{T}"),
		  signalRegion&&HasLooseLooseDeepCSV, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "p_{T}"),
		  "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mbb>90&&mbb<150&&mct>150&&mt_met_lep>150"&& WHLeptons==1&&HasMedLooseDeepCSV, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "p_{T}"),
		  "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mbb>90&&mbb<150&&mct>150&&mt_met_lep>150"&& WHLeptons==1&&HasLooseLooseDeepCSV, short_sample_list, all_plot_types);
		  
  //use these for efficiency plot
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "b jet p_{T}"),
		  noCuts, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "b jet p_{T}"),
		  HasMedMedDeepCSV, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "b jet p_{T}"),
		  HasMedLooseDeepCSV, short_sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., bJetPt, "b jet p_{T}"),
		  HasLooseLooseDeepCSV, short_sample_list, all_plot_types);
  
  /*pm.Push<Hist1D>(Axis(7, 0, 7, "ngoodjets", "N_{jets}"),
    "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV, sample_list, all_plot_types);*/

  pm.Push<Hist1D>(Axis(25, 0, 500., "leps_pt[0]", "lep p_{T} [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., LeadingBJetPt, "leading b jet p_{T} [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., SubLeadingBJetPt, "subleading b jet p_{T} [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
                  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "els_pt", "Electron p_{T}"),
		  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mus_pt", "Muon p_{T}"),
		  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "ak4pfjets_pt", "Jet p_{T}"),
		  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(12, -3, 3, "els_eta", "Electron #eta"),
		  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(12, -3, 3, "mus_eta", "Muon #eta"),
		  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(12, -3, 3, "ak4pfjets_eta", "Jet #eta"),
		  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(16, -4, 4, "els_phi", "Electron #phi"),
		  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(16, -4, 4, "mus_phi", "Muon #phi"),
		  cr2l, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(16, -4, 4, "ak4pfjets_phi", "Jet #phi"),
		  cr2l, sample_list, all_plot_types);
  //pm.Push<Hist1D>(Axis(3, 0, 3, WHLeptons,"1 good lep"), "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2&&PassTrackVeto&&PassTauVeto))&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons>=1, sample_list, all_plot_types);
  //pm.Push<Hist1D>(Axis(3, 0, 3, WHLeptons,"2nd lep veto"), "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(2, 0, 2, "PassTrackVeto","Track Veto"),
		  "nvetoleps==2&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(2, 0, 2, "PassTauVeto","Tau Veto"),
		  "nvetoleps==2&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(10, 0, 10, "ngoodjets","nGoodJets"),
		  "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(2, 0, 2, HasMedLooseCSV,"MedLooseCSV"),
		 "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet","E_{T}^{miss} [GeV]"),
		  "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep","m_{T} [GeV]"),
		  "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&pfmet>125" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);

  
  /*pm.Push<Hist1D>(Axis(25, 0, 500., "leps_pt[0]", "lep p_{T} [GeV]"),
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
  cr2l_2lep, sample_list, all_plot_types);*/

  
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
