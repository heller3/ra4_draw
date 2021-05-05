
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

  // paths, new babies
  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/s16v3/";
  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/f17v2/";
  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/a18v1/";

  string data2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/"; // slim_data_2016*.root
  string data2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/"; // slim_data_2017*.root
  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/"; // slim_data_2018*.root

  string signal2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/"; // slim*SMS*.root
  string signal2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/"; // slim*SMS*.root
  string signal2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/"; // slim*SMS*.root

//  string ZZ2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_06_02/s16v3/";
//  string ZZ2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_06_02/f17v2/";
//  string ZZ2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_06_02/a18v1/";

  std::string basic_cut = "pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2";
  std::string basic_cut_CR = "PassTrackVeto&&PassTauVeto";
  // signal cut
  //WHLeptons==1&&“pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2”

//  string hostname = execute("echo $HOSTNAME");
//  if(Contains(hostname, "cms") || Contains(hostname,"compute-")){
//    signal_dir = "/net/cms29";
//  }
 
  Palette colors("txt/colors.txt", "default");

  // Columns
  // names | type | color | file 

  //Data FIX THIS
  auto data2016 = Process::MakeShared<Baby_full>("2016 Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto data2017 = Process::MakeShared<Baby_full>("2017 Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  auto data2018 = Process::MakeShared<Baby_full>("2018 Data", Process::Type::data, colors("data"),{data2018_dir+"slim_data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);
  auto dataComb = Process::MakeShared<Baby_full>("Combined Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*.root",data2017_dir+"slim_data_2017*.root",data2018_dir+"slim_data_2018*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);

  // SM WH // use 'other' color for now, should have one named SM_WH (although it doesn't really matter for tables)
  auto SM_WH_2016 = Process::MakeShared<Baby_full>("SM WH 2016", Process::Type::background, colors("other"),{mc2016_dir+"slim_WplusH*root",mc2016_dir+"slim_WminusH*root"});
  auto SM_WH_2017 = Process::MakeShared<Baby_full>("SM WH 2017", Process::Type::background, colors("other"),{mc2017_dir+"slim_WplusH*root",mc2017_dir+"slim_WminusH*root"});
  auto SM_WH_2018 = Process::MakeShared<Baby_full>("SM WH 2018", Process::Type::background, colors("other"),{mc2018_dir+"slim_WplusH*root",mc2018_dir+"slim_WminusH*root"});
  auto SM_WH_Comb = Process::MakeShared<Baby_full>("SM WH Combined", Process::Type::background, colors("other"),{mc2016_dir+"slim_WplusH*root",mc2016_dir+"slim_WminusH*root",mc2017_dir+"slim_WplusH*root",mc2017_dir+"slim_WminusH*root",mc2018_dir+"slim_WplusH*root",mc2018_dir+"slim_WminusH*root"});

  //ttbar // separated high met // no stitch
  auto tt1l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_1lep_top_s*.root",mc2016_dir+"slim*TTJets_1lep_tbar_s*"});//,"stitch");
  auto tt2l_2016 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_s*.root"});//,"stitch");

  auto tt1l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2017", Process::Type::background, colors("tt_1l"),{mc2017_dir+"slim*TTJets_1lep_top_f*.root",mc2017_dir+"slim*TTJets_1lep_tbar_f*"});//,"stitch");
  auto tt2l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2017", Process::Type::background, colors("tt_2l"),{mc2017_dir+"slim*TTJets_2lep_f*.root"});//,"stitch");

  auto tt1l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2018", Process::Type::background, colors("tt_1l"),{mc2018_dir+"slim*TTJets_1lep_top_a*.root",mc2018_dir+"slim*TTJets_1lep_tbar_a*"});//,"stitch");
  auto tt2l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2018", Process::Type::background, colors("tt_2l"),{mc2018_dir+"slim*TTJets_2lep_a*.root"});//,"stitch");

  auto tt1l_2016_met = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016 met", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_1lep_top_*met*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*met*"});//,"stitch");
  auto tt2l_2016_met = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016 met", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*met*.root"});//,"stitch");
									 
  auto tt1l_2017_met = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2017 met", Process::Type::background, colors("tt_1l"),{mc2017_dir+"slim*TTJets_1lep_top_*met*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*met*"});//,"stitch");
  auto tt2l_2017_met = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2017 met", Process::Type::background, colors("tt_2l"),{mc2017_dir+"slim*TTJets_2lep_*met*.root"});//,"stitch");
									 
  auto tt1l_2018_met = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2018 met", Process::Type::background, colors("tt_1l"),{mc2018_dir+"slim*TTJets_1lep_top_*met*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*met*"});//,"stitch");
  auto tt2l_2018_met = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2018 met", Process::Type::background, colors("tt_2l"),{mc2018_dir+"slim*TTJets_2lep_*met*.root"});//,"stitch");

  auto tt1l_Comb = Process::MakeShared<Baby_full>("t#bar{t} (1l) Combined", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_1lep_top_s*.root",mc2016_dir+"slim*TTJets_1lep_tbar_s*",mc2017_dir+"slim*TTJets_1lep_top_f*.root",mc2017_dir+"slim*TTJets_1lep_tbar_f*",mc2018_dir+"slim*TTJets_1lep_top_a*.root",mc2018_dir+"slim*TTJets_1lep_tbar_a*"});//,"stitch");
  auto tt2l_Comb = Process::MakeShared<Baby_full>("t#bar{t} (2l) Combined", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_s*.root",mc2017_dir+"slim*TTJets_2lep_f*.root",mc2018_dir+"slim*TTJets_2lep_a*.root"});//,"stitch");
  auto tt1l_Comb_met = Process::MakeShared<Baby_full>("t#bar{t} (1l) Combined met", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_1lep_top_*met*.root",mc2016_dir+"slim*TTJets_1lep_tbar_*met*",mc2017_dir+"slim*TTJets_1lep_top_*met*.root",mc2017_dir+"slim*TTJets_1lep_tbar_*met*",mc2018_dir+"slim*TTJets_1lep_top_*met*.root",mc2018_dir+"slim*TTJets_1lep_tbar_*met*"});//,"stitch");
  auto tt2l_Comb_met = Process::MakeShared<Baby_full>("t#bar{t} (2l) Combined met", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*met*.root",mc2017_dir+"slim*TTJets_2lep_*met*.root",mc2018_dir+"slim*TTJets_2lep_*met*.root"});//,"stitch");

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

  //diboson // excluding 2017 TTWZ // excluding all ZZ // putting ZZ back in
  auto diboson_2016 = Process::MakeShared<Baby_full>("Diboson 2016", Process::Type::background, colors("other"),{mc2016_dir+"slim*WW*.root", mc2016_dir+"slim*WZ*.root" , mc2016_dir+"slim*ZZ*.root"});
  auto diboson_2017 = Process::MakeShared<Baby_full>("Diboson 2017", Process::Type::background, colors("other"),{mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*_WZ*.root", mc2017_dir+"slim*ZZ*.root"});
  auto diboson_2018 = Process::MakeShared<Baby_full>("Diboson 2018", Process::Type::background, colors("other"),{mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root" , mc2018_dir+"slim*ZZ*.root"});
  auto diboson_Comb = Process::MakeShared<Baby_full>("Diboson Combined", Process::Type::background, colors("other"),{mc2016_dir+"slim*WW*.root", mc2016_dir+"slim*WZ*.root",mc2016_dir+"slim*ZZ*.root",mc2017_dir+"slim*WW*.root", mc2017_dir+"slim*_WZ*.root",mc2017_dir+"slim*ZZ*.root",mc2018_dir+"slim*WW*.root", mc2018_dir+"slim*WZ*.root",mc2018_dir+"slim*ZZ*.root"});//, ZZ2016_dir+"slim*ZZTo*L*root",ZZ2017_dir+"slim*ZZ*root",ZZ2018_dir+"slim*ZZ*root"});

  // ttV // excluding TTZToLL_
  auto ttV_2016 = Process::MakeShared<Baby_full>("t#bar{t}V 2016", Process::Type::background, colors("ttv"),{mc2016_dir+"slim*_TTWJets*.root", mc2016_dir+"slim*_TTZ*.root"});
  auto ttV_2017 = Process::MakeShared<Baby_full>("t#bar{t}V 2017", Process::Type::background, colors("ttv"), {mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZToLLNuNu*.root", mc2017_dir+"slim*_TTZToQQ*.root"});
  auto ttV_2018 = Process::MakeShared<Baby_full>("t#bar{t}V 2018", Process::Type::background, colors("ttv"), {mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"});
  auto ttV_Comb = Process::MakeShared<Baby_full>("t#bar{t}V Combined", Process::Type::background, colors("ttv"), {mc2016_dir+"slim*_TTWJets*.root", mc2016_dir+"slim*_TTZ*.root",mc2017_dir+"slim*_TTWJets*.root", mc2017_dir+"slim*_TTZToLLNuNu*.root",mc2017_dir+"slim*_TTZToQQ*.root",mc2018_dir+"slim*_TTWJets*.root", mc2018_dir+"slim*_TTZ*.root"});

  //signals
  auto signal_2016_225_75 = Process::MakeShared<Baby_full>("2016 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*s16v3*.root"},"pass&&mass_stop==225&&mass_lsp==75");
  auto signal_2017_225_75 = Process::MakeShared<Baby_full>("2017 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2017_dir+"slim_*f17v2*.root"},"pass&&mass_stop==225&&mass_lsp==75");
  auto signal_2018_225_75 = Process::MakeShared<Baby_full>("2018 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2018_dir+"slim_*a18v1*.root"},"pass&&mass_stop==225&&mass_lsp==75");
  auto signal_comb_225_75 = Process::MakeShared<Baby_full>("Combined Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*s16v3*.root",signal2017_dir+"slim_*f17v2*.root",signal2018_dir+"slim_*a18v1*.root"},"pass&&mass_stop==225&&mass_lsp==75");

  auto signal_2016_650_300 = Process::MakeShared<Baby_full>("2016 Signal (650,300)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*s16v3*.root"},"pass&&mass_stop==650&&mass_lsp==300");
  auto signal_2017_650_300 = Process::MakeShared<Baby_full>("2017 Signal (650,300)", Process::Type::signal, colors("t1tttt"),{signal2017_dir+"slim_*f17v2*.root"},"pass&&mass_stop==650&&mass_lsp==300");
  auto signal_2018_650_300 = Process::MakeShared<Baby_full>("2018 Signal (650,300)", Process::Type::signal, colors("t1tttt"),{signal2018_dir+"slim_*a18v1*.root"},"pass&&mass_stop==650&&mass_lsp==300");
  auto signal_comb_650_300 = Process::MakeShared<Baby_full>("2016-2018 Signal (650,300)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*s16v3*.root",signal2017_dir+"slim_*f17v2*.root",signal2018_dir+"slim_*a18v1*.root"},"pass&&mass_stop==650&&mass_lsp==300");
    
  auto signal_2016_700_1 = Process::MakeShared<Baby_full>("2016 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*s16v3*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_2017_700_1 = Process::MakeShared<Baby_full>("2017 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2017_dir+"slim_*f17v2*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_2018_700_1 = Process::MakeShared<Baby_full>("2018 Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2018_dir+"slim_*a18v1*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  auto signal_comb_700_1 = Process::MakeShared<Baby_full>("Combined Signal (700,1)", Process::Type::signal, colors("t1tttt"),{signal2016_dir+"slim_*s16v3*.root",signal2017_dir+"slim_*f17v2*.root",signal2018_dir+"slim_*a18v1*.root"},"pass&&mass_stop==700&&mass_lsp==1");
  
  // Column combinations by year
  vector<shared_ptr<Process> > sample_list_2016 = {SM_WH_2016,tt2l_2016,tt1l_2016,tt2l_2016_met,tt1l_2016_met,single_t_2016,wjets_2016,ttV_2016,diboson_2016,signal_2016_225_75,signal_2016_700_1,signal_2016_650_300};
  vector<shared_ptr<Process> > sample_list_2017 = {SM_WH_2017,tt2l_2017,tt1l_2017,tt2l_2017_met,tt1l_2017_met,single_t_2017,wjets_2017,ttV_2017,diboson_2017,signal_2017_225_75,signal_2017_700_1,signal_2017_650_300};
  vector<shared_ptr<Process> > sample_list_2018 = {SM_WH_2018,tt2l_2018,tt1l_2018,tt2l_2018_met,tt1l_2018_met,single_t_2018,wjets_2018,ttV_2018,diboson_2018,signal_2018_225_75,signal_2018_700_1,signal_2018_650_300};
  // include data for CR
  vector<shared_ptr<Process> > CR_sample_list_2016 = {data2016,SM_WH_2016,tt2l_2016,tt1l_2016,tt2l_2016_met,tt1l_2016_met,single_t_2016,wjets_2016,ttV_2016,diboson_2016,signal_2016_225_75,signal_2016_700_1,signal_2016_650_300};
  vector<shared_ptr<Process> > CR_sample_list_2017 = {data2017,SM_WH_2017,tt2l_2017,tt1l_2017,tt2l_2017_met,tt1l_2017_met,single_t_2017,wjets_2017,ttV_2017,diboson_2017,signal_2017_225_75,signal_2017_700_1,signal_2017_650_300};
  vector<shared_ptr<Process> > CR_sample_list_2018 = {data2018,SM_WH_2018,tt2l_2018,tt1l_2018,tt2l_2018_met,tt1l_2018_met,single_t_2018,wjets_2018,ttV_2018,diboson_2018,signal_2018_225_75,signal_2018_700_1,signal_2018_650_300};
  //vector<shared_ptr<Process> > sample_list_comb = {tt2l_Comb,single_t_Comb,ttV_Comb, diboson_Comb, signal_comb_225_75,signal_comb_700_1,signal_comb_650_300};
  //vector<shared_ptr<Process> > sample_list_all_comb = {dataComb,tt2l_Comb,tt1l_Comb,single_t_Comb,wjets_Comb,ttV_Comb, diboson_Comb, signal_comb_225_75,signal_comb_700_1,signal_comb_650_300};

  // Column combinations all years
  vector<shared_ptr<Process> > sample_list_comb = {SM_WH_Comb, tt2l_Comb, tt1l_Comb, tt2l_Comb_met, tt1l_Comb_met, single_t_Comb, wjets_Comb, ttV_Comb, diboson_Comb, signal_comb_225_75, signal_comb_700_1, signal_comb_650_300};
  // include data for CR
  vector<shared_ptr<Process> > CR_sample_list_comb = {SM_WH_Comb, dataComb, tt2l_Comb, tt1l_Comb, tt2l_Comb_met, tt1l_Comb_met, single_t_Comb, wjets_Comb, ttV_Comb, diboson_Comb, signal_comb_225_75, signal_comb_700_1, signal_comb_650_300};
//  vector<shared_ptr<Process> > sample_list_comb = {/*dataComb,*/tt2l_Comb,/*tt1l_Comb,*/single_t_Comb,/*wjets_Comb,ttV_Comb, diboson_Comb,*/signal_comb_225_75,signal_comb_700_1,signal_comb_650_300};
//  vector<shared_ptr<Process> > sample_list_tt1l_comb = {dataComb,tt1l_Comb,signal_comb_225_75,signal_comb_700_1,signal_comb_650_300};

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
  Table & cutflow_SR_2016 = pm.Push<Table>("cutflow_SR_2016", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut, 0, 0, "w_noBtagSF"*yearWeight),
	TableRow("125$<$MET", WHLeptons==1&&basic_cut&&"pfmet>125", 0, 0, "w_noBtagSF"*yearWeight),
	TableRow("125$<$MET, 200$<$mct", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200", 0, 0, "w_noBtagSF"*yearWeight), 
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150", 0, 0, "w_noBtagSF"*yearWeight),
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", 0, 0, "w_noBtagSF"*yearWeight), 
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150, 2 good b tags", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2", 0, 0, "w_noBtagSF"*yearWeight), 
	}, sample_list_2016, false);
  Table & cutflow_SR_2017 = pm.Push<Table>("cutflow_SR_2017", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut, 0, 0, "weight"*yearWeight),
	TableRow("125$<$MET", WHLeptons==1&&basic_cut&&"pfmet>125", 0, 0, "weight"*yearWeight),
	TableRow("125$<$MET, 200$<$mct", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200", 0, 0, "weight"*yearWeight), 
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150", 0, 0, "weight"*yearWeight),
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", 0, 0, "weight"*yearWeight), 
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150, 2 good b tags", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2", 0, 0, "weight"*yearWeight), 
	}, sample_list_2017, false);
  Table & cutflow_SR_2018 = pm.Push<Table>("cutflow_SR_2018", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut, 0, 0, "weight"*yearWeight),
	TableRow("125$<$MET", WHLeptons==1&&basic_cut&&"pfmet>125", 0, 0, "weight"*yearWeight),
	TableRow("125$<$MET, 200$<$mct", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200", 0, 0, "weight"*yearWeight), 
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150", 0, 0, "weight"*yearWeight),
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", 0, 0, "weight"*yearWeight), 
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150, 2 good b tags", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2", 0, 0, "weight"*yearWeight), 
	}, sample_list_2018, false);
  Table & cutflow_SR_all = pm.Push<Table>("cutflow_SR_all", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut, 0, 0, "weight"*yearWeight),
	TableRow("125$<$MET", WHLeptons==1&&basic_cut&&"pfmet>125", 0, 0, "weight"*yearWeight),
	//	TableRow("200$<$mct", WHLeptons==1&&basic_cut&&"mct>200", 0, 0, "weight"*yearWeight),
	//	TableRow("150$<$mt MET lep", WHLeptons==1&&basic_cut&&"mt_met_lep>150", 0, 0, "weight"*yearWeight),	
	//	TableRow("90$<$mbb$<$150", WHLeptons==1&&basic_cut&&"mbb>90&&mbb<150", 0, 0, "weight"*yearWeight),
	//	TableRow("2 good b tags", WHLeptons==1&&basic_cut&&"ngoodbtags==2", 0, 0, "weight"*yearWeight),
	TableRow("125$<$MET, 200$<$mct", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200", 0, 0, "weight"*yearWeight), 
	//	TableRow("125$<$MET, 150$<$mt MET lep", WHLeptons==1&&basic_cut&&"pfmet>125&&mt_met_lep>150", 0, 0, "weight"*yearWeight),
	//	TableRow("200$<$mct, 150$<$mt MET lep", WHLeptons==1&&basic_cut&&"mct>200&&mt_met_lep>150", 0, 0, "weight"*yearWeight),
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150", 0, 0, "weight"*yearWeight),
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150", 0, 0, "weight"*yearWeight), 
	TableRow("125$<$MET, 200$<$mct, 150$<$mt MET lep, 90$<$mbb$<$150, 2 good b tags", WHLeptons==1&&basic_cut&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2", 0, 0, "weight"*yearWeight), 
//	TableRow("_____________________________________________________________________", "mct>10000"), // empty row
//	TableRow("", basic_cut),
//	TableRow("200$<$mct", basic_cut&&"mct>200"),
//	TableRow("200$<$mct, 125$<$MET", basic_cut&&"mct>200&&pfmet>125"), 
//	TableRow("200$<$mct, 125$<$MET, 150$<$mt MET lep", basic_cut&&"mct>200&&pfmet>125&&mt_met_lep>150"),
//	TableRow("200$<$mct, 125$<$MET, 150$<$mt MET lep, 90$<$mbb$<$150", basic_cut&&"mct>200&&pfmet>125&&mt_met_lep>150&&mbb>90&&mbb<150"), 
//	TableRow("200$<$mct, 125$<$MET, 150$<$mt MET lep, 90$<$mbb$<$150, 2 good b tags", basic_cut&&"mct>200&&pfmet>125&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2"), 
	}, sample_list_comb, false);

  // Control Region(s) tables
  // 1. 3 veto leps, 2 good jets
  Table & cutflow_CR_2GoodJets_2016 = pm.Push<Table>("cutflow_CR_2GoodJets_2016", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut_CR, 0, 0, "w_noBtagSF"*yearWeight),
	TableRow("3 veto leps", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3", 0, 0, "w_noBtagSF"*yearWeight),
	TableRow("3 veto leps, 2 good jets", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3&&ngoodjets==2", 0, 0, "w_noBtagSF"*yearWeight),
	TableRow("3 veto leps, 2 good jets, 125$<$MET", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3&&ngoodjets==2&&pfmet>125", 0, 0, "w_noBtagSF"*yearWeight),
	}, CR_sample_list_2016, false);
  Table & cutflow_CR_2GoodJets_2017 = pm.Push<Table>("cutflow_CR_2GoodJets_2017", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut_CR, 0, 0, "weight"*yearWeight),
	TableRow("3 veto leps", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3", 0, 0, "weight"*yearWeight),
	TableRow("3 veto leps, 2 good jets", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3&&ngoodjets==2", 0, 0, "weight"*yearWeight),
	TableRow("3 veto leps, 2 good jets, 125$<$MET", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3&&ngoodjets==2&&pfmet>125", 0, 0, "weight"*yearWeight),
	}, CR_sample_list_2017, false);
  Table & cutflow_CR_2GoodJets_2018 = pm.Push<Table>("cutflow_CR_2GoodJets_2018", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut_CR, 0, 0, "weight"*yearWeight),
	TableRow("3 veto leps", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3", 0, 0, "weight"*yearWeight),
	TableRow("3 veto leps, 2 good jets", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3&&ngoodjets==2", 0, 0, "weight"*yearWeight),
	TableRow("3 veto leps, 2 good jets, 125$<$MET", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3&&ngoodjets==2&&pfmet>125", 0, 0, "weight"*yearWeight),
	}, CR_sample_list_2018, false);
  Table & cutflow_CR_2GoodJets_all = pm.Push<Table>("cutflow_CR_2GoodJets_all", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut_CR, 0, 0, "weight"*yearWeight),
	TableRow("3 veto leps", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3", 0, 0, "weight"*yearWeight),
	TableRow("3 veto leps, 2 good jets", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3&&ngoodjets==2", 0, 0, "weight"*yearWeight),
	TableRow("3 veto leps, 2 good jets, 125$<$MET", WHLeptons==1&&basic_cut_CR&&"nvetoleps==3&&ngoodjets==2&&pfmet>125", 0, 0, "weight"*yearWeight),
	}, CR_sample_list_comb, false);
  // 2. 2 veto leps, 4 or 5 good jets
  Table & cutflow_CR_4_5GoodJets_2016 = pm.Push<Table>("cutflow_CR_4_5GoodJets_2016", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut_CR, 0, 0, "w_noBtagSF"*yearWeight),
	TableRow("2 veto leps", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2", 0, 0, "w_noBtagSF"*yearWeight),
	TableRow("2 veto leps, 4, 5 good jets", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2&&ngoodjets>=4&&ngoodjets<=5", 0, 0, "w_noBtagSF"*yearWeight),
	TableRow("2 veto leps, 4, 5 good jets, 125$<$MET", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125", 0, 0, "w_noBtagSF"*yearWeight),
	}, CR_sample_list_2016, false);
  Table & cutflow_CR_4_5GoodJets_2017 = pm.Push<Table>("cutflow_CR_4_5GoodJets_2017", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut_CR, 0, 0, "weight"*yearWeight),
	TableRow("2 veto leps", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2", 0, 0, "weight"*yearWeight),
	TableRow("2 veto leps, 4, 5 good jets", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2&&ngoodjets>=4&&ngoodjets<=5", 0, 0, "weight"*yearWeight),
	TableRow("2 veto leps, 4, 5 good jets, 125$<$MET", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125", 0, 0, "weight"*yearWeight),
	}, CR_sample_list_2017, false);
  Table & cutflow_CR_4_5GoodJets_2018 = pm.Push<Table>("cutflow_CR_4_5GoodJets_2018", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut_CR, 0, 0, "weight"*yearWeight),
	TableRow("2 veto leps", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2", 0, 0, "weight"*yearWeight),
	TableRow("2 veto leps, 4, 5 good jets", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2&&ngoodjets>=4&&ngoodjets<=5", 0, 0, "weight"*yearWeight),
	TableRow("2 veto leps, 4, 5 good jets, 125$<$MET", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125", 0, 0, "weight"*yearWeight),
	}, CR_sample_list_2018, false);
  Table & cutflow_CR_4_5GoodJets_all = pm.Push<Table>("cutflow_CR_4_5GoodJets_all", vector<TableRow>{
      TableRow("", WHLeptons==1&&basic_cut_CR, 0, 0, "weight"*yearWeight),
	TableRow("2 veto leps", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2", 0, 0, "weight"*yearWeight),
	TableRow("2 veto leps, 4, 5 good jets", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2&&ngoodjets>=4&&ngoodjets<=5", 0, 0, "weight"*yearWeight),
	TableRow("2 veto leps, 4, 5 good jets, 125$<$MET", WHLeptons==1&&basic_cut_CR&&"nvetoleps==2&&ngoodjets>=4&&ngoodjets<=5&&pfmet>125", 0, 0, "weight"*yearWeight),
	}, CR_sample_list_comb, false);


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

  if(single_thread) pm.multithreaded_ = false;
  pm.MakePlots(lumi);

  vector<GammaParams> yields_2016 = cutflow_SR_2016.BackgroundYield(lumi);
  for(const auto &yield: yields_2016) {
    cout << "my yield 2016: " << yield << endl;
  }
  cout << "---" << endl;
  vector<GammaParams> yields_2017 = cutflow_SR_2017.BackgroundYield(lumi);
  for(const auto &yield: yields_2017) {
    cout << "my yield 2017: " << yield << endl;
  }
  cout << "---" << endl;
  vector<GammaParams> yields_2018 = cutflow_SR_2018.BackgroundYield(lumi);
  for(const auto &yield: yields_2018) {
    cout << "my yield 2018: " << yield << endl;
  }
  cout << "---" << endl;
  vector<GammaParams> yields_all = cutflow_SR_all.BackgroundYield(lumi);
  for(const auto &yield: yields_all) {
    cout << "my yield all: " << yield << endl;
  }
  // 1.
  vector<GammaParams> yields_CR1_2016 = cutflow_CR_2GoodJets_2016.BackgroundYield(lumi);
  for(const auto &yield: yields_CR1_2016) {
    cout << "my yield 2016: " << yield << endl;
  }
  cout << "---" << endl;
  vector<GammaParams> yields_CR1_2017 = cutflow_CR_2GoodJets_2017.BackgroundYield(lumi);
  for(const auto &yield: yields_CR1_2017) {
    cout << "my yield 2017: " << yield << endl;
  }
  cout << "---" << endl;
  vector<GammaParams> yields_CR1_2018 = cutflow_CR_2GoodJets_2018.BackgroundYield(lumi);
  for(const auto &yield: yields_CR1_2018) {
    cout << "my yield 2018: " << yield << endl;
  }
  cout << "---" << endl;
  vector<GammaParams> yields_CR1_all = cutflow_CR_2GoodJets_all.BackgroundYield(lumi);
  for(const auto &yield: yields_CR1_all) {
    cout << "my yield all: " << yield << endl;
  }
  // 2.
  vector<GammaParams> yields_CR2_2016 = cutflow_CR_4_5GoodJets_2016.BackgroundYield(lumi);
  for(const auto &yield: yields_CR2_2016) {
    cout << "my yield 2016: " << yield << endl;
  }
  cout << "---" << endl;
  vector<GammaParams> yields_CR2_2017 = cutflow_CR_4_5GoodJets_2017.BackgroundYield(lumi);
  for(const auto &yield: yields_CR2_2017) {
    cout << "my yield 2017: " << yield << endl;
  }
  cout << "---" << endl;
  vector<GammaParams> yields_CR2_2018 = cutflow_CR_4_5GoodJets_2018.BackgroundYield(lumi);
  for(const auto &yield: yields_CR2_2018) {
    cout << "my yield 2018: " << yield << endl;
  }
  cout << "---" << endl;
  vector<GammaParams> yields_CR2_all = cutflow_CR_4_5GoodJets_all.BackgroundYield(lumi);
  for(const auto &yield: yields_CR2_all) {
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
