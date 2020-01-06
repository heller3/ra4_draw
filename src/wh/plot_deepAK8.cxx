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
  
  double lumi2016 = 36.;
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

  NamedFunc baselinef = "pass&&nvetoleps==1&&ngoodjets>=2 && ngoodjets<=3 && pfmet>125 && mt_met_lep>50 && ngoodbtags==2 && nWHLeptons==1" && PassThirdJetHighpTVeto;

  auto all_top = {mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2017_dir+"*TTJets_1lep_top_f17v2*.root",mc2017_dir+"*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2018_dir+"*TTJets_1lep_top_a18v1*.root",mc2018_dir+"*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"*TTJets_1lep_*met80*.root",mc2016_dir+"*_ST_*.root",mc2017_dir+"*_ST_*.root",mc2018_dir+"*_ST_*.root",mc2016_dir+"*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2017_dir+"*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2018_dir+"*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"*TTJets_2lep_*met80*.root"};


  auto all_other = {mc2016_dir+"*slim_W*JetsToLNu_s16v3*",mc2016_dir+"*W*Jets_NuPt200_s16v*.root",mc2017_dir+"*slim_W*JetsToLNu_f17v2*",mc2017_dir+"*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"*slim_W*JetsToLNu_a18v1*",mc2018_dir+"*W*Jets_NuPt200_a18v1*.root",mc2016_dir+"*WW*.root", mc2016_dir+"*WZ*.root",mc2016_dir+"*ZZ*.root",mc2017_dir+"*WW*.root", mc2017_dir+"*WZ*.root",mc2017_dir+"*ZZ*.root",mc2018_dir+"*WW*.root", mc2018_dir+"*WZ*.root",mc2018_dir+"*ZZ*.root",mc2016_dir+"*_TTWJets*.root", mc2016_dir+"*_TTZ*.root",mc2017_dir+"*_TTWJets*.root", mc2017_dir+"*_TTZ*.root",mc2018_dir+"*_TTWJets*.root", mc2018_dir+"*_TTZ*.root"};

  Palette colors("txt/colors.txt", "default");

  auto proc_wjets = Process::MakeShared<Baby_full>("W+jets and others", Process::Type::background, colors("wjets"), all_other,"stitch&&evt!=74125994"&&baselinef);
  auto proc_top = Process::MakeShared<Baby_full>("Top", Process::Type::background, colors("tt_1l"),all_top,"stitch"&&baselinef);


  auto proc_sig_boost = Process::MakeShared<Baby_full>("TChiWH(800,1)", Process::Type::signal, colors("t1tttt"),
  {signal_dir2016+"*TChiWH*.root",signal_dir2017+"*TChiWH*.root",signal_dir2018+"*TChiWH*.root"},"mass_stop==800&&mass_lsp==1"&&baselinef);

  auto proc_sig_compressed = Process::MakeShared<Baby_full>("TChiWH(225,75)", Process::Type::signal, colors("t1tttt"),
  {signal_dir2016+"*TChiWH*.root",signal_dir2017+"*TChiWH*.root",signal_dir2018+"*TChiWH*.root"},"mass_stop==225&&mass_lsp==75"&&baselinef);
  proc_sig_compressed->SetLineStyle(2);

    auto proc_sig_med = Process::MakeShared<Baby_full>("TChiWH(350,150)", Process::Type::signal, colors("t1tttt"),
  {signal_dir2016+"*TChiWH*.root",signal_dir2017+"*TChiWH*.root",signal_dir2018+"*TChiWH*.root"},"mass_stop==350&&mass_lsp==150"&&baselinef);
  proc_sig_med->SetLineStyle(9);



  vector<shared_ptr<Process> > all_procs = {proc_top,proc_wjets,proc_sig_boost,/*proc_sig_med,*/proc_sig_compressed};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm)
    .RatioMinimum(0.25).RatioMaximum(2.5);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .Bottom(BottomType::off);
  //  .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {/*log_lumi_info, lin_lumi_info,*/lin_shapes_info,log_shapes_info};

  vector<NamedFunc> sels = {"ngoodjets==2","mct>200&&mt_met_lep>150","mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodjets==2","mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodjets==3","mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodjets==2&&ak8pfjets_deepdisc_hbb>0.8","mct>200&&mt_met_lep>150&&ngoodjets==2&&ak8pfjets_deepdisc_hbb>0.8","mct>200&&mt_met_lep>150&&ngoodjets==3&&ak8pfjets_deepdisc_hbb>0.8"};

//  vector<NamedFunc> sels = {signal_nm1_medmed};
  vector<NamedFunc> weights = {"weight * w_pu" * yearWeight};

  
  PlotMaker * pm = new PlotMaker();



for(uint j=0;j<sels.size();j++){
if(j>2) continue;
  pm->Push<Hist1D>(Axis(22, -0.1, 1., max_ak8pfjets_deepdisc_hbb, "Max DeepAK8 H_{bb} score"),
    sels[j],all_procs, all_plot_types).Weight(weights[0]); 

  // pm->Push<Hist1D>(Axis(20, 0, 1., "ak8pfjets_deepdisc_top", "Deep top score"),
  // sels[j],all_procs, all_plot_types).Weight(weights[0]); 

  pm->Push<Hist1D>(Axis(4, -0.5, 3.5, nAK8jets, "N_{AK8 jets}"),
    sels[j],all_procs, all_plot_types).Weight(weights[0]); 

  pm->Push<Hist1D>(Axis(2, -0.5, 1.5, nHiggsTag, "N_{Higgs tag}"),
    sels[j],all_procs, all_plot_types).Weight(weights[0]); 

  pm->Push<Hist1D>(Axis(14, 0, 420., "ak8pfjets_m", "AK8 mass"),
  sels[j],all_procs, all_plot_types).Weight(weights[0]); 

  pm->Push<Hist1D>(Axis(14, 0, 420., "mbb", "m_{bb} [GeV]"),
  sels[j],all_procs, all_plot_types).Weight(weights[0]); 

  pm->Push<Hist1D>(Axis(11, 0, 4.4, deltaRLeadingJets, "Reco dR_{bb} [GeV]"),
  sels[j],all_procs, all_plot_types).Weight(weights[0]);   

  pm->Push<Hist1D>(Axis(20, 200, 1000., "ak8pfjets_pt", "AK8 p_{T}"),
  sels[j],all_procs, all_plot_types).Weight(weights[0]); 


}


    pm->min_print_ = true;

  //pm2016->MakePlots(lumi2016);
 //  pm2017->MakePlots(lumi2017);
 cout<<lumi2016<<lumi2017<<lumi2018<<endl;
   pm->MakePlots(lumi2016+lumi2017+lumi2018);
// 
}
