///// plot_ratios: plots rMJ and rmT, and combinations of these

#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>  // setw
#include <chrono>

#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "TError.h" // Controls error level reporting
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"
#include "core/utilities.hpp"
#include "core/baby.hpp"
#include "core/process.hpp"
#include "core/named_func.hpp"
#include "core/plot_maker.hpp"
#include "core/palette.hpp"
#include "core/table.hpp"
#include "core/styles.hpp"
#include "core/plot_opt.hpp"
#include "core/functions.hpp"
#include "core/wh_functions.hpp"

using namespace std;
using namespace WH_Functions;

namespace{
  //only one of these three can be true:
  bool systematic_mode=true;
  bool year_mode = false;
  bool data_mode=false;

  bool debug = true;
  float lumi=137.;

  enum Bkgs {bkg = 0, top = 1, w = 2, other = 3, data = 4};

  struct oneplot{
    bool blind;
    TString name;
    NamedFunc baseline;
    vector<NamedFunc> bincuts;
    vector<NamedFunc> denSel;
    vector<NamedFunc> numSel;
  };
}

NamedFunc offshellw("offshellw",[](const Baby &b) -> NamedFunc::ScalarType{
    for (unsigned i(0); i<b.mc_pt()->size(); i++){
      if (abs(b.mc_id()->at(i))!=24) continue;
      if (b.mc_mass()->at(i) > 140.) {
        return 1;
      }
    }
    return 0;
  });

void plotRatio(vector<vector<vector<GammaParams> > > &allyields, oneplot &plotdef,
	       vector<vector<vector<int> > > &indices, vector<TString> &leglabels,bool systematic_mode,TString tag=""){

  gStyle->SetGridStyle(3);
  gStyle->SetGridColor(14);
  size_t ngraphs = indices.size();
  size_t nbins = allyields[0][0].size();

  //// Finding all ratios for all graphs
  float val(1.), valup(1.), valdown(1.);
  vector<vector<vector<float> > > ratios(ngraphs);
  float maxr=-1., minr=1e6;
  for(size_t igraph=0; igraph<ngraphs; igraph++){
    // Finding powers to calculate ratio
    vector<float> powers;
    for(size_t ipow=0; ipow<indices[igraph].size(); ipow++) powers.push_back(indices[igraph][ipow][2]);

    // Finding ratios for each bin
    for(size_t ibin=0; ibin<nbins; ibin++){
      vector<vector<float> > entries;
      vector<vector<float> > weights;
      for(size_t ind=0; ind<indices[igraph].size(); ind++) {
	size_t ibkg = indices[igraph][ind][0];
	size_t iabcd = indices[igraph][ind][1];
      // cout<<"igraph ind ibin ibkg iabcd: " <<igraph<<" "<<ind<<" "<<ibin<<" "<<ibkg<<" "<<iabcd<<endl;
        entries.push_back(vector<float>());
        weights.push_back(vector<float>());
        entries.back().push_back(allyields[ibkg][iabcd][ibin].NEffective());
        weights.back().push_back(allyields[ibkg][iabcd][ibin].Weight());
      } // Loop over indices
      // cout<<"finished NEffective"<<endl;
      // Throwing toys to find ratios and uncertainties
      //if(igraph!=0)
      val = calcKappa(entries, weights, powers, valdown, valup);
     // else val = calcKappa(entries, weights, powers, valdown, valup,true); for data fluctuations. 
      if(valdown<0) valdown = 0;
      ratios[igraph].push_back(vector<float>({val, valdown, valup}));
      if(maxr < val+valup) maxr = val+valup;
      if(minr > val-valdown) minr = val-valdown;
    } // Loop over bins
  } // Loop over graphs

  //// Finding ytitle
  bool double_mode = false;
  TString ytitle="R(m_{CT})";
  if(indices[2].size()==4) {ytitle = "R(m_{CT}) Variation / Nominal"; double_mode=true;}
  // if(indices[0].size()==2){
  //   size_t ind0=indices[0][0][1], ind1=indices[0][1][1];
  //   if((ind0==r3&&ind1==r1) || (ind0==r4&&ind1==r2)) ytitle = "R(m_{T})";
  //   if((ind0==r4&&ind1==r3) || (ind0==r2&&ind1==r1)) ytitle = "R(M_{J})";
  // }
  // if(indices[0].size()==4){
  //   size_t ind0=indices[0][0][1], ind1=indices[0][1][1];
  //   size_t ind2=indices[0][2][1], ind3=indices[0][3][1];
  //   if((ind0==r4&&ind1==r3&&ind2==r2&&ind3==r1)) ytitle = "R(M_{J}^{high}) / R[M_{J}^{low}(bkg)]";
  // }
  //// Setting plot style
  PlotOpt opts("txt/plot_styles.txt", "Ratio");
  setPlotStyle(opts);

  //// Plotting kappas
  TCanvas can("can","");
  can.SetGridy();
  // can.SetGridx();
  TLine line; line.SetLineWidth(2); line.SetLineStyle(2);
  TLatex label; label.SetTextSize(0.05); label.SetTextFont(42); label.SetTextAlign(23);

  float minx = 0.5, maxx = nbins+0.5, miny = minr*0.55, maxy = maxr*1.45;
  miny=0;
  if(maxy>5) maxy = 5;
  TH1D histo("histo", "", nbins, minx, maxx);
  histo.SetMinimum(miny);
  histo.SetMaximum(maxy);
  histo.GetYaxis()->CenterTitle(true);
  histo.GetXaxis()->SetLabelOffset(0.008);
  histo.SetYTitle(ytitle);
  histo.Draw();

  //// Filling vx, vy vectors with kappa coordinates. Each nb cut is stored in a TGraphAsymmetricErrors
  vector<vector<double> > vx(ngraphs), vexh(ngraphs), vexl(ngraphs);
  vector<vector<double> > vy(ngraphs), veyh(ngraphs), veyl(ngraphs);
  for(size_t ibin=0; ibin<nbins; ibin++){
    //histo.GetXaxis()->SetBinLabel(ibin+1, CodeToRootTex(plotdef.bincuts[ibin].Data()).c_str());
    histo.GetXaxis()->SetBinLabel(ibin+1, CodeToRootTex(plotdef.bincuts[ibin].Name()).c_str());
    // xval is the x position of the first marker in the group
    double xval = ibin+1, minxb = 0.15, binw = 0;
    if(data_mode) minxb=0.08;
    // If there is more than one point in the group, it starts minxb to the left of the center of the bin
    // binw is the distance between points in the ngoodjets group
    if(ngraphs>1) {
      xval -= minxb;
      binw = 2*minxb/(ngraphs-1);
    }
    for(size_t igraph=0; igraph<ngraphs; igraph++){
      vx[igraph].push_back(xval);
      xval += binw;
      vexl[igraph].push_back(0);
      vexh[igraph].push_back(0);
      if(systematic_mode && double_mode && igraph==0){
        vy[igraph].push_back(1);
      }
      else vy[igraph]  .push_back(ratios[igraph][ibin][0]);
      if(igraph>0 && systematic_mode){
        veyl[igraph].push_back(0);
        veyh[igraph].push_back(0);
      }
      else{
         if(systematic_mode && double_mode && igraph==0){
            veyl[igraph].push_back(ratios[igraph][ibin][1]/ratios[igraph][ibin][0]);
            veyh[igraph].push_back(ratios[igraph][ibin][2]/ratios[igraph][ibin][0]);
         }
         else{
            veyl[igraph].push_back(ratios[igraph][ibin][1]);
            veyh[igraph].push_back(ratios[igraph][ibin][2]);
        }
    }
    } // Loop over TGraphs
  } // Loop over bin cuts

  //// Drawing legend and TGraphs
  double legX(opts.LeftMargin()+0.023), legY(1-opts.TopMargin()-0.03), legSingle = 0.055;
  double legW = 0.73/*0.19*ngraphs*/, legH = legSingle*(ngraphs+1)/2;
  if(year_mode || data_mode) {legW =0.36; legH = legSingle*(ngraphs+1);}

  TLegend leg(legX, legY-legH, legX+legW, legY);
//  leg.SetTextSize(opts.LegendEntryHeight()); 
  leg.SetFillColor(0);
  // leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(42);
  leg.SetNColumns(2/*ngraphs*/);

  Palette colors("txt/colors.txt", "default");
  //vector<int> mcolors({kRed, kGreen+1, 4, kMagenta+2});
  vector<int> mcolors({kGray+2,kRed,kRed, kGreen-3,kGreen-3, kCyan-3,kCyan-3, kMagenta+2,kMagenta+2,kOrange+2,kOrange+2,kBlue+2,kBlue+2,kGray,kGray});
  vector<int> styles({20, 22, 23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23});
  if(data_mode){
    mcolors = {1,kRed+1};
    styles= {20,20};
    leg.SetNColumns(1);
  }
  if(year_mode){
    mcolors = {kGray+3,kRed,kGreen-3,kMagenta+2,kCyan-3};
    styles= {20,20,20,20};
    leg.SetNColumns(1);
  }
  TGraphAsymmErrors graph[20]; // There's problems with vectors of TGraphs, so using an array
  for(size_t igraph=0; igraph<ngraphs; igraph++){
    graph[igraph] = TGraphAsymmErrors(vx[igraph].size(), &(vx[igraph][0]), &(vy[igraph][0]),
                                    &(vexl[igraph][0]), &(vexh[igraph][0]), &(veyl[igraph][0]), &(veyh[igraph][0]));
    graph[igraph].SetMarkerStyle(styles[igraph]); graph[igraph].SetMarkerSize(1.4);
    if(leglabels[igraph]=="All bkg.") mcolors[igraph] = 1;
    if(leglabels[igraph]=="t#bar{t} (2l)") mcolors[igraph] = colors("tt_2l");
    if(leglabels[igraph]=="t#bar{t} (1l)") mcolors[igraph] = colors("tt_1l");
    if(leglabels[igraph]=="Other") mcolors[igraph] = colors("wjets");
    graph[igraph].SetMarkerColor(mcolors[igraph]);
    graph[igraph].SetLineColor(mcolors[igraph]); graph[igraph].SetLineWidth(2);
    graph[igraph].Draw("p0 same");

    if(!double_mode || igraph>0) leg.AddEntry(&graph[igraph], leglabels[igraph], "p");
    else leg.AddEntry(&graph[igraph], "MC statistics", "lp");
    if(igraph==0 && systematic_mode) leg.AddEntry(&graph[igraph]," ","");
  } // Loop over TGraphs
  leg.Draw();

  //// Drawing CMS labels and line at 1
  TLatex cmslabel;
  cmslabel.SetTextSize(0.06);
  cmslabel.SetNDC(kTRUE);
  cmslabel.SetTextAlign(11);
  if(! data_mode) cmslabel.DrawLatex(opts.LeftMargin()+0.005, 1-opts.TopMargin()+0.015,"#font[62]{CMS} #scale[0.8]{#font[52]{Simulation}}");
  else cmslabel.DrawLatex(opts.LeftMargin()+0.005, 1-opts.TopMargin()+0.015,"#font[62]{CMS} #scale[0.8]{#font[52]{Preliminary}}");
  cmslabel.SetTextAlign(31);
  if(! data_mode) cmslabel.DrawLatex(1-opts.RightMargin()-0.005, 1-opts.TopMargin()+0.015,"#font[42]{13 TeV}");
  else cmslabel.DrawLatex(1-opts.RightMargin()-0.005, 1-opts.TopMargin()+0.015,"#font[42]{137 fb^{-1} (13 TeV)}");

  line.SetLineStyle(3); line.SetLineWidth(1);
  line.DrawLine(minx, 1, maxx, 1);
  string syst_tag = "";
  if(systematic_mode) syst_tag = "syst_";
  string fname="plots/ratio_"+syst_tag+CodeToPlainText(tag.Data())+"_"+CodeToPlainText(ytitle.Data())+"_"+plotdef.name.Data()+"_"+CodeToPlainText(plotdef.baseline.Name())+".pdf";
  can.SaveAs(fname.c_str());
  cout<<endl<<" open "<<fname<<endl;

  //Printing graphs as tables
  fname="tables/ratio_"+syst_tag+CodeToPlainText(tag.Data())+"_"+CodeToPlainText(ytitle.Data())+"_"+plotdef.name.Data()+"_"+CodeToPlainText(plotdef.baseline.Name())+".txt";
  ofstream outtable;
  outtable.open(fname);
  unsigned wname(35), wdist(2), wbin(30);
  outtable<<left<<setw(wname)<<" ";
  for(size_t ibin=0; ibin<nbins; ibin++) outtable<<left<<setw(wbin)<<CodeToRootTex(plotdef.bincuts[ibin].Name()).c_str();
  outtable<<endl;
  for(size_t igraph=0; igraph<ngraphs; igraph++){
    outtable<<left<<setw(wname)<<leglabels[igraph]<<setw(wdist)<<" ";
    for(size_t ibin=0; ibin<nbins; ibin++){
      if(!tag.Contains("double")) outtable<<left<<setw(wbin)<<RoundNumber(vy[igraph][ibin],4)+" -"+RoundNumber(veyl[igraph][ibin],3)+" +"+RoundNumber(veyh[igraph][ibin],3);
      else outtable<<left<<setw(wbin)<<RoundNumber(100.*(vy[igraph][ibin]-1.),1)+"%";
    }
    outtable<<endl;
  }

} // plotRatio


