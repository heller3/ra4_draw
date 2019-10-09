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

  double lumi = 140.;

  //string data_dir = "/home/users/rheller/wh_babies/babies_2019_02_27/";
  //string mc_dir = "/home/users/rheller/wh_babies/babies_2019_02_27/";
  string data_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_04_03/";
  string mc_dir = "/home/users/rheller/wh_babies/babies_v30_9_2019_04_03/";
  string signal_dir = "/home/users/rheller/wh_babies/babies_signal_2019_04_03/";
  
  string mc2018_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_08_13/a18v1/";
  string wjets2018_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_08_14/a18v1/";

   string mc2017_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_08_13/f17v2/";
  string mc2017_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_08_13/f17v2/";
  string wjets2017_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_08_14/f17v2/";

  


  string babytag = "v30_9";

  Palette colors("txt/colors.txt", "default");

  auto data = Process::MakeShared<Baby_full>("Data", Process::Type::data, colors("data"),{data_dir+"*data_2016*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1)");

   auto tt1l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2018", Process::Type::background, colors("tt_1l"),
    {mc2018_dir+"*TTJets_1lep_top_a18v1*.root",mc2018_dir+"*TTJets_1lep_tbar_a18v1*"});
  auto tt2l_2018 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2018", Process::Type::background, colors("tt_2l"),
    {mc2018_dir+"*TTJets_2lep_a18v1*.root"});

  auto tt1l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2017", Process::Type::background, colors("tt_1l"),
    {mc2017_dir+"*TTJets_1lep_top_f17v2*.root",mc2017_dir+"*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"*TTJets_1lep_*met150*.root"},"stitch");
  auto tt2l_2017 = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2017", Process::Type::background, colors("tt_2l"),
    {mc2017_dir+"*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"*TTJets_2lep_*met150*.root"},"stitch");


  auto wjets_2018 = Process::MakeShared<Baby_full>("W+jets 2018", Process::Type::background, colors("wjets"), {wjets2018_dir+"*slim_W*JetsToLNu_a18v1*",wjets2018_dir+"*W*Jets_NuPt200_a18v1*.root"},"stitch");
  auto wjets_2018_inclu = Process::MakeShared<Baby_full>("W+jets 2018", Process::Type::background, colors("wjets"), {wjets2018_dir+"*slim_W*JetsToLNu_a18v1*"});
    auto wjets_2017 = Process::MakeShared<Baby_full>("W+jets 2017", Process::Type::background, colors("wjets"), {wjets2017_dir+"*slim_W*JetsToLNu_f17v2*",wjets2017_dir+"*W*Jets_NuPt200_f17v2*.root"},"stitch");


  auto single_t_2018 = Process::MakeShared<Baby_full>("Single t 2018", Process::Type::background, colors("single_t"), {mc2018_dir+"*_ST_*.root"});
      auto single_t_2017 = Process::MakeShared<Baby_full>("Single t 2017", Process::Type::background, colors("single_t"), {mc2017_dir+"*_ST_*.root"});

    auto diboson_2018 = Process::MakeShared<Baby_full>("Diboson and t#bar{t}V 2018", Process::Type::background, colors("other"),{mc2018_dir+"*WW*.root", mc2018_dir+"*WZ*.root",mc2018_dir+"*ZZ*.root",mc2018_dir+"*_TTWJets*.root", mc2018_dir+"*_TTZ*.root"});
  auto diboson_2017 = Process::MakeShared<Baby_full>("Diboson and t#bar{t}V 2017", Process::Type::background, colors("other"),{mc2017_dir+"*WW*.root", mc2017_dir+"*WZ*.root",mc2017_dir+"*ZZ*.root",mc2017_dir+"*_TTWJets*.root", mc2017_dir+"*_TTZ*.root"});


   auto tchiwh_225_75 = Process::MakeShared<Baby_full>("TChiWH(300,100)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==300&&mass_lsp==100");
    tchiwh_225_75->SetLineStyle(2);
  auto tchiwh_250_1 = Process::MakeShared<Baby_full>("TChiWH(700,1)", Process::Type::signal, colors("t1tttt"),
    {signal_dir+"*SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*.root"},"mass_stop==700&&mass_lsp==1");


  vector<shared_ptr<Process> > sample_list2018 = {tt1l_2018, tt2l_2018, wjets_2018, single_t_2018, diboson_2018,tchiwh_250_1, tchiwh_225_75};
  vector<shared_ptr<Process> > sample_list2017 = {tt1l_2017, tt2l_2017, wjets_2017, single_t_2017, diboson_2017,tchiwh_250_1, tchiwh_225_75};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
   // .Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm)
    .RatioMinimum(0.5).RatioMaximum(1.5);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .Bottom(BottomType::off)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info/*lin_shapes_info*/};


  PlotMaker pm2017;
  PlotMaker pm2018;

  //Preselection
  NamedFunc signal_nm1_medmed = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&ngoodbtags==2"&& WHLeptons==1;

  vector<NamedFunc> sels = {signal_nm1_medmed};
  vector<string> weights = {"weight"};
  vector<string> tag_list = {"medmed"};

for(uint j=0;j<weights.size();j++){

      //Preselection, N-1
    pm2017.Push<Hist1D>(Axis(20, 0, 400., "mt_met_lep", "mT [GeV]"),
      sels[j] && "mct>200&&mbb<150&&mbb>90", sample_list2017, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    pm2017.Push<Hist1D>(Axis(20, 0, 400., "mct", "m_{CT} [GeV]"),
      sels[j] && "mt_met_lep>150&&mbb<150&&mbb>90", sample_list2017, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    pm2017.Push<Hist1D>(Axis(18, 50, 410., "mbb", "m_{bb} [GeV]"),
      sels[j] && "mt_met_lep>150&&mct>200", sample_list2017, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    pm2018.Push<Hist1D>(Axis(20, 0, 400., "mt_met_lep", "mT [GeV]"),
      sels[j] && "mct>200&&mbb<150&&mbb>90", sample_list2018, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    pm2018.Push<Hist1D>(Axis(20, 0, 400., "mct", "m_{CT} [GeV]"),
      sels[j] && "mt_met_lep>150&&mbb<150&&mbb>90", sample_list2018, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    pm2018.Push<Hist1D>(Axis(18, 50, 410., "mbb", "m_{bb} [GeV]"),
      sels[j] && "mt_met_lep>150&&mct>200", sample_list2018, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 



   }
  

  pm2017.MakePlots(lumi);
  pm2018.MakePlots(lumi);

}
