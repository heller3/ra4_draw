
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

using namespace std;
using namespace PlotOptTypes;

namespace{
  bool single_thread = false;
}


NamedFunc HasMedLooseCSV("HasMedLooseCSV",[](const Baby &b) -> NamedFunc::ScalarType{
    int nloose=0;
    int nmedium=0;
    for (unsigned i(0); i<b.ak4pfjets_CSV()->size(); i++){
      if (b.ak4pfjets_CSV()->at(i) > 0.5426) nloose++;
      if (b.ak4pfjets_CSV()->at(i) > 0.8484) nmedium++;
    }
    if(nloose>=2 && nmedium>=1) return 1;
    else return 0;
  });

int main(){
  gErrorIgnoreLevel = 6000;
  

  double lumi = 35.9;

  string data_dir = "/home/users/rheller/wh_babies/babies_2019_01_08/";
  string mc_dir = "/home/users/rheller/wh_babies/babies_2019_01_07/";
  string signal_dir = "/home/users/rheller/wh_babies/babies_2019_01_06/";

  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname,"compute-")){
    signal_dir = "/net/cms29";
  }
 
  Palette colors("txt/colors.txt", "default");

  auto data = Process::MakeShared<Baby_full>("Data", Process::Type::data, colors("data"),
    {data_dir+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1)");
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

   auto tchiwh_225_75 = Process::MakeShared<Baby_full>("TChiWH(225,75)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==225&&mass_lsp==75");

   auto tchiwh_250_1 = Process::MakeShared<Baby_full>("TChiWH(250,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==250&&mass_lsp==1");

   auto tchiwh_350_100 = Process::MakeShared<Baby_full>("TChiWH(350,100)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==350&&mass_lsp==100");

  auto tchiwh_nc = Process::MakeShared<Baby_full>("TChiWH(500,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==500&&mass_lsp==1");
  tchiwh_nc->SetMarkerStyle(21);
  tchiwh_nc->SetMarkerSize(0.9);
  auto tchiwh_c = Process::MakeShared<Baby_full>("TChiWH(500,125)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==500&&mass_lsp==125");


  vector<shared_ptr<Process> > sample_list = {data,ttv,single_t,diboson,wjets,tt1l,tt2l,tchiwh_225_75,tchiwh_250_1,tchiwh_350_100, tchiwh_nc, tchiwh_c};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .ShowBackgroundError(false);
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
  
  PlotMaker pm;



  Table & cutflow = pm.Push<Table>("cutflow", vector<TableRow>{
      TableRow("1 good lep,","ngoodleps>=1"),
      TableRow("2nd lep veto","ngoodleps==1"),
      TableRow("Pass track veto", "ngoodleps==1&&PassTrackVeto"),
      TableRow("Pass tau veto", "ngoodleps==1&&PassTrackVeto&&PassTauVeto"),
      TableRow("==2jets", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2"),
      TableRow("one loose, one med", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2" && HasMedLooseCSV>0.),
      TableRow("==2btags", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2"&& HasMedLooseCSV>0.),
      TableRow("M$_{\\rm b\\bar{b}}$ window", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150"&& HasMedLooseCSV>0.),
      TableRow("M$_{\\rm CT}>170$ GeV", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170"&& HasMedLooseCSV>0.),
      TableRow("E$_{\\rm T}^{\\rm miss}>125$ GeV", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125"&& HasMedLooseCSV>0.),
      TableRow("M$_{\\rm T}>150$ GeV", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>150"&& HasMedLooseCSV>0.),
      TableRow("E$_{\\rm T}^{\\rm miss}>200$ GeV", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>200&&mt_met_lep>150"&& HasMedLooseCSV>0.)


        }, sample_list,false);

  Table & cutflow_no_weight = pm.Push<Table>("cutflow_raw", vector<TableRow>{
      TableRow("1 good lep,","ngoodleps>=1",0,0,"1"),
      TableRow("2nd lep veto","ngoodleps==1",0,0,"1"),
      TableRow("Pass track veto", "ngoodleps==1&&PassTrackVeto",0,0,"1"),
      TableRow("Pass tau veto", "ngoodleps==1&&PassTrackVeto&&PassTauVeto",0,0,"1"),
      TableRow("==2jets", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2",0,0,"1"),
      TableRow("one loose, one med", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2" && HasMedLooseCSV>0.,0,0,"1"),
      TableRow("==2btags", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2"&& HasMedLooseCSV>0.,0,0,"1"),
      TableRow("M$_{\\rm b\\bar{b}}$ window", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150"&& HasMedLooseCSV>0.,0,0,"1"),
      TableRow("M$_{\\rm CT}>$ 170 GeV", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170"&& HasMedLooseCSV>0.,0,0,"1"),
      TableRow("E$_{\\rm T}^{\\rm miss}>125$ GeV", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125"&& HasMedLooseCSV>0.,0,0,"1"),
      TableRow("M$_{\\rm T}>150$ GeV", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>150"&& HasMedLooseCSV>0.,0,0,"1"),
      TableRow("E$_{\\rm T}^{\\rm miss}>200$ GeV", "ngoodleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>200&&mt_met_lep>150"&& HasMedLooseCSV>0.,0,0,"1")

        }, sample_list,false);

  Table & cutflow_no_btag_sf = pm.Push<Table>("cutflow_no_btag_sf", vector<TableRow>{
      TableRow("1 good lep","ngoodleps>=1",0,0,"w_noBtagSF"),
      TableRow("2nd lep veto","ngoodleps==1&&nvetoleps==1",0,0,"w_noBtagSF"),
      TableRow("Pass track veto", "ngoodleps==1&&nvetoleps==1&&PassTrackVeto",0,0,"w_noBtagSF"),
      TableRow("Pass tau veto", "ngoodleps==1&&nvetoleps==1&&PassTrackVeto&&PassTauVeto",0,0,"w_noBtagSF"),
      TableRow("==2jets", "ngoodleps==1&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2",0,0,"w_noBtagSF"),
      TableRow("one loose, one med btag", "ngoodleps==1&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2" && HasMedLooseCSV>0.,0,0,"w_noBtagSF"),
     // TableRow("==2btags", "ngoodleps==1&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2"&& HasMedLooseCSV>0.,0,0,"w_noBtagSF"),
      TableRow("M$_{\\rm b\\bar{b}}$ window", "ngoodleps==1&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150"&& HasMedLooseCSV>0.,0,0,"w_noBtagSF"),
      TableRow("M$_{\\rm CT}>$ 170 GeV", "ngoodleps==1&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170"&& HasMedLooseCSV>0.,0,0,"w_noBtagSF"),
      TableRow("E$_{\\rm T}^{\\rm miss}>125$ GeV", "ngoodleps==1&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125"&& HasMedLooseCSV>0.,0,0,"w_noBtagSF"),
      TableRow("M$_{\\rm T}>150$ GeV", "ngoodleps==1&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>150"&& HasMedLooseCSV>0.,0,0,"w_noBtagSF"),
      TableRow("$125<$E$_{\\rm T}^{\\rm miss}<200$ GeV", "ngoodleps==1&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&pfmet<200&&mt_met_lep>150"&& HasMedLooseCSV>0.,1,0,"w_noBtagSF"),
      TableRow("E$_{\\rm T}^{\\rm miss}>200$ GeV", "ngoodleps==1&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mbb>90&&mbb<150&&mct>170&&pfmet>200&&mt_met_lep>150"&& HasMedLooseCSV>0.,0,0,"w_noBtagSF")

        }, sample_list,false);


//   Table & cutflow = pm.Push<Table>("cutflow", vector<TableRow>{
//       TableRow("1 good lep, 2 jets, $E_{\\rm T}^{\\rm miss}>100$ GeV","ngoodleps==1&&nvetoleps==0&&ngoodjets>=2&&pfmet>100"),
      
//       TableRow("Pass track veto", "ngoodleps==1&&nvetoleps==0&&ngoodjets>=2&&pfmet>100&&PassTrackVeto"),
//       TableRow("Pass tau veto", "ngoodleps==1&&nvetoleps==0&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto"),
//       TableRow("==2jets", "ngoodleps==1&&nvetoleps==0&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2"),
//       TableRow("==2btags", "ngoodleps==1&&nvetoleps==0&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2"),
//       TableRow("M$_{\\rm b#bar{b}}$ window", "ngoodleps==1&&nvetoleps==0&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150"),
//       TableRow("M$_{\\rm CT}$", "ngoodleps==1&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170"),
//       TableRow("$E_{\\rm T}^{\\rm miss}>125$ GeV", "ngoodleps==1&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125"),
//       TableRow("M$_{\\rm T}>150$ GeV", "ngoodleps==1&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>150")


//         }, sample_list);

// Table & cutflow_no_weight = pm.Push<Table>("cutflow_raw", vector<TableRow>{
//       TableRow("1 good lep, 2 jets, $E_{\\rm T}^{\\rm miss}>100$ GeV","ngoodleps==1&&ngoodjets>=2&&pfmet>100",0,0,"1"),
      
//       TableRow("Pass track veto", "ngoodleps==1&&ngoodjets>=2&&pfmet>100&&PassTrackVeto",0,0,"1"),
//       TableRow("Pass tau veto", "ngoodleps==1&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto",0,0,"1"),
//       TableRow("==2jets", "ngoodleps==1&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2",0,0,"1"),
//       TableRow("==2btags", "ngoodleps==1&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2",0,0,"1"),
//       TableRow("M$_{\\rm b#bar{b}}$ window", "ngoodleps==1&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150",0,0,"1"),
//       TableRow("M$_{\\rm CT}$", "ngoodleps==1&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170",0,0,"1"),
//       TableRow("$E_{\\rm T}^{\\rm miss}>125$ GeV", "ngoodleps==1&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125",0,0,"1"),
//       TableRow("M$_{\\rm T}>150$ GeV", "ngoodleps==1&&ngoodjets>=2&&pfmet>100&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&ngoodbtags==2&&mbb>90&&mbb<150&&mct>170&&pfmet>125&&mt_met_lep>150",0,0,"1")


//         }, sample_list);


  if(single_thread) pm.multithreaded_ = false;
  pm.MakePlots(lumi);

  vector<GammaParams> yields = cutflow.BackgroundYield(lumi);
  for(const auto &yield: yields){
    cout << yield << endl;
  }
  vector<GammaParams> yields_no_weight = cutflow_no_weight.BackgroundYield(lumi);
  for(const auto &yield: yields_no_weight){
    cout << yield << endl;
  }
  vector<GammaParams> yields_no_btag_sf = cutflow_no_btag_sf.BackgroundYield(lumi);
  for(const auto &yield: yields_no_btag_sf){
    cout << yield << endl;
  }

}
