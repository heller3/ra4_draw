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

  string basic_cuts =            "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2";
  string basic_cuts_mct =        "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200";
  string basic_cuts_mct_mt =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150";
  string basic_cuts_mct_mbb =    "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150";
  string basic_cuts_mt_mbb =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>150&&mbb>90&&mbb<150";
  string basic_cuts_mct_mt_mbb = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";
  string dilep_basic_cuts =      "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2";
  string dilep_mct_mt_mbb =      "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";

  auto wjets_high_nu_GluonSplitting = Process::MakeShared<Baby_full>("W+jets, nu pT $>=$ 200: Gluon Splitting", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nEventsGluonSplit>=1.);
  auto wjets_high_nu_noGluonSplitting = Process::MakeShared<Baby_full>("W+jets, nu pT $>=$ 200: No Gluon Splitting", Process::Type::background, colors("qcd"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nEventsGluonSplit==0.);

  auto wjets_high_nu_modGluonSplitting = Process::MakeShared<Baby_full>("W+jets, nu pT $>=$ 200: Gluon Splitting", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nModEventsGluonSplit>=1.);
  auto wjets_high_nu_modnoGluonSplitting = Process::MakeShared<Baby_full>("W+jets, nu pT $>=$ 200: No Gluon Splitting", Process::Type::background, colors("qcd"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nModEventsGluonSplit==0.);

  auto wjets_high_nu_no_tag = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200: 0 b tags", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nDeepMedBTagged==0.);
  auto wjets_high_nu_one_tag = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200: 1 b tags", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nDeepMedBTagged==1.);
  auto wjets_high_nu_two_tag = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200: 2 b tags", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nDeepMedBTagged==2.);

  auto wjets_high_nu_glu_no_tag = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200: 0 b tags", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},gluBTagged==0.);
  auto wjets_high_nu_glu_one_tag = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200: 1 b tags", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},gluBTagged==1.);
  auto wjets_high_nu_glu_two_tag = Process::MakeShared<Baby_full>("W+jets, nu pT >= 200: 2 b tags", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},gluBTagged==2.);

  vector<shared_ptr<Process> > sample_list ={wjets_high_nu_GluonSplitting,wjets_high_nu_noGluonSplitting};
  vector<shared_ptr<Process> > b_tag_list ={wjets_high_nu_no_tag,wjets_high_nu_one_tag,wjets_high_nu_two_tag};
  vector<shared_ptr<Process> > glu_tag_list ={wjets_high_nu_glu_no_tag,wjets_high_nu_glu_one_tag,wjets_high_nu_glu_two_tag};
  vector<shared_ptr<Process> > mod_sample_list ={wjets_high_nu_modGluonSplitting,wjets_high_nu_modnoGluonSplitting};

  PlotMaker pm;

  //Selection Types
  NamedFunc no_b =    "pfmet>200" && basic_cuts_mct_mt && WHLeptons==1 && nDeepMedBTagged==0.;
  NamedFunc one_b =   "pfmet>200" && basic_cuts_mct_mt && WHLeptons==1 && nDeepMedBTagged==1 && nDeepLooseBTagged==1;
  NamedFunc two_b =   "pfmet>200" && basic_cuts_mct_mt && WHLeptons==1 && HasMedMedDeepCSV;
  NamedFunc sig_reg = "pfmet>200" && basic_cuts_mct_mt && WHLeptons==1;

  map<string, vector<shared_ptr<Process> > > categories;
  map<string, vector<shared_ptr<Process> > > categories_b_tag;
  map<string, vector<shared_ptr<Process> > > categories_glu_tag;
  map<string, vector<shared_ptr<Process> > > mod_categories;

  categories["process"] = vector<shared_ptr<Process> >();
  categories["process"].push_back(wjets_high_nu_GluonSplitting);
  categories["process"].push_back(wjets_high_nu_noGluonSplitting);

  categories_b_tag["b_tag"] = vector<shared_ptr<Process> >();
  categories_b_tag["b_tag"].push_back(wjets_high_nu_no_tag);
  categories_b_tag["b_tag"].push_back(wjets_high_nu_one_tag);
  categories_b_tag["b_tag"].push_back(wjets_high_nu_two_tag);

  categories_glu_tag["glu_tag"] = vector<shared_ptr<Process> >();
  categories_glu_tag["glu_tag"].push_back(wjets_high_nu_glu_no_tag);
  categories_glu_tag["glu_tag"].push_back(wjets_high_nu_glu_one_tag);
  categories_glu_tag["glu_tag"].push_back(wjets_high_nu_glu_two_tag);

  mod_categories["mod_process"] = vector<shared_ptr<Process> >();
  mod_categories["mod_process"].push_back(wjets_high_nu_modGluonSplitting);
  mod_categories["mod_process"].push_back(wjets_high_nu_modnoGluonSplitting);

  vector<TableRow> no_b_table_cuts;
  vector<TableRow> one_b_table_cuts;
  vector<TableRow> two_b_table_cuts;

  vector<TableRow> b_tag_table;

  vector<TableRow> glu_tag_table;

  vector<TableRow> mod_no_b_table_cuts;
  vector<TableRow> mod_one_b_table_cuts;
  vector<TableRow> mod_two_b_table_cuts;

  no_b_table_cuts.push_back(TableRow("0b", no_b,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  one_b_table_cuts.push_back(TableRow("1b", one_b,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  two_b_table_cuts.push_back(TableRow("2b", two_b,0,0,"w_noBtagSF*w_BtagSF_medmed")); 

  b_tag_table.push_back(TableRow("b_tag", sig_reg,0,0,"w_noBtagSF*w_BtagSF_medmed"));

  glu_tag_table.push_back(TableRow("glu_tag", sig_reg,0,0,"w_noBtagSF*w_BtagSF_medmed"));

  mod_no_b_table_cuts.push_back(TableRow("0b", no_b,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  mod_one_b_table_cuts.push_back(TableRow("1b", one_b,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  mod_two_b_table_cuts.push_back(TableRow("2b", two_b,0,0,"w_noBtagSF*w_BtagSF_medmed")); 

  for(auto &ipr: categories) pm.Push<Table>("mbb_chart_0B_"+ipr.first,  no_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("mbb_chart_1B_"+ipr.first,  one_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("mbb_chart_2B_"+ipr.first,  two_b_table_cuts, ipr.second, true, true, true, false);

  for(auto &ipr: categories_b_tag) pm.Push<Table>("mbb_chart_btag_"+ipr.first,  b_tag_table, ipr.second, true, true, true, false);

  for(auto &ipr: categories_glu_tag) pm.Push<Table>("mbb_chart_glutag_"+ipr.first,  glu_tag_table, ipr.second, true, true, true, false);

  for(auto &ipr: mod_categories) pm.Push<Table>("mbb_chart_mod0B_"+ipr.first,  mod_no_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: mod_categories) pm.Push<Table>("mbb_chart_mod1B_"+ipr.first,  mod_one_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: mod_categories) pm.Push<Table>("mbb_chart_mod2B_"+ipr.first,  mod_two_b_table_cuts, ipr.second, true, true, true, false);

  Table & no_b_chart = pm.Push<Table>("no_b_chart", no_b_table_cuts, sample_list, false);
  Table & one_b_chart = pm.Push<Table>("one_b_chart", one_b_table_cuts, sample_list, false);
  Table & two_b_chart = pm.Push<Table>("two_b_chart", two_b_table_cuts, sample_list, false);
  Table & btag_chart = pm.Push<Table>("btag_chart", b_tag_table, b_tag_list, false);
  Table & glutag_chart = pm.Push<Table>("glutag_chart", glu_tag_table, glu_tag_list, false);
  Table & mod_no_b_chart = pm.Push<Table>("mod_no_b_chart", mod_no_b_table_cuts, mod_sample_list, false);
  Table & mod_one_b_chart = pm.Push<Table>("mod_one_b_chart", mod_one_b_table_cuts, mod_sample_list, false);
  Table & mod_two_b_chart = pm.Push<Table>("mod_two_b_chart", mod_two_b_table_cuts, mod_sample_list, false);

  vector<GammaParams> yields_no_b = no_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_no_b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_one_b = one_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_one_b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signal = two_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_signal){
    cout << yield << endl;
  }

  vector<GammaParams> yields_btag = btag_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_btag){
    cout << yield << endl;
  }

  vector<GammaParams> yields_glutag = glutag_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_glutag){
    cout << yield << endl;
  }

    vector<GammaParams> yields_mod_no_b = mod_no_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_mod_no_b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_mod_one_b = mod_one_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_mod_one_b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_mod_signal = mod_two_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_mod_signal){
    cout << yield << endl;
  }

  pm.MakePlots(lumi);

}
