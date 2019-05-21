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
const NamedFunc NHighPtNu("NHighPtNu",[](const Baby &b) -> NamedFunc::ScalarType{
      int nnu=0;
        for (unsigned i(0); i<b.gen_pt()->size(); i++){
        if (abs(b.gen_motherid()->at(i))==24 && ( abs(b.gen_id()->at(i)) == 12 || abs(b.gen_id()->at(i)) == 14 || abs(b.gen_id()->at(i)) == 16) && b.gen_pt()->at(i) > 200 ) nnu++;
      }
      return nnu;
    });

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

  vector<shared_ptr<Process> > sample_list ={tt2l,tt1l};
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

  //Preselection
  NamedFunc preselection = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=2&&pfmet>125&&mt_met_lep>50"&& WHLeptons==1 && HasMedLooseCSV;
  NamedFunc preselection_highmT = "nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets>=2&&pfmet>125&&mt_met_lep>150"&& WHLeptons==1 && HasMedLooseCSV;

  map<string, vector<shared_ptr<Process> > > categories;

  categories["this_is_the_tag"] = vector<shared_ptr<Process> >();
  categories["this_is_the_tag"].push_back(tt2l);
  categories["this_is_the_tag"].push_back(tt1l);

  vector<TableRow> table_cuts;
  
  table_cuts.push_back(TableRow("$1$","1", 0,0,"w_noBtagSF*w_BtagSF_WH"));  
  table_cuts.push_back(TableRow("Preselection", preselection,0,0,"w_noBtagSF*w_BtagSF_WH"));  
  table_cuts.push_back(TableRow("Preselection, high mT", preselection_highmT,0,0,"w_noBtagSF*w_BtagSF_WH"));  

  for(auto &ipr: categories) pm.Push<Table>("chart_"+ipr.first,  table_cuts, ipr.second, true, true, true, true);


  pm.MakePlots(lumi);

}
