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
  
  double lumi2016 = 35.9;
  double lumi2017 = 41.6;
  double lumi2018 = 59.7;

  string data2016_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_10_05/";
  string data2017_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_05/";
  string data2018_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_05/";

  string mc2016_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/s16v3/";
  string mc2016_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/s16v3/";

  string mc2017_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/f17v2/";
  string mc2017_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/f17v2/";

  string mc2018_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/a18v1xxx/";
  string mc2018_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_07/a18v1xxx/";  


  Palette colors("txt/colors.txt", "default");


  auto all_top = {mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2017_dir+"*TTJets_1lep_top_f17v2*.root",mc2017_dir+"*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2018_dir+"*TTJets_1lep_top_a18v1*.root",mc2018_dir+"*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"*TTJets_1lep_*met80*.root",mc2016_dir+"*_ST_*.root",mc2017_dir+"*_ST_*.root",mc2018_dir+"*_ST_*.root",mc2016_dir+"*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2017_dir+"*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2018_dir+"*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"*TTJets_2lep_*met80*.root"};

  auto tt_2l ={mc2016_dir+"*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2017_dir+"*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2018_dir+"*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"*TTJets_2lep_*met80*.root"};

  auto single_t = {mc2016_dir+"*_ST_*.root",mc2017_dir+"*_ST_*.root",mc2018_dir+"*_ST_*.root"};
 
  //single lepton selections

  string single_lep_high_mT = "stitch&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150&&ngoodbtags==2&& nWHLeptons==1";
  NamedFunc single_lep_high_mT_htau = "stitch&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150&&ngoodbtags==2&& nWHLeptons==1"&&HasHadronicTau>0.;
  NamedFunc single_lep_high_mT_no_htau = "stitch&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&mt_met_lep>150&&ngoodbtags==2&& nWHLeptons==1"&&HasHadronicTau==0.;

  string single_lep = "stitch&&pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&ngoodbtags==2&& nWHLeptons==1";

  string dilep_loose_high_mT = "stitch&&pass&&nvetoleps==2&&ngoodjets==2&&pfmet>125&&mt_met_lep>150&&ngoodbtags==2 && nWHLeptons>=1";
  string dilep_loose = "stitch&&pass&&nvetoleps==2&&ngoodjets==2&&pfmet>125&&ngoodbtags==2 && nWHLeptons>=1";


  auto top_1l_high_mT = Process::MakeShared<Baby_full>("top, 1 lepton, high mT",  Process::Type::background,colors("tt_1l"), all_top, single_lep_high_mT);

  auto top_1l_high_mT_onhiggs = Process::MakeShared<Baby_full>("top, 1 lepton, high mT, on higgs",  Process::Type::background,colors("wjets"), all_top, single_lep_high_mT &&  "mbb>90&&mbb<150");
  auto top_1l = Process::MakeShared<Baby_full>("top, 1 lepton",  Process::Type::background,colors("tt_1l"), all_top,single_lep);

  auto ttbar_1l_high_mT = Process::MakeShared<Baby_full>("ttbar (2l), 1 lepton, high mT",  Process::Type::background,colors("tt_1l"), tt_2l,single_lep_high_mT);
  auto st_1l_high_mT = Process::MakeShared<Baby_full>("single top, 1 lepton, high mT",  Process::Type::background,colors("tt_1l"), single_t,single_lep_high_mT);

  auto top_1l_high_mT_hadtau = Process::MakeShared<Baby_full>("top, 1 lepton + hadr. tau, high mT",  Process::Type::background,colors("tt_ltau"), all_top, single_lep_high_mT_htau);
  auto top_1l_high_mT_nohadtau = Process::MakeShared<Baby_full>("top, 1 lepton, no hadronic tau, high mT",  Process::Type::background,colors("tt_1l"), all_top, single_lep_high_mT_no_htau);


  auto top_2l = Process::MakeShared<Baby_full>("top, 2 lepton",  Process::Type::background,colors("tt_2l"), all_top,dilep_loose);
  auto top_2l_high_mT = Process::MakeShared<Baby_full>("top, 2 lepton, high mT",  Process::Type::background,colors("tt_2l"), all_top,dilep_loose_high_mT);
  auto top_2l_high_mT_onhiggs = Process::MakeShared<Baby_full>("top, 2 lepton, high mT, on higgs",  Process::Type::background,colors("tt_ltau"), all_top,dilep_loose_high_mT && "mbb>90&&mbb<150");

  auto ttbar_2l_high_mT = Process::MakeShared<Baby_full>("ttbar (2l), 2 lepton, high mT",  Process::Type::background,colors("tt_2l"), tt_2l, dilep_loose_high_mT);
  auto st_2l_high_mT = Process::MakeShared<Baby_full>("single top, 2 lepton, high mT",  Process::Type::background,colors("tt_2l"), single_t, dilep_loose_high_mT);

 
 // ttbar vs ttbar
 vector<shared_ptr<Process> > abcd_ttbar = {ttbar_1l_high_mT,ttbar_2l_high_mT};
 vector<shared_ptr<Process> > abcd_st = {st_1l_high_mT,st_2l_high_mT};
 vector<shared_ptr<Process> > abcd_top = {top_1l_high_mT,top_2l_high_mT};
 vector<shared_ptr<Process> > abcd_top_vs_higgs = {top_1l_high_mT_onhiggs,top_1l_high_mT,top_2l_high_mT,top_2l_high_mT_onhiggs};
 vector<shared_ptr<Process> > abcd_top_tau = {top_1l_high_mT_nohadtau,top_1l_high_mT_hadtau,top_2l_high_mT};
 vector<shared_ptr<Process> > abcd_top_low_mt_dilep = {top_1l_high_mT,top_2l};
 vector<shared_ptr<Process> > abcd_plot_mt = {top_1l,top_2l};


vector<vector<shared_ptr<Process> > > abcd_variations = {abcd_top_vs_higgs ,abcd_top_tau,abcd_ttbar,abcd_st,abcd_top,abcd_top_low_mt_dilep};
vector<string> process_tags = {"abcdvshiggs","alltop_tau","ttbar","ST","alltop","all_top_lowmTdilep"};

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


  //Need different PlotMaker for each luminosity.
    //For some reason, they needed to be pointers to go into vector later on..
  PlotMaker * pm2016 = new PlotMaker();
  PlotMaker * pm2017 = new PlotMaker();
  PlotMaker * pm2018 = new PlotMaker();
  PlotMaker * pm = new PlotMaker();

 
  vector<PlotMaker *> pms = {pm2016,pm2017,pm2018,pm};
 // vector<vector<shared_ptr<Process> >> samples_Run2 = {sample_list_2016,sample_list_2017,sample_list_2018,sample_list};
  vector<string> years = {"y2016","y2017","y2018","yLegacy"};
  //vector<NamedFunc> weights = {"weight","weight * w_pu"};
  vector<NamedFunc> legacyWeights = {"weight"*yearWeight,"weight * w_pu" * yearWeight};
   vector<string> metbins = {"1","pfmet>200","pfmet>300"};

 
    for(uint imet=0;imet<metbins.size();imet++){
      // pm->Push<Hist1D>(Axis(10, 0, 400., "mt_met_lep", "m_{T} [GeV]"),
      //   metbins[imet], abcd_plot_mt, all_plot_types).Weight(legacyWeights[1]).Tag("abcd_"+years[3]);  

       for(uint iproc=0;iproc<abcd_variations.size();iproc++){
              if(iproc>1)continue;

       pm->Push<Hist1D>(Axis(6, 150, 300., "mct", "m_{CT} [GeV]"),
         metbins[imet], abcd_variations[iproc], all_plot_types).Weight(legacyWeights[1]).Tag("abcd_"+process_tags[iproc]+"_"+years[3]);  
       pm->Push<Hist1D>(Axis(6, 150, 300., "mct", "m_{CT} [GeV]"),
        "mbb>90&&mbb<150" && metbins[imet], abcd_variations[iproc], all_plot_types).Weight(legacyWeights[1]).Tag("abcd_onhiggs_"+process_tags[iproc]+"_"+years[3]);  

       pm->Push<Hist1D>(Axis(10, 0, 500., LeadingToppT, "Leading top pT [GeV]"),
         metbins[imet], abcd_variations[iproc], all_plot_types).Weight(legacyWeights[1]).Tag("abcd_"+process_tags[iproc]+"_"+years[3]);  

       pm->Push<Hist1D>(Axis(5, 0, 500., LeadingToppT, "Leading top pT [GeV]"),
         metbins[imet] && "mct>200", abcd_variations[iproc], all_plot_types).Weight(legacyWeights[1]).Tag("abcd_"+process_tags[iproc]+"_"+years[3]);  
       
       pm->Push<Hist1D>(Axis(10, 0, 500., LeadingWpT, "Leading W pT [GeV]"),
         metbins[imet], abcd_variations[iproc], all_plot_types).Weight(legacyWeights[1]).Tag("abcd_"+process_tags[iproc]+"_"+years[3]);     
      pm->Push<Hist1D>(Axis(5, 0, 500., LeadingWpT, "Leading W pT [GeV]"),
         metbins[imet]&&"mct>200", abcd_variations[iproc], all_plot_types).Weight(legacyWeights[1]).Tag("abcd_"+process_tags[iproc]+"_"+years[3]);

       pm->Push<Hist1D>(Axis(10, 0, 400., SubLeadingWpT, "Subleading W pT [GeV]"),
         metbins[imet], abcd_variations[iproc], all_plot_types).Weight(legacyWeights[1]).Tag("abcd_"+process_tags[iproc]+"_"+years[3]);     
      pm->Push<Hist1D>(Axis(5, 0, 400., SubLeadingWpT, "Subleading W pT [GeV]"),
         metbins[imet]&&"mct>200", abcd_variations[iproc], all_plot_types).Weight(legacyWeights[1]).Tag("abcd_"+process_tags[iproc]+"_"+years[3]);


       pm->Push<Hist1D>(Axis(10, 0, 500., "pfmet", "MET [GeV]"),
         metbins[imet], abcd_variations[iproc], all_plot_types).Weight(legacyWeights[1]).Tag("abcd_"+process_tags[iproc]+"_"+years[3]); 

       pm->Push<Hist1D>(Axis(10, 0, 500., "pfmet", "MET [GeV]"),
         metbins[imet] && "mct>200", abcd_variations[iproc], all_plot_types).Weight(legacyWeights[1]).Tag("abcd_"+process_tags[iproc]+"_"+years[3]);  
       
    }
  }



 //   pm2016->MakePlots(lumi2016);
 //   pm2017->MakePlots(lumi2017);
  cout<<lumi2016<<lumi2017<<lumi2018<<endl;
 //   pm2018->MakePlots(lumi2018);
  pm->MakePlots(lumi2016+lumi2017+lumi2018);
 // pm->MakePlots(lumi2016+lumi2017+lumi2018);

}
