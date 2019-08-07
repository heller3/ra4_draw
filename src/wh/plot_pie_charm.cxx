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

  double lumi = 35.9;
  string data_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_04_03/";  
  string mc_dir= "/home/users/rheller/wh_babies/babies_v30_9_2019_04_03/";

  Palette colors("txt/colors.txt", "default");

  auto tt2l = Process::MakeShared<Baby_full>("t#bar{t} (2l)", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*TTJets_2lep*.root"});
 
  auto tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l)", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_1lep*.root"});

  auto wjets_low_nu = Process::MakeShared<Baby_full>("W+jets, nu pT $<$ 200", Process::Type::background, colors("qcd"),
    {mc_dir+"*slim_W*JetsToLNu_s16v3*.root"},NHighPtNu==0.);

  auto wjets_high_nu = Process::MakeShared<Baby_full>("W+jets, nu pT $>=$ 200", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"});

  auto single_t = Process::MakeShared<Baby_full>("Single t", Process::Type::background, colors("single_t"),
    {mc_dir+"*_ST_*.root"});
  
  auto single_t_2HF = Process::MakeShared<Baby_full>("Single t, 2+ HF", Process::Type::background, kBlue,
    {mc_dir+"*_ST_*.root"},nHeavy>=2.);

  auto single_t_1HF = Process::MakeShared<Baby_full>("Single t, 1 HF", Process::Type::background, kViolet+2,
    {mc_dir+"*_ST_*.root"},nHeavy==1.);

  auto single_t_0HF = Process::MakeShared<Baby_full>("Single t, 0 HF", Process::Type::background, colors("single_t"),
    {mc_dir+"*_ST_*.root"},nHeavy==0.);

  auto other = Process::MakeShared<Baby_full>("Other", Process::Type::background, colors("dy"),
    {mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root", mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"});

  auto tt2l_2b = Process::MakeShared<Baby_full>("t#bar{t} (2l), 2+ Heavy Flavor", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*TTJets_2lep*.root"},nHeavy>1.);

  auto tt2l_1b = Process::MakeShared<Baby_full>("t#bar{t} (2l), 0 or 1 Heavy Flavor", Process::Type::background, colors("t1tttt"),
    {mc_dir+"*TTJets_2lep*.root"},nHeavy<=1.);

  auto wjets_high_nu_2HF = Process::MakeShared<Baby_full>("W+jets, nu pT $>=$ 200, 2+ Heavy Flavor", Process::Type::background, kPink+6,
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nHeavy>=2.);

  auto wjets_high_nu_1HF = Process::MakeShared<Baby_full>("W+jets, nu pT $>=$ 200, 1 Heavy Flavor", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nHeavy==1.);

  auto wjets_high_nu_0HF = Process::MakeShared<Baby_full>("W+jets, nu pT $>=$ 200, 0 Heavy Flavor", Process::Type::background, colors("tt_ltau"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nHeavy==0.);

  vector<shared_ptr<Process> > sample_list ={tt2l,tt1l,wjets_low_nu,wjets_high_nu,single_t,other};
  vector<shared_ptr<Process> > b_full_sample_list ={tt2l_2b,tt2l_1b,tt1l,wjets_low_nu,wjets_high_nu_2HF,wjets_high_nu_1HF,wjets_high_nu_0HF,single_t_2HF,single_t_1HF,single_t_0HF,other};


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

  NamedFunc bbRegion =  basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";
  NamedFunc ccRegion =  basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";
  NamedFunc bbRegion_highmbb =  basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200";
  NamedFunc ccRegion_highmbb =  basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>200&&mct>200&&mt_met_lep>150&&mbb>200";
  NamedFunc bbRegion_preselection =  basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV&&"pfmet>125&&mt_met_lep>50";
  NamedFunc ccRegion_preselection =  basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.&&"pfmet>125&&mt_met_lep>50";

  map<string, vector<shared_ptr<Process> > > categories;
  map<string, vector<shared_ptr<Process> > > categories_split_Bs;

  categories["process"] = vector<shared_ptr<Process> >();
  categories["process"].push_back(tt2l);
  categories["process"].push_back(tt1l);
  categories["process"].push_back(wjets_low_nu);
  categories["process"].push_back(wjets_high_nu);
  categories["process"].push_back(single_t);
  categories["process"].push_back(other);

  categories_split_Bs["fullRealBs"] = vector<shared_ptr<Process> >();
  categories_split_Bs["fullRealBs"].push_back(tt2l_2b);
  categories_split_Bs["fullRealBs"].push_back(tt2l_1b);
  categories_split_Bs["fullRealBs"].push_back(tt1l);
  categories_split_Bs["fullRealBs"].push_back(wjets_low_nu);
  categories_split_Bs["fullRealBs"].push_back(wjets_high_nu_2HF);
  categories_split_Bs["fullRealBs"].push_back(wjets_high_nu_1HF);
  categories_split_Bs["fullRealBs"].push_back(wjets_high_nu_0HF);
  categories_split_Bs["fullRealBs"].push_back(single_t_2HF);
  categories_split_Bs["fullRealBs"].push_back(single_t_1HF);
  categories_split_Bs["fullRealBs"].push_back(single_t_0HF);
  categories_split_Bs["fullRealBs"].push_back(other);

  vector<TableRow> bb_table_cuts;
  vector<TableRow> cc_table_cuts;
  vector<TableRow> bb_highmbb_table_cuts;
  vector<TableRow> cc_highmbb_table_cuts;
  vector<TableRow> bb_preselection_table_cuts;
  vector<TableRow> cc_preselection_table_cuts;

  bb_table_cuts.push_back(TableRow("2b Region", bbRegion,0,0,"w_noBtagSF*w_BtagSF_WH")); 
  cc_table_cuts.push_back(TableRow("2c Region", ccRegion,0,0,"w_noBtagSF*w_BtagSF_WH")); 
  bb_highmbb_table_cuts.push_back(TableRow("2b Region, mbb>200", bbRegion_highmbb,0,0,"w_noBtagSF*w_BtagSF_WH")); 
  cc_highmbb_table_cuts.push_back(TableRow("2c Region, mbb>200", ccRegion_highmbb,0,0,"w_noBtagSF*w_BtagSF_WH")); 
  bb_preselection_table_cuts.push_back(TableRow("2b Region, preselection", bbRegion_preselection,0,0,"w_noBtagSF*w_BtagSF_WH")); 
  cc_preselection_table_cuts.push_back(TableRow("2c Region, preselection", ccRegion_preselection,0,0,"w_noBtagSF*w_BtagSF_WH")); 

  for(auto &ipr: categories) pm.Push<Table>("charmChart_2b_"+ipr.first,  bb_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("charmChart_2c_"+ipr.first,  cc_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("charmChart_2b_highmbb_"+ipr.first,  bb_highmbb_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("charmChart_2c_highmbb_"+ipr.first,  cc_highmbb_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("charmChart_2b_preselection_"+ipr.first,  bb_preselection_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("charmChart_2c_preselection_"+ipr.first,  cc_preselection_table_cuts, ipr.second, true, true, true, false);

  for(auto &ipr: categories_split_Bs) pm.Push<Table>("charmChart_2b_"+ipr.first,  bb_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("charmChart_2c_"+ipr.first,  cc_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("charmChart_2b_highmbb_"+ipr.first,  bb_highmbb_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("charmChart_2c_highmbb_"+ipr.first,  cc_highmbb_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("charmChart_2b_preselection_"+ipr.first,  bb_preselection_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("charmChart_2c_preselection_"+ipr.first,  cc_preselection_table_cuts, ipr.second, true, true, true, false);

  Table & bb_chart = pm.Push<Table>("2b_chart", bb_table_cuts, sample_list, false);
  Table & cc_chart = pm.Push<Table>("2c_chart", cc_table_cuts, sample_list, false);
  Table & bb_highmbb_chart = pm.Push<Table>("2b_highmbb_chart", bb_highmbb_table_cuts, sample_list, false);
  Table & cc_highmbb_chart = pm.Push<Table>("2c_highmbb_chart", cc_highmbb_table_cuts, sample_list, false);
  Table & bb_preselection_chart = pm.Push<Table>("2b_preselection_chart", bb_preselection_table_cuts, sample_list, false);
  Table & cc_preselection_chart = pm.Push<Table>("2c_preselection_chart", cc_preselection_table_cuts, sample_list, false);

  Table & split_b_bb_chart = pm.Push<Table>("split_b_2b_chart", bb_table_cuts, b_full_sample_list, false);
  Table & split_b_cc_chart = pm.Push<Table>("split_b_2c_chart", cc_table_cuts, b_full_sample_list, false);
  Table & split_b_bb_highmbb_chart = pm.Push<Table>("split_b_2b_highmbb_chart", bb_highmbb_table_cuts, b_full_sample_list, false);
  Table & split_b_cc_highmbb_chart = pm.Push<Table>("split_b_2c_highmbb_chart", cc_highmbb_table_cuts, b_full_sample_list, false);
  Table & split_b_bb_preselection_chart = pm.Push<Table>("split_b_2b_preselection_chart", bb_preselection_table_cuts, b_full_sample_list, false);
  Table & split_b_cc_preselection_chart = pm.Push<Table>("split_b_2c_preselection_chart", cc_preselection_table_cuts, b_full_sample_list, false);

  vector<GammaParams> yields_2b = bb_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_2b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_2c = cc_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_2c){
    cout << yield << endl;
  }

  vector<GammaParams> yields_2b_highmbb = bb_highmbb_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_2b_highmbb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_2c_highmbb = cc_highmbb_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_2c_highmbb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_2b_preselection = bb_preselection_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_2b_preselection){
    cout << yield << endl;
  }

  vector<GammaParams> yields_2c_preselection = cc_preselection_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_2c_preselection){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_2b= split_b_bb_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_2b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_2c= split_b_cc_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_2c){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_2b_highmbb= split_b_bb_highmbb_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_2b_highmbb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_2c_highmbb= split_b_cc_highmbb_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_2c_highmbb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_2b_preselection= split_b_bb_preselection_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_2b_preselection){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_2c_preselection= split_b_cc_preselection_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_2c_preselection){
    cout << yield << endl;
  }

  
  pm.MakePlots(lumi);

}
