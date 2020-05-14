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
#include "core/table.hpp"
#include "core/plot_opt.hpp"
#include "core/palette.hpp"
#include "core/hist1d.hpp"
#include "core/wh_functions.hpp"

using namespace std;
using namespace PlotOptTypes;
using namespace WH_Functions;

int main(){
  gErrorIgnoreLevel = 6000;

  double lumicomb = 137.2;

  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_mc_s16v3_v33_4_2019_12_30/";
  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_mc_f17v2_v33_4_2019_12_30/";
  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_mc_a18v1_v33_4_2019_12_30/";

  Palette colors("txt/colors.txt", "default");

  //ttbar
  auto tt2l_Comb_lostLight = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016-2018, lost e or mu", Process::Type::background, colors("tt_2l"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root"},nGenLightLeps==2&&"stitch");
  auto tt2l_Comb_lostHadTau = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016-2018, lost hadronic tau", Process::Type::background, colors("single_t"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root"},LostHadTaus==1&&"stitch");

  //Single top
  auto single_t_Comb_lostLight = Process::MakeShared<Baby_full>("Single t 2016-2018, lost e or mu", Process::Type::background, colors("wjets"), {mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2);
  auto single_t_Comb_lostHadTau = Process::MakeShared<Baby_full>("Single t 2016-2018, lost hadronic tau", Process::Type::background, colors("dy"), {mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},LostHadTaus==1);

  auto Comb_lostlep_lowpT = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, low pT", Process::Type::background, colors("dy"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[1]==1.&&"stitch");
  auto Comb_lostlep_largeeta = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, large eta", Process::Type::background, colors("wjets"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[1]==2.&&"stitch");
  auto Comb_lostlep_other = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, mis-ID, non-isolated, etc", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[1]==0.&&"stitch");

  auto Comb_lostTau_lowpT = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, low pT", Process::Type::background, colors("dy"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[0]==15&&causeLostLeps[1]==1.&&"stitch");
  auto Comb_lostTau_largeeta = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, large eta", Process::Type::background, colors("wjets"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[0]==15&&causeLostLeps[1]==2.&&"stitch");
  auto Comb_lostTau_other = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, mis-ID, non-isolated, etc", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[0]==15&&causeLostLeps[1]==0.&&"stitch");

  auto Comb_lostMu_lowpT = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, low pT", Process::Type::background, colors("dy"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[0]==13.&&causeLostLeps[1]==1.&&"stitch");
  auto Comb_lostMu_largeeta = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, large eta", Process::Type::background, colors("wjets"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[0]==13.&&causeLostLeps[1]==2.&&"stitch");
  auto Comb_lostMu_other = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, mis-ID, non-isolated, etc", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[0]==13.&&causeLostLeps[1]==0.&&"stitch");

  auto Comb_lostEl_lowpT = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, low pT", Process::Type::background, colors("dy"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[0]==11.&&causeLostLeps[1]==1.&&"stitch");
  auto Comb_lostEl_largeeta = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, large eta", Process::Type::background, colors("wjets"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[0]==11.&&causeLostLeps[1]==2.&&"stitch");
  auto Comb_lostEl_other = Process::MakeShared<Baby_full>("t#bar{t} (2l) + ST 2016-2018, mis-ID, non-isolated, etc", Process::Type::background, colors("tt_1l"),{mc2016_dir+"slim*TTJets_2lep_*.root",mc2017_dir+"slim*TTJets_2lep_*.root",mc2018_dir+"slim*TTJets_2lep_*.root",mc2016_dir+"slim*_ST_*.root",mc2017_dir+"slim*_ST_*.root",mc2018_dir+"slim*_ST_*.root"},nGenLightLeps==2&&causeLostLeps[0]==11.&&causeLostLeps[1]==0.&&"stitch");



  vector<shared_ptr<Process> > sample_list ={tt2l_Comb_lostLight,tt2l_Comb_lostHadTau,single_t_Comb_lostLight,single_t_Comb_lostHadTau};
  vector<shared_ptr<Process> > cause_sample_list ={Comb_lostlep_lowpT,Comb_lostlep_largeeta,Comb_lostlep_other};
  vector<shared_ptr<Process> > causeTau_sample_list ={Comb_lostTau_lowpT,Comb_lostTau_largeeta,Comb_lostTau_other};
  vector<shared_ptr<Process> > causeMu_sample_list ={Comb_lostMu_lowpT,Comb_lostMu_largeeta,Comb_lostMu_other};
  vector<shared_ptr<Process> > causeEl_sample_list ={Comb_lostEl_lowpT,Comb_lostEl_largeeta,Comb_lostEl_other};


  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::ratio)
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


  PlotMaker pm;

  //Selection Types
  string basic_cuts =            "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2";

  NamedFunc bbRegion =  basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV&&"pfmet>125&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";

  map<string, vector<shared_ptr<Process> > > categories;
  map<string, vector<shared_ptr<Process> > > LostTau;
  map<string, vector<shared_ptr<Process> > > LostMu;
  map<string, vector<shared_ptr<Process> > > LostEl;

  /*categories["process"] = vector<shared_ptr<Process> >();
  categories["process"].push_back(tt2l_Comb_lostLight);
  categories["process"].push_back(tt2l_Comb_lostHadTau);
  categories["process"].push_back(single_t_Comb_lostLight);
  categories["process"].push_back(single_t_Comb_lostHadTau);*/

  categories["process"] = vector<shared_ptr<Process> >();
  categories["process"].push_back(Comb_lostlep_lowpT);
  categories["process"].push_back(Comb_lostlep_largeeta);
  categories["process"].push_back(Comb_lostlep_other);

  categories["LostTau"] = vector<shared_ptr<Process> >();
  categories["LostTau"].push_back(Comb_lostlep_lowpT);
  categories["LostTau"].push_back(Comb_lostlep_largeeta);
  categories["LostTau"].push_back(Comb_lostlep_other);

  categories["LostMu"] = vector<shared_ptr<Process> >();
  categories["LostMu"].push_back(Comb_lostlep_lowpT);
  categories["LostMu"].push_back(Comb_lostlep_largeeta);
  categories["LostMu"].push_back(Comb_lostlep_other);

  categories["LostEl"] = vector<shared_ptr<Process> >();
  categories["LostEl"].push_back(Comb_lostlep_lowpT);
  categories["LostEl"].push_back(Comb_lostlep_largeeta);
  categories["LostEl"].push_back(Comb_lostlep_other);


  vector<TableRow> bb_table_cuts;

  bb_table_cuts.push_back(TableRow("cause", bbRegion,0,0,"weight*w_pu"*yearWeight*mcHEMWeight)); 

  for(auto &ipr: categories) pm.Push<Table>("pieChart_cause_"+ipr.first,  bb_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: LostTau) pm.Push<Table>("pieChart_cause_Tau_"+ipr.first,  bb_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: LostMu) pm.Push<Table>("pieChart_cause_Mu_"+ipr.first,  bb_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: LostEl) pm.Push<Table>("pieChart_cause_El_"+ipr.first,  bb_table_cuts, ipr.second, true, true, true, false);

  Table & cause_chart = pm.Push<Table>("cause_chart", bb_table_cuts, cause_sample_list, false);
  Table & causeTau_chart = pm.Push<Table>("causeTau_chart", bb_table_cuts, causeTau_sample_list, false);
  Table & causeMu_chart = pm.Push<Table>("causeMu_chart", bb_table_cuts, causeMu_sample_list, false);
  Table & causeEl_chart = pm.Push<Table>("causeEl_chart", bb_table_cuts, causeEl_sample_list, false);

  vector<GammaParams> yields_cause = cause_chart.BackgroundYield(lumicomb);
  for(const auto &yield: yields_cause){
    cout << yield << endl;
  }

  vector<GammaParams> yields_causeTau = causeTau_chart.BackgroundYield(lumicomb);
  for(const auto &yield: yields_causeTau){
    cout << yield << endl;
  }

  vector<GammaParams> yields_causeMu = causeMu_chart.BackgroundYield(lumicomb);
  for(const auto &yield: yields_causeMu){
    cout << yield << endl;
  }

  vector<GammaParams> yields_causeEl = causeEl_chart.BackgroundYield(lumicomb);
  for(const auto &yield: yields_causeEl){
    cout << yield << endl;
  }

  
  pm.MakePlots(lumicomb);

}