void printDebug(vector<vector<NamedFunc> > &allcuts, vector<vector<vector<GammaParams> > > &allyields, 
		TString baseline);

void GetOptions(int argc, char *argv[]);

string ReplaceInCut(string str, const string& from, const string& to) {
size_t start_pos = 0;
while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
return str;
}
string JMEvariation(string cut, string variation){
    vector<string> listOfJetVars, listOfMETVars;
    listOfJetVars = {"mct", "mbb", "ngoodjets", "ngoodbtags"};
for (size_t ibin=0; ibin<listOfJetVars.size(); ibin++){
        cut = ReplaceInCut(cut, listOfJetVars[ibin], variation+'_'+listOfJetVars[ibin]);
        //std::cout << listOfJetVars[ibin] << std::endl;
    }
    listOfMETVars = {"pfmet", "mt_met_lep"};
for (size_t ibin=0; ibin<listOfMETVars.size(); ibin++){
        cut = ReplaceInCut(cut, listOfMETVars[ibin], listOfMETVars[ibin]+'_'+variation);
        //std::cout << listOfMETVars[ibin] << std::endl;
    }
    std::cout << cut << std::endl;
return cut;
}

string METvariation(string cut, string variation){
    vector<string> listOfMETVars;
    listOfMETVars = {"pfmet", "mt_met_lep"};
for (size_t ibin=0; ibin<listOfMETVars.size(); ibin++){
        cut = ReplaceInCut(cut, listOfMETVars[ibin], listOfMETVars[ibin]+'_'+variation);
       // std::cout << listOfMETVars[ibin] << std::endl;
    }
    std::cout << cut << std::endl;
return cut;
}

string JERvariation(string cut, string variation){
    vector<string> listOfJetVars, listOfMETVars;
    listOfJetVars = {"mct", "mbb"};
for (size_t ibin=0; ibin<listOfJetVars.size(); ibin++){
        cut = ReplaceInCut(cut, listOfJetVars[ibin], variation+'_'+listOfJetVars[ibin]);
        //::cout << listOfJetVars[ibin] << std::endl;
    }
    std::cout << cut << std::endl;
return cut;
}


