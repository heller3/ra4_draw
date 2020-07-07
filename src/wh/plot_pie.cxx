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

  double lumi = 137.;

  Palette colors("txt/colors.txt", "default");

  // Cuts in baseline speed up the yield finding
  string baseline         = "";
  NamedFunc baselinef     = "hasNano&&pass&&nvetoleps>=1&&ngoodjets>=2 && ngoodjets<=3 &&pfmet>125&&mt_met_lep>150 && ngoodbtags>=1 && nWHLeptons>=1";


  // string mc2016_dir       = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/s16v3/";
  // string mc2016_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/s16v3/";

  // string mc2017_dir       = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/f17v2/";
  // string mc2017_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/f17v2/";

  // string mc2018_dir       = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/a18v1/";
  // string mc2018_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_07/a18v1/";

  // string signal_dir2016 = "/home/users/rheller/wh_babies/babies_signal_s16v3_v32_2019_10_07/";
  // string signal_dir2016 = "/home/users/rheller/wh_babies/babies_signal_s16v3_v32_2020_05_26/";
  // // string signal_dir2017 = "/home/users/rheller/wh_babies/babies_signal_f17v2_v32_0_2019_10_07/";
  // string signal_dir2017 = "/home/users/rheller/wh_babies/babies_signal_f17v2_v32_0_2020_05_26/";
  // // string signal_dir2018 = "/home/users/rheller/wh_babies/babies_signal_a18v1_v32_0_2019_10_07/";
  // string signal_dir2018 = "/home/users/rheller/wh_babies/babies_signal_a18v1_v32_0_2020_05_26/";


  // string data2016_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/";
  // string data2017_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/";
  // string data2018_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/";


  // string mc2016_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/s16v3/";
  // string mc2016_dir_ttmet = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/s16v3/";

  // string mc2017_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/f17v2/";
  // string mc2017_dir_ttmet = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/f17v2/";

  // string mc2018_dir = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/a18v1/";
  // string mc2018_dir_ttmet = "/home/users/rheller/wh_babies/babies_v33_4_2020_05_27/a18v1/";

  string signal_dir2016 = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string signal_dir2017 = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string signal_dir2018 = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";

  string data2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string data2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";

  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/s16v3/";
  string mc2016_dir_ttmet = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/s16v3/";

  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/f17v2/";
  string mc2017_dir_ttmet = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/f17v2/";

  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/a18v1/";
  string mc2018_dir_ttmet = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/a18v1/";
  string mc2018_dir_ttmet_2l = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_06_12/a18v1/";



  // auto all_tt2l = {mc2016_dir+"slim_*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"slim_*TTJets_2lep_*met150*.root",mc2017_dir+"slim_*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"slim_*TTJets_2lep_*met150*.root",mc2018_dir+"slim_*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"slim_*TTJets_2lep_*met80*.root"};

  // auto all_tt1l = {mc2016_dir+"slim_*TTJets_1lep_top_s16v3*.root",mc2016_dir+"slim_*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"slim_*TTJets_1lep_*met150*.root",mc2017_dir+"slim_*TTJets_1lep_top_f17v2*.root",mc2017_dir+"slim_*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"slim_*TTJets_1lep_*met150*.root",mc2018_dir+"slim_*TTJets_1lep_top_a18v1*.root",mc2018_dir+"slim_*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"slim_*TTJets_1lep_*met80*.root"};

  // auto all_tt = {mc2016_dir+"slim_*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"slim_*TTJets_2lep_*met150*.root",mc2017_dir+"slim_*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"slim_*TTJets_2lep_*met150*.root",mc2018_dir+"slim_*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"slim_*TTJets_2lep_*met80*.root",mc2016_dir+"slim_*TTJets_1lep_top_s16v3*.root",mc2016_dir+"slim_*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"slim_*TTJets_1lep_*met150*.root",mc2017_dir+"slim_*TTJets_1lep_top_f17v2*.root",mc2017_dir+"slim_*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"slim_*TTJets_1lep_*met150*.root",mc2018_dir+"slim_*TTJets_1lep_top_a18v1*.root",mc2018_dir+"slim_*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"slim_*TTJets_1lep_*met80*.root"};

  auto all_ST = {mc2016_dir+"*slim_ST_*.root",mc2017_dir+"*slim_ST_*.root",mc2018_dir+"*slim_ST_*.root"};

  auto all_top = {mc2016_dir+"slim_*TTJets_1lep_top_s16v3*.root",mc2016_dir+"slim_*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"slim_*TTJets_1lep_*met150*.root",mc2017_dir+"slim_*TTJets_1lep_top_f17v2*.root",mc2017_dir+"slim_*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"slim*TTJets_1lep_*met150*.root",mc2018_dir+"slim_*TTJets_1lep_top_a18v1*.root",mc2018_dir+"slim_*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"slim*TTJets_1lep_*met80*.root",mc2016_dir+"slim*ST_*.root",mc2017_dir+"slim*ST_*.root",mc2018_dir+"slim*ST_*.root",mc2016_dir+"slim_*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"slim*TTJets_2lep_*met150*.root",mc2017_dir+"slim_*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"slim*TTJets_2lep_*met150*.root",mc2018_dir+"slim_*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet_2l+"slim_TTJets_2lep_*met80*.root"};

  auto all_tt = {mc2016_dir+"slim_*TTJets_1lep_top_s16v3*.root",mc2016_dir+"slim_*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"slim_*TTJets_1lep_*met150*.root",mc2017_dir+"slim_*TTJets_1lep_top_f17v2*.root",mc2017_dir+"slim_*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"slim*TTJets_1lep_*met150*.root",mc2018_dir+"slim_*TTJets_1lep_top_a18v1*.root",mc2018_dir+"slim_*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"slim*TTJets_1lep_*met80*.root",mc2016_dir+"slim_*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"slim*TTJets_2lep_*met150*.root",mc2017_dir+"slim_*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"slim*TTJets_2lep_*met150*.root",mc2018_dir+"slim_*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet_2l+"slim_TTJets_2lep_*met80*.root"};

  auto all_tt1l = {mc2016_dir+"slim_*TTJets_1lep_top_s16v3*.root",mc2016_dir+"slim_*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"slim_*TTJets_1lep_*met150*.root",mc2017_dir+"slim_*TTJets_1lep_top_f17v2*.root",mc2017_dir+"slim_*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"slim*TTJets_1lep_*met150*.root",mc2018_dir+"slim_*TTJets_1lep_top_a18v1*.root",mc2018_dir+"slim_*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"slim*TTJets_1lep_*met80*.root"};


  auto all_tt2l = {mc2016_dir+"slim_*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"slim*TTJets_2lep_*met150*.root",mc2017_dir+"slim_*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"slim*TTJets_2lep_*met150*.root",mc2018_dir+"slim_*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet_2l+"slim_*TTJets_2lep_*met80*.root"};

  auto all_other = {mc2016_dir+"slim_*WW*.root", mc2016_dir+"slim_*WZ*.root",mc2016_dir+"slim_*ZZ*.root",mc2017_dir+"slim_*WW*.root", mc2017_dir+"slim_*WZ*.root",mc2017_dir+"slim_*ZZ*.root",mc2018_dir+"slim_*WW*.root", mc2018_dir+"slim_*WZ*.root",mc2018_dir+"slim_*ZZ*.root",mc2016_dir+"slim_*TTWJets*.root", mc2016_dir+"slim_*TTZ*.root",mc2017_dir+"slim_*TTWJets*.root", mc2017_dir+"slim_*TTZ*.root",mc2018_dir+"slim_*TTWJets*.root", mc2018_dir+"slim_*TTZ*.root"};
  //// Contributions
  auto proc_wjets = Process::MakeShared<Baby_full>("W+jets 2016-2018", Process::Type::background, kCyan-3, {mc2016_dir+"slim_W*JetsToLNu_s16v3*",mc2016_dir+"slim_*W*Jets_NuPt200_s16v*.root",mc2017_dir+"*slim_W*JetsToLNu_f17v2*",mc2017_dir+"*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim_*W*Jets_NuPt200_a18v1*.root"},"stitch&&evt!=74125994"&&baselinef);
  auto proc_top = Process::MakeShared<Baby_full>("top 2016-2018", Process::Type::background, kGreen-3,all_top,"stitch"&&baselinef);
  auto proc_tt = Process::MakeShared<Baby_full>("tt 2016-2018", Process::Type::background, colors("tt_2l"),all_tt,"stitch"&&baselinef);
  auto proc_tt2l = Process::MakeShared<Baby_full>("tt2l 2016-2018", Process::Type::background, kRed+1,all_tt2l,"stitch"&&baselinef);
  auto proc_tt1l = Process::MakeShared<Baby_full>("tt1l 2016-2018", Process::Type::background, kRed+1,all_tt1l,"stitch"&&baselinef);
  auto proc_ST = Process::MakeShared<Baby_full>("ST 2016-2018", Process::Type::background, kRed+1,all_ST,"stitch"&&baselinef);
  auto proc_other = Process::MakeShared<Baby_full>("TTV and VV 2016-2018", Process::Type::background, kRed+1,all_other,baselinef);
  

  // auto proc_top_2realb = Process::MakeShared<Baby_full>("top, 2 real b-jets 2016-2018", Process::Type::background, kGreen-3,all_top,"stitch"&&baselinef&&nRealBs==2.);
  // auto proc_top_2realb_tt = Process::MakeShared<Baby_full>("ttbar, 2 real b-jets 2016-2018", Process::Type::background, kGreen-3,all_tt,"stitch"&&baselinef&&nRealBs==2.);
  // auto proc_top_2realb_st = Process::MakeShared<Baby_full>("ST, 2 real b-jets 2016-2018", Process::Type::background,  kRed+1,all_ST,"stitch"&&baselinef&&nRealBs==2.);
  // auto proc_top_1realb = Process::MakeShared<Baby_full>("top, 1 real b-jet 2016-2018", Process::Type::background, kBlue+1,all_top,"stitch"&&baselinef&&nRealBs==1.);
  // auto proc_top_0realb = Process::MakeShared<Baby_full>("top, 0 real b-jets 2016-2018", Process::Type::background, kOrange+7,all_top,"stitch"&&baselinef&&nRealBs==0.);


  // auto proc_top_2realb = Process::MakeShared<Baby_full>("top, 2 real b-jets 2016-2018", Process::Type::background, kGreen-3,all_top,"stitch"&&baselinef&&nRealBtags==2.);
  auto proc_tt_2realb_from_top = Process::MakeShared<Baby_full>("ttbar, 2 real b-tags from tops", Process::Type::background, kGreen-3,all_tt,"stitch"&&baselinef&&nRealBtags==2.&&nRealBsfromTop>=2.);
  auto proc_tt_2realb_extra_bs = Process::MakeShared<Baby_full>("ttbar, 2 real b-tags with extra b", Process::Type::background, kMagenta+3,all_tt,"stitch"&&baselinef&&nRealBtags==2.&&nRealBsfromTop<2.);
  auto proc_st_2realb = Process::MakeShared<Baby_full>("ST, 2 real b-tags", Process::Type::background,  kRed+1,all_ST,"stitch"&&baselinef&&nRealBtags==2.);
  auto proc_top_1realb = Process::MakeShared<Baby_full>("top, 1 real b-tags", Process::Type::background, kBlue+1,all_top,"stitch"&&baselinef&&nRealBtags==1.);
  auto proc_top_0realb = Process::MakeShared<Baby_full>("top, 0 real b-tags", Process::Type::background, kOrange+7,all_top,"stitch"&&baselinef&&nRealBtags==0.);


  auto proc_top_2realb_tt_highmct = Process::MakeShared<Baby_full>("ttbar, 2 real b-jets, m_{CT} > 200", Process::Type::background, kGreen-3,all_tt,"stitch &&mct>200 "&&baselinef&&nRealBs==2.);
  auto proc_top_1realb_tt_highmct = Process::MakeShared<Baby_full>("ttbar, 1 real b-jet, m_{CT} > 200", Process::Type::background, kBlue+1,all_tt,"stitch && mct>200 "&&baselinef&&nRealBs==1.);


  auto proc_top_2realb_tt_from_top = Process::MakeShared<Baby_full>("ttbar, 2 real b-jets from top", Process::Type::background, kGreen-3,all_tt,"stitch"&&baselinef&&nRealBtags==2.&&nRealBsfromTop==2.); //from t top
  auto proc_top_2realb_tt_extra_bs = Process::MakeShared<Baby_full>("ttbar, 2 real b-jets and extra bs", Process::Type::background, kRed+1,all_tt,"stitch"&&baselinef&&nRealBtags==2.&&nRealBsfromTop<2. && nGenBs > 2.);
  auto proc_top_2realb_tt_other = Process::MakeShared<Baby_full>("ttbar, 2 real b-jets, remaining", Process::Type::background, kCyan-3,all_tt,"stitch"&&baselinef&&nRealBtags==2.&&nRealBsfromTop<2. && nGenBs <= 2.);


 //Original set
  // auto proc_top_2realb = Process::MakeShared<Baby_full>("top, 2 real b-tags 2016-2018", Process::Type::background, kGreen-3,all_top,"stitch"&&baselinef&&nRealBtags==2.);
  // auto proc_top_2realb_tt = Process::MakeShared<Baby_full>("ttbar, 2 real b-tags 2016-2018", Process::Type::background, kGreen-3,all_tt,"stitch"&&baselinef&&nRealBtags==2.);
  // auto proc_top_2realb_st = Process::MakeShared<Baby_full>("ST, 2 real b-tags 2016-2018", Process::Type::background,  kRed+1,all_ST,"stitch"&&baselinef&&nRealBtags==2.);
  // auto proc_top_1realb = Process::MakeShared<Baby_full>("top, 1 real b-tag 2016-2018", Process::Type::background, kBlue+1,all_top,"stitch"&&baselinef&&nRealBtags==1.);
  // auto proc_top_0realb = Process::MakeShared<Baby_full>("top, 0 real b-tags 2016-2018", Process::Type::background, kOrange+7,all_top,"stitch"&&baselinef&&nRealBtags==0.);
 
  // auto proc_top_2realb = Process::MakeShared<Baby_full>("top, 2 real b-tags 2016-2018", Process::Type::background, kGreen-3,all_ST,"stitch"&&baselinef&&nRealBtags==2.);
  // auto proc_top_1realb = Process::MakeShared<Baby_full>("top, 1 real b-tag 2016-2018", Process::Type::background, kCyan-3,all_ST,"stitch"&&baselinef&&nRealBtags==1.);
  // auto proc_top_0realb = Process::MakeShared<Baby_full>("top, 0 real b-tags 2016-2018", Process::Type::background, kRed+1,all_ST,"stitch"&&baselinef&&nRealBtags==0.);
  auto proc_top_1realb_not_tagged = Process::MakeShared<Baby_full>("top, 1 real b-tag, 1b failed tag", Process::Type::background, kMagenta+3,all_top,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag>0.);
  //jet with hflavor 5, but not tagged.
  // auto proc_top_1realb_missing_genb = Process::MakeShared<Baby_full>("top, 1 real b-tag, only 1 b in gen record", Process::Type::background,kPink-2,all_top,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs<2.); //never happens

  auto proc_top_1realb_merged = Process::MakeShared<Baby_full>("top, 1 real b-tag, merged bb", Process::Type::background, kCyan-3,all_top,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>0.&&bDeltaRfromtop<0.5);
    //drbb < 0.5
  auto proc_top_1realb_forward = Process::MakeShared<Baby_full>("top, 1 real b-tag, forward b", Process::Type::background, kRed+1,all_top,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central<2.);
  //forward: gen b with eta  > 2.4
  auto proc_top_1realb_soft = Process::MakeShared<Baby_full>("top, 1 real b-tag, soft b", Process::Type::background,kGreen-3,all_top,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2.&&nGenBs_ptG30_central<2.);
  auto proc_top_1realb_lepcleaning = Process::MakeShared<Baby_full>("top, 1 real b-tag, lepton cleaning", Process::Type::background, kBlue+1,all_top,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2.&&nGenBs_ptG30_central>=2. && minDR_lep_bquark < 0.5);
  //Soft: gen b with pt < 30
  auto proc_top_1realb_extrab = Process::MakeShared<Baby_full>("top, 1 real b-tag, more than 2b", Process::Type::background, kPink-2, all_top,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2.&&nGenBs_ptG30_central>=2. && minDR_lep_bquark >= 0.5);
    auto proc_top_1realb_other = Process::MakeShared<Baby_full>("top, 1 real b-tag, other reason", Process::Type::background, kOrange+7,all_top,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs==2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2.&&nGenBs_ptG30_central>=2. && minDR_lep_bquark >= 0.5);

  auto proc_ttbar_1realb_not_tagged = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, 1b failed tag", Process::Type::background, kMagenta+3,all_ST,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag>0.);
  //jet with hflavor 5, but not tagged.
  auto proc_ttbar_1realb_missing_genb = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, only 1 b in gen record", Process::Type::background, kPink-2,all_ST,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs<2.);

  auto proc_ttbar_1realb_merged = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, merged bb", Process::Type::background, kCyan-3,all_ST,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>0.&&bDeltaRfromtop<0.5);
    //drbb < 0.5
  auto proc_ttbar_1realb_forward = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, forward b", Process::Type::background, kRed+1,all_ST,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central<2.);
  //forward: gen b with eta  > 2.4
  auto proc_ttbar_1realb_soft = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, soft b", Process::Type::background,kGreen-3,all_ST,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2&&nGenBs_ptG30_central<2.);
  //Soft: gen b with pt < 30
  auto proc_ttbar_1realb_other = Process::MakeShared<Baby_full>("ttbar, 1 real b-tag, other reason", Process::Type::background, kOrange+7,all_ST,"stitch"&&baselinef&&"ngoodbtags==2"&&nRealBtags==1.&&HasBFailedTag==0.&&nGenBs>=2.&&bDeltaRfromtop>=0.5&&nGenBs_central>=2&&nGenBs_ptG30_central>=2.);


  auto proc_top_lost_lep = Process::MakeShared<Baby_full>("top, lost e/mu 2016-2018", Process::Type::background, kGreen-3,all_top,"stitch"&&baselinef&&"ngoodbtags==2"&&n_true_emu>=2.&& HasHadronicTau==0.);
  auto proc_top_lost_htau = Process::MakeShared<Baby_full>("top, lost htau 2016-2018", Process::Type::background, kCyan-3,all_top,"stitch"&&baselinef&&"ngoodbtags==2"&&HasHadronicTau>0.);
  auto proc_top_1l = Process::MakeShared<Baby_full>("top, 1l 2016-2018", Process::Type::background, kRed+1,all_top,"stitch"&&baselinef&&"ngoodbtags==2"&&n_true_emu<2.&&HasHadronicTau==0.);


  auto proc_top_met0 = Process::MakeShared<Baby_full>("top, 125 < MET < 200", Process::Type::background, kGreen-3,all_top,"stitch&&pfmet>125&&pfmet<200"&&baselinef);
  auto proc_top_met1 = Process::MakeShared<Baby_full>("top 200 < MET < 300", Process::Type::background,  kCyan-3,all_top,"stitch&&pfmet>200&&pfmet<300"&&baselinef);
  auto proc_top_met2 = Process::MakeShared<Baby_full>("top 300 < MET < 400", Process::Type::background, kRed+1,all_top,"stitch&&pfmet>300&&pfmet<400"&&baselinef);
  auto proc_top_met3 = Process::MakeShared<Baby_full>("top MET > 400", Process::Type::background, kBlue+1,all_top,"stitch&&pfmet>400"&&baselinef);

  auto proc_top_1lep = Process::MakeShared<Baby_full>("top, 1 reco lep", Process::Type::background, kGreen-3,all_top,"stitch&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&nWHLeptons==1"&&baselinef);
  auto proc_top_2lep = Process::MakeShared<Baby_full>("top, 2 reco lep", Process::Type::background, kCyan-3,all_top,"stitch&&nvetoleps==2&&nWHLeptons>=1"&&baselinef);


  auto proc_sig = Process::MakeShared<Baby_full>("TChiWH(700,1)", Process::Type::signal, kGray+1,
  {signal_dir2016+"*TChiWH*.root",signal_dir2017+"*TChiWH*.root",signal_dir2018+"*TChiWH*.root"},"mass_stop==700&&mass_lsp==1"&&baselinef);
 auto proc_sig_compress = Process::MakeShared<Baby_full>("TChiWH(225,75)", Process::Type::signal, kGray+1,
  {signal_dir2016+"*TChiWH*.root",signal_dir2017+"*TChiWH*.root",signal_dir2018+"*TChiWH*.root"},"mass_stop==225&&mass_lsp==75"&&baselinef);
  proc_sig_compress->SetLineStyle(2);
  // auto proc_sig_med = Process::MakeShared<Baby_full>("TChiWH(500,125)", Process::Type::signal, colors("t1tttt"),
  // {signal_dir2016+"*TChiWH*.root",signal_dir2017+"*TChiWH*.root",signal_dir2018+"*TChiWH*.root"},"mass_stop==500&&mass_lsp==125"&&baselinef);
  // proc_sig_med->SetLineStyle(3);
  //   auto proc_sig_corner = Process::MakeShared<Baby_full>("TChiWH(650,250)", Process::Type::signal, colors("t1tttt"),
  // {signal_dir2016+"*TChiWH*.root",signal_dir2017+"*TChiWH*.root",signal_dir2018+"*TChiWH*.root"},"mass_stop==650&&mass_lsp==250"&&baselinef);
  // proc_sig_corner->SetLineStyle(6);

  // vector<shared_ptr<Process> > sample_list ={tt2l,tt1l,wjets_low_nu,wjets_high_nu,single_t,other};

  vector<shared_ptr<Process> > inclu_vs_2realb_highmct = {proc_tt,proc_top_2realb_tt_highmct,proc_top_1realb_tt_highmct};
  // vector<shared_ptr<Process> > real_bs_sample_list ={proc_top_2realb_tt,proc_top_2realb_st,proc_top_1realb,proc_top_0realb};
  vector<shared_ptr<Process> > real_bs_sample_list ={proc_tt_2realb_from_top,proc_tt_2realb_extra_bs,proc_st_2realb,proc_top_1realb,proc_top_0realb};


  vector<shared_ptr<Process> > two_real_bs_sample_list ={proc_top_2realb_tt_from_top,proc_top_2realb_tt_extra_bs,proc_top_2realb_tt_other};


  vector<shared_ptr<Process> > real_bs_sample_list_short ={proc_tt_2realb_from_top,proc_tt_2realb_extra_bs,proc_st_2realb,proc_top_1realb,proc_sig,proc_sig_compress};
  vector<shared_ptr<Process> > simple_sample_list ={proc_top,proc_wjets,proc_sig,proc_sig_compress};
  // vector<shared_ptr<Process> > real_bs_sample_list_short ={proc_top_ 2realb_tt,proc_top_2realb_st, proc_top_1realb};
  vector<shared_ptr<Process> > lost_b_sample_list ={proc_top_1realb_not_tagged,proc_top_1realb_merged,proc_top_1realb_forward,proc_top_1realb_soft,proc_top_1realb_lepcleaning,proc_top_1realb_extrab,proc_top_1realb_other};
  vector<shared_ptr<Process> > lost_b_sample_list_ttbar ={proc_ttbar_1realb_not_tagged,proc_ttbar_1realb_missing_genb,proc_ttbar_1realb_merged,proc_ttbar_1realb_forward,proc_ttbar_1realb_soft,proc_ttbar_1realb_other};
  vector<shared_ptr<Process> > lost_b_sample_list_simplified ={proc_top_1realb_forward,proc_top_1realb_soft,proc_top_1realb_other};
  vector<shared_ptr<Process> > lepton_sample_list ={proc_top_lost_lep,proc_top_lost_htau,proc_top_1l};
  vector<shared_ptr<Process> > met_sample_list ={proc_top_met0,proc_top_met1,proc_top_met2,proc_top_met3};
  vector<shared_ptr<Process> > lep_sample_list ={proc_top_1lep,proc_top_2lep};

//  vector<string> tag_list = {"tt1l","tt2l"};
//  vector<shared_ptr<Process> > sample_list = {data, /*tchiwh_250_1, tchiwh_225_75,*/ tt1l, tt2l, wjets_low_nu, wjets_high_nu, single_t, ttv, diboson};

  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::off)
    .YAxis(YAxisType::log)
    .Stack(StackType::lumi_shapes);
    // .RatioMinimum(0.5).RatioMaximum(1.5);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .Bottom(BottomType::off)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {lin_shapes_info,log_shapes_info,log_lumi_info,lin_shapes_info};


  PlotMaker pm;

  //Selection Types
  // string basic_cuts =            "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2";
  // string basic_cuts_mct =        "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200";
  // string basic_cuts_mct_mt =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150";
  // string basic_cuts_mct_mbb =    "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150";
  // string basic_cuts_mt_mbb =     "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mt_met_lep>150&&mbb>90&&mbb<150";
  // string basic_cuts_mct_mt_mbb = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";
  // string dilep_basic_cuts =      "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2";
  // string dilep_mct_mt_mbb =      "((!PassTrackVeto&&!PassTauVeto)||(nvetoleps==2))&&ngoodjets==2&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150";


  // NamedFunc signalRegion =        "pfmet>200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc mbbside =             "pfmet>200&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc dilep =               "pfmet>125" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc lowMCT =              "pfmet>200&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;

  // NamedFunc signalRegion_lowMET =        "pfmet>125&&pfmet<200" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc mbbside_lowMET =             "pfmet>125&&pfmet<200&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc dilep_lowMET =               "pfmet>125&&pfmet<200" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc lowMCT_lowMET =              "pfmet>125&&pfmet<200&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;

  // NamedFunc signalRegion_medMET =        "pfmet>200&&pfmet<300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc mbbside_medMET =             "pfmet>200&&pfmet<300&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc dilep_medMET =               "pfmet>200&&pfmet<300" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc lowMCT_medMET =              "pfmet>200&&pfmet<300&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;

  // NamedFunc signalRegion_highMET =        "pfmet>300" && basic_cuts_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc mbbside_highMET =             "pfmet>300&&(mbb<90||mbb>150)" && basic_cuts_mct_mt && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc dilep_highMET =               "pfmet>300" && dilep_mct_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;
  // NamedFunc lowMCT_highMET =              "pfmet>300&&mct<200" && basic_cuts_mt_mbb && WHLeptons==1 && HasMedLooseDeepCSV;

  TString single_lep      = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&nWHLeptons==1 && ngoodbtags==2";
  NamedFunc single_lep_alt      = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&nWHLeptons==1" && nMedb==2.;
  NamedFunc single_lep_tightnothing      = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&nWHLeptons==1" && nTightb==1. && nLooseb == 1.; 
  NamedFunc single_lep_tighttight      = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&nWHLeptons==1" && nTightb==2. && nLooseb == 2.;

  NamedFunc dilep_medmed      = "nvetoleps == 2 && nWHLeptons>=1" && nMedb==2.;
  NamedFunc dilep_tightnothing      = "nvetoleps == 2 && nWHLeptons>=1" && nTightb==1. && nLooseb == 1.; 
  NamedFunc dilep_tighttight      = "nvetoleps == 2 && nWHLeptons>=1" && nTightb==2. && nLooseb == 2.;




  TString dilep           = "nvetoleps==2 && ngoodbtags==2";
  vector<NamedFunc> metbins = {"pfmet>125&&pfmet<=200",/*"pfmet>200&&pfmet<=300","pfmet>300&&pfmet<400","pfmet>400",*/"pfmet>200"};
  vector<TString> metNames = {"low_MET",/*"medium_MET","high_MET","vhigh_MET",*/"inclu_highmet"};
  vector<NamedFunc> boosted_metbins = {"pfmet>125&&pfmet<=300","pfmet>300","pfmet>125"};
  // vector<NamedFunc> boosted_metbins = {"pfmet>125"};
  vector<TString> boosted_metNames = {"low MET","high MET"};

  vector<NamedFunc> njetbins = {"ngoodjets==2","ngoodjets==3"&&LeadingNonBJetPt_med<300.}; 
  vector<TString> njetNames = {"2 jets","3 jets"};

  // vector<NamedFunc> deepAK8bins = {max_ak8pfjets_deepdisc_hbb<=0.8,max_ak8pfjets_deepdisc_hbb>0.8};
  vector<NamedFunc> deepAK8bins = {"nHiggs==0","nHiggs>0"};
  TString signal_region = single_lep+"&&mct>200&&mbb>90&&mbb<150";
  TString mct_control_region = single_lep+"&&mct>150&&mct<=200&&mbb>90&&mbb<150";    
  // if(dilep_mode) mct_control_region = dilep+"&&mct>200&&mbb>90&&mbb<150";    

  TString signal_region_boost = single_lep+"&&mct>200&&mbb>90&&mbb<150";
  TString mct_control_region_boost = single_lep+"&&mct<=200&&mbb>90&&mbb<150";
  // if(dilep_mode) mct_control_region_boost = dilep+"&&mct>200&&mbb>90&&mbb<150";


  // map<string, vector<shared_ptr<Process> > > categories;
  map<string, vector<shared_ptr<Process> > > categories_Bs;
  map<string, vector<shared_ptr<Process> > > categories_2realBs;
  map<string, vector<shared_ptr<Process> > > categories_lost_Bs;
  map<string, vector<shared_ptr<Process> > > categories_lost_Bs_tt;
  // map<string, vector<shared_ptr<Process> > > categories_split_Bs;

  // categories["process"] = vector<shared_ptr<Process> >();
  // categories["process"].push_back(tt2l);
  // categories["process"].push_back(tt1l);
  // categories["process"].push_back(wjets_low_nu);
  // categories["process"].push_back(wjets_high_nu);
  // categories["process"].push_back(single_t);
  // categories["process"].push_back(other);


  categories_2realBs["twoRealBs"] = vector<shared_ptr<Process> >();
  categories_2realBs["twoRealBs"].push_back(proc_top_2realb_tt_from_top);
  categories_2realBs["twoRealBs"].push_back(proc_top_2realb_tt_extra_bs);
  categories_2realBs["twoRealBs"].push_back(proc_top_2realb_tt_other);

  // categories_Bs["RealBs"] = vector<shared_ptr<Process> >();
  // categories_Bs["RealBs"].push_back(proc_top_2realb_tt);
  // categories_Bs["RealBs"].push_back(proc_top_2realb_st);
  // categories_Bs["RealBs"].push_back(proc_top_1realb);
  // categories_Bs["RealBs"].push_back(proc_top_0realb);

  categories_Bs["RealBs"] = vector<shared_ptr<Process> >();
  categories_Bs["RealBs"].push_back(proc_tt_2realb_from_top);
  categories_Bs["RealBs"].push_back(proc_tt_2realb_extra_bs);
  categories_Bs["RealBs"].push_back(proc_st_2realb);
  categories_Bs["RealBs"].push_back(proc_top_1realb);
  categories_Bs["RealBs"].push_back(proc_top_0realb);

  categories_lost_Bs["LostBs"] = vector<shared_ptr<Process> >();
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_not_tagged);
  // categories_lost_Bs["LostBs"].push_back(proc_top_1realb_missing_genb);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_merged);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_forward);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_soft);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_lepcleaning);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_extrab);
  categories_lost_Bs["LostBs"].push_back(proc_top_1realb_other);

  categories_lost_Bs_tt["LostBs_tt"] = vector<shared_ptr<Process> >();
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_not_tagged);
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_missing_genb);
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_merged);
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_forward);
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_soft);
  categories_lost_Bs_tt["LostBs_tt"].push_back(proc_ttbar_1realb_other);

  //What happened to missing real b's in events with 1 fake?


  // categories_split_Bs["fullRealBs"] = vector<shared_ptr<Process> >();
  // categories_split_Bs["fullRealBs"].push_back(tt2l_2b);
  // categories_split_Bs["fullRealBs"].push_back(tt2l_1b);
  // categories_split_Bs["fullRealBs"].push_back(wjets_low_nu);
  // categories_split_Bs["fullRealBs"].push_back(wjets_high_nu_1b);
  // categories_split_Bs["fullRealBs"].push_back(wjets_high_nu_0b);
  // categories_split_Bs["fullRealBs"].push_back(single_t);
  // categories_split_Bs["fullRealBs"].push_back(other);


  vector<TableRow> signal_table_cuts;
  vector<TableRow> tightveto_table_cuts;
  vector<TableRow> highmbb_table_cuts;
  vector<TableRow> dilep_table_cuts;
  vector<TableRow> dilep_tighttight_table_cuts;
  vector<TableRow> lowMCT_table_cuts;
  // vector<TableRow> mbbside_table_cuts;

  NamedFunc full_weight = "weight * trig_eff" * nanoWeight * yearWeight;

  for(uint ijet=0;ijet<njetbins.size();ijet++){
    for(uint imet=0;imet<metbins.size();imet++){

      signal_table_cuts.push_back(TableRow(Form("SR, %s %s",njetNames[ijet].Data(),metNames[imet].Data()), signal_region && njetbins[ijet] && metbins[imet] && deepAK8bins[0],0,0, full_weight)); 
      tightveto_table_cuts.push_back(TableRow(Form("tight+veto, %s %s",njetNames[ijet].Data(),metNames[imet].Data()), single_lep_tightnothing && "mct>200&&mbb>90&&mbb<150" && njetbins[ijet] && metbins[imet] && deepAK8bins[0],0,0, full_weight)); 
      highmbb_table_cuts.push_back(TableRow(Form("highmbb_medmed, %s %s",njetNames[ijet].Data(),metNames[imet].Data()), single_lep_alt && "mct>200&&mbb>200" && njetbins[ijet] && metbins[imet] && deepAK8bins[0],0,0, full_weight)); 
      dilep_table_cuts.push_back(TableRow(Form("dilep_medmed, %s %s",njetNames[ijet].Data(),metNames[imet].Data()), dilep_medmed && "mct>200&&mbb>90&&mbb<150" && njetbins[ijet] && metbins[imet] && deepAK8bins[0],0,0, full_weight)); 
      dilep_tighttight_table_cuts.push_back(TableRow(Form("dilep_tightight, %s %s",njetNames[ijet].Data(),metNames[imet].Data()), dilep_tighttight && "mct>200&&mbb>90&&mbb<150" && njetbins[ijet] && metbins[imet] && deepAK8bins[0],0,0, full_weight)); 

      lowMCT_table_cuts.push_back(TableRow(Form("low mCT CR %s %s",njetNames[ijet].Data(),metNames[imet].Data()), mct_control_region && njetbins[ijet] && metbins[imet] && deepAK8bins[0] ,0,0, full_weight)); 
   
    }
    cout<<"Finished defining cuts"<<endl;
    for(uint imet_boos=0;imet_boos<boosted_metbins.size();imet_boos++){
     signal_table_cuts.push_back(TableRow(Form("SR, %s %s",njetNames[ijet].Data(),boosted_metNames[imet_boos].Data()), signal_region_boost && njetbins[ijet] && boosted_metbins[imet_boos] && deepAK8bins[1],0,0, full_weight)); 
     lowMCT_table_cuts.push_back(TableRow(Form("low mCT CR %s %s",njetNames[ijet].Data(),boosted_metNames[imet_boos].Data()), mct_control_region_boost && njetbins[ijet] && boosted_metbins[imet_boos] && deepAK8bins[1],0,0, full_weight)); 
    }  
  

     // pm.Push<Hist1D>(Axis(10, 0, 100., genBpT, "b-quark gen pT [GeV]"), signal_region && njetbins[ijet] && "pfmet>125 && pfmet<200" , lost_b_sample_list_simplified, all_plot_types).Weight(full_weight);
      // pm.Push<Hist1D>(Axis(10, 0, 100., genB_subleadingpT, "b-quark subleading gen pT [GeV]"), signal_region && njetbins[ijet] && "pfmet>125 && pfmet<200" , lost_b_sample_list_simplified, all_plot_types).Weight(full_weight);
      // pm.Push<Hist1D>(Axis(10, -5, 5., genBeta_mostForward, "b-quark most forward gen eta"), signal_region && njetbins[ijet] && "pfmet>125 && pfmet<200" , lost_b_sample_list_simplified, all_plot_types).Weight(full_weight);
      // pm.Push<Hist1D>(Axis(10, 0, 4., bDeltaR, "b-quark deltaR"), signal_region && njetbins[ijet] && "pfmet>125 && pfmet<200" , lost_b_sample_list_simplified, all_plot_types).Weight(full_weight);
      // pm.Push<Hist1D>(Axis(10, 0, 4., minDR_lep_bquark, "min dR(bquark, reco lep)"), signal_region && njetbins[ijet] && "pfmet>125 && pfmet<200" , lost_b_sample_list_simplified, all_plot_types).Weight(full_weight);


      // pm.Push<Hist1D>(Axis(10, 0, 100., genBpT, "b-quark gen pT [GeV]"), signal_region && njetbins[ijet] && "pfmet>200" , lost_b_sample_list_simplified, all_plot_types).Weight(full_weight);
     

      // pm.Push<Hist1D>(Axis(10, 0, 100., genB_subleadingpT, "b-quark subleading gen pT [GeV]"), signal_region && njetbins[ijet] && "pfmet>200" , lost_b_sample_list_simplified, all_plot_types).Weight(full_weight);
      // pm.Push<Hist1D>(Axis(10, -5, 5., genBeta_mostForward, "b-quark most forward gen eta"), signal_region && njetbins[ijet] && "pfmet>200" , lost_b_sample_list_simplified, all_plot_types).Weight(full_weight);
      // pm.Push<Hist1D>(Axis(10, 0, 4., bDeltaR, "b-quark deltaR"), signal_region && njetbins[ijet] && "pfmet>200" , lost_b_sample_list_simplified, all_plot_types).Weight(full_weight);
      // pm.Push<Hist1D>(Axis(10, 0, 4., minDR_lep_bquark, "min dR(bquark, reco lep)"), signal_region && njetbins[ijet] && "pfmet>200" , lost_b_sample_list_simplified, all_plot_types).Weight(full_weight);
 

    /*
    pm.Push<Hist1D>(Axis(16, 0.4, 3., "ak4pfjets_pt/ak4pfjets_genpt", "Jet response (reco/gen)"), single_lep_alt&&"ak4pfjets_genpt>0 &&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , inclu_vs_2realb_highmct, all_plot_types).Weight(full_weight).Tag("response");
    pm.Push<Hist1D>(Axis(16, 0.4, 3., "ak4pfjets_pt/ak4pfjets_genpt", "Jet response (reco/gen)"), single_lep_alt&&"ak4pfjets_genpt>0 &&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , inclu_vs_2realb_highmct, all_plot_types).Weight(full_weight).Tag("response");

    pm.Push<Hist1D>(Axis(16, 0.4, 3., "ak4pfjets_pt/ak4pfjets_genpt", "Jet response (reco/gen)"), single_lep_alt&&genmct<160.&&"ak4pfjets_genpt>0 &&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , inclu_vs_2realb_highmct, all_plot_types).Weight(full_weight).Tag("response");
    pm.Push<Hist1D>(Axis(16, 0.4, 3., "ak4pfjets_pt/ak4pfjets_genpt", "Jet response (reco/gen)"), single_lep_alt&&genmct<160.&&"ak4pfjets_genpt>0 &&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , inclu_vs_2realb_highmct, all_plot_types).Weight(full_weight).Tag("response");




      pm.Push<Hist1D>(Axis(15, 0, 300., genmct, "gen m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("medmed");
      pm.Push<Hist1D>(Axis(15, 0, 300., genmct, "gen m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("medmed");

      pm.Push<Hist1D>(Axis(15, 0, 300., genmct, "gen m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>300" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("medmed");
 
      pm.Push<Hist1D>(Axis(15, 0, 300., genmct, "gen m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , simple_sample_list, all_plot_types).Weight(full_weight).Tag("simple_medmed");
      pm.Push<Hist1D>(Axis(15, 0, 300., genmct, "gen m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , simple_sample_list, all_plot_types).Weight(full_weight).Tag("simple_medmed");

      pm.Push<Hist1D>(Axis(15, 0, 300., genmct, "gen m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>300" , simple_sample_list, all_plot_types).Weight(full_weight).Tag("simple_medmed");
      pm.Push<Hist1D>(Axis(15, 0, 300., genmct, "gen m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>400" , simple_sample_list, all_plot_types).Weight(full_weight).Tag("simple_medmed");
*/

if(false){
      pm.Push<Hist1D>(Axis(25, 0, 4., max_genjet_bquark_pt_ratio, "Max b-genjet / b-quark pT ratio [GeV]"), single_lep_alt&&nRealBs==2.&&nRealBsfromTop==2.&&"mbb>90&&mbb<150" && njetbins[ijet], met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(25, 0, 4., max_genjet_bquark_pt_ratio, "Max b-genjet / b-quark pT ratio [GeV]"), single_lep_alt&&nRealBs==2.&&nRealBsfromTop==2.&&"mbb>90&&mbb<150 && mct >200" && njetbins[ijet], met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");

      pm.Push<Hist1D>(Axis(10, 0, 0.5, dR_jet_bquark_max_pt_ratio, "dR(b-quark, jet), jet with highest gen pT / quark pT ratio"), single_lep_alt&&nRealBs==2.&&nRealBsfromTop==2.&&"mbb>90&&mbb<150" && njetbins[ijet], met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(10, 0, 0.5, dR_jet_bquark_max_pt_ratio, "dR(b-quark, jet), jet with highest gen pT / quark pT ratio"), single_lep_alt&&nRealBs==2.&&nRealBsfromTop==2.&&"mbb>90&&mbb<150 && mct >200" && njetbins[ijet], met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");

      pm.Push<Hist1D>(Axis(10, 0, 0.5, dR_jet_bquark_max_pt_ratio, "dR(b-quark, jet), jet with highest gen pT / quark pT ratio"), single_lep_alt&&nRealBs==2.&&nRealBsfromTop==2.&&"mbb>90&&mbb<150" && max_genjet_bquark_pt_ratio>2. && njetbins[ijet], met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(10, 0, 0.5, dR_jet_bquark_max_pt_ratio, "dR(b-quark, jet), jet with highest gen pT / quark pT ratio"), single_lep_alt&&nRealBs==2.&&nRealBsfromTop==2.&&"mbb>90&&mbb<150 && mct >200" && max_genjet_bquark_pt_ratio>2. && njetbins[ijet], met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");



      pm.Push<Hist1D>(Axis(15, 0, 300., genmct, "gen m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(15, 0, 300., genmct, "gen m_{CT} [GeV]"), single_lep_alt&&nRealBs==2.&&"mbb>90&&mbb<150" && njetbins[ijet], met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      // pm.Push<Hist1D>(Axis(15, 0, 300., genmct, "gen m_{CT} [GeV]"), single_lep_alt && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      // pm.Push<Hist1D>(Axis(15, 0, 300., mct_genpt, "gen jet m_{CT} [GeV]"), single_lep_alt && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(15, 0, 300., mct_genpt, "gen jet m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(15, 0, 300., mct_genpt, "gen jet m_{CT} [GeV]"), single_lep_alt&&nRealBs==2.&&"mbb>90&&mbb<150" && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");


      pm.Push<Hist1D>(Axis(15, 0, 300., "mct", "m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(15, 0, 300., "mct", "m_{CT} [GeV]"), single_lep_alt&&nRealBs==2.&&"mbb>90&&mbb<150" && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      // pm.Push<Hist1D>(Axis(15, 0, 300., "mct", "m_{CT} [GeV]"), single_lep_alt && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");


// if(false){
      //Lepton binned

      pm.Push<Hist1D>(Axis(6, 0, 300., "mct", "m_{CT} [GeV]"), "ngoodbtags==2&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200"  , lep_sample_list, all_plot_types).Weight(full_weight).Tag("lepbin");
       pm.Push<Hist1D>(Axis(6, 0, 300., "mct", "m_{CT} [GeV]"), "ngoodbtags==2" && njetbins[ijet]  && "pfmet>125 && pfmet<200" , lep_sample_list, all_plot_types).Weight(full_weight).Tag("lepbin");
         
      pm.Push<Hist1D>(Axis(6, 0, 300., "mct", "m_{CT} [GeV]"), "ngoodbtags==2&&mbb>90&&mbb<150" && njetbins[ijet]  && "pfmet>200 && pfmet<300" , lep_sample_list, all_plot_types).Weight(full_weight).Tag("lepbin");
      pm.Push<Hist1D>(Axis(6, 0, 300., "mct", "m_{CT} [GeV]"), "ngoodbtags==2" && njetbins[ijet] && "pfmet>200 && pfmet<300", lep_sample_list, all_plot_types).Weight(full_weight).Tag("lepbin");
      
      pm.Push<Hist1D>(Axis(6, 0, 300., "mct", "m_{CT} [GeV]"), "ngoodbtags==2&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>300", lep_sample_list, all_plot_types).Weight(full_weight).Tag("lepbin");
      pm.Push<Hist1D>(Axis(6, 0, 300., "mct", "m_{CT} [GeV]"), "ngoodbtags==2" && njetbins[ijet] && "pfmet>300" , lep_sample_list, all_plot_types).Weight(full_weight).Tag("lepbin");
            

      pm.Push<Hist1D>(Axis(16,100,500., "pfmet", "MET [GeV]"), "mct>200&&ngoodbtags==2&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125"  , lep_sample_list, all_plot_types).Weight(full_weight).Tag("lepbin");
       pm.Push<Hist1D>(Axis(16,100,500, "pfmet", "MET [GeV]"), "mct>200&&ngoodbtags==2" && njetbins[ijet]  && "pfmet>125" , lep_sample_list, all_plot_types).Weight(full_weight).Tag("lepbin");
  
       ///

      pm.Push<Hist1D>(Axis(18,50,410, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>100&&mbb>50&&mbb<410" && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(18,50,410, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>100&&mbb>50&&mbb<410" && max_ak8pfjets_deepdisc_hbb<=0.8 && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(18,50,410, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>100&&mbb>50&&mbb<410" && max_ak8pfjets_deepdisc_hbb>0.8 && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");

      pm.Push<Hist1D>(Axis(18,50,410, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>100 && mct<200&&mbb>50&&mbb<410" && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(18,50,410, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>100&&mct<200&&mbb>50&&mbb<410" && max_ak8pfjets_deepdisc_hbb<=0.8 && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(18,50,410, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>100&&mct<200&&mbb>50&&mbb<410" && max_ak8pfjets_deepdisc_hbb>0.8 && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");

      pm.Push<Hist1D>(Axis(18,50,410, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>200&&mbb>50&&mbb<410" && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(18,50,410, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>200&&mbb>50&&mbb<410" && max_ak8pfjets_deepdisc_hbb<=0.8 && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");
      pm.Push<Hist1D>(Axis(18,50,410, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>200&&mbb>50&&mbb<410" && max_ak8pfjets_deepdisc_hbb>0.8 && njetbins[ijet] , met_sample_list, all_plot_types).Weight(full_weight).Tag("metbin");


          pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), single_lep_alt&&"mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("medmed");;
      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), single_lep_alt&&"mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("medmed");

      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), single_lep_alt&&"mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>300" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("medmed");


      pm.Push<Hist1D>(Axis(26,40,300, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>100 && mbb > 40 && mbb <300" && njetbins[ijet] && "pfmet>125 && pfmet<200" , simple_sample_list, all_plot_types).Weight(full_weight).Tag("medmed");;
      pm.Push<Hist1D>(Axis(26,40,300, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>100 && mbb > 40 && mbb <300" && njetbins[ijet] && "pfmet>200" , simple_sample_list, all_plot_types).Weight(full_weight).Tag("medmed");

      pm.Push<Hist1D>(Axis(26,40,300, "mbb", "m_{bb} [GeV]"), single_lep_alt&&"mct>100 && mbb > 40 && mbb <300" && njetbins[ijet] && "pfmet>300" , simple_sample_list, all_plot_types).Weight(full_weight).Tag("medmed");


      pm.Push<Hist1D>(Axis(15, 0, 300., "mct", "m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , simple_sample_list, all_plot_types).Weight(full_weight).Tag("simple_medmed");;
      pm.Push<Hist1D>(Axis(15, 0, 300., "mct", "m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , simple_sample_list, all_plot_types).Weight(full_weight).Tag("simple_medmed");

      pm.Push<Hist1D>(Axis(15, 0, 300., "mct", "m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>300" , simple_sample_list, all_plot_types).Weight(full_weight).Tag("simple_medmed");

            pm.Push<Hist1D>(Axis(15, 0, 300., "mct", "m_{CT} [GeV]"), single_lep_alt&&"mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>400" , simple_sample_list, all_plot_types).Weight(full_weight).Tag("simple_medmed");





      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), single_lep_alt&&"mct>100&&mbb>200" && njetbins[ijet] && "pfmet>125 && pfmet<200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("highmbb_medmed");;
      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), single_lep_alt&&"mct>100&&mbb>200" && njetbins[ijet] && "pfmet>200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("highmbb_medmed");


      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), single_lep_tightnothing && "mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("tightnothing");
      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), single_lep_tightnothing && "mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("tightnothing");


      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), single_lep_tighttight && "mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("tightight");
      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), single_lep_tighttight && "mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("tightight");


      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), dilep_medmed&&"mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("medmed");;
      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), dilep_medmed&&"mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("medmed");

      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), dilep_medmed&&"mct>100&&mbb>200" && njetbins[ijet] && "pfmet>125 && pfmet<200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("highmbb_medmed");;
      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), dilep_medmed&&"mct>100&&mbb>200" && njetbins[ijet] && "pfmet>200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("highmbb_medmed");

      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), dilep_tightnothing && "mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("tightnothing");
      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), dilep_tightnothing && "mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("tightnothing");


      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), dilep_tighttight && "mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>125 && pfmet<200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("tightight");
      pm.Push<Hist1D>(Axis(10, 100, 300., "mct", "m_{CT} [GeV]"), dilep_tighttight && "mct>100&&mbb>90&&mbb<150" && njetbins[ijet] && "pfmet>200" , real_bs_sample_list_short, all_plot_types).Weight(full_weight).Tag("tightight");

    }
  }

  // signal_table_cuts.push_back(TableRow("Signal Region, low MET", signalRegion_lowMET,0,0, full_weight));
  // signal_table_cuts.push_back(TableRow("Signal Region, med MET", signalRegion_medMET,0,0, full_weight));
  // signal_table_cuts.push_back(TableRow("Signal Region, high MET", signalRegion_highMET,0,0, full_weight)); 
  
  // mbbside_table_cuts.push_back(TableRow("mbb Sideband", mbbside,0,0, full_weight));
  // mbbside_table_cuts.push_back(TableRow("mbb Sideband, low MET", mbbside_lowMET,0,0, full_weight));
  // mbbside_table_cuts.push_back(TableRow("mbb Sideband, med MET", mbbside_medMET,0,0, full_weight));
  // mbbside_table_cuts.push_back(TableRow("mbb Sideband, high MET", mbbside_highMET,0,0, full_weight));
  
  // dilep_table_cuts.push_back(TableRow("dilep", dilep,0,0, full_weight));
  // dilep_table_cuts.push_back(TableRow("dilep, low MET", dilep_lowMET,0,0, full_weight));
  // dilep_table_cuts.push_back(TableRow("dilep, med MET", dilep_medMET,0,0, full_weight));
  // dilep_table_cuts.push_back(TableRow("dilep, high MET", dilep_highMET,0,0, full_weight));

  // lowMCT_table_cuts.push_back(TableRow("low mCT", lowMCT,0,0, full_weight));
  // lowMCT_table_cuts.push_back(TableRow("low mCT, low MET", lowMCT_lowMET,0,0, full_weight));
  // lowMCT_table_cuts.push_back(TableRow("low mCT, med MET", lowMCT_medMET,0,0, full_weight));
  // lowMCT_table_cuts.push_back(TableRow("low mCT, high MET", lowMCT_highMET,0,0, full_weight));


  // for(auto &ipr: categories) pm.Push<Table>("chart_PRESELECTION_"+ipr.first,  preselection_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_PRESELECTION_HIGH_MT_"+ipr.first,  preselection_highmT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_SIGNAL_"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_MBBSIDE_"+ipr.first,  mbbside_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_0B_"+ipr.first,  no_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_1B_"+ipr.first,  one_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_DILEP_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_LOW_MT_"+ipr.first,  lowMT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_LOW_MCT_"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_PRESELECTION_MEDMED_"+ipr.first,  preselection_medmed_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_SIGNAL_MEDMED_"+ipr.first,  signal_medmed_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_0B_MEDMED_"+ipr.first,  no_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_1B_MEDMED_"+ipr.first,  one_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories) pm.Push<Table>("chart_DILEP_MEDMED_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);

  for(auto &ipr: categories_Bs) pm.Push<Table>("chart_SIGNAL_"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  for(auto &ipr: categories_2realBs ) pm.Push<Table>("chart_SIGNAL_"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_Bs) pm.Push<Table>("chart_tightnothing_"+ipr.first,  tightveto_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_Bs) pm.Push<Table>("chart_highmbbL_"+ipr.first,  highmbb_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_Bs) pm.Push<Table>("chart_dilep_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_Bs) pm.Push<Table>("chart_dilep_tighttight_"+ipr.first,  dilep_tighttight_table_cuts, ipr.second, true, true, true, false);
 

  // for(auto &ipr: categories_Bs) pm.Push<Table>("chart_MBBSIDE_"+ipr.first,  mbbside_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_Bs) pm.Push<Table>("chart_DILEP_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_Bs) pm.Push<Table>("chart_LOW_MCT_"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);

 // for(auto &ipr: categories_lost_Bs) pm.Push<Table>("chart_SIGNAL_lostb"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
 


  // for(auto &ipr: categories_lost_Bs) pm.Push<Table>("chart_LOW_MCT_lostb"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);

  // for(auto &ipr: categories_lost_Bs_tt) pm.Push<Table>("chart_SIGNAL_lostb_tt"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_lost_Bs_tt) pm.Push<Table>("chart_LOW_MCT_lostb_tt"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);


  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_PRESELECTION_"+ipr.first,  preselection_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_PRESELECTION_HIGH_MT_"+ipr.first,  preselection_highmT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_SIGNAL_"+ipr.first,  signal_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_MBBSIDE_"+ipr.first,  mbbside_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_0B_"+ipr.first,  no_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_1B_"+ipr.first,  one_b_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_DILEP_"+ipr.first,  dilep_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_LOW_MT_"+ipr.first,  lowMT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_LOW_MCT_"+ipr.first,  lowMCT_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_PRESELECTION_MEDMED_"+ipr.first,  preselection_medmed_table_cuts, ipr.second, true, true, true, false);
  // for(auto &ipr: categories_split_Bs) pm.Push<Table>("chart_SIGNAL_MEDMED_"+ipr.first,  signal_medmed_table_cuts, ipr.second, true, true, true, false);

 Table & signal_chart = pm.Push<Table>("signal_chart", signal_table_cuts, real_bs_sample_list, false); 
 Table & signal_chart_2realb = pm.Push<Table>("signal_chart_2realb", signal_table_cuts, two_real_bs_sample_list, false); 


  // Table & signal_chart_lostb = pm.Push<Table>("signal_chart_lostb", signal_table_cuts, categories_lost_Bs["LostBs"], false);
  


  // Table & signal_chart_lostb = pm.Push<Table>("signal_chart_lostb", signal_table_cuts, lost_b_sample_list, false);
  // Table & signal_chart_lostb_tt = pm.Push<Table>("signal_chart_lostb_tt", signal_table_cuts, categories_lost_Bs_tt["LostBs_tt"], false);
  // Table & signal_chart_lostb_tt = pm.Push<Table>("signal_chart_lostb_tt", signal_table_cuts, lost_b_sample_list_ttbar, false);
  // Table & mbbside_chart = pm.Push<Table>("mbbside_chart", mbbside_table_cuts, sample_list, false);
  // Table & dilep_chart = pm.Push<Table>("dilep_chart", dilep_table_cuts, sample_list, false);
  // Table & lowmCT_chart = pm.Push<Table>("lowmCT_chart", lowMCT_table_cuts, real_bs_sample_list, false);
  // Table & lowmCT_chart_lostb = pm.Push<Table>("lowmCT_chart_lostb", lowMCT_table_cuts, lost_b_sample_list, false);
  // Table & lowmCT_chart_lostb_tt = pm.Push<Table>("lowmCT_chart_lostb_tt", lowMCT_table_cuts, lost_b_sample_list_ttbar, false);

  // Table & real_b_preselection_chart = pm.Push<Table>("real_b_preselection_chart", preselection_table_cuts, real_bs_sample_list, false);
  // Table & real_b_preselection_highmT_chart = pm.Push<Table>("real_b_preselection_highmT_chart", preselection_highmT_table_cuts, real_bs_sample_list, false);
  // Table & real_b_signal_chart = pm.Push<Table>("real_b_signal_chart", signal_table_cuts, real_bs_sample_list, false);
  // Table & real_b_mbbside_chart = pm.Push<Table>("real_b_mbbside_chart", mbbside_table_cuts, real_bs_sample_list, false);
  // Table & real_b_no_b_chart = pm.Push<Table>("real_b_no_b_chart", no_b_table_cuts, real_bs_sample_list, false);
  // Table & real_b_one_b_chart = pm.Push<Table>("real_b_one_b_chart", one_b_table_cuts, real_bs_sample_list, false);
  // Table & real_b_dilep_chart = pm.Push<Table>("real_b_dilep_chart", dilep_table_cuts, real_bs_sample_list, false);
  // Table & real_b_lowmT_chart = pm.Push<Table>("real_b_lowmT_chart", lowMT_table_cuts, real_bs_sample_list, false);
  // Table & real_b_lowmCT_chart = pm.Push<Table>("real_b_lowmCT_chart", lowMCT_table_cuts, real_bs_sample_list, false);
  // Table & real_b_preselection_medmed_chart = pm.Push<Table>("real_b_preselection_medmed_chart", preselection_medmed_table_cuts, real_bs_sample_list, false);
  // Table & real_b_signal_medmed_chart = pm.Push<Table>("real_b_signal_medmed_chart", signal_medmed_table_cuts, real_bs_sample_list, false);

  // Table & split_b_preselection_chart = pm.Push<Table>("split_b_preselection_chart", preselection_table_cuts, b_full_sample_list, false);
  // Table & split_b_preselection_highmT_chart = pm.Push<Table>("split_b_preselection_highmT_chart", preselection_highmT_table_cuts, b_full_sample_list, false);
  // Table & split_b_signal_chart = pm.Push<Table>("split_b_signal_chart", signal_table_cuts, b_full_sample_list, false);
  // Table & split_b_mbbside_chart = pm.Push<Table>("split_b_mbbside_chart", mbbside_table_cuts, b_full_sample_list, false);
  // Table & split_b_no_b_chart = pm.Push<Table>("split_b_no_b_chart", no_b_table_cuts, b_full_sample_list, false);
  // Table & split_b_one_b_chart = pm.Push<Table>("split_b_one_b_chart", one_b_table_cuts, b_full_sample_list, false);
  // Table & split_b_dilep_chart = pm.Push<Table>("split_b_dilep_chart", dilep_table_cuts, b_full_sample_list, false);
  // Table & split_b_lowmT_chart = pm.Push<Table>("split_b_lowmT_chart", lowMT_table_cuts, b_full_sample_list, false);
  // Table & split_b_lowmCT_chart = pm.Push<Table>("split_b_lowmCT_chart", lowMCT_table_cuts, b_full_sample_list, false);
  // Table & split_b_preselection_medmed_chart = pm.Push<Table>("split_b_preselection_medmed_chart", preselection_medmed_table_cuts, b_full_sample_list, false);
  // Table & split_b_signal_medmed_chart = pm.Push<Table>("split_b_signal_medmed_chart", signal_medmed_table_cuts, b_full_sample_list, false);

  pm.min_print_ = true;
  pm.MakePlots(lumi);

  cout<<"Finished making plots."<<endl;
  vector<GammaParams> yields_signal = signal_chart.BackgroundYield(lumi);
  for(const auto &yield: yields_signal){
    cout << yield << endl;
  }
  vector<GammaParams> yields_signal_2realb = signal_chart_2realb.BackgroundYield(lumi);
  for(const auto &yield: yields_signal_2realb){
    cout << yield << endl;
  }

  //   vector<GammaParams> yields_signal_lb = signal_chart_lostb.BackgroundYield(lumi);
  // for(const auto &yield: yields_signal_lb){
  //   cout << yield << endl;
  // }

  // vector<GammaParams> yields_signal_lb_tt = signal_chart_lostb_tt.BackgroundYield(lumi);
  // for(const auto &yield: yields_signal_lb_tt){
  //   cout << yield << endl;
  // }
  // vector<GammaParams> yields_mbbside = mbbside_chart.BackgroundYield(lumi);
  // for(const auto &yield: yields_mbbside){
  //   cout << yield << endl;
  // }


  // vector<GammaParams> yields_dilep = dilep_chart.BackgroundYield(lumi);
  // for(const auto &yield: yields_dilep){
  //   cout << yield << endl;
  // }


  // vector<GammaParams> yields_lowmCT = lowmCT_chart.BackgroundYield(lumi);
  // for(const auto &yield: yields_lowmCT){
  //   cout << yield << endl;
  // }

  //   vector<GammaParams> yields_lowmCT_lb = lowmCT_chart_lostb.BackgroundYield(lumi);
  // for(const auto &yield: yields_lowmCT_lb){
  //   cout << yield << endl;
  // }


  //   vector<GammaParams> yields_lowmCT_lb_tt = lowmCT_chart_lostb_tt.BackgroundYield(lumi);
  // for(const auto &yield: yields_lowmCT_lb_tt){
  //   cout << yield << endl;
  // }


}
