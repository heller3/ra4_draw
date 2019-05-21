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
#include "core/hist2d.hpp"

using namespace std;
using namespace PlotOptTypes;
using namespace WH_Functions;


int main(){
  gErrorIgnoreLevel = 6000;

  double lumi = 35.9; //133.53;

  string data_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_04_03/";
  string mc_dir = "/home/users/rheller/wh_babies/babies_v30_9_2019_04_03/";
  string signal_dir = "/home/users/rheller/wh_babies/babies_signal_2019_04_03/";

  // string data_dir = "/home/users/rheller/wh_babies/babies_2019_02_12/";
  string mc_dir2 = "/home/users/rheller/wh_babies/babies_2019_02_11/";
  // string signal_dir = "/home/users/rheller/wh_babies/babies_2019_02_12/";

  Palette colors("txt/colors.txt", "default");

  auto data = Process::MakeShared<Baby_full>("Data", Process::Type::data, colors("data"),
    {data_dir+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1)");
  auto tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_1lep*.root"});
  auto tt2l = Process::MakeShared<Baby_full>("t#bar{t} (2l)", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*TTJets_2lep*.root"});
  auto wjets_low_nu = Process::MakeShared<Baby_full>("W+jets, nu pT < 200", Process::Type::background, colors("qcd"),
    {mc_dir+"slim_W*JetsToLNu*.root"},NHighPtNu==0.);
  auto wjets_high_nu = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"});
  auto single_t = Process::MakeShared<Baby_full>("Single t", Process::Type::background, colors("single_t"),
    {mc_dir+"*_ST_*.root"});
  auto ttv = Process::MakeShared<Baby_full>("t#bar{t}V", Process::Type::background, colors("ttv"),
    {mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root"});
  auto diboson = Process::MakeShared<Baby_full>("Diboson", Process::Type::background, colors("other"),
    {mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"});

  auto wjets = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200", Process::Type::background, colors("wjets"),
    {mc_dir2+"*W*JetsToLNu*.root"});

  auto tchiwh_225_75 = Process::MakeShared<Baby_full>("TChiWH(225,75)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==225&&mass_lsp==75");
    tchiwh_225_75->SetLineStyle(2);
  auto tchiwh_250_1 = Process::MakeShared<Baby_full>("TChiWH(250,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==250&&mass_lsp==1");
  auto tchiwh_350_100 = Process::MakeShared<Baby_full>("TChiWH(350,100)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==350&&mass_lsp==100");
    tchiwh_350_100->SetLineStyle(2);
  auto tchiwh_500_125 = Process::MakeShared<Baby_full>("TChiWH(500,125)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==500&&mass_lsp==125");
  auto tchiwh_500_1 = Process::MakeShared<Baby_full>("TChiWH(500,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==500&&mass_lsp==1");
    tchiwh_500_1->SetLineStyle(2);
  auto tchiwh_700_1 = Process::MakeShared<Baby_full>("TChiWH(700,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==700&&mass_lsp==1");

  vector<shared_ptr<Process> > signal = {tchiwh_250_1, tt1l, tt2l, wjets_low_nu, wjets_high_nu};  
  vector<shared_ptr<Process> > signal1 = {tchiwh_225_75, tt1l, tt2l, wjets_low_nu, wjets_high_nu};
  vector<shared_ptr<Process> > signal2 = {tchiwh_500_1, tt1l, tt2l};//, wjets_low_nu, wjets_high_nu};
  vector<shared_ptr<Process> > signal3 = {tchiwh_500_125, tt1l, tt2l};//, wjets_low_nu, wjets_high_nu};
  vector<shared_ptr<Process> > all_signals = {tchiwh_250_1, tchiwh_225_75, tchiwh_500_1, tchiwh_500_125, tchiwh_700_1};
  vector<shared_ptr<Process> > ttbar = {tt1l, tt2l};
  vector<shared_ptr<Process> > sample_list = {data, tchiwh_250_1, tchiwh_225_75, tchiwh_500_125, tchiwh_500_1, tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson};
  vector<shared_ptr<Process> > sample_list2 = {data, tchiwh_250_1, tchiwh_225_75, tchiwh_500_125, tchiwh_500_1, tt1l, tt2l, wjets, single_t, ttv, diboson};
  vector<shared_ptr<Process> > control_sample_list = {data, tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson};
  vector<shared_ptr<Process> > jets_sample_list = {tchiwh_250_1, tchiwh_225_75, tchiwh_500_125, tchiwh_500_1, tt1l, tt2l};//, wjets_low_nu, wjets_high_nu}; //sample list to look at three jet region
  vector<shared_ptr<Process> > two_jet_sample_list = {tchiwh_250_1, tchiwh_225_75, tchiwh_500_125, tchiwh_500_1, tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::ratio)
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
  PlotOpt style("txt/plot_styles.txt", "Scatter");
  vector<PlotOpt> bkg_hist = {style().Stack(StackType::signal_overlay).Title(TitleType::preliminary)};
  vector<PlotOpt> bkg_pts = {style().Stack(StackType::shapes).Title(TitleType::preliminary)};
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info, lin_shapes_info, log_shapes_info}; //lin_lumi_info, lin_shapes_info};
  vector<PlotOpt> lin_types = {lin_shapes_info, lin_lumi_info};
  vector<PlotOpt> log_types = {log_shapes_info, log_lumi_info};
  vector<PlotOpt> lin_shapes_type = {lin_shapes_info};
  vector<PlotOpt> log_shapes_type = {log_shapes_info};
  vector<PlotOpt> lin_lumi_type = {lin_lumi_info};
  vector<PlotOpt> log_lumi_type = {log_lumi_info};


  //"ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>150"
  NamedFunc no_cuts = true;
  NamedFunc min_cuts = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3" && HasMedLooseDeepCSV && WHLeptons==1;
  NamedFunc min_cuts_2medb = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3" && HasMedMedDeepCSV && WHLeptons==1;
  NamedFunc preselection = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && HasMedLooseDeepCSV && WHLeptons==1;
  NamedFunc preselection_nojetcuts = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&pfmet>125&&mt_met_lep>50" && WHLeptons==1;// &&  HasMedLooseDeepCSV;
  NamedFunc preselection_multijets = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&mt_met_lep>50&&pfmet>125" && HasMedLooseDeepCSV && WHLeptons==1;
  NamedFunc preselection_multijets_2medb_notaucuts = "ngoodjets==3&&mt_met_lep>50&&pfmet>125" && HasMedMedDeepCSV;// && WHLeptons==1;
  NamedFunc preselection_multijets_2medb = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&mt_met_lep>50&&pfmet>125" && HasMedMedDeepCSV && WHLeptons==1;
  NamedFunc cr_base = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>170&&pfmet>125&&mt_met_lep>150" && WHLeptons==1;
  NamedFunc cr0b = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && NBJets==0. && WHLeptons==1;
  NamedFunc crmbb = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&!(mbb>90&&mbb<150)&&pfmet>125&&mt_met_lep>50" && HasMedLooseDeepCSV && WHLeptons==1;
  NamedFunc signalRegion = "mct>170&&mt_met_lep>150"; //mbb>90&&mbb<150&&


  PlotMaker pm;

  // pm.Push<Hist1D>(Axis(30, 0, 300., "mct", "m_{CT} [GeV]"), preselection_true, sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[j]);
  // .Weight("weight")
  // .Weight("w_noBtagSF")
  // .Weight("w_noBtagSF *  w_BtagSF_WH")
  //.Tag("name of file")

  //cr0b
  // pm.Push<Hist1D>(Axis(21, 85, 505., "pfmet", "E_{T}^{miss} [GeV]"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(21, 85, 505., "pfmet", "E_{T}^{miss} [GeV]"),
  //                 cr0b, sample_list2, all_plot_types).Tag("wjets_cr0b");
  // pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "leps_pt[0]", "Lepton p_{T} [GeV]"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingBJetPt, "Leading b-jet p_{T} [GeV]"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., SubLeadingBJetPt, "Subleading b-jet p_{T} [GeV]"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(50, 0, 500., "mbb", "M_{bb} [GeV]"),
		//               cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "els_pt", "Electron p_{T}"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "mus_pt", "Muon p_{T}"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "ak4pfjets_pt", "Jet p_{T}"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(12, -3, 3, "els_eta", "Electron #eta"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(12, -3, 3, "mus_eta", "Muon #eta"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(12, -3, 3, "ak4pfjets_eta", "Jet #eta"),
  //                 cr0b, sample_list, all_plot_types);
   pm.Push<Hist1D>(Axis(32, -4, 4, "els_phi", "Electron #phi"),
                   cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(16, -4, 4, "mus_phi", "Muon #phi"),
  //                 cr0b, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(16, -4, 4, "ak4pfjets_phi", "Jet #phi"),
  //                 cr0b, sample_list, all_plot_types);

  // //crmbb
  // pm.Push<Hist1D>(Axis(21, 85, 505., "pfmet", "E_{T}^{miss} [GeV]"),
  //                 crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(21, 85, 505., "pfmet", "E_{T}^{miss} [GeV]"),
  //                 crmbb, sample_list2, all_plot_types).Tag("wjets_crmbb");
  // pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
  //                 crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "leps_pt[0]", "Lepton p_{T} [GeV]"),
  //                 crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingBJetPt, "Leading b-jet p_{T} [GeV]"),
  //                 crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., SubLeadingBJetPt, "Subleading b-jet p_{T} [GeV]"),
  //                 crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
		//               crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(50, 0, 500., "mbb", "M_{bb} [GeV]"),
		//               crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "els_pt", "Electron p_{T}"),
  //                 crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "mus_pt", "Muon p_{T}"),
  //                 crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "ak4pfjets_pt", "Jet p_{T}"),
  //                 crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(12, -3, 3, "els_eta", "Electron #eta"),
  //                 crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(12, -3, 3, "mus_eta", "Muon #eta"),
  //                 crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(12, -3, 3, "ak4pfjets_eta", "Jet #eta"),
  //                 crmbb, sample_list, all_plot_types);
   pm.Push<Hist1D>(Axis(32, -4, 4, "els_phi", "Electron #phi"),
                   crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(16, -4, 4, "mus_phi", "Muon #phi"),
  //                 crmbb, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(16, -4, 4, "ak4pfjets_phi", "Jet #phi"),
  //                 crmbb, sample_list, all_plot_types);


  //jet regions
  // pm.Push<Hist1D>(Axis(6, 0, 6, "ngoodjets", "N_{j}"),
  //                 no_cuts, jets_sample_list, lin_shapes_type);
  // pm.Push<Hist1D>(Axis(6, 0, 6, "ngoodjets", "N_{j}"),
  //                 preselection_nojetcuts, jets_sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(6, 0, 6, "ngoodjets", "N_{j}"),
  // preselection_nojetcuts&&signalRegion, jets_sample_list, all_plot_types);

  // pm.Push<Hist1D>(Axis(6, 0, 6., truthOrigin3rdJet, "type of \"jet\""), 
  //                    preselection_multijets && truthOrigin3rdJet>=0., ttbar, lin_shapes_type);
  // pm.Push<Hist1D>(Axis(6, 0, 6., truthOrigin3rdJet, "type of \"jet\""), 
  //                    preselection_multijets && signalRegion && truthOrigin3rdJet>=0., ttbar, lin_shapes_type);

  // pm.Push<Hist1D>(Axis(30, 0, 30., truthOrigin3rdJetUntaggedB, "type of \"jet\""), 
  //                    preselection_multijets && truthOrigin3rdJet>=0., ttbar, lin_shapes_type);
  // pm.Push<Hist1D>(Axis(30, 0, 30., truthOrigin3rdJetUntaggedB, "type of \"jet\""), 
  //                    preselection_multijets && signalRegion && truthOrigin3rdJet>=0., ttbar, lin_shapes_type);

  // pm.Push<Hist1D>(Axis(6, 0, 6., truthOrigin3rdJet, "type of \"jet\""), 
  //                    preselection_multijets_2medb_notaucuts && truthOrigin3rdJet>=0., ttbar, lin_shapes_type);
  // pm.Push<Hist1D>(Axis(6, 0, 6., truthOrigin3rdJet, "type of \"jet\""), 
  //                    preselection_multijets_2medb_notaucuts && signalRegion && truthOrigin3rdJet>=0., ttbar, lin_shapes_type);

  
  // pm.Push<Hist1D>(Axis(6, 0, 6., NBJets, "type of \"jet\""), 
  //                    preselection_nojetcuts, all_signals, lin_shapes_type);
  // pm.Push<Hist1D>(Axis(6, 0, 6., NBJets, "N bjets > loose"), 
  //                    preselection_nojetcuts && signalRegion, all_signals, lin_shapes_type);

  // pm.Push<Hist1D>(Axis(6, 0, 6., "ngoodjets", "type of \"jet\""), 
  //                    preselection_nojetcuts, all_signals, lin_shapes_type);
  // pm.Push<Hist1D>(Axis(6, 0, 6., "ngoodjets", "N good jets"), 
  //                    preselection_nojetcuts && signalRegion, all_signals, lin_shapes_type);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  

  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingNonBJetPt_med, "Leading non-b<med Tp_{T} [GeV]"),
  //                 preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med>0., jets_sample_list, lin_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingFakeNonBJetPt_med, "Leading fake non-b<med Tp_{T} [GeV]"),
  //                 preselection_multijets_2medb && signalRegion && LeadingFakeNonBJetPt_med>0., jets_sample_list, lin_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingRealNonBJetPt_med, "Leading real non-b<med Tp_{T} [GeV]"),
  //                 preselection_multijets_2medb && signalRegion && LeadingRealNonBJetPt_med>0., jets_sample_list, lin_types);

  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingNonBJetPt, "Leading non-b<loose p_{T} [GeV]"),
  //                 preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>0., jets_sample_list, lin_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingFakeNonBJetPt, "Leading fake non-b<loose p_{T} [GeV]"),
  //                 preselection_multijets_2medb && signalRegion&& LeadingFakeNonBJetPt>0., jets_sample_list, lin_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingRealNonBJetPt, "Leading real non-b<loose p_{T} [GeV]"),
  //                 preselection_multijets_2medb && signalRegion && LeadingRealNonBJetPt>0., jets_sample_list, lin_types);

  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingNonBJetPt, "Leading non-b<loose p_{T} [GeV]"),
  //                 preselection_multijets && signalRegion && LeadingNonBJetPt>0., jets_sample_list, lin_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingFakeNonBJetPt, "Leading fake non-b<loose p_{T} [GeV]"),
  //                 preselection_multijets && signalRegion && LeadingFakeNonBJetPt>0., jets_sample_list, lin_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingRealNonBJetPt, "Leading real non-b<loose p_{T} [GeV]"),
  //                 preselection_multijets && signalRegion && LeadingRealNonBJetPt>0., jets_sample_list, lin_types);


  // pm.Push<Hist1D>(Axis(15, 0, 600., LeadingBJetPt, "Leading b>loose p_{T} [GeV]"),
  //                 preselection && signalRegion && LeadingBJetPt>0., two_jet_sample_list, log_types);
  // pm.Push<Hist1D>(Axis(15, 0, 600., LeadingRealBJetPt, "Leading real b>loose p_{T} [GeV]"),
  //                 preselection && signalRegion && LeadingRealBJetPt>0., two_jet_sample_list, log_types);
  // pm.Push<Hist1D>(Axis(15, 0, 600., LeadingFakeBJetPt, "Leading fake b>loose p_{T} [GeV]"),
  //                 preselection && signalRegion && LeadingFakeBJetPt>0., two_jet_sample_list, log_types);

  // pm.Push<Hist1D>(Axis(15, 0, 600., SubLeadingBJetPt, "Leading b>loose p_{T} [GeV]"),
  //                 preselection && signalRegion && SubLeadingBJetPt>0., two_jet_sample_list, log_types);
  // pm.Push<Hist1D>(Axis(15, 0, 600., SubLeadingRealBJetPt, "Leading real b>loose p_{T} [GeV]"),
  //                 preselection && signalRegion && SubLeadingRealBJetPt>0., two_jet_sample_list, log_types);
  // pm.Push<Hist1D>(Axis(15, 0, 600., SubLeadingFakeBJetPt, "Leading fake b>loose p_{T} [GeV]"),
  //                 preselection && signalRegion && SubLeadingFakeBJetPt>0., two_jet_sample_list, log_types);


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]"),
  //                 min_cuts && LeadingNonBJetPt>0., jets_sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]"),
  //                 preselection_multijets && LeadingNonBJetPt>0., jets_sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]"),
  //                 preselection_multijets && signalRegion && LeadingNonBJetPt>0., jets_sample_list, lin_types);

  // pm.Push<Hist1D>(Axis(25, 0, 4., LeadingNonBJetDeltaPhi, "Leading non-b dPhi(jet,MET)"),
  //                 min_cuts && LeadingNonBJetDeltaPhi>0., jets_sample_list, lin_types);
  // pm.Push<Hist1D>(Axis(25, 0, 4., LeadingNonBJetDeltaPhi, "Leading non-b dPhi(jet,MET)"),
  //                 preselection_multijets && LeadingNonBJetDeltaPhi>0., jets_sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 4., LeadingNonBJetDeltaPhi, "Leading non-b dPhi(jet,MET"),
  //                 preselection_multijets && signalRegion && LeadingNonBJetDeltaPhi>0., jets_sample_list, lin_types);

  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]"),
  //                 min_cuts_2medb && LeadingNonBJetPt>0., jets_sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]"),
  //                 preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>0., jets_sample_list, all_plot_types);

  // pm.Push<Hist1D>(Axis(25, 0, 4., LeadingNonBJetDeltaPhi, "Leading non-b dPhi(jet,MET)"),
  //                 min_cuts_2medb && LeadingNonBJetDeltaPhi>0., jets_sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 4., LeadingNonBJetDeltaPhi, "Leading non-b dPhi(jet,MET"),
  //                 preselection_multijets_2medb && signalRegion && LeadingNonBJetDeltaPhi>0., jets_sample_list, all_plot_types);
  
  // pm.Push<Hist1D>(Axis(25, 0, 4., LeadingDeltaPhiDeltaPhi, "Leading non-b (by dPhi) dPhi(jet,MET)"),
  //                 preselection_multijets, jets_sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 4., LeadingDeltaPhiDeltaPhi, "Leading non-b (by dPhi) dPhi(jet,MET)"),
  //                 preselection_multijets && signalRegion, jets_sample_list, all_plot_types);

  // pm.Push<Hist1D>(Axis(25, 0, 500., genISRPt, "truth ISR p_{T} [GeV]"),
  //                 preselection_multijets && genISRPt>0., jets_sample_list, lin_types);
  // pm.Push<Hist1D>(Axis(15, 0, 600., genISRPt, "truth ISR p_{T} [GeV]"),
  //                 preselection_multijets && signalRegion && genISRPt>0., jets_sample_list, log_lumi_type);
  // pm.Push<Hist1D>(Axis(15, 0, 600., genISRPt, "truth ISR p_{T} [GeV]"),
  //                 preselection_multijets && signalRegion && genISRPt>0. && "mt_met_lep>100", jets_sample_list, lin_lumi_type);
  // pm.Push<Hist1D>(Axis(15, 0, 600., genISRPt, "truth ISR p_{T} [GeV]"),
  //                 preselection_multijets && signalRegion && genISRPt>0. && "mt_met_lep>120", jets_sample_list, lin_lumi_type);

  // pm.Push<Hist1D>(Axis(15, 0, 600., genISRPt, "truth ISR p_{T} [GeV]"),
  //                 preselection_multijets && signalRegion && genISRPt>0. && "mt_met_lep>120", jets_sample_list, lin_lumi_type);
  // pm.Push<Hist1D>(Axis(15, 0, 600., genISRPt, "truth ISR p_{T} [GeV]"),
  //                 preselection_multijets && signalRegion && genISRPt>0. && "mt_met_lep>130", jets_sample_list, lin_lumi_type);
  // pm.Push<Hist1D>(Axis(15, 0, 600., genISRPt, "truth ISR p_{T} [GeV]"),
  //                 preselection_multijets && signalRegion && genISRPt>0. && "mt_met_lep>140", jets_sample_list, lin_lumi_type);
  // pm.Push<Hist1D>(Axis(15, 0, 600., genISRPt, "truth ISR p_{T} [GeV]"),
  //                 preselection_multijets && signalRegion && genISRPt>0. && "mt_met_lep>150", jets_sample_list, lin_lumi_type);


  // pm.Push<Hist1D>(Axis(25, 0, 4., genISRMETdPhi, "truth ISR dPhi(genISR, genMET)"),
  //                 preselection_multijets && genISRMETdPhi>0., jets_sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 4., genISRMETdPhi, "truth ISR dPhi(genISR, genMET)"),
  //                 preselection_multijets && signalRegion && genISRMETdPhi>0., jets_sample_list, all_plot_types);

  // pm.Push<Hist1D>(Axis(25, 0, 4., genISRrecoMETdPhi, "truth ISR dPhi(genISR, recoMET)"),
  //                 preselection_multijets && genISRMETdPhi>0., jets_sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 4., genISRrecoMETdPhi, "truth ISR dPhi(genISR, recoMET)"),
  //                 preselection_multijets && signalRegion && genISRMETdPhi>0., jets_sample_list, all_plot_types);

  // pm.Push<Hist1D>(Axis(25, 0, 4., genISRrecoISRdPhi, "dPhi(genISR, reco3rdJet)"),
  //                 preselection_multijets && genISRrecoISRdPhi>0., jets_sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 4., genISRrecoISRdPhi, "dPhi(genISR, reco3rdJet)"),
  //                 preselection_multijets && signalRegion && genISRrecoISRdPhi>0., jets_sample_list, all_plot_types);

  // pm.Push<Hist1D>(Axis(30, -300, 300., genISRrecoISRDeltaPt, "3rd-jet - genISR"),
  //                 preselection_multijets && genISRrecoISRDeltaPt<1000., jets_sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(30, -300, 300., genISRrecoISRDeltaPt, "3rd-jet - genISR"),
  //                 preselection_multijets && signalRegion && genISRrecoISRDeltaPt<1000., jets_sample_list, all_plot_types);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // pm.Push<Hist2D>(Axis(25, 0., 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]"), 
		//               Axis(25, 0., 4., LeadingNonBJetDeltaPhi,   "Leading non-b dPhi(jet,MET)"),  
		//               preselection_multijets && LeadingNonBJetPt>0. && LeadingNonBJetDeltaPhi>0., signal, bkg_hist).Tag("Pt_dPhi_2D_plots");

  // pm.Push<Hist2D>(Axis(25, 0., 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]"),
  //                 Axis(25, 0., 4., LeadingNonBJetDeltaPhi,   "Leading non-b dPhi(jet,MET)"),
  //                 preselection_multijets && LeadingNonBJetPt>0. && LeadingNonBJetDeltaPhi>0., signal1, bkg_hist).Tag("Pt_dPhi_2D_plots1");

  // pm.Push<Hist2D>(Axis(25, 0., 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]"),
  //                 Axis(25, 0., 4., LeadingNonBJetDeltaPhi,   "Leading non-b dPhi(jet,MET)"),
  //                 preselection_multijets && LeadingNonBJetPt>0. && LeadingNonBJetDeltaPhi>0., signal2, bkg_hist).Tag("Pt_dPhi_2D_plots2");
  
  // pm.Push<Hist2D>(Axis(25, 0., 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]"),
  //                 Axis(25, 0., 4., LeadingNonBJetDeltaPhi,   "Leading non-b dPhi(jet,MET)"),
  //                 preselection_multijets && LeadingNonBJetPt>0. && LeadingNonBJetDeltaPhi>0., signal3, bkg_hist).Tag("Pt_dPhi_2D_plots3");

  // pm.Push<Hist2D>(Axis(25, 0., 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]"),
  //                 Axis(25, 0., 4., LeadingNonBJetDeltaPhi,   "Leading non-b dPhi(jet,MET)"),
  //                 preselection_multijets && signalRegion && LeadingNonBJetPt>0. && LeadingNonBJetDeltaPhi>0., signal, bkg_hist).Tag("Pt_dPhi_2D_plots");

  // pm.Push<Hist2D>(Axis(25, 0., 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]1"),
  //                 Axis(25, 0., 4., LeadingNonBJetDeltaPhi,   "Leading non-b dPhi(jet,MET)"),
  //                 preselection_multijets && signalRegion && LeadingNonBJetPt>0. && LeadingNonBJetDeltaPhi>0., signal1, bkg_hist).Tag("Pt_dPhi_2D_plots1");

  // pm.Push<Hist2D>(Axis(25, 0., 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]2"),
  //                 Axis(25, 0., 4., LeadingNonBJetDeltaPhi,   "Leading non-b dPhi(jet,MET)"),
  //                 preselection_multijets && signalRegion && LeadingNonBJetPt>0.&& LeadingNonBJetDeltaPhi>0., signal2, bkg_hist).Tag("Pt_dPhi_2D_plots2");
  
  // pm.Push<Hist2D>(Axis(25, 0., 500., LeadingNonBJetPt, "Leading non-b p_{T} [GeV]3"),
  //                 Axis(25, 0., 4., LeadingNonBJetDeltaPhi,   "Leading non-b dPhi(jet,MET)"),
  //                 preselection_multijets && signalRegion && LeadingNonBJetPt>0. && LeadingNonBJetDeltaPhi>0., signal3, bkg_hist).Tag("Pt_dPhi_2D_plots3");




  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 //Signal region
  // pm.Push<Hist1D>(Axis(25, 0, 500., "pfmet", "E_{T}^{miss} [GeV]"),
  //                 preselection&&signalRegion, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "mt_met_lep", "m_{T} [GeV]"),
  //                 preselection&&signalRegion, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "leps_pt[0]", "Lepton p_{T} [GeV]"),
  //                 preselection&&signalRegion, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., LeadingBJetPt, "Leading b-jet p_{T} [GeV]"),
  //                 preselection&&signalRegion, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., SubLeadingBJetPt, "Subleading b-jet p_{T} [GeV]"),
  //                 preselection&&signalRegion, sample_list, all_plot_types);
  // pm.Push<Hist1D>(Axis(25, 0, 500., "mct", "M_{CT} [GeV]"),
  //                 preselection&&signalRegion, sample_list, all_plot_types);

 

  pm.MakePlots(lumi);

}
