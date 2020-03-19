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

  //string data_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_04_03/";
  string mc_dir = "/home/users/rheller/wh_babies/babies_v30_3_2019_09_13/";
  //string signal_dir = "/home/users/rheller/wh_babies/babies_signal_2019_04_03/";

  string basic_cuts =            "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2";
  string basic_cuts_mct =        "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200";
  string basic_cuts_mct_mt =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150";
  string basic_cuts_mct_mbb =    "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150";
  string basic_cuts_mt_mbb =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>150&&mbb>90&&mbb<150";
  string basic_cuts_mt =         "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>150";
  string basic_cuts_mct_mt_mbb = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";

  string signal_cuts =           "(ngoodjets==2||ngoodjets==3)&&ngoodleps==1&&((abs(leps_pdgid[0])==11)||(abs(leps_pdgid[0])==13&&abs(leps_eta[0])<2.1))&&PassTrackVeto&&PassTauVeto&&pass==1";

  
  Palette colors("txt/colors.txt", "default");
  auto W1_wjets_high_nu = Process::MakeShared<Baby_full>("W1 W+jets, nu pT >= 200", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*slim_W1Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto W2_wjets_high_nu = Process::MakeShared<Baby_full>("W2 W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W2Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto W3_wjets_high_nu = Process::MakeShared<Baby_full>("W3 W+jets, nu pT >= 200", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W3Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto W4_wjets_high_nu = Process::MakeShared<Baby_full>("W4 W+jets, nu pT >= 200", Process::Type::background, colors("qcd"),
    {mc_dir+"*slim_W4Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);

  auto ST_schan = Process::MakeShared<Baby_full>("Single top: s-channel", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*ST_schan*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto ST_tW = Process::MakeShared<Baby_full>("Single top: tW", Process::Type::background, colors("wjets"),
    {mc_dir+"*ST_tW*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto ST_tchan = Process::MakeShared<Baby_full>("Single top: t-channel", Process::Type::background, colors("qcd"),
    {mc_dir+"*ST_tchan*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);

  auto jetHT = Process::MakeShared<Baby_full>("JetHT", Process::Type::background, colors("qcd"),
    {mc_dir+"slim_data*jetht*.root"},basic_cuts);

  vector<shared_ptr<Process> > sample_list = {W1_wjets_high_nu,W2_wjets_high_nu,W3_wjets_high_nu,W4_wjets_high_nu};
  vector<shared_ptr<Process> > ST_sample_list = {ST_schan,ST_tW,ST_tchan};
  vector<shared_ptr<Process> > new_sample_list = {jetHT,ST_schan};

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

  pm.Push<Hist1D>(Axis(10, 0, 1000, "ak4_HT", "mht"),
            "HLT_PFHT_prescaled==1", new_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Test");

  pm.MakePlots(lumi);

}
