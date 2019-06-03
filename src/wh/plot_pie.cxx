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

  auto other = Process::MakeShared<Baby_full>("Other", Process::Type::background, colors("dy"),
    {mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root", mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"});

  auto tt2l_2b = Process::MakeShared<Baby_full>("t#bar{t} (2l), 2 Real b", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*TTJets_2lep*.root"},nRealBs==2.);

  auto tt2l_1b = Process::MakeShared<Baby_full>("t#bar{t} (2l), 0 or 1 Real b", Process::Type::background, colors("t1tttt"),
    {mc_dir+"*TTJets_2lep*.root"},nRealBs<2.);

  auto wjets_high_nu_1b = Process::MakeShared<Baby_full>("W+jets, nu pT $>=$ 200, 1 or more Real b", Process::Type::background, colors("wjets"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nRealBs>=1.);

  auto wjets_high_nu_0b = Process::MakeShared<Baby_full>("W+jets, nu pT $>=$ 200, 0 Real b", Process::Type::background, colors("tt_ltau"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},nRealBs==0.);

  auto tot_background_2B = Process::MakeShared<Baby_full>("All Background, 2 Real b", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*TTJets_2lep*.root", mc_dir+"*TTJets_1lep*.root", mc_dir+"*slim_W*Jets_NuPt200*.root", mc_dir+"*_ST_*.root", mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root", mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"},nRealBs==2.);

  auto tot_background_1B = Process::MakeShared<Baby_full>("All Background, 1 Real b", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*TTJets_2lep*.root", mc_dir+"*TTJets_1lep*.root", mc_dir+"*slim_W*Jets_NuPt200*.root", mc_dir+"*_ST_*.root", mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root", mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"},nRealBs==1.);

  auto tot_background_0B = Process::MakeShared<Baby_full>("All Background, 0 Real b", Process::Type::background, colors("dy"),
    {mc_dir+"*TTJets_2lep*.root", mc_dir+"*TTJets_1lep*.root", mc_dir+"*slim_W*Jets_NuPt200*.root", mc_dir+"*_ST_*.root", mc_dir+"*_TTWJets*.root", mc_dir+"*_TTZ*.root", mc_dir+"*WW*.root", mc_dir+"*WZ*.root",mc_dir+"*ZZ*.root"},nRealBs==0.);

  vector<shared_ptr<Process> > sample_list ={tt2l,tt1l,wjets_low_nu,wjets_high_nu,single_t,other};
  vector<shared_ptr<Process> > real_bs_sample_list ={tot_background_2B,tot_background_1B,tot_background_0B};
  vector<shared_ptr<Process> > b_full_sample_list ={tt2l_2b,tt2l_1b,tt1l,wjets_low_nu,wjets_high_nu_1b,wjets_high_nu_0b,single_t,other};

//  vector<string> tag_list = {"tt1l","tt2l"};
//  vector<shared_ptr<Process> > sample_list = {data, /*tchiwh_250_1, tchiwh_225_75,*/ tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson};

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
  string basic_cuts_mct =        "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200";
  string basic_cuts_mct_mt =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150";
  string basic_cuts_mct_mbb =    "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150";
  string basic_cuts_mt_mbb =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>150&&mbb>90&&mbb<150";
  string basic_cuts_mct_mt_mbb = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";
  string dilep_basic_cuts =      "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2";
  string dilep_mct_mt_mbb =      "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";

  NamedFunc preselection =        "pfmet>125&&mt_met_lep>50" && basic_cuts && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc preselection_highmT = "pfmet>125&&mt_met_lep>150" && basic_cuts && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc signalRegion =        "pfmet>200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc mbbside =             "pfmet>200&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc no_b =                "pfmet>200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && nDeepMedBTagged==0.;
  NamedFunc one_b =               "pfmet>200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && nDeepMedBTagged==1 && nDeepLooseBTagged==1;
  NamedFunc dilep =               "pfmet>125" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc lowMT =               "pfmet>200&&mt_met_lep>50&&mt_met_lep<150" && basic_cuts_mct_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc lowMCT =              "pfmet>200&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc preselection_medmed = "pfmet>125&&mt_met_lep>50" && basic_cuts && WHLeptons==1 && HasMedMedDeepCSV;
  NamedFunc signalRegion_medmed = "pfmet>200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedMedDeepCSV;

  NamedFunc preselection_lowMET =        "pfmet>125&&pfmet<200&&mt_met_lep>50" && basic_cuts && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc preselection_highmT_lowMET = "pfmet>125&&pfmet<200&&mt_met_lep>150" && basic_cuts && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc signalRegion_lowMET =        "pfmet>125&&pfmet<200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc mbbside_lowMET =             "pfmet>125&&pfmet<200&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc no_b_lowMET =                "pfmet>125&&pfmet<200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && nDeepMedBTagged==0.;
  NamedFunc one_b_lowMET =               "pfmet>125&&pfmet<200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && nDeepMedBTagged==1 && nDeepLooseBTagged==1;
  NamedFunc dilep_lowMET =               "pfmet>125&&pfmet<200" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc lowMT_lowMET =               "pfmet>125&&pfmet<200&&mt_met_lep>50&&mt_met_lep<150" && basic_cuts_mct_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc lowMCT_lowMET =              "pfmet>125&&pfmet<200&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc preselection_medmed_lowMET = "pfmet>125&&pfmet<200&&mt_met_lep>50" && basic_cuts && WHLeptons==1 && HasMedMedDeepCSV;
  NamedFunc signalRegion_medmed_lowMET = "pfmet>125&&pfmet<200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedMedDeepCSV;

  NamedFunc preselection_medMET =        "pfmet>200&&pfmet<300&&mt_met_lep>50" && basic_cuts && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc preselection_highmT_medMET = "pfmet>200&&pfmet<300&&mt_met_lep>150" && basic_cuts && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc signalRegion_medMET =        "pfmet>200&&pfmet<300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc mbbside_medMET =             "pfmet>200&&pfmet<300&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc no_b_medMET =                "pfmet>200&&pfmet<300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && nDeepMedBTagged==0.;
  NamedFunc one_b_medMET =               "pfmet>200&&pfmet<300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && nDeepMedBTagged==1 && nDeepLooseBTagged==1;
  NamedFunc dilep_medMET =               "pfmet>200&&pfmet<300" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc lowMT_medMET =               "pfmet>200&&pfmet<300&&mt_met_lep>50&&mt_met_lep<150" && basic_cuts_mct_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc lowMCT_medMET =              "pfmet>200&&pfmet<300&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc preselection_medmed_medMET = "pfmet>200&&pfmet<300&&mt_met_lep>50" && basic_cuts && WHLeptons==1 && HasMedMedDeepCSV;
  NamedFunc signalRegion_medmed_medMET = "pfmet>200&&pfmet<300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedMedDeepCSV;

  NamedFunc preselection_highMET =        "pfmet>300&&mt_met_lep>50" && basic_cuts && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc preselection_highmT_highMET = "pfmet>300&&mt_met_lep>150" && basic_cuts && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc signalRegion_highMET =        "pfmet>300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc mbbside_highMET =             "pfmet>300&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc no_b_highMET =                "pfmet>300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && nDeepMedBTagged==0.;
  NamedFunc one_b_highMET =               "pfmet>300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && nDeepMedBTagged==1 && nDeepLooseBTagged==1;
  NamedFunc dilep_highMET =               "pfmet>300" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc lowMT_highMET =               "pfmet>300&&mt_met_lep>50&&mt_met_lep<150" && basic_cuts_mct_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc lowMCT_highMET =              "pfmet>300&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  NamedFunc preselection_medmed_highMET = "pfmet>300&&mt_met_lep>50" && basic_cuts && WHLeptons==1 && HasMedMedDeepCSV;
  NamedFunc signalRegion_medmed_highMET = "pfmet>300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedMedDeepCSV;

  map<string, vector<shared_ptr<Process> > > categories;
  map<string, vector<shared_ptr<Process> > > categories_Bs;
  map<string, vector<shared_ptr<Process> > > categories_split_Bs;

  categories["process"] = vector<shared_ptr<Process> >();
  categories["process"].push_back(tt2l);
  categories["process"].push_back(tt1l);
  categories["process"].push_back(wjets_low_nu);
  categories["process"].push_back(wjets_high_nu);
  categories["process"].push_back(single_t);
  categories["process"].push_back(other);

  categories_Bs["RealBs"] = vector<shared_ptr<Process> >();
  categories_Bs["RealBs"].push_back(tot_background_2B);
  categories_Bs["RealBs"].push_back(tot_background_1B);
  categories_Bs["RealBs"].push_back(tot_background_0B);

  categories_split_Bs["fullRealBs"] = vector<shared_ptr<Process> >();
  categories_split_Bs["fullRealBs"].push_back(tt2l_2b);
  categories_split_Bs["fullRealBs"].push_back(tt2l_1b);
  categories_split_Bs["fullRealBs"].push_back(wjets_low_nu);
  categories_split_Bs["fullRealBs"].push_back(wjets_high_nu_1b);
  categories_split_Bs["fullRealBs"].push_back(wjets_high_nu_0b);
  categories_split_Bs["fullRealBs"].push_back(single_t);
  categories_split_Bs["fullRealBs"].push_back(other);


  vector<TableRow> preselection_table_cuts;
  vector<TableRow> preselection_highmT_table_cuts;
  vector<TableRow> signal_table_cuts;
  vector<TableRow> mbbside_table_cuts;
  vector<TableRow> no_b_table_cuts;
  vector<TableRow> one_b_table_cuts;
  vector<TableRow> dilep_table_cuts;
  vector<TableRow> lowMT_table_cuts;
  vector<TableRow> lowMCT_table_cuts;
  vector<TableRow> preselection_medmed_table_cuts;
  vector<TableRow> signal_medmed_table_cuts;
  vector<TableRow> no_b_medmed_table_cuts;
  vector<TableRow> one_b_medmed_table_cuts;
  vector<TableRow> dilep_medmed_table_cuts;

  preselection_table_cuts.push_back(TableRow("Preselection", preselection,0,0,"w_noBtagSF*w_BtagSF_WH"));  
  preselection_table_cuts.push_back(TableRow("Preselection, low MET", preselection_lowMET,0,0,"w_noBtagSF*w_BtagSF_WH"));  
  preselection_table_cuts.push_back(TableRow("Preselection, med MET", preselection_medMET,0,0,"w_noBtagSF*w_BtagSF_WH"));  
  preselection_table_cuts.push_back(TableRow("Preselection, high MET", preselection_highMET,0,0,"w_noBtagSF*w_BtagSF_WH"));  
  
  preselection_highmT_table_cuts.push_back(TableRow("Preselection, high mT", preselection_highmT,0,0,"w_noBtagSF*w_BtagSF_WH"));  
  preselection_highmT_table_cuts.push_back(TableRow("Preselection, high mT, low MET", preselection_highmT_lowMET,0,0,"w_noBtagSF*w_BtagSF_WH"));  
  preselection_highmT_table_cuts.push_back(TableRow("Preselection, high mT, med MET", preselection_highmT_medMET,0,0,"w_noBtagSF*w_BtagSF_WH"));  
  preselection_highmT_table_cuts.push_back(TableRow("Preselection, high mT, high MET", preselection_highmT_highMET,0,0,"w_noBtagSF*w_BtagSF_WH"));  

  signal_table_cuts.push_back(TableRow("Signal Region", signalRegion,0,0,"w_noBtagSF*w_BtagSF_WH")); 
  signal_table_cuts.push_back(TableRow("Signal Region, low MET", signalRegion_lowMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  signal_table_cuts.push_back(TableRow("Signal Region, med MET", signalRegion_medMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  signal_table_cuts.push_back(TableRow("Signal Region, high MET", signalRegion_highMET,0,0,"w_noBtagSF*w_BtagSF_WH")); 
  
  mbbside_table_cuts.push_back(TableRow("mbb Sideband", mbbside,0,0,"w_noBtagSF*w_BtagSF_WH"));
  mbbside_table_cuts.push_back(TableRow("mbb Sideband, low MET", mbbside_lowMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  mbbside_table_cuts.push_back(TableRow("mbb Sideband, med MET", mbbside_medMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  mbbside_table_cuts.push_back(TableRow("mbb Sideband, high MET", mbbside_highMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  
  no_b_table_cuts.push_back(TableRow("0b", no_b,0,0,"w_noBtagSF*w_BtagSF_WH"));
  no_b_table_cuts.push_back(TableRow("0b, low MET", no_b_lowMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  no_b_table_cuts.push_back(TableRow("0b, med MET", no_b_medMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  no_b_table_cuts.push_back(TableRow("0b, high MET", no_b_highMET,0,0,"w_noBtagSF*w_BtagSF_WH"));

  one_b_table_cuts.push_back(TableRow("1b", one_b,0,0,"w_noBtagSF*w_BtagSF_WH"));
  one_b_table_cuts.push_back(TableRow("1b, low MET", one_b_lowMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  one_b_table_cuts.push_back(TableRow("1b, med MET", one_b_medMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  one_b_table_cuts.push_back(TableRow("1b, high MET", one_b_highMET,0,0,"w_noBtagSF*w_BtagSF_WH"));

  dilep_table_cuts.push_back(TableRow("dilep", dilep,0,0,"w_noBtagSF*w_BtagSF_WH"));
  dilep_table_cuts.push_back(TableRow("dilep, low MET", dilep_lowMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  dilep_table_cuts.push_back(TableRow("dilep, med MET", dilep_medMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  dilep_table_cuts.push_back(TableRow("dilep, high MET", dilep_highMET,0,0,"w_noBtagSF*w_BtagSF_WH"));

  lowMT_table_cuts.push_back(TableRow("low mT", lowMT,0,0,"w_noBtagSF*w_BtagSF_WH"));
  lowMT_table_cuts.push_back(TableRow("low mT, low MET", lowMT_lowMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  lowMT_table_cuts.push_back(TableRow("low mT, med MET", lowMT_medMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  lowMT_table_cuts.push_back(TableRow("low mT, high MET", lowMT_highMET,0,0,"w_noBtagSF*w_BtagSF_WH"));

  lowMCT_table_cuts.push_back(TableRow("low mCT", lowMCT,0,0,"w_noBtagSF*w_BtagSF_WH"));
  lowMCT_table_cuts.push_back(TableRow("low mCT, low MET", lowMCT_lowMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  lowMCT_table_cuts.push_back(TableRow("low mCT, med MET", lowMCT_medMET,0,0,"w_noBtagSF*w_BtagSF_WH"));
  lowMCT_table_cuts.push_back(TableRow("low mCT, high MET", lowMCT_highMET,0,0,"w_noBtagSF*w_BtagSF_WH"));

  preselection_medmed_table_cuts.push_back(TableRow("Preselection (medmed)", preselection_medmed,0,0,"w_noBtagSF*w_BtagSF_medmed"));  
  preselection_medmed_table_cuts.push_back(TableRow("Preselection (medmed), low MET", preselection_medmed_lowMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));  
  preselection_medmed_table_cuts.push_back(TableRow("Preselection (medmed), med MET", preselection_medmed_medMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));  
  preselection_medmed_table_cuts.push_back(TableRow("Preselection (medmed), high MET", preselection_medmed_highMET,0,0,"w_noBtagSF*w_BtagSF_medmed")); 

  signal_medmed_table_cuts.push_back(TableRow("Signal Region (medmed)", signalRegion_medmed,0,0,"w_noBtagSF*w_BtagSF_medmed"));  
  signal_medmed_table_cuts.push_back(TableRow("Signal Region (medmed), low MET", signalRegion_medmed_lowMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));  
  signal_medmed_table_cuts.push_back(TableRow("Signal Region (medmed), med MET", signalRegion_medmed_medMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));  
  signal_medmed_table_cuts.push_back(TableRow("Signal Region (medmed), high MET", signalRegion_medmed_highMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));  

  no_b_medmed_table_cuts.push_back(TableRow("0b (medmed)", no_b,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  no_b_medmed_table_cuts.push_back(TableRow("0b (medmed), low MET", no_b_lowMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  no_b_medmed_table_cuts.push_back(TableRow("0b (medmed), med MET", no_b_medMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  no_b_medmed_table_cuts.push_back(TableRow("0b (medmed), high MET", no_b_highMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));

  one_b_medmed_table_cuts.push_back(TableRow("1b (medmed)", one_b,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  one_b_medmed_table_cuts.push_back(TableRow("1b (medmed), low MET", one_b_lowMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  one_b_medmed_table_cuts.push_back(TableRow("1b (medmed), med MET", one_b_medMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  one_b_medmed_table_cuts.push_back(TableRow("1b (medmed), high MET", one_b_highMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));

  dilep_medmed_table_cuts.push_back(TableRow("dilep (medmed)", dilep,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  dilep_medmed_table_cuts.push_back(TableRow("dilep (medmed), low MET", dilep_lowMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  dilep_medmed_table_cuts.push_back(TableRow("dilep (medmed), med MET", dilep_medMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));
  dilep_medmed_table_cuts.push_back(TableRow("dilep (medmed), high MET", dilep_highMET,0,0,"w_noBtagSF*w_BtagSF_medmed"));

  for(auto &ipr: categories) pm.Push<Table>("chart_PRESELECTION_"+ipr.first,  preselection_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_PRESELECTION_HIGH_MT_"+ipr.first,  preselection_highmT_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_SIGNAL_"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_MBBSIDE_"+ipr.first,  mbbside_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_0B_"+ipr.first,  no_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_1B_"+ipr.first,  one_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_DILEP_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_LOW_MT_"+ipr.first,  lowMT_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_LOW_MCT_"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_PRESELECTION_MEDMED_"+ipr.first,  preselection_medmed_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_SIGNAL_MEDMED_"+ipr.first,  signal_medmed_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_0B_MEDMED_"+ipr.first,  no_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_1B_MEDMED_"+ipr.first,  one_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories) pm.Push<Table>("chart_DILEP_MEDMED_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);

  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_PRESELECTION_"+ipr.first,  preselection_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_PRESELECTION_HIGH_MT_"+ipr.first,  preselection_highmT_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_SIGNAL_"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_MBBSIDE_"+ipr.first,  mbbside_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_0B_"+ipr.first,  no_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_1B_"+ipr.first,  one_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_DILEP_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_LOW_MT_"+ipr.first,  lowMT_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_LOW_MCT_"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_PRESELECTION_MEDMED_"+ipr.first,  preselection_medmed_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_SIGNAL_MEDMED_"+ipr.first,  signal_medmed_table_cuts, ipr.second, true, true, true, false);

  for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_PRESELECTION_"+ipr.first,  preselection_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_PRESELECTION_HIGH_MT_"+ipr.first,  preselection_highmT_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_SIGNAL_"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_MBBSIDE_"+ipr.first,  mbbside_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_0B_"+ipr.first,  no_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_1B_"+ipr.first,  one_b_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_DILEP_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_LOW_MT_"+ipr.first,  lowMT_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_LOW_MCT_"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_PRESELECTION_MEDMED_"+ipr.first,  preselection_medmed_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_SIGNAL_MEDMED_"+ipr.first,  signal_medmed_table_cuts, ipr.second, true, true, true, false);

  Table & preselection_chart = pm.Push<Table>("preselection_chart", preselection_table_cuts, sample_list, false);
  Table & preselection_highmT_chart = pm.Push<Table>("preselection_highmT_chart", preselection_highmT_table_cuts, sample_list, false);
  Table & signal_chart = pm.Push<Table>("signal_chart", signal_table_cuts, sample_list, false);
  Table & mbbside_chart = pm.Push<Table>("mbbside_chart", mbbside_table_cuts, sample_list, false);
  Table & no_b_chart = pm.Push<Table>("no_b_chart", no_b_table_cuts, sample_list, false);
  Table & one_b_chart = pm.Push<Table>("one_b_chart", one_b_table_cuts, sample_list, false);
  Table & dilep_chart = pm.Push<Table>("dilep_chart", dilep_table_cuts, sample_list, false);
  Table & lowmT_chart = pm.Push<Table>("lowmT_chart", lowMT_table_cuts, sample_list, false);
  Table & lowmCT_chart = pm.Push<Table>("lowmCT_chart", lowMCT_table_cuts, sample_list, false);
  Table & preselection_medmed_chart = pm.Push<Table>("preselection_medmed_chart", preselection_medmed_table_cuts, sample_list, false);
  Table & signal_medmed_chart = pm.Push<Table>("signal_medmed_chart", signal_medmed_table_cuts, sample_list, false);
  Table & no_b_medmed_chart = pm.Push<Table>("no_b_medmed_chart", no_b_table_cuts, sample_list, false);
  Table & one_b_medmed_chart = pm.Push<Table>("one_b_medmed_chart", one_b_table_cuts, sample_list, false);
  Table & dilep_medmed_chart = pm.Push<Table>("dilep_medmed_chart", dilep_table_cuts, sample_list, false);

  Table & real_b_preselection_chart = pm.Push<Table>("real_b_preselection_chart", preselection_table_cuts, real_bs_sample_list, false);
  Table & real_b_preselection_highmT_chart = pm.Push<Table>("real_b_preselection_highmT_chart", preselection_highmT_table_cuts, real_bs_sample_list, false);
  Table & real_b_signal_chart = pm.Push<Table>("real_b_signal_chart", signal_table_cuts, real_bs_sample_list, false);
  Table & real_b_mbbside_chart = pm.Push<Table>("real_b_mbbside_chart", mbbside_table_cuts, real_bs_sample_list, false);
  Table & real_b_no_b_chart = pm.Push<Table>("real_b_no_b_chart", no_b_table_cuts, real_bs_sample_list, false);
  Table & real_b_one_b_chart = pm.Push<Table>("real_b_one_b_chart", one_b_table_cuts, real_bs_sample_list, false);
  Table & real_b_dilep_chart = pm.Push<Table>("real_b_dilep_chart", dilep_table_cuts, real_bs_sample_list, false);
  Table & real_b_lowmT_chart = pm.Push<Table>("real_b_lowmT_chart", lowMT_table_cuts, real_bs_sample_list, false);
  Table & real_b_lowmCT_chart = pm.Push<Table>("real_b_lowmCT_chart", lowMCT_table_cuts, real_bs_sample_list, false);
  Table & real_b_preselection_medmed_chart = pm.Push<Table>("real_b_preselection_medmed_chart", preselection_medmed_table_cuts, real_bs_sample_list, false);
  Table & real_b_signal_medmed_chart = pm.Push<Table>("real_b_signal_medmed_chart", signal_medmed_table_cuts, real_bs_sample_list, false);

  Table & split_b_preselection_chart = pm.Push<Table>("split_b_preselection_chart", preselection_table_cuts, b_full_sample_list, false);
  Table & split_b_preselection_highmT_chart = pm.Push<Table>("split_b_preselection_highmT_chart", preselection_highmT_table_cuts, b_full_sample_list, false);
  Table & split_b_signal_chart = pm.Push<Table>("split_b_signal_chart", signal_table_cuts, b_full_sample_list, false);
  Table & split_b_mbbside_chart = pm.Push<Table>("split_b_mbbside_chart", mbbside_table_cuts, b_full_sample_list, false);
  Table & split_b_no_b_chart = pm.Push<Table>("split_b_no_b_chart", no_b_table_cuts, b_full_sample_list, false);
  Table & split_b_one_b_chart = pm.Push<Table>("split_b_one_b_chart", one_b_table_cuts, b_full_sample_list, false);
  Table & split_b_dilep_chart = pm.Push<Table>("split_b_dilep_chart", dilep_table_cuts, b_full_sample_list, false);
  Table & split_b_lowmT_chart = pm.Push<Table>("split_b_lowmT_chart", lowMT_table_cuts, b_full_sample_list, false);
  Table & split_b_lowmCT_chart = pm.Push<Table>("split_b_lowmCT_chart", lowMCT_table_cuts, b_full_sample_list, false);
  Table & split_b_preselection_medmed_chart = pm.Push<Table>("split_b_preselection_medmed_chart", preselection_medmed_table_cuts, b_full_sample_list, false);
  Table & split_b_signal_medmed_chart = pm.Push<Table>("split_b_signal_medmed_chart", signal_medmed_table_cuts, b_full_sample_list, false);

  vector<GammaParams> yields_preselection = preselection_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_preselection){
    cout << yield << endl;
  }

  vector<GammaParams> yields_preselection_highmT = preselection_highmT_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_preselection_highmT){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signal = signal_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_signal){
    cout << yield << endl;
  }

  vector<GammaParams> yields_mbbside = mbbside_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_mbbside){
    cout << yield << endl;
  }

  vector<GammaParams> yields_no_b = no_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_no_b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_one_b = one_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_one_b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_dilep = dilep_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_dilep){
    cout << yield << endl;
  }

  vector<GammaParams> yields_lowmT = lowmT_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_lowmT){
    cout << yield << endl;
  }

  vector<GammaParams> yields_lowmCT = lowmCT_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_lowmCT){
    cout << yield << endl;
  }

  vector<GammaParams> yields_preselection_medmed = preselection_medmed_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_preselection_medmed){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signal_medmed = signal_medmed_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_signal_medmed){
    cout << yield << endl;
  }

  vector<GammaParams> yields_no_b_medmed = no_b_medmed_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_no_b_medmed){
    cout << yield << endl;
  }

  vector<GammaParams> yields_one_b_medmed = one_b_medmed_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_one_b_medmed){
    cout << yield << endl;
  }

  vector<GammaParams> yields_dilep_medmed = dilep_medmed_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_dilep_medmed){
    cout << yield << endl;
  }

  vector<GammaParams> yields_real_b_preselection = real_b_preselection_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_real_b_preselection){
    cout << yield << endl;
  }

  vector<GammaParams> yields_real_b_preselection_highmT = real_b_preselection_highmT_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_real_b_preselection_highmT){
    cout << yield << endl;
  }

  vector<GammaParams> yields_real_b_signal = real_b_signal_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_real_b_signal){
    cout << yield << endl;
  }

  vector<GammaParams> yields_real_b_mbbside = real_b_mbbside_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_real_b_mbbside){
    cout << yield << endl;
  }

  vector<GammaParams> yields_real_b_no_b = real_b_no_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_real_b_no_b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_real_b_one_b = real_b_one_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_real_b_one_b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_real_b_dilep = real_b_dilep_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_real_b_dilep){
    cout << yield << endl;
  }

  vector<GammaParams> yields_real_b_lowmT = real_b_lowmT_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_real_b_lowmT){
    cout << yield << endl;
  }

  vector<GammaParams> yields_real_b_lowmCT = real_b_lowmCT_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_real_b_lowmCT){
    cout << yield << endl;
  }

  vector<GammaParams> yields_real_b_preselection_medmed = real_b_preselection_medmed_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_real_b_preselection_medmed){
    cout << yield << endl;
  }

  vector<GammaParams> yields_real_b_signal_medmed = real_b_signal_medmed_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_real_b_signal_medmed){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_preselection = split_b_preselection_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_preselection){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_preselection_highmT = split_b_preselection_highmT_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_preselection_highmT){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_signal = split_b_signal_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_signal){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_mbbside = split_b_mbbside_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_mbbside){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_no_b = split_b_no_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_no_b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_one_b = split_b_one_b_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_one_b){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_dilep = split_b_dilep_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_dilep){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_lowmT = split_b_lowmT_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_lowmT){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_lowmCT = split_b_lowmCT_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_lowmCT){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_preselection_medmed = split_b_preselection_medmed_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_preselection_medmed){
    cout << yield << endl;
  }

  vector<GammaParams> yields_split_b_signal_medmed = split_b_signal_medmed_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_split_b_signal_medmed){
    cout << yield << endl;
  }

  pm.MakePlots(lumi);

}
