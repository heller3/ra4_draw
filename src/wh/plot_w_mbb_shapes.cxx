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

  double lumi = 35.9;

  string data_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_04_03/";
  string mc_dir = "/home/users/rheller/wh_babies/babies_v30_9_2019_04_03/";
  string signal_dir = "/home/users/rheller/wh_babies/babies_signal_2019_04_03/";
  string z_dir = "/home/users/rheller/wh_babies/babies_v30_9_2019_04_03/";


  string basic_cuts =            "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2";

  Palette colors("txt/colors.txt", "default");


  auto signal_wjets_high_nu = Process::MakeShared<Baby_full>("signal W+jets, nu pT >= 200", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedLooseDeepCSV);
  auto no_b_wjets_high_nu = Process::MakeShared<Baby_full>("0b W+jets, nu pT >= 200", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==0.);
  auto one_b_wjets_high_nu = Process::MakeShared<Baby_full>("1b W+jets, nu pT >= 200", Process::Type::background, colors("ttv"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedBTagged==1&&nDeepLooseBTagged==1);
  
  auto signal_medmed_wjets_high_nu = Process::MakeShared<Baby_full>("2b W+jets, nu pT >= 200", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&HasMedMedDeepCSV);

  auto no_bflav_wjets_high_nu = Process::MakeShared<Baby_full>("0 b-flavor W+jets", Process::Type::background, colors("tt_2l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nRealBs==0.);
  auto one_bflav_wjets_high_nu = Process::MakeShared<Baby_full>("1 b-flavor W+jets", Process::Type::background, colors("ttv"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nRealBs==1.&&nDeepLooseBTagged==1);
  
  auto two_bflav_wjets_high_nu = Process::MakeShared<Baby_full>("2 b-flavor W+jets", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nRealBs==2.);

    auto no_btag_wjets_two_bquark = Process::MakeShared<Baby_full>("0 b-tags W+jets", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1.&&nGenBs_ptG30==2. && "ngoodbtags==0" );
  auto one_btag_wjets_two_bquark = Process::MakeShared<Baby_full>("1 b-tag W+jets", Process::Type::background, kGreen+2,
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1.&&nGenBs_ptG30==2. && "ngoodbtags==1");
  
  auto two_btag_wjets_two_bquark = Process::MakeShared<Baby_full>("2 b-tags W+jets", Process::Type::background, colors("dy"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nGenBs_ptG30==2. && "ngoodbtags==2");

    auto incl_btag_wjets_two_bquark = Process::MakeShared<Baby_full>("W+jets, 2 b-quarks", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nGenBs_ptG30==2.);




    auto two_c_wjets_high_nu = Process::MakeShared<Baby_full>("2c (1med 1tight )-flavor W+jets", Process::Type::background, colors("tt_1l"),
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepMedCTagged==2.&&nDeepTightCTagged>=1.&&nDeepMedBTagged<2.);

    auto two_ctight_wjets_high_nu = Process::MakeShared<Baby_full>("2c tight W+jets, nu pT >= 200", Process::Type::background, kGreen+2,
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged<2.);

    auto two_ctight_2bmed_wjets_high_nu = Process::MakeShared<Baby_full>("2c/2b W+jets, nu pT >= 200", Process::Type::background, kGreen+2,
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged==2.);

     auto two_ctight_2bmed_2cnotb_wjets_high_nu = Process::MakeShared<Baby_full>("2c/2b, 1 CvB med W+jets, nu pT >= 200", Process::Type::background, kGreen+2,
    {mc_dir+"*slim_W*Jets_NuPt200*.root"},basic_cuts&&WHLeptons==1&&nDeepTightCTagged==2.&&nDeepMedBTagged==2.&&nDeepMedCvBTagged>=1.);

  vector<shared_ptr<Process> > wjets_medmed_sample_list = {signal_medmed_wjets_high_nu,two_ctight_wjets_high_nu,/*two_c_wjets_high_nu,*/one_b_wjets_high_nu,no_b_wjets_high_nu};
  vector<shared_ptr<Process> > wjets_bflav_sample_list = {two_bflav_wjets_high_nu,one_bflav_wjets_high_nu,no_bflav_wjets_high_nu};
  vector<shared_ptr<Process> > incl_w_sample_list = {incl_btag_wjets_two_bquark};

  vector<shared_ptr<Process> > wjets_2bquark = {two_btag_wjets_two_bquark,one_btag_wjets_two_bquark,no_btag_wjets_two_bquark};
  vector< vector<shared_ptr<Process> > > sample_list_list ={wjets_bflav_sample_list,wjets_medmed_sample_list};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::off)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .Bottom(BottomType::ratio)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info)
    .Bottom(BottomType::ratio);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {log_lumi_info, lin_lumi_info,lin_shapes_info,log_shapes_info};

 	PlotMaker pm;

 vector<string> sels = { "pfmet>200&&mt_met_lep>50","pfmet>200&&mct>200&&mt_met_lep>150", "pfmet>200&&mct>200&&mt_met_lep<150&&mt_met_lep>50", "pfmet>200&&mct<200&&mt_met_lep>150"};
 vector<string> tags = {"presel","signal","lowmT","lowmCT"};
 vector<string> samps = {"truth","tagged"};



 for(uint i=0;i<sels.size();i++){

   pm.Push<Hist1D>(Axis(8, 30, 510., bbmass, "M_{b#bar{b}}"),
        sels[i], wjets_2bquark, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("quarks_"+tags[i]);


   pm.Push<Hist1D>(Axis(12, 0, 3., bDeltaR, "delta R_{bb}"),
        sels[i], wjets_2bquark, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("quarks_"+tags[i]);
   pm.Push<Hist1D>(Axis(12, 0, 3., bDeltaR, "delta R_{bb}"),
        sels[i], incl_w_sample_list, all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("quarks_"+tags[i]);


for (uint j=0;j<sample_list_list.size();j++){
continue;
  pm.Push<Hist1D>(Axis(8, 30, 510., "mbb", "M_{b#bar{b}}"),
		    sels[i], sample_list_list[j], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag(samps[j]+"_"+tags[i]);

pm.Push<Hist1D>(Axis(20, 0, 600., LeadingJetPt, "Leading jet p_{T} [GeV]"),
	    sels[i], sample_list_list [j], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag(samps[j]+"_"+tags[i]);

pm.Push<Hist1D>(Axis(20, 0, 300., SubLeadingJetPt, "Subleading jet p_{T} [GeV]"),
	    sels[i], sample_list_list [j], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag(samps[j]+"_"+tags[i]);

pm.Push<Hist1D>(Axis(20, 0, 500., LeadingJetPt-SubLeadingJetPt, "delta p_{T} [GeV]"),
	    sels[i], sample_list_list [j], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag(samps[j]+"_"+tags[i]);


pm.Push<Hist1D>(Axis(16, 0, 4., deltaRLeadingJets, "delta R_{bb}"),
	    sels[i], sample_list_list [j], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag(samps[j]+"_"+tags[i]);


pm.Push<Hist1D>(Axis(12, 0, 4., deltaRLeadingJets, "delta R_{bb}"),
      sels[i], sample_list_list [j], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag("coarse_"+samps[j]+"_"+tags[i]);

pm.Push<Hist1D>(Axis(15, 0, 3.14, deltaPhiLeadingJets, "delta Phi_{bb}"),
	    sels[i], sample_list_list [j], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag(samps[j]+"_"+tags[i]);

pm.Push<Hist1D>(Axis(20, 0, 500, "leps_pt[0]", "lepton p_{t} [GeV]"),
	    sels[i], sample_list_list [j], all_plot_types).Weight("w_noBtagSF * w_BtagSF_medmed").Tag(samps[j]+"_"+tags[i]);

}

}



  pm.MakePlots(lumi);

}