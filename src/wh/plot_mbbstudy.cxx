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

  string basic_cuts =            "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2";
  string basic_cuts_mct =        "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200";
  string basic_cuts_mct_mt =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150";
  string basic_cuts_mct_mbb =    "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150";
  string basic_cuts_mt_mbb =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>150&&mbb>90&&mbb<150";
  string basic_cuts_mt =         "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>150";
  string basic_cuts_mct_mt_mbb = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";

  
  Palette colors("txt/colors.txt", "default");
  auto no_b_wjets_high_nu = Process::MakeShared<Baby_full>("0b W+jets, nu pT >= 200", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.);
  auto one_b_wjets_high_nu = Process::MakeShared<Baby_full>("1b W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1);
  auto two_b_medmed_wjets_high_nu = Process::MakeShared<Baby_full>("2b W+jets, nu pT >= 200", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);

  auto two_b_medmed_wjets_high_nu_gluon_split = Process::MakeShared<Baby_full>("2b W+jets, nu pT >= 200, Gluon Splitting", Process::Type::background, colors("qcd"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV&&nEventsGluonSplit>0.);
  auto one_b_wjets_high_nu_gluon_split = Process::MakeShared<Baby_full>("1b W+jets, nu pT >= 200, Gluon Splitting", Process::Type::background, colors("t1tttt"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1);
  auto no_b_wjets_high_nu_gluon_split = Process::MakeShared<Baby_full>("0b W+jets, nu pT >= 200, Gluon Splitting", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.);
  
  auto no_b_wjets_high_nu_no_gluon_split = Process::MakeShared<Baby_full>("0b W+jets, nu pT >= 200", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.&&nEventsGluonSplit==0.); 
  auto one_b_wjets_high_nu_no_gluon_split = Process::MakeShared<Baby_full>("1b W+jets, nu pT >= 200, No Gluon Splitting", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1&&nEventsGluonSplit==0.);
  auto two_b_medmed_wjets_high_nu_no_gluon_split = Process::MakeShared<Baby_full>("2b W+jets, nu pT >= 200, No Gluon Splitting", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV&&nEventsGluonSplit==0.);

  auto exactly_one_b_wjets_high_nu = Process::MakeShared<Baby_full>("1b W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1);

  auto tchiwh_225_75 = Process::MakeShared<Baby_full>("TChiWH(225,75)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==225&&mass_lsp==75");
    tchiwh_225_75->SetLineStyle(1);
  auto tchiwh_225_150 = Process::MakeShared<Baby_full>("TChiWH(225,150)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==225&&mass_lsp==150");
    tchiwh_225_75->SetLineStyle(2);
  auto tchiwh_250_1 = Process::MakeShared<Baby_full>("TChiWH(250,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==250&&mass_lsp==1");
    tchiwh_250_1->SetLineStyle(3);
  auto tchiwh_700_1 = Process::MakeShared<Baby_full>("TChiWH(700,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==700&&mass_lsp==1");
    tchiwh_700_1->SetLineStyle(4);
  auto tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_1lep*ext1*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto tt2l = Process::MakeShared<Baby_full>("t#bar{t} (2l)", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto wjets_low_nu = Process::MakeShared<Baby_full>("W+jets, nu pT < 200", Process::Type::background, colors("qcd"),
    {mc_dir+"*slim_W*JetsToLNu_s16v3*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV&&NHighPtNu==0.);
  auto wjets_high_nu = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);

  vector<shared_ptr<Process> > wjets_medmed_sample_list = {two_b_medmed_wjets_high_nu,one_b_wjets_high_nu,no_b_wjets_high_nu};
  vector<shared_ptr<Process> > short_gluon_sample_list = {two_b_medmed_wjets_high_nu_no_gluon_split,one_b_wjets_high_nu_no_gluon_split,two_b_medmed_wjets_high_nu_gluon_split,one_b_wjets_high_nu_gluon_split};
  vector<shared_ptr<Process> > full_gluon_sample_list = {two_b_medmed_wjets_high_nu_no_gluon_split,one_b_wjets_high_nu_no_gluon_split,no_b_wjets_high_nu_no_gluon_split,two_b_medmed_wjets_high_nu_gluon_split,one_b_wjets_high_nu_gluon_split,no_b_wjets_high_nu_gluon_split};
  vector<shared_ptr<Process> > short_sample_list = {two_b_medmed_wjets_high_nu,one_b_wjets_high_nu};
  vector<shared_ptr<Process> > exactly_wjets_medmed_sample_list = {two_b_medmed_wjets_high_nu,exactly_one_b_wjets_high_nu,no_b_wjets_high_nu};
  vector<shared_ptr<Process> > sample_list = {tchiwh_250_1, tchiwh_225_75, tchiwh_225_150, tchiwh_700_1, tt1l, tt2l, wjets_low_nu, wjets_high_nu};

  vector<string> mbb_cuts_vector = {"mbb>90&&mbb<150","mbb>0","mbb>300","mbb<90"};
  vector<string> mbb_cuts_tags = {"MBB_WINDOW_","NO_MBB_CUT_","HIGH_MBB_","LOW_MBB_"};

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
  pm.Push<Hist1D>(Axis(50, 0, 500., "mbb", "N_{b quarks} in Event"),
            "pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_nBs_NO_MBB_CUT_");

  pm.Push<Hist1D>(Axis(6, 0, 6., nGenBs, "N_{b quarks} in Event"),
            "pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_nBs_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(6, 0, 6., nGenBsFromGluons, "N_{b quarks} from Gluons in Event"),
            "pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_nBsfromGlu_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(6, 0, 6., nGenBs_ptG15, "N_{b quarks} in Event"),
            "pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_nBs_NO_MBB_CUT_ptg15_");
  pm.Push<Hist1D>(Axis(6, 0, 6., nGenBsFromGluons_ptG15, "N_{b quarks} from Gluons in Event"),
            "pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_nBsfromGlu_NO_MBB_CUT_ptg15_");
  pm.Push<Hist1D>(Axis(6, 0, 6., nGenBs_ptG30, "N_{b quarks} in Event"),
            "pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_nBs_NO_MBB_CUT_ptg30_");
  pm.Push<Hist1D>(Axis(6, 0, 6., nGenBsFromGluons_ptG30, "N_{b quarks} from Gluons in Event"),
            "pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_nBsfromGlu_NO_MBB_CUT_ptg30_");
  pm.Push<Hist1D>(Axis(13, 0, 390., genBpT, "p_{T} of gen-level b"),
            "pfmet>200&&mct>200&&mt_met_lep>150", short_gluon_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_genBpT_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(13, 0, 390., genB_leadingpT, "p_{T} of leading gen-level b"),
            "pfmet>200&&mct>200&&mt_met_lep>150"&&nGenBs>0., short_gluon_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_genB_leadingpT_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(13, 0, 390., genB_subleadingpT, "p_{T} of subleading gen-level b"),
            "pfmet>200&&mct>200&&mt_met_lep>150"&&nGenBs>0., short_gluon_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_genB_subleadingpT_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(6, 0, 3., bDeltaR, "dR of bb"),
            "pfmet>200&&mct>200&&mt_met_lep>150", short_gluon_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_bDeltaR_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(30, -5, 25., bMother, "pdgID of b's Mother"),
            "pfmet>200&&mct>200&&mt_met_lep>150", short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_bMother_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(30, -5, 25., bMother_pt15, "pdgID of b's Mother"),
            "pfmet>200&&mct>200&&mt_met_lep>150", short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_bMother_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(30, -5, 25., bMother_pt30, "pdgID of b's Mother"),
            "pfmet>200&&mct>200&&mt_met_lep>150", short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_bMother_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(20, 0, 4., bmetMinDeltaPhi, "min dPhi (b,MET)"),
            "pfmet>200&&mct>200&&mt_met_lep>150"&&bmetMinDeltaPhi<10., short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_mindPhi_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(20, 0, 4., bmetMinDeltaPhi, "min dPhi (b,MET)"),
            "pfmet>200&&mct>200&&mt_met_lep>150"&&bmetMinDeltaPhi<10., sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_backgrounds_medmed_mindPhi_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(15, 0, 3., bDeltaPhi, "dPhi (b1,b2)"),
            "pfmet>200&&mct>200&&mt_met_lep>150"&&bDeltaPhi>0., short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_dPhi_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(15, 0, 3., bDeltaPhi, "dPhi (b1,b2)"),
            "pfmet>200&&mct>200&&mt_met_lep>150"&&bDeltaPhi>0., sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_backgrounds_medmed_dPhi_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		        "pfmet>200&&mct>200&&mt_met_lep>150", full_gluon_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background__wjets_medmed_mbb_full_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(6, 0, 6., nHeavy, "N (b or c jets) in Event"),
            "pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_nHeavy_NO_MBB_CUT_");
  pm.Push<Hist1D>(Axis(6, 0, 6., nLight, "N (u, d, or s jets) in Event"),
            "pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_nLight_NO_MBB_CUT_");

  pm.Push<Hist1D>(Axis(8, 30, 510., "mbb", "M_{b#bar{b}}"),
		        "pfmet>200&&mct>200&&mt_met_lep>150"&&bDeltaPhi<1.6, wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background__wjets_medmed_mbbwithdphi_full_NO_MBB_CUT_");

  for(unsigned j(0); j<mbb_cuts_vector.size(); j++){
      pm.Push<Hist1D>(Axis(25, 0, 500., sortedJetsPt_Leading, "Leading Jet p_{T} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_Distributions_background_wjets_medmed_sortedJetsPt_Leading_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(25, 0, 500., sortedJetsPt_subLeading, "Subleading Jet p_{T} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_Distributions_background_wjets_medmed_sortedJetsPt_subLeading_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(25, 0, 500., sortedJetsCSV_Leading, "High CSV Jet p_{T} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_Distributions_background_wjets_medmed_sortedJetsCSV_Leading_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(25, 0, 500., sortedJetsCSV_subLeading, "Low CSV Jet p_{T} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_Distributions_background_wjets_medmed_sortedJetsCSV_subLeading_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(10, 0, 2., sortedJetsCSV_deltaR, "deltaR"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_Distributions_background_wjets_medmed_deltaR_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(25, 0, 500, "ptbb", "p_{T,bb} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_Distributions_background_wjets_medmedptbb"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(50, -500, 500., sortedJetsPt_Leading-sortedJetsPt_subLeading, "Leading Jet p_{T} - Subleading Jet p_{T} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_Distributions_background_wjets_medmed_sortedJetsPt_deltaPt_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(50, -500, 500., sortedJetsCSV_Leading-sortedJetsCSV_subLeading, "High CSV Jet p_{T} - Low CSV Jet p_{T} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_Distributions_background_wjets_medmed_sortedJetsCSV_deltaPt_"+mbb_cuts_tags[j]);
  }

  for(unsigned j(0); j<mbb_cuts_vector.size(); j++){
      pm.Push<Hist1D>(Axis(10, 0, 500., sortedJetsPt_Leading, "Leading Jet p_{T} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_sortedJetsPt_Leading_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(10, 0, 500., sortedJetsPt_subLeading, "Subleading Jet p_{T} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_sortedJetsPt_subLeading_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(10, 0, 500., sortedJetsCSV_Leading, "High CSV Jet p_{T} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_sortedJetsCSV_Leading_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(10, 0, 500., sortedJetsCSV_subLeading, "Low CSV Jet p_{T} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_sortedJetsCSV_subLeading_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(10, 0, 2., sortedJetsCSV_deltaR, "deltaR"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_deltaR_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(10, 0, 500, "ptbb", "p_{T,bb} [GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_ptbb_"+mbb_cuts_tags[j]); 
      pm.Push<Hist1D>(Axis(20, -500, 500., sortedJetsPt_Leading-sortedJetsPt_subLeading, "Leading Jet p_{T} - Subleading Jet p_{T}[GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_sortedJetsPt_deltaPt_"+mbb_cuts_tags[j]);
      pm.Push<Hist1D>(Axis(20, -500, 500., sortedJetsCSV_Leading-sortedJetsCSV_subLeading, "High CSV Jet p_{T} - Low CSV Jet p_{T}[GeV]"),
	                mbb_cuts_vector[j]&&"pfmet>200&&mct>200&&mt_met_lep>150", wjets_medmed_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("Coarse_compositionStudy_Distributions_background_wjets_medmed_sortedJetsCSV_deltaPt_"+mbb_cuts_tags[j]);
  }

  pm.MakePlots(lumi);
}
