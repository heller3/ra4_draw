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

  string mc2018_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/a18v1/";
  string mc2018_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_07/a18v1/";  

  string signal_dir2016 = "/home/users/rheller/wh_babies/babies_signal_s16v3_v32_2019_10_07/";
  string signal_dir2017 = "/home/users/rheller/wh_babies/babies_signal_f17v2_v32_0_2019_10_07/";
  string signal_dir2018 = "/home/users/rheller/wh_babies/babies_signal_a18v1_v32_0_2019_10_07/";


  Palette colors("txt/colors.txt", "default");
  NamedFunc baselinef = "pass&&nvetoleps>=1&&ngoodjets>=2 && ngoodjets<=3 &&pfmet>125&&mt_met_lep>50 && ngoodbtags==2 && nWHLeptons>=1";


  //Data
  auto data =  Process::MakeShared<Baby_full>("2016-2018 Data", Process::Type::data, colors("data"),{data2016_dir+"*data_2016*.root",data2017_dir+"*data_2017*.root",data2018_dir+"*data_2018*.root"},baselinef&&"(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");

  auto tt1l = Process::MakeShared<Baby_full>("t#bar{t} (1l) 2016-2018", Process::Type::background, colors("tt_1l"),
    {mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2017_dir+"*TTJets_1lep_top_f17v2*.root",mc2017_dir+"*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2018_dir+"*TTJets_1lep_top_a18v1*.root",mc2018_dir+"*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"*TTJets_1lep_*met80*.root"},"stitch"&&baselinef);
  auto tt2l = Process::MakeShared<Baby_full>("t#bar{t} (2l) 2016-2018", Process::Type::background, colors("tt_2l"),
    {mc2016_dir+"*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2017_dir+"*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2018_dir+"*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"*TTJets_2lep_*met80*.root"},"stitch"&&baselinef);


  //WJets
  auto wjets = Process::MakeShared<Baby_full>("W+jets 2016-2018", Process::Type::background, colors("wjets"), {mc2016_dir+"*slim_W*JetsToLNu_s16v3*",mc2016_dir+"*W*Jets_NuPt200_s16v*.root",mc2017_dir+"*slim_W*JetsToLNu_f17v2*",mc2017_dir+"*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"*slim_W*JetsToLNu_a18v1*",mc2018_dir+"*W*Jets_NuPt200_a18v1*.root"},"stitch&&evt!=74125994"&&baselinef);


  //Single top
  auto single_t = Process::MakeShared<Baby_full>("Single t 2016-2018", Process::Type::background, colors("single_t"), {mc2016_dir+"*_ST_*.root",mc2017_dir+"*_ST_*.root",mc2018_dir+"*_ST_*.root"},baselinef);

    //diboson
  auto diboson = Process::MakeShared<Baby_full>("Diboson 2016-2018", Process::Type::background, colors("other"),
  {mc2016_dir+"*WW*.root", mc2016_dir+"*WZ*.root",mc2016_dir+"*ZZ*.root",mc2017_dir+"*WW*.root", mc2017_dir+"*WZ*.root",mc2017_dir+"*ZZ*.root",mc2018_dir+"*WW*.root", mc2018_dir+"*WZ*.root",mc2018_dir+"*ZZ*.root"},baselinef);

  // ttV
  auto ttV = Process::MakeShared<Baby_full>("t#bar{t}V 2016-2018", Process::Type::background, colors("ttv"),{mc2016_dir+"*_TTWJets*.root", mc2016_dir+"*_TTZ*.root",mc2017_dir+"*_TTWJets*.root", mc2017_dir+"*_TTZ*.root",mc2018_dir+"*_TTWJets*.root", mc2018_dir+"*_TTZ*.root"},baselinef);


 auto proc_sig = Process::MakeShared<Baby_full>("2016-2018 TChiWH(700,1)", Process::Type::signal, colors("t1tttt"),
  {signal_dir2016+"*TChiWH*.root",signal_dir2017+"*TChiWH*.root",signal_dir2018+"*TChiWH*.root"},"mass_stop==700&&mass_lsp==1"&&baselinef);

  auto proc_sig_compress = Process::MakeShared<Baby_full>("2016-2018 TChiWH(225,75)", Process::Type::signal, colors("t1tttt"),
  {signal_dir2016+"*TChiWH*.root",signal_dir2017+"*TChiWH*.root",signal_dir2018+"*TChiWH*.root"},"mass_stop==225&&mass_lsp==75"&&baselinef);
  proc_sig_compress->SetLineStyle(2);
  auto proc_sig_med = Process::MakeShared<Baby_full>("2016-2018 TChiWH(400,150)", Process::Type::signal, colors("t1tttt"),
  {signal_dir2016+"*TChiWH*.root",signal_dir2017+"*TChiWH*.root",signal_dir2018+"*TChiWH*.root"},"mass_stop==400&&mass_lsp==150"&&baselinef);
  proc_sig_med->SetLineStyle(3);
    auto proc_sig_corner = Process::MakeShared<Baby_full>("2016-2018 TChiWH(650,250)", Process::Type::signal, colors("t1tttt"),
  {signal_dir2016+"*TChiWH*.root",signal_dir2017+"*TChiWH*.root",signal_dir2018+"*TChiWH*.root"},"mass_stop==650&&mass_lsp==250"&&baselinef);
  proc_sig_corner->SetLineStyle(6);

  vector<shared_ptr<Process> > sample_list = {tt1l, tt2l, wjets, single_t, diboson, ttV,proc_sig, proc_sig_corner,proc_sig_med,proc_sig_compress};

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
  PlotMaker pm;

  //Preselection
  NamedFunc single_lep ="nvetoleps==1&&PassTrackVeto&&PassTauVeto&&nWHLeptons==1";

  // TString signal_region = single_lep+"&&mct>200&&mbb>90&&mbb<150";
  // TString mct_control_region = single_lep+"&&mct>150&&mct<=200&&mbb>90&&mbb<150";    
  // TString signal_region_boost = single_lep+"&&mct>200&&mbb>90&&mbb<150";
  // TString mct_control_region_boost = single_lep+"&&mct<=200&&mbb>90&&mbb<150";


  NamedFunc signal_nm1 = single_lep && "ngoodjets==2";
  NamedFunc signal_nm1_3jet = single_lep && LeadingNonBJetPt_med<100. && "ngoodjets==3";

  NamedFunc signal_nm1_res = single_lep && max_ak8pfjets_deepdisc_hbb<0.8 && "ngoodjets==2";
  NamedFunc signal_nm1_3jet_res = single_lep && max_ak8pfjets_deepdisc_hbb<0.8 && LeadingNonBJetPt_med<100. && "ngoodjets==3";

  NamedFunc boosted_nm1 = single_lep && max_ak8pfjets_deepdisc_hbb>0.8 && "ngoodjets==2";
  NamedFunc boosted_nm1_3jet = single_lep && LeadingNonBJetPt_med<100. && max_ak8pfjets_deepdisc_hbb>0.8 && "ngoodjets==3";


  vector<NamedFunc> sels = {signal_nm1,signal_nm1_3jet,signal_nm1_res,signal_nm1_3jet_res,boosted_nm1};
  vector<string> tag_list = {"signal_nm1","signal_nm1_3jet","signal_nm1_res","signal_nm1_3jet_res","boosted_nm1"};
  vector<NamedFunc> weights = {"weight * w_pu" * yearWeight};
  

for(uint j=0;j<weights.size();j++){
for(uint isel=0;isel<sels.size();isel++){
    pm.Push<Hist1D>(Axis(20, 0, 400., "mt_met_lep", "mT [GeV]"),
      sels[isel] && "pfmet>200 && mct>200&&mbb<150&&mbb>90", sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[isel]); 

    pm.Push<Hist1D>(Axis(20, 0, 400., "mct", "m_{CT} [GeV]"),
      sels[isel] && "pfmet>200 && mt_met_lep>150&&mbb<150&&mbb>90", sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[isel]); 

    pm.Push<Hist1D>(Axis(18, 50, 410., "mbb", "m_{bb} [GeV]"),
      sels[isel] && "pfmet>200 && mt_met_lep>150&&mct>200", sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[isel]); 

    pm.Push<Hist1D>(Axis(25, 50, 300., "mbb", "m_{bb} [GeV]"),
      sels[isel] && "mbb<300&&pfmet>200 && mt_met_lep>150&&mct>200", sample_list, all_plot_types).Weight(weights[j]).Tag("fine_"+tag_list[isel]); 


     pm.Push<Hist1D>(Axis(10, 125, 600., "pfmet", "MET [GeV]"),
    sels[isel] && "mct>200&&mbb<150&&mbb>90&&mt_met_lep>150", sample_list, all_plot_types).Weight(weights[j]).Tag(tag_list[isel]); 


    if(isel==1 || isel==3){
          pm.Push<Hist1D>(Axis(12, 0, 600., LeadingNonBJetPt_med, "3rd jet p_{T} [GeV]"),
         single_lep && "ngoodjets==3&&mct>200&&mbb<150&&mbb>90&&mt_met_lep>150" &&  LeadingNonBJetPt_med>0., sample_list,all_plot_types).Weight(weights[j]).Tag(tag_list[isel]);  
   
    }

    if(isel==4 || isel == 2){
      pm.Push<Hist1D>(Axis(18, 50, 410., "mbb", "m_{bb} [GeV]"),
      sels[isel] && "pfmet>200 && mt_met_lep>150&&mct<200", sample_list, all_plot_types).Weight(weights[j]).Tag("CR_"+tag_list[isel]); 
      pm.Push<Hist1D>(Axis(20, 0, 400., "mt_met_lep", "mT [GeV]"),
      sels[isel] && "pfmet>200 && mct<200&&mbb<150&&mbb>90", sample_list, all_plot_types).Weight(weights[j]).Tag("CR_"+tag_list[isel]); 


    }

  }

      //Preselection, N-1
    // pm2017.Push<Hist1D>(Axis(20, 0, 400., "mt_met_lep", "mT [GeV]"),
    //   sels[j] && "mct>200&&mbb<150&&mbb>90", sample_list2017, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    // pm2017.Push<Hist1D>(Axis(20, 0, 400., "mct", "m_{CT} [GeV]"),
    //   sels[j] && "mt_met_lep>150&&mbb<150&&mbb>90", sample_list2017, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    // pm2017.Push<Hist1D>(Axis(18, 50, 410., "mbb", "m_{bb} [GeV]"),
    //   sels[j] && "mt_met_lep>150&&mct>200", sample_list2017, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    // pm2018.Push<Hist1D>(Axis(20, 0, 400., "mt_met_lep", "mT [GeV]"),
    //   sels[j] && "mct>200&&mbb<150&&mbb>90", sample_list2018, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    // pm2018.Push<Hist1D>(Axis(20, 0, 400., "mct", "m_{CT} [GeV]"),
    //   sels[j] && "mt_met_lep>150&&mbb<150&&mbb>90", sample_list2018, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 

    // pm2018.Push<Hist1D>(Axis(18, 50, 410., "mbb", "m_{bb} [GeV]"),
    //   sels[j] && "mt_met_lep>150&&mct>200", sample_list2018, all_plot_types).Weight(weights[j]).Tag(tag_list[j]); 



   }
  
  pm.min_print_ = true;
  pm.MakePlots(lumi2016+lumi2017+lumi2018);
  

}
