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
  string dilep_basic_cuts =      "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2";
  string dilep_mct_mt_mbb =      "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";
  string dilep_mt =              "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&mt_met_lep>150";
  
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
  auto diboson = Process::MakeShared<Baby_full>("Diboson", Process::Type::background, colors("other"),
    {mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"});
  auto other = Process::MakeShared<Baby_full>("Other", Process::Type::background, colors("dy"),
    {mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root", mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"});

    // {mc_dir+"*DYJetsToLL*.root", mc_dir+"*QCD_HT*0_Tune*.root", mc_dir+"*QCD_HT*Inf_Tune*.root",
    //     mc_dir+"*_ZJet*.root", mc_dir+"*_ttHTobb_M125_*.root",
    //     mc_dir+"*_TTGJets*.root", mc_dir+"*_TTTT_*.root",
    //     mc_dir+"*_WH_HToBB*.root", mc_dir+"*_ZH_HToBB*.root", 
    //     mc_dir+"*_WWTo*.root", mc_dir+"*_WZ*.root",
    //     mc_dir+"_ZZ_*.root"}, "stitch_met");
  auto signal_tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l) (no mT cut)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_1lep*ext1*.root"},basic_cuts&&WHLeptons==1&&HasMedLooseDeepCSV);

  auto signal_tt2l = Process::MakeShared<Baby_full>("signal t#bar{t} (2l)", Process::Type::background, colors("t1tttt"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&HasMedLooseDeepCSV);
  auto one_b_tt2l = Process::MakeShared<Baby_full>("1b t#bar{t} (2l)", Process::Type::background, colors("ttv"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1);
  auto dilep_tt2l = Process::MakeShared<Baby_full>("dilep t#bar{t} (2l), (MET>200)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},dilep_basic_cuts&&WHLeptons==1&&HasMedLooseDeepCSV);

  auto signal_wjets_high_nu = Process::MakeShared<Baby_full>("signal W+jets, nu pT >= 200", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedLooseDeepCSV);
  auto no_b_wjets_high_nu = Process::MakeShared<Baby_full>("0b W+jets, nu pT >= 200", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.);
  auto one_b_wjets_high_nu = Process::MakeShared<Baby_full>("1b W+jets, nu pT >= 200", Process::Type::background, colors("ttv"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1);

  auto signal_medmed_tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l) (no mT cut)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_1lep*ext1*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);

  auto signal_medmed_tt2l = Process::MakeShared<Baby_full>("signal t#bar{t} (2l) (no mT cut)", Process::Type::background, colors("t1tttt"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);
  auto dilep_medmed_tt2l = Process::MakeShared<Baby_full>("dilep t#bar{t} (2l), (MET>200)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},dilep_basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);

  auto signal_medmed_wjets_high_nu = Process::MakeShared<Baby_full>("signal W+jets, nu pT >= 200", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);

  auto signal_mt_tt2l = Process::MakeShared<Baby_full>("signal t#bar{t} (2l)", Process::Type::background, colors("qcd"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts_mt&&WHLeptons==1&&HasMedLooseDeepCSV);
  auto one_b_mt_tt2l = Process::MakeShared<Baby_full>("1b t#bar{t} (2l)", Process::Type::background, colors("ttv"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts_mt&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1);
  auto dilep_mt_tt2l = Process::MakeShared<Baby_full>("dilep t#bar{t} (2l), (MET>200)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},dilep_mt&&WHLeptons==1&&HasMedLooseDeepCSV);

  auto signal_mt_medmed_tt2l = Process::MakeShared<Baby_full>("signal t#bar{t} (2l)", Process::Type::background, colors("qcd"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},basic_cuts_mt&&WHLeptons==1&&HasMedMedDeepCSV);
  auto dilep_mt_medmed_tt2l = Process::MakeShared<Baby_full>("dilep t#bar{t} (2l), (MET>200)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_2lep*ext1*.root"},dilep_mt&&WHLeptons==1&&HasMedMedDeepCSV);

  auto signal_mt_wjets_high_nu = Process::MakeShared<Baby_full>("signal W+jets, nu pT >= 200", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts_mt&&WHLeptons==1&&HasMedLooseDeepCSV);
  auto signal_mt_medmed_wjets_high_nu = Process::MakeShared<Baby_full>("signal W+jets, nu pT >= 200", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts_mt&&WHLeptons==1&&HasMedMedDeepCSV);

  auto tchiwh_225_75 = Process::MakeShared<Baby_full>("TChiWH(225,75)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==225&&mass_lsp==75");
    tchiwh_225_75->SetLineStyle(2);
  auto tchiwh_250_1 = Process::MakeShared<Baby_full>("TChiWH(250,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==250&&mass_lsp==1");
  auto tchiwh_700_1 = Process::MakeShared<Baby_full>("TChiWH(700,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==700&&mass_lsp==1");

  auto signal_medmed_wjets_high_nu_no_gluon_split = Process::MakeShared<Baby_full>("signal W+jets, nu pT >= 200", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV&&nEventsGluonSplit==0.);
  auto no_b_wjets_high_nu_no_gluon_split = Process::MakeShared<Baby_full>("0b W+jets, nu pT >= 200", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.&&nEventsGluonSplit==0.);
  auto one_b_wjets_high_nu_no_gluon_split = Process::MakeShared<Baby_full>("1b W+jets, nu pT >= 200", Process::Type::background, colors("ttv"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1&&nEventsGluonSplit==0.);

  vector<shared_ptr<Process> > sample_list = {data, tchiwh_250_1, tchiwh_225_75, tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson};
  vector<shared_ptr<Process> > short_sample_list = {tchiwh_250_1, tchiwh_225_75, tt1l, tt2l, wjets_low_nu, wjets_high_nu};
  vector<shared_ptr<Process> > highmass_sample_list = {data, tchiwh_225_75,tchiwh_700_1, tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson};
  vector<shared_ptr<Process> > highmass_nolowpt_sample_list = {tchiwh_225_75,tchiwh_700_1, tt1l, tt2l, wjets_high_nu};
  vector<shared_ptr<Process> > highmass_nolowpt_data_sample_list = {data,tchiwh_225_75,tchiwh_700_1, tt1l, tt2l, wjets_high_nu};
  vector<shared_ptr<Process> > highmass_short_sample_list = {tchiwh_225_75, tchiwh_700_1, tt1l, tt2l, wjets_high_nu, wjets_low_nu};
  vector<shared_ptr<Process> > background_opt_sample_list = {wjets_high_nu, wjets_low_nu, tt2l, tt1l, single_t, other};
  vector<shared_ptr<Process> > tt2l_sample_list = {signal_tt2l,one_b_tt2l,dilep_tt2l};
  vector<shared_ptr<Process> > wjets_sample_list = {signal_wjets_high_nu,no_b_wjets_high_nu,one_b_wjets_high_nu};
  vector<shared_ptr<Process> > tt2l_medmed_sample_list = {signal_medmed_tt2l,one_b_tt2l,dilep_medmed_tt2l};
  vector<shared_ptr<Process> > wjets_medmed_sample_list = {signal_medmed_wjets_high_nu,no_b_wjets_high_nu,one_b_wjets_high_nu};
  vector<shared_ptr<Process> > tt2l_mTstudy_sample_list = {signal_mt_tt2l,signal_tt1l,one_b_mt_tt2l,dilep_mt_tt2l};
  vector<shared_ptr<Process> > tt2l_medmed_mTstudy_sample_list = {signal_mt_medmed_tt2l,signal_medmed_tt1l,one_b_mt_tt2l,dilep_mt_medmed_tt2l};
  vector<shared_ptr<Process> > wjets_medmed_no_gluon_split_sample_list = {signal_medmed_wjets_high_nu_no_gluon_split,no_b_wjets_high_nu_no_gluon_split,one_b_wjets_high_nu_no_gluon_split};

  vector<vector<shared_ptr<Process> > > sample_vector = {tt2l_sample_list,wjets_sample_list,tt2l_medmed_sample_list,wjets_medmed_sample_list,wjets_medmed_no_gluon_split_sample_list};
  vector<string> sample_tags_vector = {"tt2l","wjets","tt2l_medmed","wjets_medmed","wjets_medmed_noGluonSplit"};
  vector<string> weight_vector = {"w_noBtagSF * w_BtagSF_WH","w_noBtagSF * w_BtagSF_WH","w_noBtagSF * w_BtagSF_medmed","w_noBtagSF * w_BtagSF_medmed","w_noBtagSF * w_BtagSF_medmed"};
  vector<vector<shared_ptr<Process> > > mTstudy_sample_vector = {tt2l_mTstudy_sample_list,tt2l_medmed_mTstudy_sample_list};
  vector<string> mTstudy_sample_tags_vector ={"tt2l","tt2l_medmed"};
  vector<string> mTstudy_weight_vector = {"w_noBtagSF * w_BtagSF_WH","w_noBtagSF * w_BtagSF_medmed"};

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

//"ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>150"
  NamedFunc preselection = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc preselectionNoBTagging = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && WHLeptons==1;
  NamedFunc preselectionNoB3Jet = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&mt_met_lep>50" && WHLeptons==1;
  NamedFunc preselection_highmT = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && WHLeptons==1 && HasMedLooseDeepCSV;
  //NamedFunc cr2l_1lep = "!PassTrackVeto&&!PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1;
  //NamedFunc cr2l_2lep = "nvetoleps==2&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1;
  NamedFunc cr2l =                   "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV;
  NamedFunc signalRegion =           "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mbb>90&&mbb<150&&mt_met_lep>150"&&HasMedLooseDeepCSV&&WHLeptons==1;
  NamedFunc signalRegionNoMct =       "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mt_met_lep>150&&mbb>90&&mbb<150"&& WHLeptons==1&&HasMedLooseDeepCSV;
  NamedFunc signalRegionMedMedDeepCSV = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150"&& WHLeptons==1&&HasMedMedDeepCSV;
  NamedFunc signalRegionMedLooseDeepCSV = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150&&mct>200"&& WHLeptons==1&&HasMedLooseDeepCSV;
  NamedFunc signalRegionLooseLooseDeepCSV = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150"&& WHLeptons==1&&HasLooseLooseDeepCSV;
  NamedFunc noCuts = true;

  NamedFunc mbbside =             "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mt_met_lep>150&&(mbb<90||mbb>150)"&&WHLeptons==1&&HasMedLooseDeepCSV;
  NamedFunc no_b =                "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&nDeepMedBTagged==0.;
  NamedFunc one_b =               "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1;
  NamedFunc lowMT =               "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mt_met_lep>50&&mt_met_lep<150&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV;
  NamedFunc lowMCT =              "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV;

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

  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&nvetoleps==1"&&HasMedLooseDeepCSV&& WHLeptons==1, highmass_nolowpt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("Signal_Weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
      signalRegion, highmass_nolowpt_data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("Signal_Weight");
  
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&(mbb<90||mbb>150)&&mct>200&&mt_met_lep>150"&& WHLeptons==1&&HasMedLooseDeepCSV, highmass_nolowpt_data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("Signal_Weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&(mbb<90||mbb>150)&&mct>200&&mt_met_lep>150"&& WHLeptons==1&&HasMedLooseDeepCSV, highmass_nolowpt_data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("Signal_Weight");

  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
		  "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150"&& WHLeptons==1&&HasMedLooseDeepCSV, highmass_nolowpt_data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("Signal_Weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150"&& WHLeptons==1&&HasMedLooseDeepCSV, highmass_nolowpt_data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("Signal_Weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
		  "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150"&& WHLeptons==1&&HasMedLooseDeepCSV, highmass_nolowpt_data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("Signal_Weight");
  
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
		  "PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150"&& WHLeptons==1&&HasMedLooseDeepCSV, highmass_nolowpt_data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("Signal_Weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150"&& WHLeptons==1&&HasMedLooseDeepCSV, highmass_nolowpt_data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("Signal_Weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
		  "PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150"&& WHLeptons==1&&HasMedLooseDeepCSV, highmass_nolowpt_data_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("Signal_Weight");

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
  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepMedBTagged, "N_{medium jets}"),
		  cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH");
  pm.Push<Hist1D>(Axis(5, 0, 5, nDeepLooseBTagged, "N_{loose jets}"),
		  cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH");

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

  //Dilep Control Region

  pm.Push<Hist1D>(Axis(25, 0, 500., "leps_pt[0]", "lep p_{T} [GeV]"),
      cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., LeadingBJetPt, "leading b jet p_{T} [GeV]"),
      cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., SubLeadingBJetPt, "subleading b jet p_{T} [GeV]"),
      cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
      cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
      cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
      cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
      cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "els_pt", "Electron p_{T}"),
		  cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mus_pt", "Muon p_{T}"),
		  cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(25, 0, 500., "ak4pfjets_pt", "Jet p_{T}"),
		  cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(12, -3, 3, "els_eta", "Electron #eta"),
		  cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(12, -3, 3, "mus_eta", "Muon #eta"),
		  cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(12, -3, 3, "ak4pfjets_eta", "Jet #eta"),
		  cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(16, -4, 4, "els_phi", "Electron #phi"),
		  cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(16, -4, 4, "mus_phi", "Muon #phi"),
		  cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  pm.Push<Hist1D>(Axis(16, -4, 4, "ak4pfjets_phi", "Jet #phi"),
		  cr2l, highmass_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("cr2l_weight");
  //pm.Push<Hist1D>(Axis(3, 0, 3, WHLeptons,"1 good lep"), "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2&&PassTrackVeto&&PassTauVeto))&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons>=1, sample_list, all_plot_types);
  //pm.Push<Hist1D>(Axis(3, 0, 3, WHLeptons,"2nd lep veto"), "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(2, 0, 2, "PassTrackVeto","Track Veto"),
		  "nvetoleps==2&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(2, 0, 2, "PassTauVeto","Tau Veto"),
		  "nvetoleps==2&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(10, 0, 10, "ngoodjets","nGoodJets"),
		  "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&pfmet>125&&mt_met_lep>150" && HasMedLooseDeepCSV && WHLeptons==1, sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH");
  pm.Push<Hist1D>(Axis(2, 0, 2, HasMedLooseCSV,"MedLooseCSV"),
		 "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&pfmet>125&&mt_met_lep>150" && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet","E_{T}^{miss} [GeV]"),
		  "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&mt_met_lep>150" && HasMedLooseDeepCSV && WHLeptons==1, sample_list, all_plot_types);
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep","m_{T} [GeV]"),
		  "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&pfmet>125" && HasMedLooseDeepCSV && WHLeptons==1, sample_list, all_plot_types);

  
  //Lepton pT study
  pm.Push<Hist1D>(Axis(20, 0, 100., "leps_pt[0]", "Lepton p_{T} [GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&nvetoleps==1"&&HasMedLooseDeepCSV, highmass_short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("lepStudy_noRemovals");
  pm.Push<Hist1D>(Axis(20, 0, 100., "leps_pt[0]", "Lepton p_{T} [GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150&&nvetoleps==1"&&HasMedLooseDeepCSV, highmass_short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("lepStudy_noMCT");
  pm.Push<Hist1D>(Axis(20,0,100.,"leps_pt[0]","Lepton p_{T}[GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mt_met_lep>150&&nvetoleps==1"&&HasMedLooseDeepCSV, highmass_short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("lepStudy_noMBB");
  pm.Push<Hist1D>(Axis(20,0,100.,"leps_pt[0]","Lepton p_{T}[GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mct>200&&nvetoleps==1"&&HasMedLooseDeepCSV, highmass_short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("lepStudy_noMT");

  pm.Push<Hist1D>(Axis(20, 0, 100., "leps_pt[0]", "Lepton p_{T} [GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mbb>90&&mbb<150&&mct>200&&mt_met_lep>150&&nvetoleps==1"&&HasMedLooseDeepCSV, highmass_short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("lepStudy_MET_g125_noRemovals");
  pm.Push<Hist1D>(Axis(20, 0, 100., "leps_pt[0]", "Lepton p_{T} [GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mbb>90&&mbb<150&&mt_met_lep>150&&nvetoleps==1"&&HasMedLooseDeepCSV, highmass_short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("lepStudy_METg125_noMCT");
  pm.Push<Hist1D>(Axis(20,0,100.,"leps_pt[0]","Lepton p_{T}[GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&nvetoleps==1"&&HasMedLooseDeepCSV, highmass_short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("lepStudy_METg125_noMBB");
  pm.Push<Hist1D>(Axis(20,0,100.,"leps_pt[0]","Lepton p_{T}[GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mbb>90&&mbb<150&&mct>200&&nvetoleps==1"&&HasMedLooseDeepCSV, highmass_short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("lepStudy_METg125_noMT");

  pm.Push<Hist1D>(Axis(20, 0, 100., "leps_pt[0]", "Lepton p_{T} [GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mbb>90&&mbb<150&&mt_met_lep>150&&nvetoleps==1"&&HasMedLooseDeepCSV, highmass_short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("lepStudy_METg300_noMCT");
  pm.Push<Hist1D>(Axis(20,0,100.,"leps_pt[0]","Lepton p_{T}[GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&nvetoleps==1"&&HasMedLooseDeepCSV, highmass_short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("lepStudy_METg300_noMBB");
  pm.Push<Hist1D>(Axis(20,0,100.,"leps_pt[0]","Lepton p_{T}[GeV]"),
      "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mbb>90&&mbb<150&&mct>200&&nvetoleps==1"&&HasMedLooseDeepCSV, highmass_short_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("lepStudy_METg300_noMT");
  //End Lepton pT study

  //Distribution Plots for Potential CRs
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>50"&& WHLeptons==1 && HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_preselection_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125"&& WHLeptons==1 && HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_preselection_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50"&& WHLeptons==1 && HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_preselection_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		              "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50"&& WHLeptons==1 && HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_preselection_");

  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>150"&& WHLeptons==1 && HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_preselection_highmT");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125"&& WHLeptons==1 && HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_preselection_highmT");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150"&& WHLeptons==1 && HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_preselection_highmT");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		              "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150"&& WHLeptons==1 && HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_preselection_highmT");

  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150&&mt_met_lep>150"&&HasMedLooseDeepCSV&&WHLeptons==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_signalRegion_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mbb>90&&mbb<150"&&HasMedLooseDeepCSV&&WHLeptons==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_signalRegion_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150"&&HasMedLooseDeepCSV&&WHLeptons==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_signalRegion_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		              "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mt_met_lep>150"&&HasMedLooseDeepCSV&&WHLeptons==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_signalRegion_");

  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150&&(mbb<90||mbb>150)"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_mbbside_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&(mbb<90||mbb>150)"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_mbbside_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mt_met_lep>150&&(mbb<90||mbb>150)"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_mbbside_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		              "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mt_met_lep>150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_mbbside_");

  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&nDeepMedBTagged==0., background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_no_b_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mbb>90&&mbb<150"&&WHLeptons==1&&nDeepMedBTagged==0., background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_no_b_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&nDeepMedBTagged==0., background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_no_b_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		              "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mt_met_lep>150"&&WHLeptons==1&&nDeepMedBTagged==0., background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_no_b_");

  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_one_b_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mbb>90&&mbb<150"&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_one_b_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_one_b_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		              "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mt_met_lep>150"&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_one_b_");

  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_dilep_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&pfmet>125&&mct>200&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_dilep_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&pfmet>125&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_dilep_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		              "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&pfmet>125&&mct>200&&mt_met_lep>150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_dilep_");

  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>50&&mt_met_lep<150&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_lowMT_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_lowMT_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mt_met_lep>50&&mt_met_lep<150&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_lowMT_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		              "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mt_met_lep>50&&mt_met_lep<150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_lowMT_");

  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct<200&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_lowMCT_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct<200&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_lowMCT_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mt_met_lep>150&&mbb>90&&mbb<150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_lowMCT_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		              "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct<200&&mt_met_lep>150"&&WHLeptons==1&&HasMedLooseDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_WH").Tag("compositionStudy_lowMCT_");
    
  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>50"&& WHLeptons==1 && HasMedMedDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_preselection_medmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125"&& WHLeptons==1 && HasMedMedDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_preselection_medmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50"&& WHLeptons==1 && HasMedMedDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_preselection_medmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		              "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50"&& WHLeptons==1 && HasMedMedDeepCSV, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_preselection_medmed_");

  pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150&&mt_met_lep>150"&&HasMedMedDeepCSV&&WHLeptons==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_signalRegion_medmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mbb>90&&mbb<150"&&HasMedMedDeepCSV&&WHLeptons==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_signalRegion_medmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                  "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mbb>90&&mbb<150&&mt_met_lep>150"&&HasMedMedDeepCSV&&WHLeptons==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_signalRegion_medmed_");
  pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		              "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&mct>200&&mt_met_lep>150"&&HasMedMedDeepCSV&&WHLeptons==1, background_opt_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("compositionStudy_signalRegion_medmed_");

  for(unsigned i(0); i<sample_vector.size(); i++){
    pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                    "mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
                    "pfmet>200&&mct>200&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                    "pfmet>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		                "pfmet>200&&mct>200&&mt_met_lep>150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(25, 0, 500., sortedJetsPt_Leading, "p_{T} [GeV]"),
		                "pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(25, 0, 500., sortedJetsPt_subLeading, "p_{T} [GeV]"),
		                "pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(25, 0, 500., sortedJetsCSV_Leading, "p_{T} [GeV]"),
		                "pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(25, 0, 500., sortedJetsCSV_subLeading, "p_{T} [GeV]"),
		                "pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(10, 0, 2., sortedJetsCSV_deltaR, "deltaR"),
		                "pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("compositionStudy_Distributions_background_"+sample_tags_vector[i]);
  }

  for(unsigned i(0); i<sample_vector.size(); i++){
    pm.Push<Hist1D>(Axis(5, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                    "mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("Coarse_compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(4, 0, 600., "mt_met_lep", "m_{T} [GeV]"),
                    "pfmet>200&&mct>200&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("Coarse_compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(3, 0, 600., "mct", "M_{CT} [GeV]"),
                    "pfmet>200&&mt_met_lep>150&&mbb>90&&mbb<150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("Coarse_compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(8, 30, 510., "mbb", "M_{b#bar{b}}"),
		                "pfmet>200&&mct>200&&mt_met_lep>150", sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("Coarse_compositionStudy_Distributions_background_"+sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(8, 30, 510., "mbb", "M_{b#bar{b}}"),
		                "pfmet>200&&mct>200&&mt_met_lep>150"&&bDeltaPhi<1.6, sample_vector[i], all_plot_types).Weight(weight_vector[i]).Tag("Coarse_compositionStudy_Distributions_background_dphicut_"+sample_tags_vector[i]);
  }

  for(unsigned i(0); i<mTstudy_sample_vector.size(); i++){
    pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                    "mct>200&&mbb>90&&mbb<150", mTstudy_sample_vector[i], all_plot_types).Weight(mTstudy_weight_vector[i]).Tag("mTcompositionStudy_Distributions_background_"+mTstudy_sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
                    "pfmet>200&&mbb>90&&mbb<150", mTstudy_sample_vector[i], all_plot_types).Weight(mTstudy_weight_vector[i]).Tag("mTcompositionStudy_Distributions_background_"+mTstudy_sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(25, 0, 500., "mbb", "M_{b#bar{b}}"),
		                "pfmet>200&&mct>200", mTstudy_sample_vector[i], all_plot_types).Weight(mTstudy_weight_vector[i]).Tag("mTcompositionStudy_Distributions_background_"+mTstudy_sample_tags_vector[i]);
  }

  for(unsigned i(0); i<mTstudy_sample_vector.size(); i++){
    pm.Push<Hist1D>(Axis(5, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
                    "mct>200&&mbb>90&&mbb<150", mTstudy_sample_vector[i], all_plot_types).Weight(mTstudy_weight_vector[i]).Tag("Coarse_mTcompositionStudy_Distributions_background_"+mTstudy_sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(3, 0, 600., "mct", "M_{CT} [GeV]"),
                    "pfmet>200&&mbb>90&&mbb<150", mTstudy_sample_vector[i], all_plot_types).Weight(mTstudy_weight_vector[i]).Tag("Coarse_mTcompositionStudy_Distributions_background_"+mTstudy_sample_tags_vector[i]);
    pm.Push<Hist1D>(Axis(8, 30, 510., "mbb", "M_{b#bar{b}}"),
		                "pfmet>200&&mct>200", mTstudy_sample_vector[i], all_plot_types).Weight(mTstudy_weight_vector[i]).Tag("Coarse_mTcompositionStudy_Distributions_background_"+mTstudy_sample_tags_vector[i]);
  }

    pm.Push<Hist1D>(Axis(8, 30, 510., gluBTagged, "Test"),
		                "pfmet>200&&mct>200", sample_list, all_plot_types).Tag("TEST");

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
