
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <unistd.h>
#include <getopt.h>

#include "TError.h"
#include "TColor.h"

#include "core/baby.hpp"
#include "core/process.hpp"
#include "core/named_func.hpp"
#include "core/plot_maker.hpp"
#include "core/plot_opt.hpp"
#include "core/palette.hpp"
#include "core/table.hpp"
#include "core/event_scan.hpp"
#include "core/hist1d.hpp"
#include "core/hist2d.hpp"
#include "core/utilities.hpp"
#include "core/functions.hpp"
#include "core/wh_functions.hpp"

using namespace std;
using namespace PlotOptTypes;
using namespace WH_Functions;

namespace{
  bool single_thread = false;
}


int main(){
  gErrorIgnoreLevel = 6000;
  

  double lumi = 133.53;

  string data_dir = "/home/users/rheller/wh_babies/babies_2019_02_12/";
  string mc_dir = "/home/users/rheller/wh_babies/babies_2019_02_11/";
  string signal_dir = "/home/users/rheller/wh_babies/babies_2019_02_12/";
  // string data_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_04_03/";
  // string mc_dir = "/home/users/rheller/wh_babies/babies_v30_9_2019_04_03/";
  // string signal_dir = "/home/users/rheller/wh_babies/babies_signal_2019_04_03/";

  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname,"compute-")){
    signal_dir = "/net/cms29";
  }
 
  Palette colors("txt/colors.txt", "default");

  // auto data = Process::MakeShared<Baby_full>("Data", Process::Type::data, colors("data"),
  //   {data_dir+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1)");
  // auto tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l)", Process::Type::background, colors("tt_1l"),
  //   {mc_dir+"*TTJets_1lep*ext1*.root"});
  // auto tt2l = Process::MakeShared<Baby_full>("t#bar{t} (2l)", Process::Type::background, colors("tt_2l"),
  //   {mc_dir+"*TTJets_2lep*ext1*.root"});
  // auto wjets = Process::MakeShared<Baby_full>("W+jets", Process::Type::background, colors("wjets"),
  //   {mc_dir+"*W*JetsToLNu*.root"});
  // auto single_t = Process::MakeShared<Baby_full>("Single t", Process::Type::background, colors("single_t"),
  //   {mc_dir+"*_ST_*.root"});
  // auto ttv = Process::MakeShared<Baby_full>("t#bar{t}V", Process::Type::background, colors("ttv"),
  //   {mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root"});
  // auto diboson = Process::MakeShared<Baby_full>("Diboson", Process::Type::background, colors("other"),
  //   {mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"});

  //  auto tchiwh_225_75 = Process::MakeShared<Baby_full>("TChiWH(225,75)", Process::Type::signal, colors("t1tttt"),
  //   {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==225&&mass_lsp==75");

  //  auto tchiwh_250_1 = Process::MakeShared<Baby_full>("TChiWH(250,1)", Process::Type::signal, colors("t1tttt"),
  //   {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==250&&mass_lsp==1");

  //  auto tchiwh_350_100 = Process::MakeShared<Baby_full>("TChiWH(350,100)", Process::Type::signal, colors("t1tttt"),
  //   {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==350&&mass_lsp==100");

  //  auto tchiwh_500_125 = Process::MakeShared<Baby_full>("TChiWH(500,125)", Process::Type::signal, colors("t1tttt"),
  //   {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==500&&mass_lsp==125");

  //  auto tchiwh_500_1 = Process::MakeShared<Baby_full>("TChiWH(500,1)", Process::Type::signal, colors("t1tttt"),
  //   {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==500&&mass_lsp==1");

  //  auto tchiwh_700_1 = Process::MakeShared<Baby_full>("TChiWH(700,1)", Process::Type::signal, colors("t1tttt"),
  //   {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==700&&mass_lsp==1");

  // auto tchiwh_nc = Process::MakeShared<Baby_full>("TChiWH(500,1)", Process::Type::signal, colors("t1tttt"),
  //   {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==500&&mass_lsp==1");
  // tchiwh_nc->SetMarkerStyle(21);
  // tchiwh_nc->SetMarkerSize(0.9);
  // auto tchiwh_c = Process::MakeShared<Baby_full>("TChiWH(500,125)", Process::Type::signal, colors("t1tttt"),
  //   {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==500&&mass_lsp==125");

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

  // vector<shared_ptr<Process> > sample_list = {data,ttv,single_t,diboson,wjets,tt1l,tt2l,tchiwh_225_75,tchiwh_250_1,tchiwh_350_100, tchiwh_nc, tchiwh_c};
  // vector<shared_ptr<Process> > jet_sample_list = {tt1l,tt2l,tchiwh_225_75,tchiwh_250_1,tchiwh_350_100, tchiwh_500_1, tchiwh_500_125, tchiwh_700_1};
  // vector<shared_ptr<Process> > twojet_sample_list = {ttv,single_t,diboson,wjets,tt1l,tt2l,tchiwh_225_75,tchiwh_250_1,tchiwh_350_100, tchiwh_500_1, tchiwh_500_125, tchiwh_700_1};

  vector<shared_ptr<Process> > sample_list = {data,ttv,single_t,diboson,wjets_low_nu, wjets_high_nu,tt1l,tt2l,tchiwh_225_75,tchiwh_250_1,tchiwh_350_100};
  vector<shared_ptr<Process> > jet_sample_list = {tt1l,tt2l,tchiwh_225_75,tchiwh_250_1,tchiwh_350_100, tchiwh_500_1, tchiwh_500_125, tchiwh_700_1};
  vector<shared_ptr<Process> > twojet_sample_list = {ttv,single_t,diboson,wjets_low_nu, wjets_high_nu,tt1l,tt2l,tchiwh_225_75,tchiwh_250_1,tchiwh_350_100, tchiwh_500_1, tchiwh_500_125, tchiwh_700_1};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .ShowBackgroundError(true);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {log_lumi, lin_lumi, log_shapes, lin_shapes,
                                    log_lumi_info, lin_lumi_info, log_shapes_info, lin_shapes_info};
  PlotOpt style2D("txt/plot_styles.txt", "Scatter");
  vector<PlotOpt> bkg_hist = {style2D().Stack(StackType::data_norm).Title(TitleType::preliminary)};
  vector<PlotOpt> bkg_pts = {style2D().Stack(StackType::lumi_shapes).Title(TitleType::info)};
  
  
  NamedFunc no_cuts = true;
  NamedFunc min_cuts = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3" && HasMedLooseCSV && WHLeptons==1;
  NamedFunc min_cuts_2medb = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3 && ngoodbtags==2" && WHLeptons==1;
  NamedFunc preselection = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>50" && HasMedLooseCSV && WHLeptons==1;
  NamedFunc preselection_nojetcuts = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&pfmet>125&&mt_met_lep>50" &&  HasMedLooseCSV && WHLeptons==1;// && HasISRCandidate;
  NamedFunc preselection_multijets = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&mt_met_lep>50&&pfmet>125" && HasMedLooseCSV && WHLeptons==1;
  NamedFunc preselection_multijets_2medb = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&mt_met_lep>50&&pfmet>125 && ngoodbtags==2"  && WHLeptons==1;
  NamedFunc cr_base = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>170&&pfmet>125&&mt_met_lep>150" && WHLeptons==1;
  NamedFunc cr0b = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>50" && NBJets==0. && WHLeptons==1;
  NamedFunc crmbb = "PassTrackVeto&&PassTauVeto&&ngoodjets==2&&!(mbb>90&&mbb<150)&&pfmet>125&&mt_met_lep>150" && HasMedLooseCSV && WHLeptons==1;
  NamedFunc signalRegion = "mbb>90&&mbb<150&&mct>170&&mt_met_lep>150"; //mct>170&&


  PlotMaker pm;

  // Table & pt_yeilds_medloose_loose = pm.Push<Table>("pt_yeilds_medloose_loose", vector<TableRow>{
  //     TableRow("Total", preselection_multijets && signalRegion && LeadingNonBJetPt!=0.),
  //     TableRow("30 $<$ P_T $\\leq$ 80\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt<=80 && LeadingNonBJetPt!=0.),
  //     TableRow("80 $<$ P_T $\\leq$ 130\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>80 && LeadingNonBJetPt<=130),
  //     TableRow("130 $<$ P_T $\\leq$ 180\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>130 && LeadingNonBJetPt<=180),
  //     TableRow("180 $<$ P_T $\\leq$ 230\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>180 && LeadingNonBJetPt<=230),
  //     TableRow("230 $<$ P_T \\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>230),
      
  //     }, jet_sample_list,true);


  // Table & pt_yeilds_medmed_med = pm.Push<Table>("pt_yeilds_medmed_med", vector<TableRow>{
  //     TableRow("Total", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med!=0.),
  //     TableRow("30 $<$ P_T $\\leq$ 80\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med<=80 && LeadingNonBJetPt_med!=0.),
  //     TableRow("80 $<$ P_T $\\leq$ 130\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med>80 && LeadingNonBJetPt_med<=130),
  //     TableRow("130 $<$ P_T $\\leq$ 180\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med>130 && LeadingNonBJetPt_med<=180),
  //     TableRow("180 $<$ P_T $\\leq$ 230\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med>180 && LeadingNonBJetPt_med<=230),
  //     TableRow("230 $<$ P_T \\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med>230),
      
  //     }, jet_sample_list,true);


  // Table & pt_yeilds_medmed_loose = pm.Push<Table>("pt_yeilds_medmed_loose", vector<TableRow>{
  //     TableRow("Total", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt!=0.),
  //     TableRow("30 $<$ P_T $\\leq$ 80\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt<=80 && LeadingNonBJetPt!=0.),
  //     TableRow("80 $<$ P_T $\\leq$ 130\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>80 && LeadingNonBJetPt<=130),
  //     TableRow("130 $<$ P_T $\\leq$ 180\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>130 && LeadingNonBJetPt<=180),
  //     TableRow("180 $<$ P_T $\\leq$ 230\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>180 && LeadingNonBJetPt<=230),
  //     TableRow("230 $<$ P_T \\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>230),
      
  //     }, jet_sample_list,true);

  // //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // MET 125-200

  // Table & pt_medloose_loose_first_met_bin = pm.Push<Table>("pt_medloose_loose_first_met_bin", vector<TableRow>{
  //     TableRow("Total", preselection_multijets && signalRegion && LeadingNonBJetPt!=0. && "pfmet<=200"),
  //     TableRow("30 $<$ P_T $\\leq$ 150\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt<=150 && LeadingNonBJetPt!=0. && "pfmet<=200"),
  //     TableRow("150 $<$ P_T $\\leq$ 250\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>150 && LeadingNonBJetPt<=250 && "pfmet<=200"),
  //     TableRow("250 $<$ P_T \\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>250 && "pfmet<=200"),
      
  //     }, jet_sample_list,true);


  Table & pt_medmed_med_first_met_bin = pm.Push<Table>("pt_medmed_med_first_met_bin", vector<TableRow>{
      TableRow("Total", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med!=0. && "pfmet<=200"),
      TableRow("30 $<$ P_T $\\leq$ 100\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med<=100 && LeadingNonBJetPt_med!=0. && "pfmet<=200"),
      TableRow("100 $<$ P_T \\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med>100 && "pfmet<=200"),
      
      }, jet_sample_list,true);


  // Table & pt_medmed_loose_first_met_bin = pm.Push<Table>("pt_medmed_loose_first_met_bin", vector<TableRow>{
  //     TableRow("Total", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt!=0. && "pfmet<=200"),
  //     TableRow("30 $<$ P_T $\\leq$ 150\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt<=150 && LeadingNonBJetPt!=0. && "pfmet<=200"),
  //     TableRow("150 $<$ P_T $\\leq$ 250\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>150 && LeadingNonBJetPt<=250 && "pfmet<=200"),
  //     TableRow("250 $<$ P_T \\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>250&& "pfmet<=200"),
      
  //     }, jet_sample_list,true);

  // //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // // MET 200-300

  // Table & pt_medloose_loose_second_met_bin = pm.Push<Table>("pt_medloose_loose_second_met_bin", vector<TableRow>{
  //     TableRow("Total", preselection_multijets && signalRegion && LeadingNonBJetPt!=0. && "pfmet>200" && "pfmet<=300"),
  //     TableRow("30 $<$ P_T $\\leq$ 100\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt<=100 && LeadingNonBJetPt!=0. && "pfmet>200" && "pfmet<=300"),
  //     TableRow("100 $<$ P_T $\\leq$ 200\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>100 && LeadingNonBJetPt<=200 && "pfmet>200" && "pfmet<=300"),
  //     TableRow("200 $<$ P_T \\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>200 && "pfmet>200" && "pfmet<=300"),
      
  //     }, jet_sample_list,true);


  // Table & pt_medmed_med_second_met_bin = pm.Push<Table>("pt_medmed_med_second_met_bin", vector<TableRow>{
  //     TableRow("Total", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med!=0. && "pfmet>200" && "pfmet<=300"),
  //     TableRow("30 $<$ P_T $\\leq$ 100\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med<=100 && LeadingNonBJetPt_med!=0. && "pfmet>200" && "pfmet<=300"),
  //     TableRow("100 $<$ P_T $\\leq$ 200\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med>100 && LeadingNonBJetPt_med<=200 && "pfmet>200" && "pfmet<=300"),
  //     TableRow("200 $<$ P_T \\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med>200 && "pfmet>200" && "pfmet<=300"),
      
  //     }, jet_sample_list,true);


  // Table & pt_medmed_loose_second_met_bin = pm.Push<Table>("pt_medmed_loose_second_met_bin", vector<TableRow>{
  //     TableRow("Total", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt!=0. && "pfmet>200" && "pfmet<=300"),
  //     TableRow("30 $<$ P_T $\\leq$ 100\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt<=100 && LeadingNonBJetPt!=0. && "pfmet>200" && "pfmet<=300"),
  //     TableRow("100 $<$ P_T $\\leq$ 200\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>100 && LeadingNonBJetPt<=200 && "pfmet>200" && "pfmet<=300"),
  //     TableRow("200 $<$ P_T \\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>200&& "pfmet>200" && "pfmet<=300"),
      
  //     }, jet_sample_list,true);

  // // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // // MET 300-400

  // Table & pt_medloose_loose_third_met_bin = pm.Push<Table>("pt_medloose_loose_third_met_bin", vector<TableRow>{
  //     TableRow("Total", preselection_multijets && signalRegion && LeadingNonBJetPt!=0. && "pfmet>300" && "pfmet<=400"),
  //     TableRow("30 $<$ P_T $\\leq$ 100\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt<=100 && LeadingNonBJetPt!=0. && "pfmet>300" && "pfmet<=400"),
  //     TableRow("100 $<$ P_T $\\leq$ 200\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>100 && LeadingNonBJetPt<=200 && "pfmet>300" && "pfmet<=400"),
  //     TableRow("200 $<$ P_T \\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>200 && "pfmet>300" && "pfmet<=400"),
      
  //     }, jet_sample_list,true);


  // Table & pt_medmed_med_third_met_bin = pm.Push<Table>("pt_medmed_med_third_met_bin", vector<TableRow>{
  //     TableRow("Total", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med!=0. && "pfmet>300" && "pfmet<=400"),
  //     TableRow("30 $<$ P_T $\\leq$ 100\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med<=100 && LeadingNonBJetPt_med!=0. && "pfmet>300" && "pfmet<=400"),
  //     TableRow("100 $<$ P_T $\\leq$ 200\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med>100 && LeadingNonBJetPt_med<=200 && "pfmet>300" && "pfmet<=400"),
  //     TableRow("200 $<$ P_T \\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med>200 && "pfmet>300" && "pfmet<=400"),
      
  //     }, jet_sample_list,true);


  // Table & pt_medmed_loose_third_met_bin = pm.Push<Table>("pt_medmed_loose_third_met_bin", vector<TableRow>{
  //     TableRow("Total", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt!=0. && "pfmet>300" && "pfmet<=400"),
  //     TableRow("30 $<$ P_T $\\leq$ 100\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt<=100 && LeadingNonBJetPt!=0. && "pfmet>300" && "pfmet<=400"),
  //     TableRow("100 $<$ P_T $\\leq$ 200\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>100 && LeadingNonBJetPt<=200 && "pfmet>300" && "pfmet<=400"),
  //     TableRow("200 $<$ P_T \\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>200&& "pfmet>300" && "pfmet<=400"),
      
  //     }, jet_sample_list,true);

  // //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // // MET > 200

  // Table & pt_medloose_loose_fourth_met_bin = pm.Push<Table>("pt_medloose_loose_fourth_met_bin", vector<TableRow>{
  //     TableRow("Total", preselection_multijets && signalRegion && LeadingNonBJetPt!=0. && "pfmet>200"),
  //     TableRow("30 $<$ P_T $\\leq$ 200\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt<=200 && LeadingNonBJetPt!=0. && "pfmet>200"),
  //     TableRow("200 $<$ P_T $\\leq$ 300\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>200 && LeadingNonBJetPt<=300 && "pfmet>200"),
  //     TableRow("300 $<$ P_T \\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>300 && "pfmet>200"),
      
  //     }, jet_sample_list,true);


  Table & pt_medmed_med_fourth_met_bin = pm.Push<Table>("pt_medmed_med_fourth_met_bin", vector<TableRow>{
      TableRow("Total", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med!=0. && "pfmet>200"),
      TableRow("30 $<$ P_T $\\leq$ 100\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med<=100 && LeadingNonBJetPt_med!=0. && "pfmet>200"),
      TableRow("100 $<$ P_T \\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt_med>100 && "pfmet>200"),
      
      }, jet_sample_list,true);


  // Table & pt_medmed_loose_fourth_met_bin = pm.Push<Table>("pt_medmed_loose_fourth_met_bin", vector<TableRow>{
  //     TableRow("Total", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt!=0. && "pfmet>200"),
  //     TableRow("30 $<$ P_T $\\leq$ 150\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt<=150 && LeadingNonBJetPt!=0. && "pfmet>200"),
  //     TableRow("150 $<$ P_T $\\leq$ 250\\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>150 && LeadingNonBJetPt<=250 && "pfmet>200"),
  //     TableRow("250 $<$ P_T \\,GeV", preselection_multijets_2medb && signalRegion && LeadingNonBJetPt>250&& "pfmet>200"),
      
  //     }, jet_sample_list,true);

  // //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



  // Table & dphi_geq_two = pm.Push<Table>("dphi_geq_two", vector<TableRow>{
  //     TableRow("Total", preselection_multijets && signalRegion),
  //     TableRow("30 $<$ P_T $\\leq$ 80\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt<=80 && LeadingNonBJetDeltaPhi>=2),
  //     TableRow("80 $<$ P_T $\\leq$ 130\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>80 && LeadingNonBJetPt<=130 && LeadingNonBJetDeltaPhi>=2),
  //     TableRow("130 $<$ P_T $\\leq$ 180\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>130 && LeadingNonBJetPt<=180 && LeadingNonBJetDeltaPhi>=2),
  //     TableRow("180 $<$ P_T $\\leq$ 230\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>180 && LeadingNonBJetPt<=230 && LeadingNonBJetDeltaPhi>=2),
  //     TableRow("230 $<$ P_T \\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>230 && LeadingNonBJetDeltaPhi>=2),
      
  //     }, jet_sample_list,false);

  // Table & dphi_geq_one_le_two = pm.Push<Table>("dphi_geq_one_le_two", vector<TableRow>{
  //     TableRow("Total", preselection_multijets && signalRegion),
  //     TableRow("30 $<$ P_T $\\leq$ 80\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt<=80 && LeadingNonBJetDeltaPhi>=1 && LeadingNonBJetDeltaPhi<2),
  //     TableRow("80 $<$ P_T $\\leq$ 130\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>80 && LeadingNonBJetPt<=130 && LeadingNonBJetDeltaPhi>=1 && LeadingNonBJetDeltaPhi<2),
  //     TableRow("130 $<$ P_T $\\leq$ 180\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>130 && LeadingNonBJetPt<=180 && LeadingNonBJetDeltaPhi>=1 && LeadingNonBJetDeltaPhi<2),
  //     TableRow("180 $<$ P_T $\\leq$ 230\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>180 && LeadingNonBJetPt<=230 && LeadingNonBJetDeltaPhi>=1 && LeadingNonBJetDeltaPhi<2),
  //     TableRow("230 $<$ P_T \\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>230 && LeadingNonBJetDeltaPhi>=1 && LeadingNonBJetDeltaPhi<2),
  
  //     }, jet_sample_list,false);

  // Table & dphi_le_one = pm.Push<Table>("dphi_le_one", vector<TableRow>{
  //     TableRow("Total", preselection_multijets && signalRegion),
  //     TableRow("30 $<$ P_T $\\leq$ 80\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt<=80 && LeadingNonBJetDeltaPhi<1),
  //     TableRow("80 $<$ P_T $\\leq$ 130\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>80 && LeadingNonBJetPt<=130 && LeadingNonBJetDeltaPhi<1),
  //     TableRow("130 $<$ P_T $\\leq$ 180\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>130 && LeadingNonBJetPt<=180 && LeadingNonBJetDeltaPhi<1),
  //     TableRow("180 $<$ P_T $\\leq$ 230\\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>180 && LeadingNonBJetPt<=230 && LeadingNonBJetDeltaPhi<1),
  //     TableRow("230 $<$ P_T \\,GeV", preselection_multijets && signalRegion && LeadingNonBJetPt>230 && LeadingNonBJetDeltaPhi<1),

  //     }, jet_sample_list,false);



//   Table & cutflow = pm.Push<Table>("cutflow", vector<TableRow>{
//       TableRow("1 good lep, 2 jets, $E_{\\rm T}^{\\rm miss}>100$ GeV",WHLeptons==1&&"nvetoleps==0&&ngoodjets>=2&&pfmet>100"),
      
//       TableRow("Pass track veto", WHLeptons==1&&"nvetoleps==0&&ngoodjets>=2&&pfmet>100&&PassTrackVeto"),
//       TableRow("Pass tau veto", WHLeptons==1&&"nvetoleps==0&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto"),
//       TableRow("==2jets", WHLeptons==1&&"nvetoleps==0&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2"),
//       TableRow("==2btags", WHLeptons==1&&"nvetoleps==0&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2"),
//       TableRow("M$_{\\rm b#bar{b}}$ window", WHLeptons==1&&"nvetoleps==0&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150"),
//       TableRow("M$_{\\rm CT}$", WHLeptons==1&&"ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170"),
//       TableRow("$E_{\\rm T}^{\\rm miss}>125$ GeV", WHLeptons==1&&"ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125"),
//       TableRow("M$_{\\rm T}>150$ GeV", WHLeptons==1&&"ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>150")


//         }, sample_list);

// Table & cutflow_no_weight = pm.Push<Table>("cutflow_raw", vector<TableRow>{
//       TableRow("1 good lep, 2 jets, $E_{\\rm T}^{\\rm miss}>100$ GeV",WHLeptons==1&&"ngoodjets>=2&&pfmet>100",0,0,"1"),
      
//       TableRow("Pass track veto", WHLeptons==1&&"ngoodjets>=2&&pfmet>100&&PassTrackVeto",0,0,"1"),
//       TableRow("Pass tau veto", WHLeptons==1&&"ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto",0,0,"1"),
//       TableRow("==2jets", WHLeptons==1&&"ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2",0,0,"1"),
//       TableRow("==2btags", WHLeptons==1&&"ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2",0,0,"1"),
//       TableRow("M$_{\\rm b#bar{b}}$ window", WHLeptons==1&&"ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150",0,0,"1"),
//       TableRow("M$_{\\rm CT}$", WHLeptons==1&&"ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170",0,0,"1"),
//       TableRow("$E_{\\rm T}^{\\rm miss}>125$ GeV", WHLeptons==1&&"ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125",0,0,"1"),
//       TableRow("M$_{\\rm T}>150$ GeV", WHLeptons==1&&"ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>150",0,0,"1")


//         }, sample_list);


  // Table & real_fake_leading_b = pm.Push<Table>("real_fake_leading_b", vector<TableRow>{
  //     TableRow("Total", preselection && signalRegion && LeadingBJetPt!=0.),
  //     TableRow("real", preselection && signalRegion && LeadingRealBJetPt!=0.),
  //     TableRow("fake", preselection && signalRegion && LeadingFakeBJetPt!=0.),
      
  //     }, twojet_sample_list,true);

  // Table & real_fake_subleading_b = pm.Push<Table>("real_fake_subleading_b", vector<TableRow>{
  //     TableRow("Total", preselection && signalRegion && SubLeadingBJetPt!=0.),
  //     TableRow("real", preselection && signalRegion && SubLeadingRealBJetPt!=0.),
  //     TableRow("fake", preselection && signalRegion && SubLeadingFakeBJetPt!=0.),
      
  //     }, twojet_sample_list,true);


  if(single_thread) pm.multithreaded_ = false;
  pm.MakePlots(lumi);

  // vector<GammaParams> yields = pt_yeilds_medloose_loose.BackgroundYield(lumi);
  // for(const auto &yield: yields){
  //   cout << yield << endl;
  // }

  // vector<GammaParams> yields1 = pt_yeilds_medmed_med.BackgroundYield(lumi);
  // for(const auto &yield: yields1){
  //   cout << yield << endl;
  // }

  // vector<GammaParams> yields2 = pt_yeilds_medmed_loose.BackgroundYield(lumi);
  // for(const auto &yield: yields2){
  //   cout << yield << endl;
  // }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // vector<GammaParams> yields_met1 = pt_medloose_loose_first_met_bin.BackgroundYield(lumi);
  // for(const auto &yield: yields_met1){
  //   cout << yield << endl;
  // }

  vector<GammaParams> yields1_met1 = pt_medmed_med_first_met_bin.BackgroundYield(lumi);
  for(const auto &yield: yields1_met1){
    cout << yield << endl;
  }

  // vector<GammaParams> yields2_met1 = pt_medmed_loose_first_met_bin.BackgroundYield(lumi);
  // for(const auto &yield: yields2_met1){
  //   cout << yield << endl;
  // }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  // vector<GammaParams> yields_met2 = pt_medloose_loose_second_met_bin.BackgroundYield(lumi);
  // for(const auto &yield: yields_met2){
  //   cout << yield << endl;
  // }

  // vector<GammaParams> yields1_met2 = pt_medmed_med_second_met_bin.BackgroundYield(lumi);
  // for(const auto &yield: yields1_met2){
  //   cout << yield << endl;
  // }

  // vector<GammaParams> yields2_met2 = pt_medmed_loose_second_met_bin.BackgroundYield(lumi);
  // for(const auto &yield: yields2_met2){
  //   cout << yield << endl;
  // }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // vector<GammaParams> yields_met3 = pt_medloose_loose_third_met_bin.BackgroundYield(lumi);
  // for(const auto &yield: yields_met3){
  //   cout << yield << endl;
  // }

  // vector<GammaParams> yields1_met3 = pt_medmed_med_third_met_bin.BackgroundYield(lumi);
  // for(const auto &yield: yields1_met3){
  //   cout << yield << endl;
  // }

  // vector<GammaParams> yields2_met3 = pt_medmed_loose_third_met_bin.BackgroundYield(lumi);
  // for(const auto &yield: yields2_met3){
  //   cout << yield << endl;
  // }

  // //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // vector<GammaParams> yields_met4 = pt_medloose_loose_fourth_met_bin.BackgroundYield(lumi);
  // for(const auto &yield: yields_met4){
  //   cout << yield << endl;
  // }

  vector<GammaParams> yields1_met4 = pt_medmed_med_fourth_met_bin.BackgroundYield(lumi);
  for(const auto &yield: yields1_met4){
    cout << yield << endl;
  }

  // vector<GammaParams> yields2_met4 = pt_medmed_loose_fourth_met_bin.BackgroundYield(lumi);
  // for(const auto &yield: yields2_met4){
  //   cout << yield << endl;
  // }

  // vector<GammaParams> yields3 = dphi_geq_two.BackgroundYield(lumi);
  // for(const auto &yield: yields3){
  //   cout << yield << endl;
  // }
  // vector<GammaParams> yields4 = dphi_geq_one_le_two.BackgroundYield(lumi);
  // for(const auto &yield: yields4){
  //   cout << yield << endl;
  // }
  // vector<GammaParams> yields5 = dphi_le_one.BackgroundYield(lumi);
  // for(const auto &yield: yields6){
  //   cout << yield << endl;
  // }

  // vector<GammaParams> leading = real_fake_leading_b.BackgroundYield(lumi);
  // for(const auto &yield: leading){
  //   cout << yield << endl;
  // }

  // vector<GammaParams> subleading = real_fake_subleading_b.BackgroundYield(lumi);
  // for(const auto &yield: subleading){
  //   cout << yield << endl;
  // }

}
