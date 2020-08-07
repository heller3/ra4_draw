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
  
  double lumicomb = 137.2;

  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/s16v3/";
  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/f17v2/";
  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/a18v1/";

  string data2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string data2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  
  string mc2018_ttbar_ext_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_06_12/a18v1/";
  
  string signal_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_07_09/";
  string FullSim_signal_dir = "/home/users/dspitzba/wh_babies/babies_v33_8_2020_07_16/";

  Palette colors("txt/colors.txt", "default");

  //Data
  auto data2016 = Process::MakeShared<Baby_full>("2016 Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto data2017 = Process::MakeShared<Baby_full>("2017 Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto data2018 = Process::MakeShared<Baby_full>("2018 Data", Process::Type::data, colors("data"),{data2018_dir+"slim_data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);
  auto dataComb = Process::MakeShared<Baby_full>("2016-2018 Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*.root",data2017_dir+"slim_data_2017*.root",data2018_dir+"slim_data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);

  //ttbar
  auto tt1l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*.root"},"stitch");
  auto ttbar_2016 = Process::MakeShared<Baby_full>("t#bar{t} 2016", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*",mc2016_dir+"slim*TTJets_2lep_*.root"},"stitch");

  auto tt1l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2017", Process::Type::background, colors("tt_1l"),{mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2017", Process::Type::background, colors("tt_2l"),{mc2017_dir+"slim*TTJets_2lep_*.root"},"stitch");
  auto ttbar_2017 = Process::MakeShared<Baby_full>("t#bar{t} 2017", Process::Type::background, colors("tt_2l"),{mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*",mc2017_dir+"slim*TTJets_2lep_*.root"},"stitch");

  auto tt1l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2018", Process::Type::background, colors("tt_1l"),{mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2018", Process::Type::background, colors("tt_2l"),{mc2018_dir+"slim*TTJets_2lep_a18v1*.root",mc2018_ttbar_ext_dir+"slim*"},"stitch");
  auto ttbar_2018 = Process::MakeShared<Baby_full>("t#bar{t} 2018", Process::Type::background, colors("tt_2l"),{mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_2lep_a18v1*.root",mc2018_ttbar_ext_dir+"slim*"},"stitch");

  auto tt1l_Comb = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016-2018", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*",mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_Comb = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016-2018", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_a18v1*.root"},"stitch");
  auto ttbar_Comb = Process::MakeShared<Baby_full>("t#bar{t} 2016-2018", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*",mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*",mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_a18v1*.root",mc2018_ttbar_ext_dir+"slim*"},"stitch");

  //WJets
  auto wjets_2016 = Process::MakeShared<Baby_full>("W+jets 2016", Process::Type::background, colors("wjets"), {mc2016_dir+"slim*_W*JetsToLNu_s16v3*",mc2016_dir+"slim*W*Jets_NuPt200_s16v*.root"},"stitch");
  auto wjets_2017 = Process::MakeShared<Baby_full>("W+jets 2017", Process::Type::background, colors("wjets"), {mc2017_dir+"slim*_W*JetsToLNu_f17v2*",mc2017_dir+"slim*W*Jets_NuPt200_f17v2*.root"},"stitch");
  auto wjets_2018 = Process::MakeShared<Baby_full>("W+jets 2018", Process::Type::background, colors("wjets"), {mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root"},"stitch");
  auto wjets_Comb = Process::MakeShared<Baby_full>("W+jets 2016-2018", Process::Type::background, colors("wjets"), {mc2016_dir+"slim*_W*JetsToLNu_s16v3*",mc2016_dir+"slim*W*Jets_NuPt200_s16v*.root",mc2017_dir+"slim_W*JetsToLNu_f17v2*",mc2017_dir+"slim*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root"},"stitch");

  //Single top
  auto single_t_2016 = Process::MakeShared<Baby_full>("Single t 2016", Process::Type::background, colors("single_t"), {mc2016_dir+"slim*_ST_*.root"});
  auto single_t_2017 = Process::MakeShared<Baby_full>("Single t 2017", Process::Type::background, colors("single_t"), {mc2017_dir+"slim*_ST_*.root"});
  auto single_t_2018 = Process::MakeShared<Baby_full>("Single t 2018", Process::Type::background, colors("single_t"), {mc2018_dir+"slim*_ST_*.root"});
  auto single_t_Comb = Process::MakeShared<Baby_full>("Single t 2016-2018", Process::Type::background, colors("single_t"), {mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"});

  //diboson
  auto diboson_2016 = Process::MakeShared<Baby_full>("Diboson 2016", Process::Type::background, colors("other"),{mc2016_dir+"slim*WW*.root", mc2016_dir+"slim*WZ*.root",mc2016_dir+"slim*ZZ*.root"});
  auto diboson_2017 = Process::MakeShared<Baby_full>("Diboson 2017", Process::Type::background, colors("other"),{mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*WZ*.root",mc2017_dir+"slim*ZZ*.root"});
  auto diboson_2018 = Process::MakeShared<Baby_full>("Diboson 2018", Process::Type::background, colors("other"),{mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"});
  auto diboson_Comb = Process::MakeShared<Baby_full>("Diboson 2016-2018", Process::Type::background, colors("other"),{mc2016_dir+"slim*WW*.root", mc2016_dir+"slim*WZ*.root",mc2016_dir+"slim*ZZ*.root",mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*WZ*.root",mc2017_dir+"slim*ZZ*.root",mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"});

  // ttV
  auto ttV_2016 = Process::MakeShared<Baby_full>("t#bar{t}V 2016", Process::Type::background, colors("ttv"),{mc2016_dir+"slim*_TTWJets*.root", mc2016_dir+"slim*_TTZ*.root"});
  auto ttV_2017 = Process::MakeShared<Baby_full>("t#bar{t}V 2017", Process::Type::background, colors("ttv"), {mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZ*.root"});
  auto ttV_2018 = Process::MakeShared<Baby_full>("t#bar{t}V 2018", Process::Type::background, colors("ttv"), {mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"});
  auto ttV_Comb = Process::MakeShared<Baby_full>("t#bar{t}V 2016-2018", Process::Type::background, colors("ttv"), {mc2016_dir+"slim*_TTWJets*.root", mc2016_dir+"slim*_TTZ*.root",mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZ*.root",mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"});

  //signals
  auto signal_comb_225_75 = Process::MakeShared<Baby_full>("2016-2018 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
    signal_comb_225_75->SetLineStyle(2);

  auto signal_comb_650_300 = Process::MakeShared<Baby_full>("2016-2018 Signal (650,300)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_*.root"},"pass&&mass_stop==650&&mass_lsp==300");
    signal_comb_650_300->SetLineStyle(3);

  auto signal_comb_700_1 = Process::MakeShared<Baby_full>("2016-2018 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");

  auto signal_comb_750_1_2016 = Process::MakeShared<Baby_full>("2016 Signal (750,1)", Process::Type::signal, colors("t1tttt"),{FullSim_signal_dir+"SMS_TChiWH_mCh750_mLSP1_s16v3/slim_*.root"},"pass&&mass_stop==750&&mass_lsp==1");
  auto signal_comb_750_1_2018 = Process::MakeShared<Baby_full>("2018 Signal (750,1)", Process::Type::signal, colors("t1tttt"),{FullSim_signal_dir+"SMS_TChiWH_mCh750_mLSP1_a18v1/slim_*.root"},"pass&&mass_stop==750&&mass_lsp==1");
  auto signal_comb_350_100_2016 = Process::MakeShared<Baby_full>("2016 Signal (350,100)", Process::Type::signal, colors("t1tttt"),{FullSim_signal_dir+"SMS_TChiWH_mCh350_mLSP100_s16v3/slim_*.root"},"pass&&mass_stop==350&&mass_lsp==100");
  auto signal_comb_350_100_2018 = Process::MakeShared<Baby_full>("2018 Signal (350,100)", Process::Type::signal, colors("t1tttt"),{FullSim_signal_dir+"SMS_TChiWH_mCh350_mLSP100_a18v1/slim_*.root"},"pass&&mass_stop==350&&mass_lsp==100");

  //List of processes

  vector<shared_ptr<Process> > sample_list_Comb = {signal_comb_700_1,signal_comb_650_300,signal_comb_225_75};
  vector<shared_ptr<Process> > FullSim_sample_list_Comb = {signal_comb_350_100_2016,signal_comb_350_100_2018,signal_comb_750_1_2016,signal_comb_750_1_2018};
  vector<shared_ptr<Process> > FullSim2016_sample_list_Comb = {tt1l_2016, tt2l_2016, wjets_2016,single_t_2016, diboson_2016, ttV_2016, signal_comb_350_100_2016,signal_comb_750_1_2016};
  vector<shared_ptr<Process> > FullSim2018_sample_list_Comb = {tt1l_2018, tt2l_2018, wjets_2018,single_t_2018, diboson_2018, ttV_2018, signal_comb_350_100_2018,signal_comb_750_1_2018};


  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
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
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info).FileExtensions({"pdf"});
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info).FileExtensions({"pdf"});
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info).FileExtensions({"pdf"});
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info).FileExtensions({"pdf"});
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info, log_shapes_info};

  //Need different PlotMaker for each luminosity.
    //For some reason, they needed to be pointers to go into vector later on..

  PlotMaker * pmComb = new PlotMaker();
  PlotMaker * pm2016 = new PlotMaker();
  PlotMaker * pm2018 = new PlotMaker();

  //Selections
  NamedFunc signalRegion       = "hasNano&&pass&&nvetoleps==1&&ngoodjets==2&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&ngoodbtags==2&&PassTrackVeto&&PassTauVeto" && WHLeptons==1;
  NamedFunc signalRegion_3jet  = "hasNano&&pass&&nvetoleps==1&&ngoodjets==3&&pfmet>125&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&ngoodbtags==2&&PassTrackVeto&&PassTauVeto" && WHLeptons==1 && LeadingNonBJetPt_med<300;
  NamedFunc signalRegion_noMbb       = "hasNano&&pass&&nvetoleps==1&&ngoodjets==2&&pfmet>125&&mct>200&&mt_met_lep>150&&ngoodbtags==2&&PassTrackVeto&&PassTauVeto" && WHLeptons==1;
  NamedFunc signalRegion_3jet_noMbb  = "hasNano&&pass&&nvetoleps==1&&ngoodjets==3&&pfmet>125&&mct>200&&mt_met_lep>150&&ngoodbtags==2&&PassTrackVeto&&PassTauVeto" && WHLeptons==1 && LeadingNonBJetPt_med<300;

  NamedFunc looseSignalRegion       = "hasNano&&pass&&nvetoleps==1&&ngoodjets==2&&pfmet>125&&ngoodbtags==2&&PassTrackVeto&&PassTauVeto" && WHLeptons==1;

  vector<NamedFunc> sels   = {signalRegion}; 
  vector<string> sels_tags = {"_signalRegion"}; 
  vector<PlotMaker *> pms = {pmComb,pm2016,pm2018};
  vector<vector<shared_ptr<Process> > > samples_Run2 = {sample_list_Comb,FullSim2016_sample_list_Comb,FullSim2018_sample_list_Comb};
  vector<string> years = {"yAll","y2016_FullSim","y2018_FullSim"};
  vector<NamedFunc> weights = {"weight * trig_eff"*nanoWeight*yearWeight};

  for(uint isel=0;isel<sels.size();isel++){
    for(uint iyear=0;iyear<samples_Run2.size();iyear++){
      for(uint iweight=0;iweight<weights.size();iweight++){

          pms[iyear]->Push<Hist1D>(Axis(10, 0., 500., "FatJet_pt_nom", "FatJet_pt_nom [GeV]"),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("FatJetStudy_"+years[iyear]+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(10, 0., 500., "FatJet_genPt", "FatJet_genPt"),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("FatJetStudy_"+years[iyear]+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(5, 0., 500., "pfmet", "pfmet"),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("FullSimStudy_"+years[iyear]+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(5, 0., 500., "mct", "mct"),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("FullSimStudy_"+years[iyear]+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(5, 0., 500., "mt_met_lep", "mt_met_lep"),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("FullSimStudy_"+years[iyear]+sels_tags[isel]);
          
          pms[iyear]->Push<Hist1D>(Axis(5, 30., 330., "mbb", "mbb"),
            sels[isel], samples_Run2[iyear], all_plot_types).Weight(weights[iweight]).Tag("FullSimStudy_"+years[iyear]+sels_tags[isel]);

      }
    }
  }

  pmComb->MakePlots(lumicomb);
  pm2016->MakePlots(lumicomb);
  pm2018->MakePlots(lumicomb);


}
