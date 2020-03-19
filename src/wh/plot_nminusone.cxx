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

  string data_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_04_03/";
  string mc_dir = "/home/users/rheller/wh_babies/babies_v30_9_2019_04_03/";
  string signal_dir = "/home/users/rheller/wh_babies/babies_signal_2019_04_03/";

  string basic_cuts = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2";
  
  Palette colors("txt/colors.txt", "default");

  auto data = Process::MakeShared<Baby_full>("Data", Process::Type::data, colors("data"),{data_dir+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1)");
  auto tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_1lep*ext1*.root"});
  auto tt2l = Process::MakeShared<Baby_full>("t#bar{t} (2l)", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*TTJets_2lep*ext1*.root"});
  auto wjets = Process::MakeShared<Baby_full>("W+jets", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*JetsToLNu_s16v3*.root",mc_dir+"*slim_W*Jets_NuPt200*.root"});
  auto single_t = Process::MakeShared<Baby_full>("Single t", Process::Type::background, colors("single_t"),
    {mc_dir+"*_ST_*.root"});
  auto ttv = Process::MakeShared<Baby_full>("Diboson and t#bar{t}V", Process::Type::background, colors("qcd"),
    {mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root",mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"});

  auto tchiwh_300_100 = Process::MakeShared<Baby_full>("TChiWH(300,100)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==300&&mass_lsp==100");
    tchiwh_300_100->SetLineStyle(2);
  auto tchiwh_700_1 = Process::MakeShared<Baby_full>("TChiWH(700,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==700&&mass_lsp==1");

  vector<shared_ptr<Process> > sample_list = {tchiwh_700_1, tchiwh_300_100, tt1l, tt2l, wjets, single_t, ttv};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    //.Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .Bottom(BottomType::ratio)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info)
    .Bottom(BottomType::ratio);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info,lin_shapes_info,log_shapes_info};

  PlotMaker pm;

  //Preselection
  pm.Push<Hist1D>(Axis(10, 200, 400., "pfmet", "E_{T}^{miss} [GeV]"),
        basic_cuts&&HasMedMedDeepCSV&&WHLeptons==1&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("nminus1_");
  pm.Push<Hist1D>(Axis(20, 0, 400., "mt_met_lep", "m_{T} [GeV]"),
        basic_cuts&&HasMedMedDeepCSV&&WHLeptons==1&&"pfmet>200", sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("nminus1_");
  pm.Push<Hist1D>(Axis(20, 0, 400., "mct", "M_{CT} [GeV]"),
        basic_cuts&&HasMedMedDeepCSV&&WHLeptons==1&&"pfmet>200&&mt_met_lep>150", sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("nminus1_");
  pm.Push<Hist1D>(Axis(20, 0, 400., "mbb", "M_{b#bar{b}}"),
		    basic_cuts&&HasMedMedDeepCSV&&WHLeptons==1&&"mct>200&&mt_met_lep>150&&pfmet>200", sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("nminus1_");

  pm.MakePlots(lumi);

}
