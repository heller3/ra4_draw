
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

  // luminosity
  double lumi = 137.2;

  // paths, old babies
  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_mc_s16v3_v33_4_2019_12_30/";
  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_mc_f17v2_v33_4_2019_12_30/";
  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_mc_a18v1_v33_4_2019_12_30/";

//  string data2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2019_12_30/";
//  string data2017_dir = "/home/users/dspitzba/wh_babies/babies_Run2017_v33_4_2019_12_30/";
//  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_Run2018_v33_4_2019_12_30/";

  string signal2016_dir = "/home/users/dspitzba/wh_babies/babies_signal_s16v3_v33_4_2019_12_30/";
  string signal2017_dir = "/home/users/dspitzba/wh_babies/babies_signal_f17v2_v33_4_2019_12_30/";
  string signal2018_dir = "/home/users/dspitzba/wh_babies/babies_signal_a18v1_v33_4_2019_12_30/";

  // paths, new babies
  string mc2016_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/s16v3/";
  string mc2017_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/f17v2/";
  string mc2018_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/a18v1/";

//  string data2016_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27babies_v33_4_2019_12_30/";
//  string data2017_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27babies_Run2017_v33_4_2019_12_30/";
//  string data2018_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27babies_Run2018_v33_4_2019_12_30/";

  string signal2016_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27babies_signal_s16v3_v33_4_2019_12_30/";
  string signal2017_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27babies_signal_f17v2_v33_4_2019_12_30/";
  string signal2018_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27babies_signal_a18v1_v33_4_2019_12_30/";


  //  string signal_dir = "hadoop/cms/store/user/dspitzba/WH_babies/";

  std::string random_cut = "ak4pfjets_pt>30";
  std::string basic_cut = "nWHLeptons==1&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2";
  // signal cut
  //WHLeptons==1&&“pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2”
  // basic cut 
  //  string basic_cuts =            "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2";

//  string hostname = execute("echo $HOSTNAME");
//  if(Contains(hostname, "cms") || Contains(hostname,"compute-")){
//    signal_dir = "/net/cms29";
//  }
 
  Palette colors("txt/colors.txt", "default");
  //  std::string bootlegcolor = "data";

  // Columns
  // names | type | color | file 

  //auto wh = Process::MakeShared<Baby_full>("wh", Process::Type::signal, colors(bootlegcolor),
  //  {signal_dir + "WH_FastSim_JEC.root"});
  //  auto signal = Process::MakeShared<Baby_full>("2017 Signal (225, 75)", Process::Type::signal, colors("t1tttt"), 
  //  {signal2017_dir + "slim_*.root"}, "pass&&mass_stop==225&&mass_lsp==75");

  //Data
