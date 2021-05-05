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
  
  double lumicomb = 137;

  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/s16v3/";
  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/f17v2/";
  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/a18v1/";

  string data2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string data2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  
  string mc2018_ttbar_ext_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_06_12/a18v1/";
  
  string signal_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_07_09/";

  Palette colors("txt/colors.txt", "default");

  //Data
  auto dataComb = Process::MakeShared<Baby_full>("Observed", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*.root",data2017_dir+"slim_data_2017*.root",data2018_dir+"slim_data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);

  //ttbar
  auto ttbar_Comb = Process::MakeShared<Baby_full>("t#bar{t}", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*",mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*",mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_a18v1*.root",mc2018_ttbar_ext_dir+"slim*",mc2016_dir+"slim*_TTWJets*.root", mc2016_dir+"slim*_TTZ*.root",mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZ*.root",mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"},"stitch");

  //WJets
  auto wjets_Comb = Process::MakeShared<Baby_full>("W Boson", Process::Type::background, colors("wjets"), {mc2016_dir+"slim*_W*JetsToLNu_s16v3*",mc2016_dir+"slim*W*Jets_NuPt200_s16v*.root",mc2017_dir+"slim_W*JetsToLNu_f17v2*",mc2017_dir+"slim*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root",mc2016_dir+"slim*WW*.root", mc2016_dir+"slim*WZ*.root",mc2016_dir+"slim*ZZ*.root",mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*WZ*.root",mc2017_dir+"slim*ZZ*.root",mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"},"stitch&&evt!=74125994");

  //Single top
  auto single_t_Comb = Process::MakeShared<Baby_full>("Single t", Process::Type::background, colors("qcd"), {mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"});

  //SM WH
  auto smWH_Comb = Process::MakeShared<Baby_full>("SM WH", Process::Type::background, colors("dy"), {mc2016_dir+"slim_WminusH*.root", mc2016_dir+"slim_WplusH*.root",mc2017_dir+"slim_WminusH*.root", mc2017_dir+"slim_WplusH*.root",mc2018_dir+"slim_WminusH*.root", mc2018_dir+"slim_WplusH*.root"});

  //signals
  auto signal_comb_225_75 = Process::MakeShared<Baby_full>("(225,75)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
    signal_comb_225_75->SetLineStyle(3);

  auto signal_comb_425_150 = Process::MakeShared<Baby_full>("(425,150)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_*.root"},"pass&&mass_stop==425&&mass_lsp==150");
    signal_comb_425_150->SetLineStyle(2);

  auto signal_comb_800_100 = Process::MakeShared<Baby_full>("(800,100)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_*.root"},"pass&&mass_stop==800&&mass_lsp==100");

  //List of processes

  vector<shared_ptr<Process> > blind_list_Comb = {ttbar_Comb, single_t_Comb, wjets_Comb,smWH_Comb, signal_comb_800_100,signal_comb_425_150,signal_comb_225_75};


  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::simulation_preliminary)
    //.Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm)
    .RatioMinimum(0).RatioMaximum(2.0)
    .FileExtensions({"pdf"});
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear).FileExtensions({"pdf"});
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .Bottom(BottomType::ratio)
    .ShowBackgroundError(false)
    .FileExtensions({"pdf"});
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear).FileExtensions({"pdf"});
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::simulation_preliminary).FileExtensions({"pdf"});
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::simulation_preliminary).FileExtensions({"pdf"});
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::simulation_preliminary).FileExtensions({"pdf"});
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::simulation_preliminary).FileExtensions({"pdf"});
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info, log_shapes_info};

  //Need different PlotMaker for each luminosity.
    //For some reason, they needed to be pointers to go into vector later on..

  PlotMaker * pmComb = new PlotMaker();

  //Selections
  NamedFunc preselection        = "hasNano&&pass&&nvetoleps==1&&ngoodjets==2&&ngoodbtags==2&&pfmet>125&&mt_met_lep>50&&PassTrackVeto&&PassTauVeto" && WHLeptons==1;

  NamedFunc signalRegion        = "hasNano&&pass&&nvetoleps==1&&ngoodjets==2&&ngoodbtags==2&&pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&PassTrackVeto&&PassTauVeto" && WHLeptons==1 && nFatJet250>=1;
  NamedFunc signalRegion_3jet   = "hasNano&&pass&&nvetoleps==1&&(ngoodjets==2||ngoodjets==3)&&ngoodbtags==2&&pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&PassTrackVeto&&PassTauVeto" && WHLeptons==1 && LeadingNonBJetPt_med<300;

  NamedFunc signalRegion_noJet        = "hasNano&&pass&&nvetoleps==1&&ngoodbtags==2&&pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&PassTrackVeto&&PassTauVeto" && WHLeptons==1 && nFatJet250>=1;

  NamedFunc signalRegion_noMET        = "hasNano&&pass&&nvetoleps==1&&ngoodjets==2&&ngoodbtags==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&PassTrackVeto&&PassTauVeto" && WHLeptons==1;
  NamedFunc signalRegion_3jet_noMET   = "hasNano&&pass&&nvetoleps==1&&(ngoodjets==2||ngoodjets==3)&&ngoodbtags==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&PassTrackVeto&&PassTauVeto" && WHLeptons==1 && LeadingNonBJetPt_med<300;

  NamedFunc signalRegion_noMCT        = "hasNano&&pass&&nvetoleps==1&&ngoodjets==2&&ngoodbtags==2&&pfmet>125&&mt_met_lep>150&&mbb>90&&mbb<150&&PassTrackVeto&&PassTauVeto" && WHLeptons==1;
  NamedFunc signalRegion_3jet_noMCT   = "hasNano&&pass&&nvetoleps==1&&(ngoodjets==2||ngoodjets==3)&&ngoodbtags==2&&pfmet>125&&mt_met_lep>150&&mbb>90&&mbb<150&&PassTrackVeto&&PassTauVeto" && WHLeptons==1 && LeadingNonBJetPt_med<300;

  NamedFunc signalRegion_noMT        = "hasNano&&pass&&nvetoleps==1&&ngoodjets==2&&ngoodbtags==2&&pfmet>125&&mct>200&&mbb>90&&mbb<150&&PassTrackVeto&&PassTauVeto" && WHLeptons==1;
  NamedFunc signalRegion_3jet_noMT   = "hasNano&&pass&&nvetoleps==1&&(ngoodjets==2||ngoodjets==3)&&ngoodbtags==2&&pfmet>125&&mct>200&&mbb>90&&mbb<150&&PassTrackVeto&&PassTauVeto" && WHLeptons==1 && LeadingNonBJetPt_med<300;

  NamedFunc signalRegion_noMBB        = "hasNano&&pass&&nvetoleps==1&&ngoodjets==2&&ngoodbtags==2&&pfmet>125&&mct>200&&mt_met_lep>150&&PassTrackVeto&&PassTauVeto" && WHLeptons==1;
  NamedFunc signalRegion_3jet_noMBB   = "hasNano&&pass&&nvetoleps==1&&(ngoodjets==2||ngoodjets==3)&&ngoodbtags==2&&pfmet>125&&mct>200&&mt_met_lep>150&&PassTrackVeto&&PassTauVeto" && WHLeptons==1 && LeadingNonBJetPt_med<300;


  vector<double> met_bins = {125,200,300,400,500};
  vector<double> mbb_bins = {60,90,100,110,120,130,140,150,180,210,240,270,300,330,360,390,420,450,480,510};
  vector<double> higgs_bins = {0,1,2};

  vector<NamedFunc> sels   = {signalRegion,signalRegion_3jet,signalRegion_noMET,signalRegion_3jet_noMET,signalRegion_noMCT,signalRegion_3jet_noMCT,signalRegion_noMT,signalRegion_3jet_noMT,signalRegion_noMBB,signalRegion_3jet_noMBB,signalRegion_noJet}; 
  vector<string> sels_tags = {"_SR","_SR_3jet","_SR_noMET","_SR_3jet_noMET","_SR_noMCT","_SR_3jet_noMCT","_SR_noMT","_SR_3jet_noMT","_SR_noMBB","_SR_3jet_noMBB","_SR_noJet"}; 
  vector<PlotMaker *> pms = {pmComb};
  vector<vector<shared_ptr<Process> > > samples_Run2 = {blind_list_Comb};
  vector<string> years = {"yAll"};
  vector<NamedFunc> weights = {"weight * trig_eff"*nanoWeight*yearWeight};

  for(uint isel=0;isel<sels.size();isel++){
    for(uint iyear=0;iyear<samples_Run2.size();iyear++){
      for(uint iweight=0;iweight<weights.size();iweight++){

          pms[iyear]->Push<Hist1D>(Axis(10, 0., 500., "pfmet", "p^{miss}_{T} [GeV]", {200.}),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).RatioTitle("Obs.","Sim.").Tag("PAS__blind_ext_"+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(15, 125., 500., "pfmet", "p^{miss}_{T} [GeV]", {125.,200.,300.,400.}),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).RatioTitle("Obs.","Sim.").Tag("PAS__blind_"+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(8, 0., 400., "mct", "m_{CT} [GeV]", {200.}),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).RatioTitle("Obs.","Sim.").Tag("PAS__blind_"+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(6, 0., 300., "mt_met_lep", "m_{T} [GeV]", {150.}),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).RatioTitle("Obs.","Sim.").Tag("PAS__blind_"+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(15,60,510, "mbb", "m_{b#bar{b}} [GeV]", {90.,150.}),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).RatioTitle("Obs.","Sim.").Tag("PAS__blind_"+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(higgs_bins, "nHiggs", "nHiggs"),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).RatioTitle("Obs.","Sim.").Tag("PAS__blind_"+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(10,0,1, FatJet_HighestMDHScore, "H tagging Discriminator Output"),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).RatioTitle("Obs.","Sim.").Tag("PAS__blind_"+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(5, 1.5, 6.5, "ngoodjets", "N_{jets}",{2.5,3.5}),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).RatioTitle("Obs.","Sim.").Tag("PAS__blind_"+sels_tags[isel]);
          

      }
    }
  }

  pmComb->MakePlots(lumicomb);


}