int main(int argc, char *argv[]){
  gErrorIgnoreLevel=6000; // Turns off ROOT errors due to missing branches
  GetOptions(argc, argv);

  chrono::high_resolution_clock::time_point begTime;
  begTime = chrono::high_resolution_clock::now();

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////// Defining processes //////////////////////////////////////////


  Palette colors("txt/colors.txt", "default");

  // Cuts in baseline speed up the yield finding
  string baseline="";
  NamedFunc baselinef = "hasNano&& pass&&nvetoleps>=1&&ngoodjets>=2 && ngoodjets<=3 &&pfmet>125&&mt_met_lep>150 && ngoodbtags>=1 && nWHLeptons>=1";

  if (systematic_mode) baselinef = "pass&&nvetoleps>=1&& nWHLeptons>=1" &&pass_any_njets_variation && pass_any_nb_variation && pass_any_mt_variation && pass_any_met_variation;
  
  TString single_lep ="nvetoleps==1&&PassTrackVeto&&PassTauVeto&&nWHLeptons==1 && pfmet>125 && mt_met_lep>150";
  TString dilep="nvetoleps==2 && pfmet>125 && mt_met_lep>150";

  // string data2016_dir = "/home/users/rheller/wh_babies/babies_v31_1_2019_10_05/";
  // string data2017_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_05/";
  // string data2018_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_05/";


  // string mc2016_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/s16v3/";
  // string mc2016_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/s16v3/";

  // string mc2017_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/f17v2/";
  // string mc2017_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/f17v2/";

  // string mc2018_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/a18v1/";
  // string mc2018_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_07/a18v1/";

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


  auto all_top = {mc2016_dir+"slim_*TTJets_1lep_top_s16v3*.root",mc2016_dir+"slim_*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"slim_*TTJets_1lep_*met150*.root",mc2017_dir+"slim_*TTJets_1lep_top_f17v2*.root",mc2017_dir+"slim_*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"slim*TTJets_1lep_*met150*.root",mc2018_dir+"slim_*TTJets_1lep_top_a18v1*.root",mc2018_dir+"slim_*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"slim*TTJets_1lep_*met80*.root",mc2016_dir+"slim_*ST_*.root",mc2017_dir+"slim_*ST_*.root",mc2018_dir+"slim_*ST_*.root",mc2016_dir+"slim_*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"slim*TTJets_2lep_*met150*.root",mc2017_dir+"slim_*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"slim*TTJets_2lep_*met150*.root",mc2018_dir+"slim_*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet_2l+"slim_TTJets_2lep_met80_a18v1*.root"};


  auto all_other = {mc2016_dir+"slim_*WW*.root", mc2016_dir+"slim_*WZ*.root",mc2016_dir+"slim_*ZZ*.root",mc2017_dir+"slim_*WW*.root", mc2017_dir+"slim_*WZ*.root",mc2017_dir+"slim_*ZZ*.root",mc2018_dir+"slim_*WW*.root", mc2018_dir+"slim_*WZ*.root",mc2018_dir+"slim_*ZZ*.root",mc2016_dir+"slim_*TTWJets*.root", mc2016_dir+"slim_*TTZ*.root",mc2017_dir+"slim_*TTWJets*.root", mc2017_dir+"slim_*TTZ*.root",mc2018_dir+"slim_*TTWJets*.root", mc2018_dir+"slim_*TTZ*.root"};

  //// Contributions
  auto proc_wjets = Process::MakeShared<Baby_full>("W+jets 2016-2018", Process::Type::background, kCyan-3, {mc2016_dir+"slim_W*JetsToLNu_s16v3*",mc2016_dir+"slim_*W*Jets_NuPt200_s16v*.root",mc2017_dir+"slim_W*JetsToLNu_f17v2*",mc2017_dir+"slim_*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim_W*Jets_NuPt200_a18v1*.root"},"stitch&&evt!=74125994"&&baselinef);
  auto proc_top = Process::MakeShared<Baby_full>("top 2016-2018", Process::Type::background, kRed,all_top,"stitch"&&baselinef);
  auto proc_other = Process::MakeShared<Baby_full>("TTV and VV 2016-2018", Process::Type::background, kRed,all_other,baselinef);

  auto proc_data =  Process::MakeShared<Baby_full>("Data", Process::Type::data, colors("data"),{data2016_dir+"slim*data_2016*singleel*.root",data2016_dir+"slim*data_2016*singlemu*.root",data2016_dir+"slim*data_2016*met*.root",data2017_dir+"slim*data_2017*singleel*.root",data2017_dir+"slim*data_2017*singlemu*.root",data2017_dir+"slim*data_2017*met*.root",data2018_dir+"slim*data_2018*singlemu*.root",data2018_dir+"slim*data_2018*met*.root",data2018_dir+"slim*data_2018*egamma*.root"},baselinef&&"(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  //Extra primary datasets
  // auto proc_data =  Process::MakeShared<Baby_full>("Data", Process::Type::data, colors("data"),{data2016_dir+"slim*data_2016*.root",data2017_dir+"slim*data_2017*.root",data2018_dir+"slim*data_2018*.root"},baselinef&&"(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  vector<shared_ptr<Process> > all_procs;
  if(!data_mode) all_procs = {proc_top,proc_wjets,proc_other};
  else  all_procs = {proc_data,proc_top,proc_wjets,proc_other};

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////// Defining cuts ///////////////////////////////////////////////
  // baseline defined above

  TString denominator_lowmet = single_lep+"&&mct>100&&mct<=200&&mbb>90&&mbb<150";
  TString denominator_highmet = single_lep+"&&mct>100&&mct<=200&&mbb>90&&mbb<300";
  TString denominator_boosted_lowmet = single_lep+"&&mct<=200&&mbb>90&&mbb<150";
  TString denominator_boosted_highmet = single_lep+"&&mct<=200&&mbb>90&&mbb<300";
  TString numerator = single_lep+"&&mct>200&&mbb>90&&mbb<150";

  TString denominator_highmbb = single_lep+"&&mct>100&&mct<=200&&mbb>150"; //Lowered from mbb 200
  TString numerator_highmbb = single_lep+"&&mct>200&&mbb>150";
  // TString denominator_highmbb_boosted = single_lep+"&&mct>100&&mct<=200&&mbb>200";
  TString denominator_highmbb_boosted = single_lep+"&&mct<=200&&mbb>150";
  TString numerator_highmbb_boosted = single_lep+"&&mct>200&&mbb>150";

  TString denominator_dilep = dilep+"&&mct>100&&mct<=200&&mbb>90&&mbb<150";
  TString denominator_dilep_highmet = dilep+"&&mct>100&&mct<=200&&mbb>90&&mbb<300";
  TString numerator_dilep = dilep+"&&mct>200&&mbb>90&&mbb<150";
  TString denominator_dilep_boosted = dilep+"&&mct>100&&mct<=200&&mbb>90&&mbb<150";
  TString denominator_dilep_boosted_highmet = dilep+"&&mct>100&&mct<=200&&mbb>90&&mbb<300";
  TString numerator_dilep_boosted = dilep+"&&mct>200&&mbb>90&&mbb<150";
 


  ////JER/JEC variations
  TString denominator_lowmet_jup = JMEvariation(denominator_lowmet.Data(), "jup");
  TString denominator_lowmet_jdown = JMEvariation(denominator_lowmet.Data(), "jdown");
  TString denominator_lowmet_resdown = METvariation(denominator_lowmet.Data(), "resdown");
  TString denominator_lowmet_resup = METvariation(denominator_lowmet.Data(), "resup");
  TString denominator_lowmet_jer = JERvariation(denominator_lowmet.Data(), "jer");
  TString denominator_lowmet_jercdown = JERvariation(denominator_lowmet.Data(), "jercdown");

  TString denominator_highmet_jup = JMEvariation(denominator_highmet.Data(), "jup");
  TString denominator_highmet_jdown = JMEvariation(denominator_highmet.Data(), "jdown");
  TString denominator_highmet_resdown = METvariation(denominator_highmet.Data(), "resdown");
  TString denominator_highmet_resup = METvariation(denominator_highmet.Data(), "resup");
  TString denominator_highmet_jer = JERvariation(denominator_highmet.Data(), "jer");
  TString denominator_highmet_jercdown = JERvariation(denominator_highmet.Data(), "jercdown");

  TString denominator_boosted_lowmet_jup = JMEvariation(denominator_boosted_lowmet.Data(), "jup");
  TString denominator_boosted_lowmet_jdown = JMEvariation(denominator_boosted_lowmet.Data(), "jdown");
  TString denominator_boosted_lowmet_resdown = METvariation(denominator_boosted_lowmet.Data(), "resdown");
  TString denominator_boosted_lowmet_resup = METvariation(denominator_boosted_lowmet.Data(), "resup");
  TString denominator_boosted_lowmet_jer = JERvariation(denominator_boosted_lowmet.Data(), "jer");
  TString denominator_boosted_lowmet_jercdown = JERvariation(denominator_boosted_lowmet.Data(), "jercdown");

  TString denominator_boosted_highmet_jup = JMEvariation(denominator_boosted_highmet.Data(), "jup");
  TString denominator_boosted_highmet_jdown = JMEvariation(denominator_boosted_highmet.Data(), "jdown");
  TString denominator_boosted_highmet_resdown = METvariation(denominator_boosted_highmet.Data(), "resdown");
  TString denominator_boosted_highmet_resup = METvariation(denominator_boosted_highmet.Data(), "resup");
  TString denominator_boosted_highmet_jer = JERvariation(denominator_boosted_highmet.Data(), "jer");
  TString denominator_boosted_highmet_jercdown = JERvariation(denominator_boosted_highmet.Data(), "jercdown");

  TString numerator_jup = JMEvariation(numerator.Data(), "jup");
  TString numerator_jdown = JMEvariation(numerator.Data(), "jdown");
  TString numerator_resdown = METvariation(numerator.Data(), "resdown");
  TString numerator_resup = METvariation(numerator.Data(), "resup");
  TString numerator_jer = JERvariation(numerator.Data(), "jer");
  TString numerator_jercdown = JERvariation(numerator.Data(), "jercdown");


  TString denominator_highmbb_boosted_jup = JMEvariation(denominator_highmbb_boosted.Data(), "jup");
  TString denominator_highmbb_boosted_jdown = JMEvariation(denominator_highmbb_boosted.Data(), "jdown");
  TString denominator_highmbb_boosted_resdown = METvariation(denominator_highmbb_boosted.Data(), "resdown");
  TString denominator_highmbb_boosted_resup = METvariation(denominator_highmbb_boosted.Data(), "resup");
  TString denominator_highmbb_boosted_jer =JERvariation(denominator_highmbb_boosted.Data(), "jer");
  TString denominator_highmbb_boosted_jercdown =JERvariation(denominator_highmbb_boosted.Data(), "jercdown");

  TString numerator_highmbb_boosted_jup = JMEvariation(numerator_highmbb_boosted.Data(), "jup");
  TString numerator_highmbb_boosted_jdown = JMEvariation(numerator_highmbb_boosted.Data(), "jdown");
  TString numerator_highmbb_boosted_resdown = METvariation(numerator_highmbb_boosted.Data(), "resdown");
  TString numerator_highmbb_boosted_resup = METvariation(numerator_highmbb_boosted.Data(), "resup");
  TString numerator_highmbb_boosted_jer = JERvariation(numerator_highmbb_boosted.Data(), "jer");
  TString numerator_highmbb_boosted_jercdown = JERvariation(numerator_highmbb_boosted.Data(), "jercdown");

  TString denominator_highmbb_jup = JMEvariation(denominator_highmbb.Data(), "jup");
  TString denominator_highmbb_jdown = JMEvariation(denominator_highmbb.Data(), "jdown");
  TString denominator_highmbb_resdown = METvariation(denominator_highmbb.Data(), "resdown");
  TString denominator_highmbb_resup = METvariation(denominator_highmbb.Data(), "resup");
  TString denominator_highmbb_jer = JERvariation(denominator_highmbb.Data(), "jer");
  TString denominator_highmbb_jercdown = JERvariation(denominator_highmbb.Data(), "jercdown");

  TString numerator_highmbb_jup = JMEvariation(numerator_highmbb.Data(), "jup");
  TString numerator_highmbb_jdown = JMEvariation(numerator_highmbb.Data(), "jdown");
  TString numerator_highmbb_resdown = METvariation(numerator_highmbb.Data(), "resdown");
  TString numerator_highmbb_resup = METvariation(numerator_highmbb.Data(), "resup");
  TString numerator_highmbb_jer = JERvariation(numerator_highmbb.Data(), "jer");
  TString numerator_highmbb_jercdown = JERvariation(numerator_highmbb.Data(), "jercdown");

  TString denominator_dilep_jup = JMEvariation(denominator_dilep.Data(), "jup");
  TString denominator_dilep_jdown = JMEvariation(denominator_dilep.Data(), "jdown");
  TString denominator_dilep_resdown = METvariation(denominator_dilep.Data(), "resdown");
  TString denominator_dilep_resup = METvariation(denominator_dilep.Data(), "resup");
  TString denominator_dilep_jer = JERvariation(denominator_dilep.Data(), "jer");
  TString denominator_dilep_jercdown = JERvariation(denominator_dilep.Data(), "jercdown");

  TString numerator_dilep_jup = JMEvariation(numerator_dilep.Data(), "jup");
  TString numerator_dilep_jdown = JMEvariation(numerator_dilep.Data(), "jdown");
  TString numerator_dilep_resdown = METvariation(numerator_dilep.Data(), "resdown");
  TString numerator_dilep_resup = METvariation(numerator_dilep.Data(), "resup");
  TString numerator_dilep_jer = JERvariation(numerator_dilep.Data(), "jer");
  TString numerator_dilep_jercdown = JERvariation(numerator_dilep.Data(), "jercdown");

  TString denominator_dilep_boosted_jup = JMEvariation(denominator_dilep_boosted.Data(), "jup");
  TString denominator_dilep_boosted_jdown = JMEvariation(denominator_dilep_boosted.Data(), "jdown");
  TString denominator_dilep_boosted_resdown = METvariation(denominator_dilep_boosted.Data(), "resdown");
  TString denominator_dilep_boosted_resup = METvariation(denominator_dilep_boosted.Data(), "resup");
  TString denominator_dilep_boosted_jer = JERvariation(denominator_dilep_boosted.Data(), "jer");
  TString denominator_dilep_boosted_jercdown = JERvariation(denominator_dilep_boosted.Data(), "jercdown");

  TString numerator_dilep_boosted_jup = JMEvariation(numerator_dilep_boosted.Data(), "jup");
  TString numerator_dilep_boosted_jdown = JMEvariation(numerator_dilep_boosted.Data(), "jdown");
  TString numerator_dilep_boosted_resdown = METvariation(numerator_dilep_boosted.Data(), "resdown");
  TString numerator_dilep_boosted_resup = METvariation(numerator_dilep_boosted.Data(), "resup");
  TString numerator_dilep_boosted_jer = JERvariation(numerator_dilep_boosted.Data(), "jer");
  TString numerator_dilep_boosted_jercdown = JERvariation(numerator_dilep_boosted.Data(), "jercdown");


  TString denominator_dilep_highmet_jup = JMEvariation(denominator_dilep_highmet.Data(), "jup");
  TString denominator_dilep_highmet_jdown = JMEvariation(denominator_dilep_highmet.Data(), "jdown");
  TString denominator_dilep_highmet_resdown = METvariation(denominator_dilep_highmet.Data(), "resdown");
  TString denominator_dilep_highmet_resup = METvariation(denominator_dilep_highmet.Data(), "resup");
  TString denominator_dilep_highmet_jer = JERvariation(denominator_dilep_highmet.Data(), "jer");
  TString denominator_dilep_highmet_jercdown = JERvariation(denominator_dilep_highmet.Data(), "jercdown");

  TString denominator_dilep_boosted_highmet_jup = JMEvariation(denominator_dilep_boosted_highmet.Data(), "jup");
  TString denominator_dilep_boosted_highmet_jdown = JMEvariation(denominator_dilep_boosted_highmet.Data(), "jdown");
  TString denominator_dilep_boosted_highmet_resdown = METvariation(denominator_dilep_boosted_highmet.Data(), "resdown");
  TString denominator_dilep_boosted_highmet_resup = METvariation(denominator_dilep_boosted_highmet.Data(), "resup");
  TString denominator_dilep_boosted_highmet_jer = JERvariation(denominator_dilep_boosted_highmet.Data(), "jer");
  TString denominator_dilep_boosted_highmet_jercdown = JERvariation(denominator_dilep_boosted_highmet.Data(), "jercdown");



  // vector<NamedFunc> deepAK8bins = {max_ak8pfjets_deepdisc_hbb<=0.8,max_ak8pfjets_deepdisc_hbb>0.8};
  vector<NamedFunc> deepAK8bins = {"nHiggs==0","nHiggs>0"};
  vector<NamedFunc> njetbins = {"ngoodjets==2","ngoodjets==3"&&LeadingNonBJetPt_med<200.,"ngoodjets>=2 &&ngoodjets<=3"&&LeadingNonBJetPt_med<200.}; 
  vector<NamedFunc> njetbins_old = {"ngoodjets==2","ngoodjets==3"&&LeadingNonBJetPt_med<200.}; 
  vector<NamedFunc> njetbins_jup = {"jup_ngoodjets==2","jup_ngoodjets==3"&&LeadingNonBJetPt_med_jup<200.}; 
  vector<NamedFunc> njetbins_jdown = {"jdown_ngoodjets==2","jdown_ngoodjets==3"&&LeadingNonBJetPt_med_jdown<200.}; 
 
  vector<TString> string_metbins = {"pfmet>125&&pfmet<=200","pfmet>200&&pfmet<=300","pfmet>300&&pfmet<=400","pfmet>400"/*,"pfmet>200"*/};
  vector<NamedFunc> metbins; for(uint ib=0;ib<string_metbins.size();ib++) metbins.push_back(string_metbins[ib]);
  vector<NamedFunc> metbins_jup; for(uint ib=0;ib<string_metbins.size();ib++) metbins_jup.push_back(JMEvariation(string_metbins[ib].Data(), "jup"));
  vector<NamedFunc> metbins_jdown; for(uint ib=0;ib<string_metbins.size();ib++) metbins_jdown.push_back(JMEvariation(string_metbins[ib].Data(), "jdown"));
  vector<NamedFunc> metbins_resup; for(uint ib=0;ib<string_metbins.size();ib++) metbins_resup.push_back(METvariation(string_metbins[ib].Data(), "resup"));
  vector<NamedFunc> metbins_resdown; for(uint ib=0;ib<string_metbins.size();ib++) metbins_resdown.push_back(METvariation(string_metbins[ib].Data(), "resdown"));

  vector<TString> string_metbins_validation = {"pfmet>125&&pfmet<=200","pfmet>200&&pfmet<=300","pfmet>300"/*,"pfmet>200"*/};
  vector<NamedFunc> metbins_validation; for(uint ib=0;ib<string_metbins_validation.size();ib++) metbins_validation.push_back(string_metbins_validation[ib]);
  vector<NamedFunc> metbins_validation_jup; for(uint ib=0;ib<string_metbins_validation.size();ib++) metbins_validation_jup.push_back(JMEvariation(string_metbins_validation[ib].Data(), "jup"));
  vector<NamedFunc> metbins_validation_jdown; for(uint ib=0;ib<string_metbins_validation.size();ib++) metbins_validation_jdown.push_back(JMEvariation(string_metbins_validation[ib].Data(), "jdown"));
  vector<NamedFunc> metbins_validation_resup; for(uint ib=0;ib<string_metbins_validation.size();ib++) metbins_validation_resup.push_back(METvariation(string_metbins_validation[ib].Data(), "resup"));
  vector<NamedFunc> metbins_validation_resdown; for(uint ib=0;ib<string_metbins_validation.size();ib++) metbins_validation_resdown.push_back(METvariation(string_metbins_validation[ib].Data(), "resdown"));

  vector<NamedFunc> metbin_denominators = {denominator_lowmet,denominator_lowmet,denominator_highmet,denominator_highmet};
  vector<NamedFunc> metbin_denominators_jup = {denominator_lowmet_jup,denominator_lowmet_jup,denominator_highmet_jup,denominator_highmet_jup};
  vector<NamedFunc> metbin_denominators_jdown = {denominator_lowmet_jdown,denominator_lowmet_jdown,denominator_highmet_jdown,denominator_highmet_jdown};
  vector<NamedFunc> metbin_denominators_resup = {denominator_lowmet_resup,denominator_lowmet_resup,denominator_highmet_resup,denominator_highmet_resup};
  vector<NamedFunc> metbin_denominators_resdown = {denominator_lowmet_resdown,denominator_lowmet_resdown,denominator_highmet_resdown,denominator_highmet_resdown};
  vector<NamedFunc> metbin_denominators_jer = {denominator_lowmet_jer,denominator_lowmet_jer,denominator_highmet_jer,denominator_highmet_jer};
  vector<NamedFunc> metbin_denominators_jercdown = {denominator_lowmet_jercdown,denominator_lowmet_jercdown,denominator_highmet_jercdown,denominator_highmet_jercdown};

  vector<NamedFunc> metbin_denominators_validation = {denominator_lowmet,denominator_lowmet,denominator_highmet};
  vector<NamedFunc> metbin_denominators_validation_jup = {denominator_lowmet_jup,denominator_lowmet_jup,denominator_highmet_jup};
  vector<NamedFunc> metbin_denominators_validation_jdown = {denominator_lowmet_jdown,denominator_lowmet_jdown,denominator_highmet_jdown};
  vector<NamedFunc> metbin_denominators_validation_resup = {denominator_lowmet_resup,denominator_lowmet_resup,denominator_highmet_resup};
  vector<NamedFunc> metbin_denominators_validation_resdown = {denominator_lowmet_resdown,denominator_lowmet_resdown,denominator_highmet_resdown};
  vector<NamedFunc> metbin_denominators_validation_jer = {denominator_lowmet_jer,denominator_lowmet_jer,denominator_highmet_jer};
  vector<NamedFunc> metbin_denominators_validation_jercdown = {denominator_lowmet_jercdown,denominator_lowmet_jercdown,denominator_highmet_jercdown};

  vector<NamedFunc> metbins_boosted = {"pfmet>125&&pfmet<=300" && njetbins[0],"pfmet>300" && njetbins[0],"pfmet>125&&pfmet<=300" && njetbins[1],"pfmet>300" && njetbins[1]/*,"pfmet>200"*/};
  vector<NamedFunc> metbins_boosted_jup =  {"pfmet_jup>125&&pfmet_jup<=300" && njetbins_jup[0],"pfmet_jup>300" && njetbins_jup[0],"pfmet_jup>125&&pfmet_jup<=300" && njetbins_jup[1],"pfmet_jup>300" && njetbins_jup[1]/*,"pfmet_jup>200"*/};
  vector<NamedFunc> metbins_boosted_jdown =  {"pfmet_jdown>125&&pfmet_jdown<=300" && njetbins_jdown[0],"pfmet_jdown>300" && njetbins_jdown[0],"pfmet_jdown>125&&pfmet_jdown<=300" && njetbins_jdown[1],"pfmet_jdown>300" && njetbins_jdown[1]/*,"pfmet_jdown>200"*/};
  vector<NamedFunc> metbins_boosted_resup = {"pfmet_resup>125&&pfmet_resup<=300" && njetbins[0],"pfmet_resup>300" && njetbins[0],"pfmet_resup>125&&pfmet_resup<=300" && njetbins[1],"pfmet_resup>300" && njetbins[1]/*,"pfmet_resup>200"*/};
  vector<NamedFunc> metbins_boosted_resdown = {"pfmet_resdown>125&&pfmet_resdown<=300" && njetbins[0],"pfmet_resdown>300" && njetbins[0],"pfmet_resdown>125&&pfmet_resdown<=300" && njetbins[1],"pfmet_resdown>300" && njetbins[1]/*,"pfmet_resdown>200"*/};

  vector<NamedFunc> metbins_boosted_mergednjets = {"pfmet>125&&pfmet<=300" && njetbins[2],"pfmet>300" && njetbins[2] };


  vector<NamedFunc> metbin_denominators_boosted = {denominator_boosted_lowmet,denominator_boosted_highmet,denominator_boosted_lowmet,denominator_boosted_highmet};
  vector<NamedFunc> metbin_denominators_boosted_jup = {denominator_boosted_lowmet_jup,denominator_boosted_highmet_jup,denominator_boosted_lowmet_jup,denominator_boosted_highmet_jup};
  vector<NamedFunc> metbin_denominators_boosted_jdown = {denominator_boosted_lowmet_jdown,denominator_boosted_highmet_jdown,denominator_boosted_lowmet_jdown,denominator_boosted_highmet_jdown};
  vector<NamedFunc> metbin_denominators_boosted_resup = {denominator_boosted_lowmet_resup,denominator_boosted_highmet_resup,denominator_boosted_lowmet_resup,denominator_boosted_highmet_resup};
  vector<NamedFunc> metbin_denominators_boosted_resdown = {denominator_boosted_lowmet_resdown,denominator_boosted_highmet_resdown,denominator_boosted_lowmet_resdown,denominator_boosted_highmet_resdown};
  vector<NamedFunc> metbin_denominators_boosted_jer = {denominator_boosted_lowmet_jer,denominator_boosted_highmet_jer,denominator_boosted_lowmet_jer,denominator_boosted_highmet_jer};
  vector<NamedFunc> metbin_denominators_boosted_jercdown = {denominator_boosted_lowmet_jercdown,denominator_boosted_highmet_jercdown,denominator_boosted_lowmet_jercdown,denominator_boosted_highmet_jercdown};


  vector<NamedFunc> numerators = {numerator,numerator,numerator,numerator};
  vector<NamedFunc> numerators_jup = {numerator_jup,numerator_jup,numerator_jup,numerator_jup};
  vector<NamedFunc> numerators_jdown = {numerator_jdown,numerator_jdown,numerator_jdown,numerator_jdown};
  vector<NamedFunc> numerators_resup = {numerator_resup,numerator_resup,numerator_resup,numerator_resup};
  vector<NamedFunc> numerators_resdown = {numerator_resdown,numerator_resdown,numerator_resdown,numerator_resdown};
  vector<NamedFunc> numerators_jer = {numerator_jer,numerator_jer,numerator_jer,numerator_jer};
  vector<NamedFunc> numerators_jercdown = {numerator_jercdown,numerator_jercdown,numerator_jercdown,numerator_jercdown};

  vector<NamedFunc> numerators_boosted = {numerator,numerator,numerator,numerator};
  vector<NamedFunc> numerators_boosted_jup = {numerator_jup,numerator_jup,numerator_jup,numerator_jup};
  vector<NamedFunc> numerators_boosted_jdown = {numerator_jdown,numerator_jdown,numerator_jdown,numerator_jdown};
  vector<NamedFunc> numerators_boosted_resup = {numerator_resup,numerator_resup,numerator_resup,numerator_resup};
  vector<NamedFunc> numerators_boosted_resdown = {numerator_resdown,numerator_resdown,numerator_resdown,numerator_resdown};
  vector<NamedFunc> numerators_boosted_jer = {numerator_jer,numerator_jer,numerator_jer,numerator_jer};
  vector<NamedFunc> numerators_boosted_jercdown = {numerator_jercdown,numerator_jercdown,numerator_jercdown,numerator_jercdown};

  // Makes a plot for each vector in plotcuts
  vector<oneplot> plotcuts({
    // {"ngoodjets", "pfmet>200" && deepAK8bins[0], njetbins},
    {true, "pfmet_SR", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins, metbin_denominators,numerators},
    {true, "pfmet_SR", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins, metbin_denominators,numerators},
    // {"ngoodjets", "pfmet>200" && deepAK8bins[1], njetbins},
    // {true,"pfmet", "ngoodbtags==2" && njetbins[0] && deepAK8bins[1], metbins_boosted, metbin_denominators_boosted,numerators_boosted},
    // {true,"pfmet", "ngoodbtags==2" && njetbins[1] && deepAK8bins[1], metbins_boosted, metbin_denominators_boosted,numerators_boosted},
    {true,"pfmet_SR", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted, metbin_denominators_boosted,numerators_boosted},


    // {false,"pfmet", nTightb==1. && nLooseb==1. && njetbins[0] && deepAK8bins[0], metbins_validation, metbin_denominators_validation,numerators},
    // {false,"pfmet", nTightb==1. && nLooseb==1. && njetbins[1] && deepAK8bins[0], metbins_validation, metbin_denominators_validation,numerators},
    // {false,"pfmet", nTightb==1. && nLooseb==1.  && deepAK8bins[1], metbins_boosted, metbin_denominators_boosted,numerators_boosted},
    // // {false,"pfmet", nTightb==1. && nLooseb==1.  && njetbins[1] && deepAK8bins[1], metbins_boosted, metbin_denominators_boosted,numerators_boosted}

    {false,"pfmet_highmbb", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_validation, {denominator_highmbb,denominator_highmbb,denominator_highmbb},{numerator_highmbb,numerator_highmbb,numerator_highmbb}},
    {false,"pfmet_highmbb", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_validation, {denominator_highmbb,denominator_highmbb,denominator_highmbb},{numerator_highmbb,numerator_highmbb,numerator_highmbb}},
    {false,"pfmet_highmbb", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted, {denominator_highmbb_boosted,denominator_highmbb_boosted,denominator_highmbb_boosted,denominator_highmbb_boosted},{numerator_highmbb_boosted,numerator_highmbb_boosted,numerator_highmbb_boosted,numerator_highmbb_boosted}},
    
    {false,"pfmet_highmbb_mergenjets", "ngoodbtags==2" && njetbins[2] && deepAK8bins[0], metbins_validation, {denominator_highmbb,denominator_highmbb,denominator_highmbb},{numerator_highmbb,numerator_highmbb,numerator_highmbb}},
    {false,"pfmet_highmbb_mergenjets", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted_mergednjets, {denominator_highmbb_boosted,denominator_highmbb_boosted,denominator_highmbb_boosted,denominator_highmbb_boosted},{numerator_highmbb_boosted,numerator_highmbb_boosted,numerator_highmbb_boosted,numerator_highmbb_boosted}},
    {false,"pfmet_highmbb_mergenjets_allmet", "ngoodbtags==2" && njetbins[2] && deepAK8bins[0], metbins, {denominator_highmbb,denominator_highmbb,denominator_highmbb,denominator_highmbb},{numerator_highmbb,numerator_highmbb,numerator_highmbb,numerator_highmbb}},


    {false,"pfmet_highmbb_mergemet", "ngoodbtags==2 && pfmet>125" && deepAK8bins[0], njetbins_old, {denominator_highmbb,denominator_highmbb},{numerator_highmbb,numerator_highmbb}},
    {false,"pfmet_highmbb_mergemet", "ngoodbtags==2 && pfmet>125" && deepAK8bins[1], njetbins_old, {denominator_highmbb_boosted,denominator_highmbb_boosted},{numerator_highmbb_boosted,numerator_highmbb_boosted}},

    {false,"pfmet_highmbb_mergemet", "ngoodbtags==2 && pfmet>200" && deepAK8bins[0], njetbins_old, {denominator_highmbb,denominator_highmbb},{numerator_highmbb,numerator_highmbb}},
    {false,"pfmet_highmbb_mergemet", "ngoodbtags==2 && pfmet>200" && deepAK8bins[1], njetbins_old, {denominator_highmbb_boosted,denominator_highmbb_boosted},{numerator_highmbb_boosted,numerator_highmbb_boosted}},


    // {false,"pfmet_dilep", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_validation, {denominator_dilep,denominator_dilep,denominator_dilep_highmet},{numerator_dilep,numerator_dilep,numerator_dilep}},
    // {false,"pfmet_dilep", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_validation, {denominator_dilep,denominator_dilep,denominator_dilep_highmet},{numerator_dilep,numerator_dilep,numerator_dilep}},
    // {false,"pfmet_dilep", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted, {denominator_dilep_boosted,denominator_dilep_boosted_highmet,denominator_dilep_boosted,denominator_dilep_boosted_highmet},{numerator_dilep_boosted,numerator_dilep_boosted,numerator_dilep_boosted,numerator_dilep_boosted}},

    // {false,"pfmet_dilep", nTightb==2. && njetbins[0] && deepAK8bins[0], metbins_validation, {denominator_dilep,denominator_dilep,denominator_dilep_highmet},{numerator_dilep,numerator_dilep,numerator_dilep}},
    // {false,"pfmet_dilep", nTightb==2. && njetbins[1] && deepAK8bins[0], metbins_validation, {denominator_dilep,denominator_dilep,denominator_dilep_highmet},{numerator_dilep,numerator_dilep,numerator_dilep}},
    // {false,"pfmet_dilep", nTightb==2. && deepAK8bins[1], metbins_boosted, {denominator_dilep_boosted,denominator_dilep_boosted_highmet,denominator_dilep_boosted,denominator_dilep_boosted_highmet},{numerator_dilep_boosted,numerator_dilep_boosted,numerator_dilep_boosted,numerator_dilep_boosted}},

});


  vector<oneplot> plotcuts_jup({
    // {"ngoodjets", "pfmet>200" && deepAK8bins[0], njetbins},
    {true, "pfmet_SR", nMedb_jup==2. && njetbins_jup[0] && deepAK8bins[0], metbins_jup, metbin_denominators_jup,numerators_jup},
    {true, "pfmet_SR", nMedb_jup==2. && njetbins_jup[1] && deepAK8bins[0], metbins_jup, metbin_denominators_jup,numerators_jup},
    // {"ngoodjets", "pfmet>200" && deepAK8bins[1], njetbins_jup},
    // {true,"pfmet", nMedb_jup==2. && njetbins_jup[0] && deepAK8bins[1], metbins_boosted_jup, metbin_denominators_boosted_jup,numerators_boosted},
    // {true,"pfmet", nMedb_jup==2. && njetbins_jup[1] && deepAK8bins[1], metbins_boosted_jup, metbin_denominators_boosted_jup,numerators_boosted},
    {true,"pfmet_SR", nMedb_jup==2. && deepAK8bins[1], metbins_boosted_jup, metbin_denominators_boosted_jup,numerators_boosted_jup},


    {false,"pfmet", nTightb_jup==1. && nLooseb_jup==1. && njetbins_jup[0] && deepAK8bins[0], metbins_validation_jup, metbin_denominators_validation_jup,numerators_jup},
    {false,"pfmet", nTightb_jup==1. && nLooseb_jup==1. && njetbins_jup[1] && deepAK8bins[0], metbins_validation_jup, metbin_denominators_validation_jup,numerators_jup},
    {false,"pfmet", nTightb_jup==1. && nLooseb_jup==1.  && deepAK8bins[1], metbins_boosted_jup, metbin_denominators_boosted,numerators_boosted_jup},
    // {false,"pfmet", nTightb_jup==1. && nLooseb_jup==1.  && njetbins_jup[1] && deepAK8bins[1], metbins_boosted_jup, metbin_denominators_boosted,numerators_boosted_jup}

    {false,"pfmet_highmbb", nMedb_jup==2. && njetbins_jup[0] && deepAK8bins[0], metbins_validation_jup, {denominator_highmbb_jup,denominator_highmbb_jup,denominator_highmbb_jup},{numerator_highmbb_jup,numerator_highmbb_jup,numerator_highmbb_jup}},
    {false,"pfmet_highmbb", nMedb_jup==2. && njetbins_jup[1] && deepAK8bins[0], metbins_validation_jup, {denominator_highmbb_jup,denominator_highmbb_jup,denominator_highmbb_jup},{numerator_highmbb_jup,numerator_highmbb_jup,numerator_highmbb_jup}},
    {false,"pfmet_highmbb", nMedb_jup==2. && deepAK8bins[1], metbins_boosted_jup, {denominator_highmbb_boosted_jup,denominator_highmbb_boosted_jup,denominator_highmbb_boosted_jup,denominator_highmbb_boosted_jup},{numerator_highmbb_boosted_jup,numerator_highmbb_boosted_jup,numerator_highmbb_boosted_jup,numerator_highmbb_boosted_jup}},

    {false,"pfmet_dilep", nMedb_jup==2. && njetbins_jup[0] && deepAK8bins[0], metbins_validation_jup, {denominator_dilep_jup,denominator_dilep_jup,denominator_dilep_highmet_jup},{numerator_dilep_jup,numerator_dilep_jup,numerator_dilep_jup}},
    {false,"pfmet_dilep", nMedb_jup==2. && njetbins_jup[1] && deepAK8bins[0], metbins_validation_jup, {denominator_dilep_jup,denominator_dilep_jup,denominator_dilep_highmet_jup},{numerator_dilep_jup,numerator_dilep_jup,numerator_dilep_jup}},
    {false,"pfmet_dilep", nMedb_jup==2. && deepAK8bins[1], metbins_boosted_jup, {denominator_dilep_boosted_jup,denominator_dilep_boosted_highmet_jup,denominator_dilep_boosted_jup,denominator_dilep_boosted_highmet_jup},{numerator_dilep_boosted_jup,numerator_dilep_boosted_jup,numerator_dilep_boosted_jup,numerator_dilep_boosted_jup}},

    {false,"pfmet_dilep", nTightb_jup==2. && njetbins_jup[0] && deepAK8bins[0], metbins_validation_jup, {denominator_dilep_jup,denominator_dilep_jup,denominator_dilep_highmet_jup},{numerator_dilep_jup,numerator_dilep_jup,numerator_dilep_jup}},
    {false,"pfmet_dilep", nTightb_jup==2. && njetbins_jup[1] && deepAK8bins[0], metbins_validation_jup, {denominator_dilep_jup,denominator_dilep_jup,denominator_dilep_highmet_jup},{numerator_dilep_jup,numerator_dilep_jup,numerator_dilep_jup}},
    {false,"pfmet_dilep", nTightb_jup==2. && deepAK8bins[1], metbins_boosted_jup, {denominator_dilep_boosted_jup,denominator_dilep_boosted_highmet_jup,denominator_dilep_boosted_jup,denominator_dilep_boosted_highmet_jup},{numerator_dilep_boosted_jup,numerator_dilep_boosted_jup,numerator_dilep_boosted_jup,numerator_dilep_boosted_jup}},

});

  vector<oneplot> plotcuts_jdown({
    // {"ngoodjets", "pfmet>200" && deepAK8bins[0], njetbins},
    {true, "pfmet_SR", nMedb_jdown==2. && njetbins_jdown[0] && deepAK8bins[0], metbins_jdown, metbin_denominators_jdown,numerators_jdown},
    {true, "pfmet_SR", nMedb_jdown==2. && njetbins_jdown[1] && deepAK8bins[0], metbins_jdown, metbin_denominators_jdown,numerators_jdown},
    // {"ngoodjets", "pfmet>200" && deepAK8bins[1], njetbins_jdown},
    // {true,"pfmet", nMedb_jdown==2. && njetbins_jdown[0] && deepAK8bins[1], metbins_boosted_jdown, metbin_denominators_boosted_jdown,numerators_boosted},
    // {true,"pfmet", nMedb_jdown==2. && njetbins_jdown[1] && deepAK8bins[1], metbins_boosted_jdown, metbin_denominators_boosted_jdown,numerators_boosted},
    {true,"pfmet_SR", nMedb_jdown==2. && deepAK8bins[1], metbins_boosted_jdown, metbin_denominators_boosted_jdown,numerators_boosted_jdown},


    {false,"pfmet", nTightb_jdown==1. && nLooseb_jdown==1. && njetbins_jdown[0] && deepAK8bins[0], metbins_validation_jdown, metbin_denominators_validation_jdown,numerators_jdown},
    {false,"pfmet", nTightb_jdown==1. && nLooseb_jdown==1. && njetbins_jdown[1] && deepAK8bins[0], metbins_validation_jdown, metbin_denominators_validation_jdown,numerators_jdown},
    {false,"pfmet", nTightb_jdown==1. && nLooseb_jdown==1.  && deepAK8bins[1], metbins_boosted_jdown, metbin_denominators_boosted,numerators_boosted_jdown},
    // {false,"pfmet", nTightb_jdown==1. && nLooseb_jdown==1.  && njetbins_jdown[1] && deepAK8bins[1], metbins_boosted_jdown, metbin_denominators_boosted,numerators_boosted_jdown}

    {false,"pfmet_highmbb", nMedb_jdown==2. && njetbins_jdown[0] && deepAK8bins[0], metbins_validation_jdown, {denominator_highmbb_jdown,denominator_highmbb_jdown,denominator_highmbb_jdown},{numerator_highmbb_jdown,numerator_highmbb_jdown,numerator_highmbb_jdown}},
    {false,"pfmet_highmbb", nMedb_jdown==2. && njetbins_jdown[1] && deepAK8bins[0], metbins_validation_jdown, {denominator_highmbb_jdown,denominator_highmbb_jdown,denominator_highmbb_jdown},{numerator_highmbb_jdown,numerator_highmbb_jdown,numerator_highmbb_jdown}},
    {false,"pfmet_highmbb", nMedb_jdown==2. && deepAK8bins[1], metbins_boosted_jdown, {denominator_highmbb_boosted_jdown,denominator_highmbb_boosted_jdown,denominator_highmbb_boosted_jdown,denominator_highmbb_boosted_jdown},{numerator_highmbb_boosted_jdown,numerator_highmbb_boosted_jdown,numerator_highmbb_boosted_jdown,numerator_highmbb_boosted_jdown}},

    {false,"pfmet_dilep", nMedb_jdown==2. && njetbins_jdown[0] && deepAK8bins[0], metbins_validation_jdown, {denominator_dilep_jdown,denominator_dilep_jdown,denominator_dilep_highmet_jdown},{numerator_dilep_jdown,numerator_dilep_jdown,numerator_dilep_jdown}},
    {false,"pfmet_dilep", nMedb_jdown==2. && njetbins_jdown[1] && deepAK8bins[0], metbins_validation_jdown, {denominator_dilep_jdown,denominator_dilep_jdown,denominator_dilep_highmet_jdown},{numerator_dilep_jdown,numerator_dilep_jdown,numerator_dilep_jdown}},
    {false,"pfmet_dilep", nMedb_jdown==2. && deepAK8bins[1], metbins_boosted_jdown, {denominator_dilep_boosted_jdown,denominator_dilep_boosted_highmet_jdown,denominator_dilep_boosted_jdown,denominator_dilep_boosted_highmet_jdown},{numerator_dilep_boosted_jdown,numerator_dilep_boosted_jdown,numerator_dilep_boosted_jdown,numerator_dilep_boosted_jdown}},

    {false,"pfmet_dilep", nTightb_jdown==2. && njetbins_jdown[0] && deepAK8bins[0], metbins_validation_jdown, {denominator_dilep_jdown,denominator_dilep_jdown,denominator_dilep_highmet_jdown},{numerator_dilep_jdown,numerator_dilep_jdown,numerator_dilep_jdown}},
    {false,"pfmet_dilep", nTightb_jdown==2. && njetbins_jdown[1] && deepAK8bins[0], metbins_validation_jdown, {denominator_dilep_jdown,denominator_dilep_jdown,denominator_dilep_highmet_jdown},{numerator_dilep_jdown,numerator_dilep_jdown,numerator_dilep_jdown}},
    {false,"pfmet_dilep", nTightb_jdown==2. && deepAK8bins[1], metbins_boosted_jdown, {denominator_dilep_boosted_jdown,denominator_dilep_boosted_highmet_jdown,denominator_dilep_boosted_jdown,denominator_dilep_boosted_highmet_jdown},{numerator_dilep_boosted_jdown,numerator_dilep_boosted_jdown,numerator_dilep_boosted_jdown,numerator_dilep_boosted_jdown}},

});

  vector<oneplot> plotcuts_resup({
    // {"ngoodjets", "pfmet>200" && deepAK8bins[0], njetbins},
    {true, "pfmet_SR", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_resup, metbin_denominators_resup,numerators_resup},
    {true, "pfmet_SR", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_resup, metbin_denominators_resup,numerators_resup},
    // {"ngoodjets", "pfmet>200" && deepAK8bins[1], njetbins},
    // {true,"pfmet", "ngoodbtags_resup==2" && njetbins[0] && deepAK8bins[1], metbins_boosted_resup, metbin_denominators_boosted_resup,numerators_boosted},
    // {true,"pfmet", "ngoodbtags_resup==2" && njetbins[1] && deepAK8bins[1], metbins_boosted_resup, metbin_denominators_boosted_resup,numerators_boosted},
    {true,"pfmet_SR", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted_resup, metbin_denominators_boosted_resup,numerators_boosted_resup},


    {false,"pfmet", nTightb==1. && nLooseb==1. && njetbins[0] && deepAK8bins[0], metbins_validation_resup, metbin_denominators_validation_resup,numerators_resup},
    {false,"pfmet", nTightb==1. && nLooseb==1. && njetbins[1] && deepAK8bins[0], metbins_validation_resup, metbin_denominators_validation_resup,numerators_resup},
    {false,"pfmet", nTightb==1. && nLooseb==1.  && deepAK8bins[1], metbins_boosted_resup, metbin_denominators_boosted,numerators_boosted_resup},
    // {false,"pfmet", nTightb==1. && nLooseb==1.  && njetbins[1] && deepAK8bins[1], metbins_boosted_resup, metbin_denominators_boosted,numerators_boosted_resup}

    {false,"pfmet_highmbb", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_validation_resup, {denominator_highmbb_resup,denominator_highmbb_resup,denominator_highmbb_resup},{numerator_highmbb_resup,numerator_highmbb_resup,numerator_highmbb_resup}},
    {false,"pfmet_highmbb", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_validation_resup, {denominator_highmbb_resup,denominator_highmbb_resup,denominator_highmbb_resup},{numerator_highmbb_resup,numerator_highmbb_resup,numerator_highmbb_resup}},
    {false,"pfmet_highmbb", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted_resup, {denominator_highmbb_boosted_resup,denominator_highmbb_boosted_resup,denominator_highmbb_boosted_resup,denominator_highmbb_boosted_resup},{numerator_highmbb_boosted_resup,numerator_highmbb_boosted_resup,numerator_highmbb_boosted_resup,numerator_highmbb_boosted_resup}},

    {false,"pfmet_dilep", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_validation_resup, {denominator_dilep_resup,denominator_dilep_resup,denominator_dilep_highmet_resup},{numerator_dilep_resup,numerator_dilep_resup,numerator_dilep_resup}},
    {false,"pfmet_dilep", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_validation_resup, {denominator_dilep_resup,denominator_dilep_resup,denominator_dilep_highmet_resup},{numerator_dilep_resup,numerator_dilep_resup,numerator_dilep_resup}},
    {false,"pfmet_dilep", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted_resup, {denominator_dilep_boosted_resup,denominator_dilep_boosted_highmet_resup,denominator_dilep_boosted_resup,denominator_dilep_boosted_highmet_resup},{numerator_dilep_boosted_resup,numerator_dilep_boosted_resup,numerator_dilep_boosted_resup,numerator_dilep_boosted_resup}},

    {false,"pfmet_dilep", nTightb==2. && njetbins[0] && deepAK8bins[0], metbins_validation_resup, {denominator_dilep_resup,denominator_dilep_resup,denominator_dilep_highmet_resup},{numerator_dilep_resup,numerator_dilep_resup,numerator_dilep_resup}},
    {false,"pfmet_dilep", nTightb==2. && njetbins[1] && deepAK8bins[0], metbins_validation_resup, {denominator_dilep_resup,denominator_dilep_resup,denominator_dilep_highmet_resup},{numerator_dilep_resup,numerator_dilep_resup,numerator_dilep_resup}},
    {false,"pfmet_dilep", nTightb==2. && deepAK8bins[1], metbins_boosted_resup, {denominator_dilep_boosted_resup,denominator_dilep_boosted_highmet_resup,denominator_dilep_boosted_resup,denominator_dilep_boosted_highmet_resup},{numerator_dilep_boosted_resup,numerator_dilep_boosted_resup,numerator_dilep_boosted_resup,numerator_dilep_boosted_resup}},

});

  vector<oneplot> plotcuts_resdown({
    // {"ngoodjets", "pfmet>200" && deepAK8bins[0], njetbins},
    {true, "pfmet_SR", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_resdown, metbin_denominators_resdown,numerators_resdown},
    {true, "pfmet_SR", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_resdown, metbin_denominators_resdown,numerators_resdown},
    // {"ngoodjets", "pfmet>200" && deepAK8bins[1], njetbins},
    // {true,"pfmet", "ngoodbtags_resdown==2" && njetbins[0] && deepAK8bins[1], metbins_boosted_resdown, metbin_denominators_boosted_resdown,numerators_boosted},
    // {true,"pfmet", "ngoodbtags_resdown==2" && njetbins[1] && deepAK8bins[1], metbins_boosted_resdown, metbin_denominators_boosted_resdown,numerators_boosted},
    {true,"pfmet_SR", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted_resdown, metbin_denominators_boosted_resdown,numerators_boosted_resdown},


    {false,"pfmet", nTightb==1. && nLooseb==1. && njetbins[0] && deepAK8bins[0], metbins_validation_resdown, metbin_denominators_validation_resdown,numerators_resdown},
    {false,"pfmet", nTightb==1. && nLooseb==1. && njetbins[1] && deepAK8bins[0], metbins_validation_resdown, metbin_denominators_validation_resdown,numerators_resdown},
    {false,"pfmet", nTightb==1. && nLooseb==1.  && deepAK8bins[1], metbins_boosted_resdown, metbin_denominators_boosted,numerators_boosted_resdown},
    // {false,"pfmet", nTightb==1. && nLooseb==1.  && njetbins[1] && deepAK8bins[1], metbins_boosted_resdown, metbin_denominators_boosted,numerators_boosted_resdown}

    {false,"pfmet_highmbb", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_validation_resdown, {denominator_highmbb_resdown,denominator_highmbb_resdown,denominator_highmbb_resdown},{numerator_highmbb_resdown,numerator_highmbb_resdown,numerator_highmbb_resdown}},
    {false,"pfmet_highmbb", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_validation_resdown, {denominator_highmbb_resdown,denominator_highmbb_resdown,denominator_highmbb_resdown},{numerator_highmbb_resdown,numerator_highmbb_resdown,numerator_highmbb_resdown}},
    {false,"pfmet_highmbb", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted_resdown, {denominator_highmbb_boosted_resdown,denominator_highmbb_boosted_resdown,denominator_highmbb_boosted_resdown,denominator_highmbb_boosted_resdown},{numerator_highmbb_boosted_resdown,numerator_highmbb_boosted_resdown,numerator_highmbb_boosted_resdown,numerator_highmbb_boosted_resdown}},

    {false,"pfmet_dilep", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_validation_resdown, {denominator_dilep_resdown,denominator_dilep_resdown,denominator_dilep_highmet_resdown},{numerator_dilep_resdown,numerator_dilep_resdown,numerator_dilep_resdown}},
    {false,"pfmet_dilep", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_validation_resdown, {denominator_dilep_resdown,denominator_dilep_resdown,denominator_dilep_highmet_resdown},{numerator_dilep_resdown,numerator_dilep_resdown,numerator_dilep_resdown}},
    {false,"pfmet_dilep", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted_resdown, {denominator_dilep_boosted_resdown,denominator_dilep_boosted_highmet_resdown,denominator_dilep_boosted_resdown,denominator_dilep_boosted_highmet_resdown},{numerator_dilep_boosted_resdown,numerator_dilep_boosted_resdown,numerator_dilep_boosted_resdown,numerator_dilep_boosted_resdown}},

    {false,"pfmet_dilep", nTightb==2. && njetbins[0] && deepAK8bins[0], metbins_validation_resdown, {denominator_dilep_resdown,denominator_dilep_resdown,denominator_dilep_highmet_resdown},{numerator_dilep_resdown,numerator_dilep_resdown,numerator_dilep_resdown}},
    {false,"pfmet_dilep", nTightb==2. && njetbins[1] && deepAK8bins[0], metbins_validation_resdown, {denominator_dilep_resdown,denominator_dilep_resdown,denominator_dilep_highmet_resdown},{numerator_dilep_resdown,numerator_dilep_resdown,numerator_dilep_resdown}},
    {false,"pfmet_dilep", nTightb==2. && deepAK8bins[1], metbins_boosted_resdown, {denominator_dilep_boosted_resdown,denominator_dilep_boosted_highmet_resdown,denominator_dilep_boosted_resdown,denominator_dilep_boosted_highmet_resdown},{numerator_dilep_boosted_resdown,numerator_dilep_boosted_resdown,numerator_dilep_boosted_resdown,numerator_dilep_boosted_resdown}},

});


  vector<oneplot> plotcuts_jer({
    // {"ngoodjets", "pfmet>200" && deepAK8bins[0], njetbins},
    {true, "pfmet_SR", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins, metbin_denominators_jer,numerators_jer},
    {true, "pfmet_SR", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins, metbin_denominators_jer,numerators_jer},
    // {"ngoodjets", "pfmet>200" && deepAK8bins[1], njetbins},
    // {true,"pfmet", "ngoodbtags_jer==2" && njetbins[0] && deepAK8bins[1], metbins_boosted_jer, metbin_denominators_boosted_jer,numerators_boosted},
    // {true,"pfmet", "ngoodbtags_jer==2" && njetbins[1] && deepAK8bins[1], metbins_boosted_jer, metbin_denominators_boosted_jer,numerators_boosted},
    {true,"pfmet_SR", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted, metbin_denominators_boosted_jer,numerators_boosted_jer},


    {false,"pfmet", nTightb==1. && nLooseb==1. && njetbins[0] && deepAK8bins[0], metbins_validation, metbin_denominators_validation_jer,numerators_jer},
    {false,"pfmet", nTightb==1. && nLooseb==1. && njetbins[1] && deepAK8bins[0], metbins_validation, metbin_denominators_validation_jer,numerators_jer},
    {false,"pfmet", nTightb==1. && nLooseb==1.  && deepAK8bins[1], metbins_boosted, metbin_denominators_boosted,numerators_boosted_jer},
    // {false,"pfmet", nTightb==1. && nLooseb==1.  && njetbins[1] && deepAK8bins[1], metbins_boosted, metbin_denominators_boosted,numerators_boosted_jer}

    {false,"pfmet_highmbb", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_validation, {denominator_highmbb_jer,denominator_highmbb_jer,denominator_highmbb_jer},{numerator_highmbb_jer,numerator_highmbb_jer,numerator_highmbb_jer}},
    {false,"pfmet_highmbb", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_validation, {denominator_highmbb_jer,denominator_highmbb_jer,denominator_highmbb_jer},{numerator_highmbb_jer,numerator_highmbb_jer,numerator_highmbb_jer}},
    {false,"pfmet_highmbb", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted, {denominator_highmbb_boosted_jer,denominator_highmbb_boosted_jer,denominator_highmbb_boosted_jer,denominator_highmbb_boosted_jer},{numerator_highmbb_boosted_jer,numerator_highmbb_boosted_jer,numerator_highmbb_boosted_jer,numerator_highmbb_boosted_jer}},

    {false,"pfmet_dilep", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_validation, {denominator_dilep_jer,denominator_dilep_jer,denominator_dilep_highmet_jer},{numerator_dilep_jer,numerator_dilep_jer,numerator_dilep_jer}},
    {false,"pfmet_dilep", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_validation, {denominator_dilep_jer,denominator_dilep_jer,denominator_dilep_highmet_jer},{numerator_dilep_jer,numerator_dilep_jer,numerator_dilep_jer}},
    {false,"pfmet_dilep", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted, {denominator_dilep_boosted_jer,denominator_dilep_boosted_highmet_jer,denominator_dilep_boosted_jer,denominator_dilep_boosted_highmet_jer},{numerator_dilep_boosted_jer,numerator_dilep_boosted_jer,numerator_dilep_boosted_jer,numerator_dilep_boosted_jer}},

    {false,"pfmet_dilep", nTightb==2. && njetbins[0] && deepAK8bins[0], metbins_validation, {denominator_dilep_jer,denominator_dilep_jer,denominator_dilep_highmet_jer},{numerator_dilep_jer,numerator_dilep_jer,numerator_dilep_jer}},
    {false,"pfmet_dilep", nTightb==2. && njetbins[1] && deepAK8bins[0], metbins_validation, {denominator_dilep_jer,denominator_dilep_jer,denominator_dilep_highmet_jer},{numerator_dilep_jer,numerator_dilep_jer,numerator_dilep_jer}},
    {false,"pfmet_dilep", nTightb==2. && deepAK8bins[1], metbins_boosted, {denominator_dilep_boosted_jer,denominator_dilep_boosted_highmet_jer,denominator_dilep_boosted_jer,denominator_dilep_boosted_highmet_jer},{numerator_dilep_boosted_jer,numerator_dilep_boosted_jer,numerator_dilep_boosted_jer,numerator_dilep_boosted_jer}},

});


    vector<oneplot> plotcuts_jercdown({
      // {"ngoodjets", "pfmet>200" && deepAK8bins[0], njetbins},
      {true, "pfmet_SR", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins, metbin_denominators_jercdown,numerators_jercdown},
      {true, "pfmet_SR", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins, metbin_denominators_jercdown,numerators_jercdown},
      // {"ngoodjets", "pfmet>200" && deepAK8bins[1], njetbins},
      // {true,"pfmet", "ngoodbtags_jercdown==2" && njetbins[0] && deepAK8bins[1], metbins_boosted_jercdown, metbin_denominators_boosted_jercdown,numerators_boosted},
      // {true,"pfmet", "ngoodbtags_jercdown==2" && njetbins[1] && deepAK8bins[1], metbins_boosted_jercdown, metbin_denominators_boosted_jercdown,numerators_boosted},
      {true,"pfmet_SR", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted, metbin_denominators_boosted_jercdown,numerators_boosted_jercdown},


      {false,"pfmet", nTightb==1. && nLooseb==1. && njetbins[0] && deepAK8bins[0], metbins_validation, metbin_denominators_validation_jercdown,numerators_jercdown},
      {false,"pfmet", nTightb==1. && nLooseb==1. && njetbins[1] && deepAK8bins[0], metbins_validation, metbin_denominators_validation_jercdown,numerators_jercdown},
      {false,"pfmet", nTightb==1. && nLooseb==1.  && deepAK8bins[1], metbins_boosted, metbin_denominators_boosted,numerators_boosted_jercdown},
      // {false,"pfmet", nTightb==1. && nLooseb==1.  && njetbins[1] && deepAK8bins[1], metbins_boosted, metbin_denominators_boosted,numerators_boosted_jercdown}

      {false,"pfmet_highmbb", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_validation, {denominator_highmbb_jercdown,denominator_highmbb_jercdown,denominator_highmbb_jercdown},{numerator_highmbb_jercdown,numerator_highmbb_jercdown,numerator_highmbb_jercdown}},
      {false,"pfmet_highmbb", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_validation, {denominator_highmbb_jercdown,denominator_highmbb_jercdown,denominator_highmbb_jercdown},{numerator_highmbb_jercdown,numerator_highmbb_jercdown,numerator_highmbb_jercdown}},
      {false,"pfmet_highmbb", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted, {denominator_highmbb_boosted_jercdown,denominator_highmbb_boosted_jercdown,denominator_highmbb_boosted_jercdown,denominator_highmbb_boosted_jercdown},{numerator_highmbb_boosted_jercdown,numerator_highmbb_boosted_jercdown,numerator_highmbb_boosted_jercdown,numerator_highmbb_boosted_jercdown}},

      {false,"pfmet_dilep", "ngoodbtags==2" && njetbins[0] && deepAK8bins[0], metbins_validation, {denominator_dilep_jercdown,denominator_dilep_jercdown,denominator_dilep_highmet_jercdown},{numerator_dilep_jercdown,numerator_dilep_jercdown,numerator_dilep_jercdown}},
      {false,"pfmet_dilep", "ngoodbtags==2" && njetbins[1] && deepAK8bins[0], metbins_validation, {denominator_dilep_jercdown,denominator_dilep_jercdown,denominator_dilep_highmet_jercdown},{numerator_dilep_jercdown,numerator_dilep_jercdown,numerator_dilep_jercdown}},
      {false,"pfmet_dilep", "ngoodbtags==2" && deepAK8bins[1], metbins_boosted, {denominator_dilep_boosted_jercdown,denominator_dilep_boosted_highmet_jercdown,denominator_dilep_boosted_jercdown,denominator_dilep_boosted_highmet_jercdown},{numerator_dilep_boosted_jercdown,numerator_dilep_boosted_jercdown,numerator_dilep_boosted_jercdown,numerator_dilep_boosted_jercdown}},

      {false,"pfmet_dilep", nTightb==2. && njetbins[0] && deepAK8bins[0], metbins_validation, {denominator_dilep_jercdown,denominator_dilep_jercdown,denominator_dilep_highmet_jercdown},{numerator_dilep_jercdown,numerator_dilep_jercdown,numerator_dilep_jercdown}},
      {false,"pfmet_dilep", nTightb==2. && njetbins[1] && deepAK8bins[0], metbins_validation, {denominator_dilep_jercdown,denominator_dilep_jercdown,denominator_dilep_highmet_jercdown},{numerator_dilep_jercdown,numerator_dilep_jercdown,numerator_dilep_jercdown}},
      {false,"pfmet_dilep", nTightb==2. && deepAK8bins[1], metbins_boosted, {denominator_dilep_boosted_jercdown,denominator_dilep_boosted_highmet_jercdown,denominator_dilep_boosted_jercdown,denominator_dilep_boosted_highmet_jercdown},{numerator_dilep_boosted_jercdown,numerator_dilep_boosted_jercdown,numerator_dilep_boosted_jercdown,numerator_dilep_boosted_jercdown}},

  });


//Years: different global cut per entry
//Validation: different num/dem per entry
    //with data: duplicate with different sample.
//systematic mode: different weight per entry.

//   vector<oneplot> plotcuts({
//     {"ngoodjets", "pfmet>200", {"ngoodjets==2", "ngoodjets==3"&&LeadingNonBJetPt_med<100.}},
//     {"pfmet", "ngoodjets==2", {"pfmet>125 && pfmet<=200","pfmet>200 && pfmet<=300","pfmet>300&&pfmet<=400","pfmet>400"}},
//   	{"pfmet", "ngoodjets==3"&&LeadingNonBJetPt_med<200., {"pfmet>125 && pfmet<=200","pfmet>200 && pfmet<=300","pfmet>300&&pfmet<=400","pfmet>400"}}


// });
    string tag = "";
    if(year_mode) tag += "year_";
    if(data_mode) tag += "data_newtagger_expandmbbdenominator_mbb150_correctpaths_newbabies_yesHasNano";

  vector<NamedFunc> weights; size_t nsels; //vector<NamedFunc> numerators,denominators;
  vector<TString> leglabels;
  vector<string> cuts;
  /////// Systematic weight mode //////
  if(systematic_mode){ // Same numerator and denominator, vary weights
    weights= {  "weight * trig_eff " * nanoWeight * yearWeight,
                "weight * trig_eff " * nanoWeight * yearWeight * ST_up,
                "weight * trig_eff " * nanoWeight * yearWeight * ST_down,
                "weight * trig_eff * w_puUp" * nanoWeight * yearWeight,
                "weight * trig_eff * w_puDown" * nanoWeight * yearWeight,
                "weight * trig_eff " * nanoWeight * yearWeight * ttbar_genmet_fix,
                "weight * trig_eff " * nanoWeight * yearWeight * ttbar_genmet_antifix,
                "weight * trig_eff * w_btagLFUp" * nanoWeight * yearWeight,
                "weight * trig_eff * w_btagLFDown" * nanoWeight * yearWeight,
                "weight * trig_eff * w_btagHFUp" * nanoWeight * yearWeight,
                "weight * trig_eff * w_btagHFDown" * nanoWeight * yearWeight,
                // "weight * trig_eff * w_topPtSFUp" * nanoWeight * yearWeight,
                "weight * trig_eff * trig_eff" * nanoWeight * yearWeight,
                "weight * trig_eff " * nanoWeight * nanoWeight * yearWeight,
                // "weight * trig_eff * w_topPtSFDown" * nanoWeight * yearWeight,
                 };
    for(uint ivar=0;ivar<weights.size();ivar++){
      cuts.push_back("1");
    }
    tag+="realistic1_";
    leglabels = {"Nominal","Single top x1.5","Single top x0.5","PU weights up","PU weights down","Reweight genMET to 2016","Reweight genMET away from 2016",
                  "b-tag mistag up","b-tag mistag down","b-tag HF up","b-tag HF down","trig eff",/*"top p_{T} up",*/ "nano weight"};

    // weights= { "weight * trig_eff " * nanoWeight * yearWeight,
    //             "weight * trig_eff * w_lepSFUp" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_lepSFDown" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_tauSFUp" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_tauSFDown" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_L1prefireUp" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_L1prefireDown" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_ISRnjets * w_ISRUp" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_ISRnjets * w_ISRDown" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_pdfUp" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_pdfDown" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_alphasUp" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_alphasDown" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_q2Up" * nanoWeight * yearWeight ,
    //             "weight * trig_eff * w_q2Down" * nanoWeight * yearWeight 
    //              };
      // for(uint ivar=0;ivar<weights.size();ivar++){
      //   cuts.push_back("1");
      // }
    // tag+="realistic2_";
    // leglabels = {"Nominal","Lepton SF up","Lepton SF down","Tau SF up","Tau SF down","L1 prefire up","L1 prefire down","ISR njets up","ISR njets down","PDF up","PDF down",
    //             "alphas up","alphas down","q2 up","q2 down"};


    // weights={ "weight * trig_eff " * nanoWeight * yearWeight,
    //           "weight * trig_eff " * nanoWeight * yearWeight,
    //           "weight * trig_eff " * nanoWeight * yearWeight,
    //           "weight * trig_eff " * nanoWeight * yearWeight,
    //           "weight * trig_eff " * nanoWeight * yearWeight,
    //           "weight * trig_eff " * nanoWeight * yearWeight,
    //           "weight * trig_eff " * nanoWeight * yearWeight,   
    // };
    // cuts={ "1",
    //           "jup",
    //           "jdown",
    //           "resup",
    //           "resdown",
    //           "jer",
    //           "jercdown"
    // };
    // tag+="jet_";
    // leglabels = {"Nominal","JES up","JES down", "MET res up","MET res down","JER corr","JER cdown"};


    // weights= { "weight * trig_eff " * nanoWeight * yearWeight,
    //             "weight * trig_eff " * nanoWeight * yearWeight * ST_up,
    //             "weight * trig_eff " * nanoWeight * yearWeight * ST_down,
    //             "weight * trig_eff  * w_pu" * nanoWeight * yearWeight,
    //             "weight * trig_eff / w_pu" * nanoWeight * yearWeight,
    //             "weight * trig_eff " * nanoWeight * yearWeight * ttbar_genmet_fix,
    //             "weight * trig_eff " * nanoWeight * yearWeight * ttbar_genmet_antifix,
    //             "weight * trig_eff " * nanoWeight * yearWeight * fake_up,
    //             "weight * trig_eff " * nanoWeight * yearWeight * fake_down
    //              };
    // leglabels = {"Nominal"/* 1-lepton m_{bb} on higgs"*/,"Single top x1.5","Single top x0.5","PU weights up","PU weights down","Reweight genMET to 2016","Reweight genMET away from 2016","b-tag mistag up","b-tag mistag down","b-tag HF up","b-tag HF down","top p_{T} up", "top p_{t} down"};
    // leglabels = {"Nominal"/* 1-lepton m_{bb} on higgs"*/,"Single top x1.5","Single top x0.5","PU weights squared","PU weights off","Reweight genMET to 2016","Reweight genMET away from 2016","b-tag fake rate x1.5","b-tag fake rate x0.5"};


    nsels = 2*weights.size();
  } //systematic mode

  if(year_mode){ 
    cuts= { "1","year==2016","year==2017","year==2018"};
    leglabels = {"Full Run II","2016 only","2017 only","2018 only"};
   
    for(uint ivar=0;ivar<cuts.size();ivar++){
      weights.push_back("weight * trig_eff" * nanoWeight * yearWeight);
    }
    nsels = 2*cuts.size();

  } //year mode

    if(data_mode){ 
    cuts= { "1","1"}; //data, mc
    leglabels = {"Data, 2016-2018","MC, 2016-2018"};
   
    for(uint ivar=0;ivar<cuts.size();ivar++){
      weights.push_back("weight * trig_eff" * nanoWeight * yearWeight);
    }
    nsels = 2*cuts.size();

  } //year mode




  /////// Selection variation mode //////
  // else{ // same weights, vary numerator/denominator selections
  //   numerators = {single_lep&&"mct>200",
  //                 single_lep&&"mct>200&&mbb>90&&mbb<150",
  //                 dilep&&"mct>200",
  //                 dilep&&"mct>200&&mbb>90&&mbb<150"};

  //   denominators = {single_lep&&"mct>150&&mct<=200",
  //                   single_lep&&"mct>150&&mct<=200&&mbb>90&&mbb<150",
  //                   dilep&&"mct>150&&mct<=200",
  //                   dilep&&"mct>150&&mct<=200&&mbb>90&&mbb<150"};

  //   leglabels = {"1-lepton inclusive m_{bb}","1-lepton m_{bb} on higgs","2-lepton inclusive m_{bb}","2-lepton m_{bb} on higgs"};

  //   for(uint ivar=0;ivar<numerators.size();ivar++){
  //       weights.push_back("weight * w_pu" * yearWeight);

   
  //   }
  //    nsels = 2*weights.size();
  // }//selection mode


  vector<int> proc_options = {0,1,2,3,4};
  // vector<int> proc_options = {0,1};//allbkg, top only
  vector<TString> tags = {"allbkg","top"};

  vector<vector<vector<int> > > indices;
  vector<vector<vector<int> > > indices_double_ratio;

  for(int i=0;i<static_cast<int>(nsels/2);i++){
    //numerator is always top only; W predicted separately.
    //Except for data mode.
    Bkgs num = top;
    if(data_mode) num = bkg;

    if(data_mode && i%2==0) indices.push_back(vector<vector<int> >({{proc_options[data],2*i,-1},{proc_options[data],2*i+1,1}})); //data ratios are even indices
    else indices.push_back(vector<vector<int> >({{proc_options[bkg],2*i,-1},{proc_options[num],2*i+1,1}}));
    if(data_mode){
      if(i>0){
        indices_double_ratio.push_back(vector<vector<int> >({
          {proc_options[bkg],2*i,1},{proc_options[num],2*i+1,-1},
      {proc_options[data],0,-1},{proc_options[data],1,1} //Assume 0th is data.
    }));
    }
    }//data mode
    else{
      if(i>0){
        indices_double_ratio.push_back(vector<vector<int> >({
          {proc_options[bkg],2*i,-1},{proc_options[num],2*i+1,1},
      {proc_options[bkg],0,1},{proc_options[num],1,-1} //Assume 0th is nominal.
    }));
      }
      else{
        indices_double_ratio.push_back(vector<vector<int> >({
          {proc_options[bkg],2*i,-1},{proc_options[num],2*i+1,1}
        }));
      }
  }//not data mode
}

  for(uint ii=0;ii<indices.size();ii++){
    cout<<"Row "<<ii<<"; ";
    for(uint ij=0;ij < indices[ii].size();ij++){
      cout<<" element "<<ij<<" ";
      for(uint ik=0;ik<indices[ii][ij].size();ik++){
        cout<<indices[ii][ij][ik]<<" ";
      }
      cout<<endl;
    }
  }
  cout<<"finished dumping indices"<<endl;
  PlotMaker pm;
  vector<vector<vector<NamedFunc> > > allcuts(plotcuts.size(), vector<vector<NamedFunc> > (nsels));
  //vector<vector<vector<TString> > > allcuts(plotcuts.size(), vector<vector<TString> > (nsels));
  for(size_t iplot=0; iplot<plotcuts.size(); iplot++){
  //  if(data_mode && plotcuts[iplot].blind) continue;
    for(uint ivar=0; ivar<nsels/2; ivar++){
      vector<TableRow> table_cuts_den;
      for(size_t ibin=0; ibin<plotcuts[iplot].bincuts.size(); ibin++){
        //TString totcut=plotcuts[iplot].baseline+" && "+plotcuts[iplot].bincuts[ibin]+" && "+denominators[ivar];
        // NamedFunc totcut=plotcuts[iplot].baseline && plotcuts[iplot].bincuts[ibin] && denominators[ivar];
      
      NamedFunc totcut="1";
      if (cuts[ivar]=="jup") totcut=plotcuts_jup[iplot].baseline && plotcuts_jup[iplot].bincuts[ibin] && plotcuts_jup[iplot].denSel[ibin];
      else if (cuts[ivar]=="jdown") totcut=plotcuts_jdown[iplot].baseline && plotcuts_jdown[iplot].bincuts[ibin] && plotcuts_jdown[iplot].denSel[ibin];
      else if (cuts[ivar]=="resup") totcut=plotcuts_resup[iplot].baseline && plotcuts_resup[iplot].bincuts[ibin] && plotcuts_resup[iplot].denSel[ibin];
      else if (cuts[ivar]=="resdown") totcut=plotcuts_resdown[iplot].baseline && plotcuts_resdown[iplot].bincuts[ibin] && plotcuts_resdown[iplot].denSel[ibin];
      else if (cuts[ivar]=="jer") totcut=plotcuts_jer[iplot].baseline && plotcuts_jer[iplot].bincuts[ibin] && plotcuts_jer[iplot].denSel[ibin];
      else if (cuts[ivar]=="jercdown") totcut=plotcuts_jercdown[iplot].baseline && plotcuts_jercdown[iplot].bincuts[ibin] && plotcuts_jercdown[iplot].denSel[ibin];
      else totcut=plotcuts[iplot].baseline && plotcuts[iplot].bincuts[ibin] && plotcuts[iplot].denSel[ibin] && cuts[ivar];
      
      table_cuts_den.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
        

        allcuts[iplot][2*ivar].push_back(totcut);
        cout<<"Denominator: "<<totcut.Name()<<endl;
      }
      TString tname = "rmct"; tname += iplot; tname += 2*ivar;
      pm.Push<Table>(tname.Data(),  table_cuts_den, all_procs, false, false);
      vector<TableRow> table_cuts_num;
      for(size_t ibin=0; ibin<plotcuts[iplot].bincuts.size(); ibin++){
        NamedFunc totcut="1";
        // NamedFunc totcut=plotcuts[iplot].baseline && plotcuts[iplot].bincuts[ibin] && numerators[ivar];
      if (cuts[ivar]=="jup") totcut=plotcuts_jup[iplot].baseline && plotcuts_jup[iplot].bincuts[ibin] && plotcuts_jup[iplot].numSel[ibin];
      else if (cuts[ivar]=="jdown") totcut=plotcuts_jdown[iplot].baseline && plotcuts_jdown[iplot].bincuts[ibin] && plotcuts_jdown[iplot].numSel[ibin];
      else if (cuts[ivar]=="resup") totcut=plotcuts_resup[iplot].baseline && plotcuts_resup[iplot].bincuts[ibin] && plotcuts_resup[iplot].numSel[ibin];
      else if (cuts[ivar]=="resdown") totcut=plotcuts_resdown[iplot].baseline && plotcuts_resdown[iplot].bincuts[ibin] && plotcuts_resdown[iplot].numSel[ibin];
      else if (cuts[ivar]=="jer") totcut=plotcuts_jer[iplot].baseline && plotcuts_jer[iplot].bincuts[ibin] && plotcuts_jer[iplot].numSel[ibin];
      else if (cuts[ivar]=="jercdown") totcut=plotcuts_jercdown[iplot].baseline && plotcuts_jercdown[iplot].bincuts[ibin] && plotcuts_jercdown[iplot].numSel[ibin];      
      else totcut=plotcuts[iplot].baseline && plotcuts[iplot].bincuts[ibin] && plotcuts[iplot].numSel[ibin] && cuts[ivar];
       // TString totcut=plotcuts[iplot].baseline+" && "+plotcuts[iplot].bincuts[ibin]+" && "+numerators[ivar];
      table_cuts_num.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
        allcuts[iplot][2*ivar+1].push_back(totcut);
        cout<<"numerator: "<<totcut.Name()<<endl;
      } // Loop over bins
      tname = "rmct"; tname += iplot; tname += 2*ivar+1; 
      pm.Push<Table>(tname.Data(),  table_cuts_num, all_procs, false, false);
    } // Loop over abcdcuts
  } // Loop over plots

 
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////// Finding all yields ///////////////////////////////////////////////

  pm.min_print_ = true;
  pm.MakePlots(lumi);

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////// Calculating preds/kappas and printing table //////////////////////////////////////

  for(size_t iplot=0; iplot<plotcuts.size(); iplot++){
    if(data_mode && plotcuts[iplot].blind) continue;
    // allyields: [0] All bkg, [1] tt1l, [2] tt2l, [3] other [4] data
    vector<vector<vector<GammaParams> > > allyields(5,vector<vector<GammaParams> >(nsels));
    for(size_t idens=0; idens<nsels/2; idens++){
      Table * yield_table = static_cast<Table*>(pm.Figures()[iplot*nsels+2*idens].get());
      allyields[bkg]  [2*idens] = yield_table->BackgroundYield(lumi);
      //cout<<"allyields "<<allyields[bkg]  [idens]<<endl;
      allyields[top] [2*idens] = yield_table->Yield(proc_top.get(), lumi);
      allyields[w] [2*idens] = yield_table->Yield(proc_wjets.get(), lumi);
      allyields[other] [2*idens] = yield_table->Yield(proc_other.get(), lumi);
      if(data_mode) allyields[data] [2*idens] = yield_table->DataYield();

      yield_table = static_cast<Table*>(pm.Figures()[iplot*nsels+2*idens+1].get());
      allyields[bkg]  [2*idens+1] = yield_table->BackgroundYield(lumi);
      //cout<<"allyields "<<allyields[bkg]  [2*idens+1]<<endl;
      allyields[top] [2*idens+1] = yield_table->Yield(proc_top.get(), lumi);
      allyields[w] [2*idens+1] = yield_table->Yield(proc_wjets.get(), lumi);
      allyields[other] [2*idens+1] = yield_table->Yield(proc_other.get(), lumi);
      if(data_mode) allyields[data] [2*idens+1] = yield_table->DataYield();

      // allyields[other][iabcd] = yield_table->Yield(proc_other.get(), lumi);
    } // Loop over ABCD cuts

    //// Print MC/Data yields, cuts applied, kappas, preds
    if(debug) printDebug(allcuts[iplot], allyields, baseline);


    plotRatio(allyields, plotcuts[iplot], indices, leglabels,systematic_mode,tag);
    if(systematic_mode) plotRatio(allyields, plotcuts[iplot], indices_double_ratio, leglabels,systematic_mode,tag+"double");

   vector<vector<vector<int> > > indices_top = indices;
   vector<vector<vector<int> > > indices_top_double_ratio = indices_double_ratio;
 
   if(! data_mode){
   for(uint il=0;il<indices_top.size();il++){
    for(uint ij=0;ij<indices_top[il].size();ij++){
      indices_top[il][ij][0]=1; //convert "bkg" to "top"
      indices_top_double_ratio[il][ij][0]=1; //convert "bkg" to "top"
    }
   }
 }
   // plotRatio(allyields, plotcuts[iplot], indices_top, leglabels,systematic_mode,"toponly");
   // if(systematic_mode) plotRatio(allyields, plotcuts[iplot], indices_top_double_ratio, leglabels,systematic_mode,"double_toponly");

  } // Loop over plots


  double seconds = (chrono::duration<double>(chrono::high_resolution_clock::now() - begTime)).count();
  TString hhmmss = HoursMinSec(seconds);
  cout<<endl<<"Finding "<<plotcuts.size()<<" plots took "<<round(seconds)<<" seconds ("<<hhmmss<<")"<<endl<<endl;
} // main

////////////////////////////////////////// End of main //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


// allyields: [0] All bkg, [1] tt1l, [2] tt2l, [3] other
void printDebug(vector<vector<NamedFunc> > &allcuts, vector<vector<vector<GammaParams> > > &allyields, 
		TString baseline){
  int digits = 3;
  cout<<endl<<endl<<"============================ Printing cuts  ============================"<<endl;
  cout<<"-- Baseline cuts: "<<baseline<<endl<<endl;
  for(size_t ibin=0; ibin<allcuts[0].size(); ibin++){
    for(size_t iabcd=0; iabcd<allcuts.size(); iabcd++){
      cout<<"MC: "    <<setw(9)<<RoundNumber(allyields[bkg]  [iabcd][ibin].Yield(), digits)
	  <<"  top: "<<setw(9)<<RoundNumber(allyields[top] [iabcd][ibin].Yield(), digits)
	  <<"  wjets: "<<setw(9)<<RoundNumber(allyields[w] [iabcd][ibin].Yield(), digits)
    <<" other: "<<setw(9)<<RoundNumber(allyields[other][iabcd][ibin].Yield(), digits);
	  if(data_mode) cout<<" data: "<<setw(9)<<RoundNumber(allyields[data][iabcd][ibin].Yield(), digits)<<"  - "<< allcuts[iabcd][ibin].Name()<<endl;
	  else cout<< allcuts[iabcd][ibin].Name()<<endl;
    } // Loop over ABCD cuts
    cout<<endl;
  } // Loop over bin cuts
cout<<"completed printDebug"<<endl;
} // printDebug


void GetOptions(int argc, char *argv[]){
  while(true){
    static struct option long_options[] = {
      {"lumi", required_argument, 0, 'l'},    // Luminosity to normalize MC with (no data)
      {"debug", no_argument, 0, 'd'},         // Debug: prints yields and cuts used
      {0, 0, 0, 0}
    };

    char opt = -1;
    int option_index;
    opt = getopt_long(argc, argv, "s:l:d", long_options, &option_index);
    if(opt == -1) break;

    string optname;
    switch(opt){
    case 'l':
      lumi = atof(optarg);
      break;
    case 'd':
      debug = true;
      break;
    case 0:
      break;
    default:
      printf("Bad option! getopt_long returned character code 0%o\n", opt);
      break;
    }
  }
}