//  auto data2016 = Process::MakeShared<Baby_full>("2016 Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
//  auto data2017 = Process::MakeShared<Baby_full>("2017 Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
//  auto data2018 = Process::MakeShared<Baby_full>("2018 Data", Process::Type::data, colors("data"),{data2018_dir+"slim_data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);
//  auto dataComb = Process::MakeShared<Baby_full>("Combined Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*.root",data2017_dir+"slim_data_2017*.root",data2018_dir+"slim_data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);

  //ttbar
  auto tt1l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*.root"},"stitch");

  auto tt1l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2017", Process::Type::background, colors("tt_1l"),{mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2017", Process::Type::background, colors("tt_2l"),{mc2017_dir+"slim*TTJets_2lep_*.root"},"stitch");

  auto tt1l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2018", Process::Type::background, colors("tt_1l"),{mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2018", Process::Type::background, colors("tt_2l"),{mc2018_dir+"slim*TTJets_2lep_*.root"},"stitch");

  auto tt1l_Comb = Process::MakeShared<Baby_full>("t#bar{t} (1l) Combined", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_1lep_top_*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*",mc2017_dir+"slim*TTJets_1lep_top_*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*",mc2018_dir+"slim*TTJets_1lep_top_*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*"},"stitch");
  auto tt2l_Comb = Process::MakeShared<Baby_full>("t#bar{t} (2l) Combined", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root"},"stitch");

  //WJets
  auto wjets_2016 = Process::MakeShared<Baby_full>("W+jets 2016", Process::Type::background, colors("wjets"), {mc2016_dir+"slim*_W*JetsToLNu_s16v3*",mc2016_dir+"slim*W*Jets_NuPt200_s16v*.root"},"stitch");
  auto wjets_2017 = Process::MakeShared<Baby_full>("W+jets 2017", Process::Type::background, colors("wjets"), {mc2017_dir+"slim*_W*JetsToLNu_f17v2*",mc2017_dir+"slim*W*Jets_NuPt200_f17v2*.root"},"stitch");
  auto wjets_2018 = Process::MakeShared<Baby_full>("W+jets 2018", Process::Type::background, colors("wjets"), {mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root"},"stitch");
  auto wjets_Comb = Process::MakeShared<Baby_full>("W+jets Combined", Process::Type::background, colors("wjets"), {mc2016_dir+"slim*_W*JetsToLNu_s16v3*",mc2016_dir+"slim*W*Jets_NuPt200_s16v*.root",mc2017_dir+"slim*_W*JetsToLNu_f17v2*",mc2017_dir+"slim*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim*W*Jets_NuPt200_a18v1*.root"},"stitch");

  //Single top
  auto single_t_2016 = Process::MakeShared<Baby_full>("Single t 2016", Process::Type::background, colors("single_t"), {mc2016_dir+"slim*_ST_*.root"});
  auto single_t_2017 = Process::MakeShared<Baby_full>("Single t 2017", Process::Type::background, colors("single_t"), {mc2017_dir+"slim*_ST_*.root"});
  auto single_t_2018 = Process::MakeShared<Baby_full>("Single t 2018", Process::Type::background, colors("single_t"), {mc2018_dir+"slim*_ST_*.root"});
  auto single_t_Comb = Process::MakeShared<Baby_full>("Single t Combined", Process::Type::background, colors("single_t"), {mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"});

  //diboson
  auto diboson_2016 = Process::MakeShared<Baby_full>("Diboson 2016", Process::Type::background, colors("other"),{mc2016_dir+"slim*WW*.root", mc2016_dir+"slim*WZ*.root",mc2016_dir+"slim*ZZ*.root"});
  auto diboson_2017 = Process::MakeShared<Baby_full>("Diboson 2017", Process::Type::background, colors("other"),{mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*WZ*.root",mc2017_dir+"slim*ZZ*.root"});
  auto diboson_2018 = Process::MakeShared<Baby_full>("Diboson 2018", Process::Type::background, colors("other"),{mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"});
  auto diboson_Comb = Process::MakeShared<Baby_full>("Diboson Combined", Process::Type::background, colors("other"),{mc2016_dir+"slim*WW*.root", mc2016_dir+"slim*WZ*.root",mc2016_dir+"slim*ZZ*.root",mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*WZ*.root",mc2017_dir+"slim*ZZ*.root",mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"});

  // ttV
  auto ttV_2016 = Process::MakeShared<Baby_full>("t#bar{t}V 2016", Process::Type::background, colors("ttv"),{mc2016_dir+"slim*_TTWJets*.root", mc2016_dir+"slim*_TTZ*.root"});
  auto ttV_2017 = Process::MakeShared<Baby_full>("t#bar{t}V 2017", Process::Type::background, colors("ttv"), {mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZ*.root"});
  auto ttV_2018 = Process::MakeShared<Baby_full>("t#bar{t}V 2018", Process::Type::background, colors("ttv"), {mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"});
  auto ttV_Comb = Process::MakeShared<Baby_full>("t#bar{t}V Combined", Process::Type::background, colors("ttv"), {mc2016_dir+"slim*_TTWJets*.root", mc2016_dir+"slim*_TTZ*.root",mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZ*.root",mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"});

  //signals
  auto signal_2016_225_75 = Process::MakeShared<Baby_full>("2016 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
  auto signal_2017_225_75 = Process::MakeShared<Baby_full>("2017 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2017_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
  auto signal_2018_225_75 = Process::MakeShared<Baby_full>("2018 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2018_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");
  auto signal_comb_225_75 = Process::MakeShared<Baby_full>("Combined Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root",signal2017_dir+"slim_*.root",signal2018_dir+"slim_*.root"},"pass&&mass_stop==225&&mass_lsp==75");

  auto signal_2016_650_300 = Process::MakeShared<Baby_full>("2016 Signal (650,300)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root"},"pass&&mass_stop==650&&mass_lsp==300");
  auto signal_2017_650_300 = Process::MakeShared<Baby_full>("2017 Signal (650,300)", Process::Type::signal, colors("t1tttt"),{signal2017_dir+"slim_*.root"},"pass&&mass_stop==650&&mass_lsp==300");
  auto signal_2018_650_300 = Process::MakeShared<Baby_full>("2018 Signal (650,300)", Process::Type::signal, colors("t1tttt"),{signal2018_dir+"slim_*.root"},"pass&&mass_stop==650&&mass_lsp==300");
  auto signal_comb_650_300 = Process::MakeShared<Baby_full>("2016-2018 Signal (650,300)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root",signal2017_dir+"slim_*.root",signal2018_dir+"slim_*.root"},"pass&&mass_stop==650&&mass_lsp==300");
    
  auto signal_2016_700_1 = Process::MakeShared<Baby_full>("2016 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_2017_700_1 = Process::MakeShared<Baby_full>("2017 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2017_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_2018_700_1 = Process::MakeShared<Baby_full>("2018 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2018_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_comb_700_1 = Process::MakeShared<Baby_full>("Combined Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*.root",signal2017_dir+"slim_*.root",signal2018_dir+"slim_*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  
  // Column combinations by year
  vector<shared_ptr<Process> > sample_list_2016 = {tt2l_2016,tt1l_2016,single_t_2016,wjets_2016,ttV_2016,diboson_2016,signal_2016_225_75,signal_2016_700_1,signal_2016_650_300};
  vector<shared_ptr<Process> > sample_list_2017 = {tt2l_2017,tt1l_2017,single_t_2017,wjets_2017,ttV_2017,diboson_2017,signal_2017_225_75,signal_2017_700_1,signal_2017_650_300};
  vector<shared_ptr<Process> > sample_list_2018 = {tt2l_2018,tt1l_2018,single_t_2018,wjets_2018,ttV_2018,diboson_2018,signal_2018_225_75,signal_2018_700_1,signal_2018_650_300};
  //vector<shared_ptr<Process> > sample_list_comb = {tt2l_Comb,single_t_Comb,ttV_Comb, diboson_Comb, signal_comb_225_75,signal_comb_700_1,signal_comb_650_300};
  //vector<shared_ptr<Process> > sample_list_all_comb = {dataComb,tt2l_Comb,tt1l_Comb,single_t_Comb,wjets_Comb,ttV_Comb, diboson_Comb, signal_comb_225_75,signal_comb_700_1,signal_comb_650_300};

  // Column combinations all years
  vector<shared_ptr<Process> > sample_list_comb = {tt2l_Comb, tt1l_Comb, single_t_Comb, wjets_Comb, ttV_Comb, diboson_Comb, signal_comb_225_75, signal_comb_700_1, signal_comb_650_300};
//  vector<shared_ptr<Process> > sample_list_comb = {/*dataComb,*/tt2l_Comb,/*tt1l_Comb,*/single_t_Comb,/*wjets_Comb,ttV_Comb, diboson_Comb,*/signal_comb_225_75,signal_comb_700_1,signal_comb_650_300};
//  vector<shared_ptr<Process> > sample_list_tt1l_comb = {dataComb,tt1l_Comb,signal_comb_225_75,signal_comb_700_1,signal_comb_650_300};

  //  vector<shared_ptr<Process> > sample_list = {wh};
  //  vector<shared_ptr<Process> > sample_list = {signal};

  // Plot graphics, sizing, etc.
//  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
//  log_lumi.Title(TitleType::preliminary)
//    .Bottom(BottomType::ratio)
//    .YAxis(YAxisType::log)
//    .Stack(StackType::data_norm);
//  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
//  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
//    .ShowBackgroundError(false);
//  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
//  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
//  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
//  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
//  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
//  vector<PlotOpt> all_plot_types = {log_lumi, lin_lumi, log_shapes, lin_shapes,
//                                    log_lumi_info, lin_lumi_info, log_shapes_info, lin_shapes_info};
//  PlotOpt style2D("txt/plot_styles.txt", "Scatter");
//  vector<PlotOpt> bkg_hist = {style2D().Stack(StackType::data_norm).Title(TitleType::preliminary)};
//  vector<PlotOpt> bkg_pts = {style2D().Stack(StackType::lumi_shapes).Title(TitleType::info)};

  // To make tables and plots
  PlotMaker pm;

  // signal cut
  // WHLeptons==1&&“pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2
  // &&pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2”

  // Making tables. Adding rows. 
//  Table & cutflow_SR_2016 = pm.Push<Table>("cutflow_SR_2016", vector<TableRow>{
//      TableRow("", basic_cut),
//	TableRow("125$<$MET", basic_cut&&"pfmet>125"),
//	TableRow("125$<$MET, 200$<$mct", basic_cut&&"pfmet>125&&mct>200"), 
//	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150"),
//	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"), 
//	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150, 2 good b tags", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2"), 
//	}, sample_list_2016, false);
//  Table & cutflow_SR_2017 = pm.Push<Table>("cutflow_SR_2017", vector<TableRow>{
//      TableRow("", basic_cut),
//	TableRow("125$<$MET", basic_cut&&"pfmet>125"),
//	TableRow("125$<$MET, 200$<$mct", basic_cut&&"pfmet>125&&mct>200"), 
//	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150"),
//	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"), 
//	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150, 2 good b tags", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2"), 
//	}, sample_list_2017, false);
//  Table & cutflow_SR_2018 = pm.Push<Table>("cutflow_SR_2018", vector<TableRow>{
//      TableRow("", basic_cut),
//	TableRow("125$<$MET", basic_cut&&"pfmet>125"),
//	TableRow("125$<$MET, 200$<$mct", basic_cut&&"pfmet>125&&mct>200"), 
//	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150"),
//	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"), 
//	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150, 2 good b tags", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2"), 
//	}, sample_list_2018, false);
  Table & cutflow_SR_all = pm.Push<Table>("cutflow_SR_all", vector<TableRow>{
      TableRow("", basic_cut, 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight),
	TableRow("125$<$MET", basic_cut&&"pfmet>125", 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight),
	//	TableRow("200$<$mct", basic_cut&&"mct>200", 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight),
	//	TableRow("150$<$mt MET lep", basic_cut&&"mt_met_lep>150", 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight),	
	//	TableRow("90$<$mbb$<$150", basic_cut&&"mbb>90&&mbb<150", 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight),
	//	TableRow("2 good b tags", basic_cut&&"ngoodbtags==2", 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight),
	TableRow("125$<$MET, 200$<$mct", basic_cut&&"pfmet>125&&mct>200", 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight), 
	//	TableRow("125$<$MET, 150$<$mt MET lep", basic_cut&&"pfmet>125&&mt_met_lep>150", 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight),
	//	TableRow("200$<$mct, 150$<$mt MET lep", basic_cut&&"mct>200&&mt_met_lep>150", 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight),
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150", 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight),
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight), 
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150, 2 good b tags", basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2", 0, 0, "weight*w_pu"*yearWeight*mcHEMWeight), 
//	TableRow("_____________________________________________________________________", "mct>10000"), // empty row
//	TableRow("", basic_cut),
//	TableRow("200$<$mct", basic_cut&&"mct>200"),
//	TableRow("200$<$mct, 125$<$MET", basic_cut&&"mct>200&&pfmet>125"), 
//	TableRow("200$<$mct, 125$<$MET, 150$<$mt MET lep", basic_cut&&"mct>200&&pfmet>125&&mt_met_lep>150"),
//	TableRow("200$<$mct, 125$<$MET, 150$<$mt MET lep, 90$<$mbb$<$150", basic_cut&&"mct>200&&pfmet>125&&mt_met_lep>150&&mbb>90&&mbb<150"), 
//	TableRow("200$<$mct, 125$<$MET, 150$<$mt MET lep, 90$<$mbb$<$150, 2 good b tags", basic_cut&&"mct>200&&pfmet>125&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2"), 
	}, sample_list_comb, false);
  

//  Table & cutflow = pm.Push<Table>("cutflow", vector<TableRow>{
//      TableRow("125$<$MET$<$300", random_cut&&"pfmet>125&&pfmet<300"),
//      TableRow("300$<$MET$<$400", random_cut&&"pfmet>300&&pfmet<400"),
//      TableRow("400$<$MET$<$500", random_cut&&"pfmet>400&&pfmet<500"),
//      TableRow("500$<$MET$<$600", random_cut&&"pfmet>500&&pfmet<600"),
//      TableRow("600$<$MET$<$700", random_cut&&"pfmet>600&&pfmet<700"),
//      TableRow("700$<$MET", random_cut&&"pfmet>700")
//	}, sample_list, false);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//  Table & cutflow = pm.Push<Table>("cutflow", vector<TableRow>{
//      TableRow("2b Region",basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2b Region, mbb>200",basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, mbb>200",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200")
//
//        }, sample_list,false);
//  
//  Table & cutflow_CvB = pm.Push<Table>("cutflow_CvB", vector<TableRow>{
//      TableRow("2b Region, $<$2 medium b",basic_cuts&&WHLeptons==1&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 tight c",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 medium c",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 tight c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 tight c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==1.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==1.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 medium c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 medium c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150")
//
//        }, sample_list,false);
//
//  Table & cutflow_CvB_highMbb = pm.Push<Table>("cutflow_CvB_highMbb", vector<TableRow>{
//      TableRow("2b Region, $<$2 medium b",basic_cuts&&WHLeptons==1&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 tight c",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 medium c",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 tight c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 tight c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==1.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==1.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 medium c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 medium c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200")
//
//        }, sample_list,false);
//
//  Table & cutflow_CvB_noMbb = pm.Push<Table>("cutflow_CvB_noMbb", vector<TableRow>{
//      TableRow("2b Region, $<$2 medium b",basic_cuts&&WHLeptons==1&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 tight c",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 medium c",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 tight c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 tight c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==1.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==1.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 medium c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 medium c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150")
//
//        }, sample_list,false);
//
//  Table & cutflow_CvB_v2 = pm.Push<Table>("cutflow_CvB_v2", vector<TableRow>{
//      TableRow("2b Region, $<$2 medium b",basic_cuts&&WHLeptons==1&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 tight c",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 medium c",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 loose CvB",basic_cuts&&WHLeptons==1&&nDeepLooseCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 tight c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 tight c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 2 tight c, 2 loose CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepLooseCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 loose CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepLooseCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150")
//
//        }, sample_list,false);
//
//  Table & cutflow_CvB_highMbb_v2 = pm.Push<Table>("cutflow_CvB_highMbb_v2", vector<TableRow>{
//      TableRow("2b Region, $<$2 medium b",basic_cuts&&WHLeptons==1&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 tight c",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 medium c",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 loose CvB",basic_cuts&&WHLeptons==1&&nDeepLooseCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 tight c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 tight c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 2 tight c, 2 loose CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepLooseCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 loose CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepLooseCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200")
//
//        }, sample_list,false);
//
//  Table & cutflow_CvB_noMbb_v2 = pm.Push<Table>("cutflow_CvB_noMbb_v2", vector<TableRow>{
//      TableRow("2b Region, $<$2 medium b",basic_cuts&&WHLeptons==1&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 tight c",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 medium c",basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 loose CvB",basic_cuts&&WHLeptons==1&&nDeepLooseCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 tight c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 tight c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 2 tight c, 2 loose CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepLooseCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 tight CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepTightCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 medium CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepMedCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150"),
//      TableRow("2c Region, 1 tight+1 medium c, 2 loose CvB",basic_cuts&&WHLeptons==1&&nDeepTightCTagged==1.&&nDeepMedCTagged==2.&&nDeepLooseCvBTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150")
//
//        }, sample_list,false);

  if(single_thread) pm.multithreaded_ = false;
  pm.MakePlots(lumi);

//  vector<GammaParams> yields_2016 = cutflow_SR_2016.BackgroundYield(lumi);
//  for(const auto &yield: yields_2016) {
//    cout << "my yield 2016: " << yield << endl;
//  }
//  vector<GammaParams> yields_2017 = cutflow_SR_2017.BackgroundYield(lumi);
//  for(const auto &yield: yields_2017) {
//    cout << "my yield 2017: " << yield << endl;
//  }
//  vector<GammaParams> yields_2018 = cutflow_SR_2018.BackgroundYield(lumi);
//  for(const auto &yield: yields_2018) {
//    cout << "my yield 2018: " << yield << endl;
//  }
  vector<GammaParams> yields_all = cutflow_SR_all.BackgroundYield(lumi);
  for(const auto &yield: yields_all) {
    cout << "my yield all: " << yield << endl;
  }

//  vector<GammaParams> yields = cutflow.BackgroundYield(lumi);
//  for(const auto &yield: yields){
//    cout << yield << endl;
//  }
//
//  vector<GammaParams> yields_CvB = cutflow_CvB.BackgroundYield(lumi);
//  for(const auto &yield: yields_CvB){
//    cout << yield << endl;
//  }
//
//  vector<GammaParams> yields_CvB_highMbb = cutflow_CvB_highMbb.BackgroundYield(lumi);
//  for(const auto &yield: yields_CvB_highMbb){
//    cout << yield << endl;
//  }
//  
//  vector<GammaParams> yields_CvB_noMbb = cutflow_CvB_noMbb.BackgroundYield(lumi);
//  for(const auto &yield: yields_CvB_noMbb){
//    cout << yield << endl;
//  }
//
//    vector<GammaParams> yields_CvB_v2 = cutflow_CvB_v2.BackgroundYield(lumi);
//  for(const auto &yield: yields_CvB_v2){
//    cout << yield << endl;
//  }
//
//  vector<GammaParams> yields_CvB_highMbb_v2 = cutflow_CvB_highMbb_v2.BackgroundYield(lumi);
//  for(const auto &yield: yields_CvB_highMbb_v2){
//    cout << yield << endl;
//  }
//  
//  vector<GammaParams> yields_CvB_noMbb_v2 = cutflow_CvB_noMbb_v2.BackgroundYield(lumi);
//  for(const auto &yield: yields_CvB_noMbb_v2){
//    cout << yield << endl;
//  }

}
