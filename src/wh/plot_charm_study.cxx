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

  Palette colors("txt/colors.txt", "default");
  auto data = Process::MakeShared<Baby_full>("Data", Process::Type::data, colors("data"),{data_dir+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1)");
  auto tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_1lep*ext1*.root"});
  auto tt2l = Process::MakeShared<Baby_full>("t#bar{t} (2l)", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*TTJets_2lep*ext1*.root"});
  auto wjets_low_nu = Process::MakeShared<Baby_full>("W+jets, nu pT < 200", Process::Type::background, colors("qcd"),
    {mc_dir+"*slim_W*JetsToLNu_s16v3*.root"},NHighPtNu==0.);
  auto wjets_high_nu = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"});
  auto single_t = Process::MakeShared<Baby_full>("Single t", Process::Type::background, colors("single_t"),
    {mc_dir+"*_ST_*.root"});
  auto ttv = Process::MakeShared<Baby_full>("t#bar{t}V", Process::Type::background, colors("ttv"),
    {mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root"});
  auto diboson = Process::MakeShared<Baby_full>("Diboson", Process::Type::background, kViolet+2,
    {mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"});
  auto other = Process::MakeShared<Baby_full>("Other", Process::Type::background, colors("dy"),
    {mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root", mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"});

  auto tchiwh_225_75 = Process::MakeShared<Baby_full>("TChiWH(225,75)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==225&&mass_lsp==75");
    tchiwh_225_75->SetLineStyle(2);
  auto tchiwh_250_1 = Process::MakeShared<Baby_full>("TChiWH(250,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==250&&mass_lsp==1");
  auto tchiwh_700_1 = Process::MakeShared<Baby_full>("TChiWH(700,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==700&&mass_lsp==1");

  auto no_b_wjets_high_nu = Process::MakeShared<Baby_full>("0b W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.);
  auto one_b_wjets_high_nu = Process::MakeShared<Baby_full>("1b W+jets, nu pT >= 200", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1);
  
  auto signal_medmed_wjets_high_nu = Process::MakeShared<Baby_full>("2b W+jets, nu pT >= 200", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);

  auto two_ctight_wjets_high_nu = Process::MakeShared<Baby_full>("2c tight W+jets, nu pT >= 200", Process::Type::background, colors("t1tttt"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.);
  auto two_cmed_wjets_high_nu = Process::MakeShared<Baby_full>("2 medium c W+jets,nu pT >= 200", Process::Type::background, colors("t1tttt"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedBTagged<2.);
  auto two_ctightmedmed_wjets_high_nu = Process::MakeShared<Baby_full>("1 tight + 1 medium c, 2 medium CvB W+jets,nu pT >= 200", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.);
  auto two_cmedmed_wjets_high_nu = Process::MakeShared<Baby_full>("2 medium c, 2 medium CvB W+jets,nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.);
  auto two_ctightmed_wjets_high_nu = Process::MakeShared<Baby_full>("2 tight c, 2 medium CvB W+jets,nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.);
  auto two_ctighttight_wjets_high_nu = Process::MakeShared<Baby_full>("2 tight c, 2 tight CvB W+jets,nu pT >= 200", Process::Type::background, kViolet+2,
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.);

  auto no_b_single_t = Process::MakeShared<Baby_full>("0b Single t", Process::Type::background, colors("wjets"),
    {mc_dir+"*_ST_*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.);
  auto one_b_single_t = Process::MakeShared<Baby_full>("1b Single t", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*_ST_*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1);
  auto two_b_single_t = Process::MakeShared<Baby_full>("2b Single t", Process::Type::background, colors("dy"),
    {mc_dir+"*_ST_*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto two_ctight_single_t = Process::MakeShared<Baby_full>("2c tight Single t", Process::Type::background, colors("t1tttt"),
    {mc_dir+"*_ST_*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.);
    
  auto no_b_tt2l = Process::MakeShared<Baby_full>("0b t#bar{t} (2l)", Process::Type::background, colors("wjets"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.);
  auto one_b_tt2l = Process::MakeShared<Baby_full>("1b t#bar{t} (2l)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1);
  auto two_b_tt2l = Process::MakeShared<Baby_full>("2b t#bar{t} (2l)", Process::Type::background, colors("dy"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto two_ctight_tt2l = Process::MakeShared<Baby_full>("2 tight c t#bar{t} (2l)", Process::Type::background, colors("t1tttt"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.);
  auto two_cmed_tt2l = Process::MakeShared<Baby_full>("2 medium c t#bar{t} (2l)", Process::Type::background, colors("t1tttt"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedBTagged<2.);
  auto two_ctightmedmed_tt2l = Process::MakeShared<Baby_full>("1 tight + 1 medium c, 2 medium CvB t#bar{t} (2l)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.);
  auto two_cmedmed_tt2l = Process::MakeShared<Baby_full>("2 medium c, 2 medium CvB t#bar{t} (2l)", Process::Type::background, colors("wjets"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.);
  auto two_ctightmed_tt2l = Process::MakeShared<Baby_full>("2 tight c, 2 medium CvB t#bar{t} (2l)", Process::Type::background, colors("wjets"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.);
  auto two_ctighttight_tt2l = Process::MakeShared<Baby_full>("2 tight c, 2 tight CvB t#bar{t} (2l)", Process::Type::background, kViolet+2,
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.);

  auto no_b_compare_wjets_high_nu = Process::MakeShared<Baby_full>("0b W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.);
  auto no_b_compare_single_t = Process::MakeShared<Baby_full>("0b Single t", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*_ST_*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.);   
  auto no_b_compare_tt2l = Process::MakeShared<Baby_full>("0b t#bar{t} (2l)", Process::Type::background, colors("dy"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.);

  auto one_b_compare_wjets_high_nu = Process::MakeShared<Baby_full>("1b W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1.&&nDeepLooseBTagged==1.);
  auto one_b_compare_single_t = Process::MakeShared<Baby_full>("1b Single t", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*_ST_*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1.&&nDeepLooseBTagged==1.);   
  auto one_b_compare_tt2l = Process::MakeShared<Baby_full>("1b t#bar{t} (2l)", Process::Type::background, colors("dy"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1.&&nDeepLooseBTagged==1.);

  auto two_b_compare_wjets_high_nu = Process::MakeShared<Baby_full>("2b W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto two_b_compare_single_t = Process::MakeShared<Baby_full>("2b Single t", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*_ST_*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);   
  auto two_b_compare_tt2l = Process::MakeShared<Baby_full>("2b t#bar{t} (2l)", Process::Type::background, colors("dy"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);

  auto two_ctight_compare_wjets_high_nu = Process::MakeShared<Baby_full>("2c W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.);
  auto two_ctight_compare_single_t = Process::MakeShared<Baby_full>("2c Single t", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*_ST_*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.);   
  auto two_ctight_compare_tt2l = Process::MakeShared<Baby_full>("2c t#bar{t} (2l)", Process::Type::background, colors("dy"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.);

  auto tt2l_wjets = Process::MakeShared<Baby_full>("tt2l&wjets", Process::Type::background, colors("dy"),
    {mc_dir+"*TTJets_2lep*ext1*.root",mc_dir+"*slim_W*Jets_NuPt200*.root"});
  auto two_b_tt2l_wjets = Process::MakeShared<Baby_full>("2b tt2l,wjets", Process::Type::background, colors("dy"),
    {mc_dir+"*TTJets_2lep*ext1*.root",mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto two_ctight_tt2l_wjets = Process::MakeShared<Baby_full>("2 tight c tt2l,wjets", Process::Type::background, colors("t1tttt"),
    {mc_dir+"*TTJets_2lep*ext1*.root",mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.);
  auto two_ctightmedmed_tt2l_wjets = Process::MakeShared<Baby_full>("1 tight + 1 medium c, 2 medium CvB tt2l,wjets", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_2lep*ext1*.root",mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.);
  auto two_ctightmed_tt2l_wjets = Process::MakeShared<Baby_full>("2 tight c, 2 medium CvB tt2l,wjets", Process::Type::background, colors("wjets"),
    {mc_dir+"*TTJets_2lep*ext1*.root",mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.);
  auto two_ctighttight_tt2l_wjets = Process::MakeShared<Baby_full>("2 tight c, 2 tight CvB tt2l,wjets", Process::Type::background, kViolet+2,
    {mc_dir+"*TTJets_2lep*ext1*.root",mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.);

  vector<shared_ptr<Process> > sample_list = {tchiwh_225_75, tchiwh_700_1, tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson, other};
  vector<shared_ptr<Process> > short_sample_list = {tt2l, wjets_high_nu,tt2l_wjets};
  vector<shared_ptr<Process> > data_sample_list = {data, tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson, other};
  vector<shared_ptr<Process> > wjets_sample_list = {signal_medmed_wjets_high_nu,two_ctight_wjets_high_nu,one_b_wjets_high_nu,no_b_wjets_high_nu};
  vector<shared_ptr<Process> > single_t_sample_list = {two_b_single_t,two_ctight_single_t,one_b_single_t,no_b_single_t};
  vector<shared_ptr<Process> > tt2l_sample_list = {two_b_tt2l,two_ctight_tt2l,one_b_tt2l,no_b_tt2l};
  vector<shared_ptr<Process> > twoB_sample_list = {two_b_compare_wjets_high_nu,two_b_compare_single_t,two_b_compare_tt2l};
  vector<shared_ptr<Process> > oneB_sample_list = {one_b_compare_wjets_high_nu,one_b_compare_single_t,one_b_compare_tt2l};
  vector<shared_ptr<Process> > noB_sample_list = {no_b_compare_wjets_high_nu,no_b_compare_single_t,no_b_compare_tt2l};
  vector<shared_ptr<Process> > twoC_sample_list = {two_ctight_compare_wjets_high_nu,two_ctight_compare_single_t,two_ctight_compare_tt2l};
  vector<shared_ptr<Process> > cc_cr_tt2l_sample_list = {two_b_tt2l,two_ctight_tt2l,two_ctightmedmed_tt2l,two_ctightmed_tt2l,two_ctighttight_tt2l};
  vector<shared_ptr<Process> > cc_cr_wjets_sample_list = {signal_medmed_wjets_high_nu,two_ctight_wjets_high_nu,two_ctightmedmed_wjets_high_nu,two_ctightmed_wjets_high_nu,two_ctighttight_wjets_high_nu};
  vector<shared_ptr<Process> > cc_cr_tt2l_wjets_sample_list = {two_b_tt2l_wjets,two_ctight_tt2l_wjets,two_ctightmedmed_tt2l_wjets,two_ctightmed_tt2l_wjets,two_ctighttight_tt2l_wjets};

  vector<vector<shared_ptr<Process> > > sample_vector = {sample_list,
                                                         wjets_sample_list,
                                                         single_t_sample_list,
                                                         tt2l_sample_list,
                                                         twoB_sample_list,
                                                         oneB_sample_list,
                                                         noB_sample_list,
                                                         twoC_sample_list,
                                                         cc_cr_tt2l_sample_list,
                                                         cc_cr_wjets_sample_list,
                                                         cc_cr_tt2l_wjets_sample_list
                                                         };
  vector<string> sample_label = {"allBkg_",
                                 "wjets_",
                                 "singleT_",
                                 "tt2l_",
                                 "2b_",
                                 "1b_",
                                 "0b_",
                                 "2c_",
                                 "cc_cr_tt2l_",
                                 "cc_cr_wjets_",
                                 "cc_cr_tt2l_wjets_"
                                 };

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::ratio)
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

   pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{Miss} [GeV]"),
        basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_purityCheck_MET_");
   pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
        basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_purityCheck_MCT_");
   pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{bb} [GeV]"),
        basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"mct>200&&mt_met_lep>150&&pfmet>200", sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_purityCheck_MBB_");

  for(unsigned i(0); i<sample_vector.size(); i++){
    pm.Push<Hist1D>(Axis(5, 0, 500., "pfmet", "E_{T}^{Miss} [GeV]"),
        "mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MET_"+sample_label[i]);
    pm.Push<Hist1D>(Axis(3, 0, 600., "mct", "M_{CT} [GeV]"),
        "pfmet>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_"+sample_label[i]);
    pm.Push<Hist1D>(Axis(4, 0, 600., "mt_met_lep", "M_{T} [GeV]"),
        "mct>200&&pfmet>200&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MT_"+sample_label[i]);
    pm.Push<Hist1D>(Axis(8, 30, 510., "mbb", "M_{bb} [GeV]"),
        "mct>200&&mt_met_lep>150&&pfmet>200", sample_vector[i], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_"+sample_label[i]);
    pm.Push<Hist1D>(Axis(3, 0, 600., "mct", "M_{CT} [GeV]"),
        "pfmet>200&&mt_met_lep>150", sample_vector[i], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_nminus2_"+sample_label[i]);
    pm.Push<Hist1D>(Axis(8, 30, 510., "mbb", "M_{bb} [GeV]"),
        "mt_met_lep>150&&pfmet>200", sample_vector[i], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_nminus2_"+sample_label[i]);
    pm.Push<Hist1D>(Axis(50, 0, 500., "mbb", "M_{bb} [GeV]"),
        "mt_met_lep>150&&pfmet>200", sample_vector[i], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_nminus2_forIndara_"+sample_label[i]);
  }

  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{bb} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mct>200&&mt_met_lep>150&&pfmet>200"&&outsideHiggsWindow==1.&&nDeepMedBTagged<2.&&nDeepTightCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_data_2tightc_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{bb} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mct>200&&mt_met_lep>150&&pfmet>200"&&outsideHiggsWindow==1.&&nDeepMedBTagged<2.&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_data_ctightmed_cvbmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{bb} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mct>200&&mt_met_lep>150&&pfmet>200"&&outsideHiggsWindow==1.&&nDeepMedBTagged<2.&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_data_2tightc_cvbmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{bb} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mct>200&&mt_met_lep>150&&pfmet>200"&&outsideHiggsWindow==1.&&nDeepMedBTagged<2.&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_data_2tightc_cvbtight_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{bb} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mct>200&&mt_met_lep>150&&pfmet>200"&&outsideHiggsWindow==1.&&nDeepMedBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_data_2medb_");

  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{bb} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mt_met_lep>150&&pfmet>200"&&nDeepMedBTagged<2.&&nDeepTightCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_data_nminus2_2tightc_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{bb} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mt_met_lep>150&&pfmet>200"&&nDeepMedBTagged<2.&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_data_nminus2_ctightmed_cvbmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{bb} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mt_met_lep>150&&pfmet>200"&&nDeepMedBTagged<2.&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_data_nminus2_2tightc_cvbmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{bb} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mt_met_lep>150&&pfmet>200"&&nDeepMedBTagged<2.&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_data_nminus2_2tightc_cvbtight_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{bb} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mt_met_lep>150&&pfmet>200"&&nDeepMedBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MBB_data_nminus2_2medb_");

  pm.Push<Hist1D>(Axis(10, 0, 200., "mct", "M_{CT} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mct<200&&mt_met_lep>150&&pfmet>200&&mbb>90&&mbb<150"&&nDeepMedBTagged<2.&&nDeepTightCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_data_2tightc_");
  pm.Push<Hist1D>(Axis(10, 0, 200., "mct", "M_{CT} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mct<200&&mt_met_lep>150&&pfmet>200&&mbb>90&&mbb<150"&&nDeepMedBTagged<2.&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_data_ctightmed_cvbmed_");
  pm.Push<Hist1D>(Axis(10, 0, 200., "mct", "M_{CT} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mct<200&&mt_met_lep>150&&pfmet>200&&mbb>90&&mbb<150"&&nDeepMedBTagged<2.&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_data_2tightc_cvbmed_");
  pm.Push<Hist1D>(Axis(10, 0, 200., "mct", "M_{CT} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mct<200&&mt_met_lep>150&&pfmet>200&&mbb>90&&mbb<150"&&nDeepMedBTagged<2.&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_data_2tightc_cvbtight_");
  pm.Push<Hist1D>(Axis(10, 0, 200., "mct", "M_{CT} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mct<200&&mt_met_lep>150&&pfmet>200&&mbb>90&&mbb<150"&&nDeepMedBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_data_2medb_");

  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mt_met_lep>150&&pfmet>200"&&nDeepMedBTagged<2.&&nDeepTightCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_data_nminus2_2tightc_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mt_met_lep>150&&pfmet>200"&&nDeepMedBTagged<2.&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_data_nminus2_ctightmed_cvbmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mt_met_lep>150&&pfmet>200"&&nDeepMedBTagged<2.&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_data_nminus2_2tightc_cvbmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mt_met_lep>150&&pfmet>200"&&nDeepMedBTagged<2.&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_data_nminus2_2tightc_cvbtight_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
      basic_cuts&&WHLeptons==1&&"mt_met_lep>150&&pfmet>200"&&nDeepMedBTagged==2., data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_MCT_data_nminus2_2medb_");

  pm.Push<Hist1D>(Axis(5, 0, 5., nDeepTightCTagged, "nTightCvL Tags"),
      basic_cuts&&WHLeptons==1.&&"mct>200&&pfmet>200&&mt_met_lep>150", data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_nTightCvL_");
  pm.Push<Hist1D>(Axis(5, 0, 5., nDeepMedCTagged, "nMedCvL Tags"),
      basic_cuts&&WHLeptons==1&&"pfmet>200&&mct>200&&mt_met_lep>150", data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_nMedCvL_");

  pm.Push<Hist1D>(Axis(5, 0, 5., nDeepTightCTagged, "nTightCvL Tags"),
      nDeepMedBTagged<2.&&basic_cuts&&WHLeptons==1.&&"mct>200&&pfmet>200&&mt_met_lep>150", data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_nTightCvL_blessthan2_");
  pm.Push<Hist1D>(Axis(5, 0, 5., nDeepMedCTagged, "nMedCvL Tags"),
      nDeepMedBTagged<2.&&basic_cuts&&WHLeptons==1&&"pfmet>200&&mct>200&&mt_met_lep>150", data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_nMedCvL_blessthan2_");

  pm.Push<Hist1D>(Axis(5, 0, 5., nDeepMedCvBTagged, "nMedCvB Tags"),
      nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepMedBTagged<2.&&basic_cuts&&WHLeptons==1.&&"mct>200&&pfmet>200&&mt_met_lep>150", data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_nMedCvB_tightmedCvL_");
  pm.Push<Hist1D>(Axis(5, 0, 5., nDeepMedCvBTagged, "nMedCvB Tags"),
      nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&basic_cuts&&WHLeptons==1&&"pfmet>200&&mct>200&&mt_met_lep>150", data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("charmCR_nMedCvB_tightCvL_");

  pm.MakePlots(lumi);

}