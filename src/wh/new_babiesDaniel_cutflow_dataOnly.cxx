
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <unistd.h>
#include <getopt.h>

#include "TError.h"
#include "TColor.h"

#include "core/baby.hpp"
#include "core/process.hpp"
#include "core/named_func.hpp"
#include "core/plot_maker.hpp"
#include "core/plot_opt.hpp"
#include "core/palette.hpp"
#include "core/table.hpp"
#include "core/event_scan.hpp"
#include "core/hist1d.hpp"
#include "core/hist2d.hpp"
#include "core/utilities.hpp"
#include "core/functions.hpp"
#include "core/wh_functions.hpp"

using namespace std;
using namespace PlotOptTypes;
using namespace WH_Functions;

namespace{
  bool single_thread = false;
}


int main(){
  gErrorIgnoreLevel = 6000;

  // luminosity
  double lumi = 137.2;

  // paths, new babies
  string data2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/"; // slim_data_2016*.root
  string data2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/"; // slim_data_2017*.root
  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/"; // slim_data_2018*.root
  Palette colors("txt/colors.txt", "default");

  // Columns
  // names | type | color | file 

  //Data FIX THIS
  auto data2016 = Process::MakeShared<Baby_full>("2016 Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*met*.root",data2016_dir+"slim_data_2016*singleel*.root",data2016_dir+"slim_data_2016*singlemu*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);
  auto data2017 = Process::MakeShared<Baby_full>("2017 Data", Process::Type::data, colors("data"),{data2017_dir+"slim_data_2017*met*.root",data2017_dir+"slim_data_2017*singleel*.root",data2017_dir+"slim_data_2017*singlemu*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);
  auto data2018 = Process::MakeShared<Baby_full>("2018 Data", Process::Type::data, colors("data"),{data2018_dir+"slim_data_2018*met*.root",data2018_dir+"slim_data_2018*egamma*.root",data2018_dir+"slim_data_2018*singlemu*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);
  auto dataComb = Process::MakeShared<Baby_full>("Combined Data", Process::Type::data, colors("data"),{data2016_dir+"slim_data_2016*met*.root",data2017_dir+"slim_data_2017*met*.root",data2018_dir+"slim_data_2018*met*.root",data2016_dir+"slim_data_2016*singleel*.root",data2017_dir+"slim_data_2017*singleel*.root",data2018_dir+"slim_data_2018*egamma*.root",data2016_dir+"slim_data_2016*singlemu*.root",data2017_dir+"slim_data_2017*singlemu*.root",data2018_dir+"slim_data_2018*singlemu*.root"},"pass&&(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)"&&HasHEMevent==0.&&HasHEMjet==0.);

  vector<shared_ptr<Process> > sample_list = {data2016, data2017, data2018, dataComb};

  // To make tables and plots
  PlotMaker pm;

  // Making tables. Adding rows. 
  Table & cutflow_data_only = pm.Push<Table>("cutflow_data_only", vector<TableRow>{
      TableRow("pass", "pass"),
	TableRow("pass, hasNano$>$0", "pass&&hasNano>0")
	}, sample_list, false);

  if(single_thread) pm.multithreaded_ = false;
  pm.MakePlots(lumi);

  vector<GammaParams> yields = cutflow_data_only.BackgroundYield(lumi);
  for(const auto &yield: yields) {
    cout << "yield: " << yield << endl;
  }
}
