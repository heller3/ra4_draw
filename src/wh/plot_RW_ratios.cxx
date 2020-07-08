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
        std::cout << listOfJetVars[ibin] << std::endl;
    }
    listOfMETVars = {"pfmet", "mt_met_lep"};
    for (size_t ibin=0; ibin<listOfMETVars.size(); ibin++){
        cut = ReplaceInCut(cut, listOfMETVars[ibin], listOfMETVars[ibin]+'_'+variation);
        std::cout << listOfMETVars[ibin] << std::endl;
    }
    std::cout << cut << std::endl;
    return cut;
}

string METvariation(string cut, string variation){
    vector<string> listOfMETVars;
    listOfMETVars = {"pfmet", "mt_met_lep"};
    for (size_t ibin=0; ibin<listOfMETVars.size(); ibin++){
        cut = ReplaceInCut(cut, listOfMETVars[ibin], listOfMETVars[ibin]+'_'+variation);
        std::cout << listOfMETVars[ibin] << std::endl;
    }
    std::cout << cut << std::endl;
    return cut;
}


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
  TString ytitle="R_{W}";
  if(indices[2].size()==4) {ytitle = "R_{W} Variation / Nominal"; double_mode=true;}
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

  float minx = 0.5, maxx = nbins+0.5, miny = minr*0.55, maxy = maxr*2.45;
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
  double legW = 0.73/*0.19*ngraphs*/, legH = legSingle*(ngraphs+1)/4;
  if(year_mode || data_mode) {legW =0.36; legH = legSingle*(ngraphs+1);}

  TLegend leg(legX, legY-legH, legX+legW, legY);
//  leg.SetTextSize(opts.LegendEntryHeight()); 
  leg.SetFillColor(0);
  // leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(42);
  leg.SetNColumns(2/*ngraphs*/);

  Palette colors("txt/colors.txt", "default");
  //vector<int> mcolors({kRed, kGreen+1, 4, kMagenta+2});
  vector<int> mcolors({kGray+2,kRed-4,kRed-4, kGreen-3,kGreen-3, kCyan-3,kCyan-3, kMagenta+2,kMagenta+2,kOrange,kOrange,kBlue+2,kBlue+2,kGray,kGray,kRed+2,kRed+2});
  vector<int> styles({20, 22, 23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23});
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

    if(!double_mode || igraph>0){
        if (igraph%2==0) leg.AddEntry(&graph[igraph], leglabels[igraph], "p");
    }
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
  unsigned wname(35), wdist(2), wbin(12);
  outtable<<left<<setw(wname)<<" ";
  for(size_t ibin=0; ibin<nbins; ibin++) outtable<<left<<setw(wbin)<<CodeToRootTex(plotdef.bincuts[ibin].Name()).c_str();
  outtable<<endl;
  for(size_t igraph=0; igraph<ngraphs; igraph++){
    outtable<<left<<setw(wname)<<leglabels[igraph]<<setw(wdist)<<" ";
    for(size_t ibin=0; ibin<nbins; ibin++){
      if(!tag.Contains("double")) outtable<<left<<setw(wbin)<<RoundNumber(vy[igraph][ibin],3);
      else outtable<<left<<setw(wbin)<<RoundNumber(100.*(vy[igraph][ibin]-1.),1)+"%";
    }
    outtable<<endl;
  }

} // plotRatio


void printDebug(vector<vector<NamedFunc> > &allcuts, vector<vector<vector<GammaParams> > > &allyields, 
		TString baseline);

void GetOptions(int argc, char *argv[]);

int main(int argc, char *argv[]){
  gErrorIgnoreLevel=6000; // Turns off ROOT errors due to missing branches
  GetOptions(argc, argv);

  chrono::high_resolution_clock::time_point begTime;
  begTime = chrono::high_resolution_clock::now();

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////// Defining processes //////////////////////////////////////////

  // Cuts in baseline speed up the yield finding
  string baseline="";
  NamedFunc baselinef = "pass && hasNano && nvetoleps==1&&PassTrackVeto&&PassTauVeto&&nWHLeptons==1";

  Palette colors("txt/colors.txt", "default");


  string data2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string data2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";
  string data2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/";


  string mc2016_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/s16v3/";
  string mc2016_dir_ttmet = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/s16v3/";

  string mc2017_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/f17v2/";
  string mc2017_dir_ttmet = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/f17v2/";

  string mc2018_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/a18v1/";
  string mc2018_dir_ttmet = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_05_27/a18v1/";

//  auto all_top = {mc2016_dir+"slim_*TTJets_1lep_top_s16v3*.root",mc2016_dir+"slim_*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"slim_*TTJets_1lep_*met150*.root",mc2017_dir+"slim_*TTJets_1lep_top_f17v2*.root",mc2017_dir+"slim_*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"slim_*TTJets_1lep_*met150*.root",mc2018_dir+"slim_*TTJets_1lep_top_a18v1*.root",mc2018_dir+"slim_*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"slim_*TTJets_1lep_*met80*.root",mc2016_dir+"slim_*_ST_*.root",mc2017_dir+"slim_*_ST_*.root",mc2018_dir+"slim_*_ST_*.root",mc2016_dir+"slim_*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"slim_*TTJets_2lep_*met150*.root",mc2017_dir+"slim_*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"slim_*TTJets_2lep_*met150*.root",mc2018_dir+"slim_*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"slim_*TTJets_2lep_*met80*.root"};

  auto all_top = {mc2016_dir+"slim_*TTJets_1lep_top_s16v3*.root"};

//  auto all_other = {mc2016_dir+"slim_*WW*.root", mc2016_dir+"slim_*WZ*.root",mc2017_dir+"slim_*WW*.root", mc2017_dir+"slim_*WZ*.root",mc2018_dir+"slim_*WW*.root", mc2018_dir+"slim_*WZ*.root",mc2016_dir+"slim_*_TTWJets*.root", mc2016_dir+"slim_*_TTZ*.root",mc2017_dir+"slim_*_TTWJets*.root", mc2017_dir+"slim_*_TTZ*.root",mc2018_dir+"slim_*_TTWJets*.root", mc2018_dir+"slim_*_TTZ*.root"};

  auto  all_other = {mc2016_dir+"slim_*WW*.root"};

  auto all_wjets = {mc2016_dir+"slim_*WW*.root", mc2016_dir+"slim_*WZ*.root",mc2017_dir+"slim_*WW*.root", mc2017_dir+"slim_*WZ*.root",mc2018_dir+"slim_*WW*.root", mc2018_dir+"slim_*WZ*.root", mc2016_dir+"slim_W*JetsToLNu_s16v3*",mc2016_dir+"slim_*W*Jets_NuPt200_s16v*.root",mc2017_dir+"slim_W*JetsToLNu_f17v2*",mc2017_dir+"slim_*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim_*W*Jets_NuPt200_a18v1*.root"};
  //auto all_wjets = {mc2016_dir+"slim_W*JetsToLNu_s16v3*",mc2016_dir+"slim_W*Jets_NuPt200_s16v*.root",mc2017_dir+"slim_W*JetsToLNu_f17v2*",mc2017_dir+"slim_W*Jets_NuPt200_f17v2*.root",mc2018_dir+"slim_W*JetsToLNu_a18v1*",mc2018_dir+"slim_*W*Jets_NuPt200_a18v1*.root"};

  //// Contributions
  auto proc_wjets = Process::MakeShared<Baby_full>("W+jets 2016-2018", Process::Type::background, kCyan-3, all_wjets,"stitch"&&baselinef); // evt!=74125994
  auto proc_top = Process::MakeShared<Baby_full>("top 2016-2018", Process::Type::background, kRed,all_top,"stitch"&&baselinef);
  auto proc_other = Process::MakeShared<Baby_full>("TTV and VV 2016-2018", Process::Type::background, kRed,all_other,baselinef);
  auto proc_data =  Process::MakeShared<Baby_full>("Data", Process::Type::data, colors("data"),{data2016_dir+"slim_*data_2016*.root",data2017_dir+"slim_*data_2017*.root",data2018_dir+"slim_*data_2018*.root"},baselinef&&"(HLT_SingleEl==1||HLT_SingleMu==1||HLT_MET_MHT==1)");
  vector<shared_ptr<Process> > all_procs;
  if(!data_mode) all_procs = {proc_top,proc_wjets,proc_other};
  else  all_procs = {proc_data,proc_top,proc_wjets,proc_other};

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////// Defining cuts ///////////////////////////////////////////////

  // baseline defined above
  //string myVar;
  //string jup_denominator     = "pfmet>125&&mt_met_lep>150&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==0";
  //myVar = JMEvariation(jup_denominator, "jup");
  //std::cout << myVar << endl;

  // Denominator with all variations
  string den_string = "pfmet>125&&mt_met_lep>150&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==0";
  NamedFunc denominator         = den_string;
  NamedFunc jdown_denominator   = JMEvariation(den_string, "jdown");
  NamedFunc jup_denominator     = JMEvariation(den_string, "jup");
  NamedFunc resup_denominator   = METvariation(den_string, "resup");
  NamedFunc resdown_denominator = METvariation(den_string, "resdown");

  // Numerator definitaions
  string num_string = "pfmet>125&&mt_met_lep>150&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2";
  NamedFunc numerator           = num_string;
  NamedFunc jdown_numerator     = JMEvariation(num_string, "jdown");
  NamedFunc jup_numerator       = JMEvariation(num_string, "jup");
  NamedFunc resup_numerator     = METvariation(num_string, "resup");
  NamedFunc resdown_numerator   = METvariation(num_string, "resdown");
  

  vector<NamedFunc> deepAK8bins = {"nHiggs==0","nHiggs>=1"};
  vector<NamedFunc> jup_deepAK8bins = {"jup_nHiggs==0","jup_nHiggs>=1"};
  vector<NamedFunc> jdown_deepAK8bins = {"jdown_nHiggs==0","jdown_nHiggs>=1"};

  // variations for 3rd jet missing - small effect?
  vector<NamedFunc> njetbins = {"ngoodjets==2","ngoodjets==3"&&LeadingNonBJetPt_med<300.}; 
  vector<NamedFunc> jup_njetbins = {"jup_ngoodjets==2","jup_ngoodjets==3"&&LeadingNonBJetPt_med<300.}; 
  vector<NamedFunc> jdown_njetbins = {"jdown_ngoodjets==2","jdown_ngoodjets==3"&&LeadingNonBJetPt_med<300.}; 
 
  vector<NamedFunc> metbins = {"pfmet>125&&pfmet<=200","pfmet>200&&pfmet<=300","pfmet>300&&pfmet<=400","pfmet>400"};
  vector<NamedFunc> jup_metbins = {"pfmet_jup>125&&pfmet_jup<=200","pfmet_jup>200&&pfmet_jup<=300","pfmet_jup>300&&pfmet_jup<=400","pfmet_jup>400"};
  vector<NamedFunc> jdown_metbins = {"pfmet_jdown>125&&pfmet_jdown<=200","pfmet_jdown>200&&pfmet_jdown<=300","pfmet_jdown>300&&pfmet_jdown<=400","pfmet_jdown>400"};
  vector<NamedFunc> resup_metbins = {"pfmet_resup>125&&pfmet_resup<=200","pfmet_resup>200&&pfmet_resup<=300","pfmet_resup>300&&pfmet_resup<=400","pfmet_resup>400"};
  vector<NamedFunc> resdown_metbins = {"pfmet_resdown>125&&pfmet_resdown<=200","pfmet_resdown>200&&pfmet_resdown<=300","pfmet_resdown>300&&pfmet_resdown<=400","pfmet_resdown>400"};

  vector<NamedFunc> metbins_boosted = {"pfmet>125&&pfmet<=300" && njetbins[0],"pfmet>300" && njetbins[0],"pfmet>125&&pfmet<=300" && njetbins[1],"pfmet>300" && njetbins[1]/*,"pfmet>200"*/};
  vector<NamedFunc> jup_metbins_boosted = {"pfmet_jup>125&&pfmet_jup<=300" && jup_njetbins[0],"pfmet_jup>300" && jup_njetbins[0],"pfmet_jup>125&&pfmet_jup<=300" && jup_njetbins[1],"pfmet_jup>300" && jup_njetbins[1]};
  vector<NamedFunc> jdown_metbins_boosted = {"pfmet_jdown>125&&pfmet_jdown<=300" && jdown_njetbins[0],"pfmet_jdown>300" && jdown_njetbins[0],"pfmet_jdown>125&&pfmet_jdown<=300" && jdown_njetbins[1],"pfmet_jdown>300" && jdown_njetbins[1]};
  vector<NamedFunc> resup_metbins_boosted = {"pfmet_resup>125&&pfmet_resup<=300" && njetbins[0],"pfmet_resup>300" && njetbins[0],"pfmet_resup>125&&pfmet_resup<=300" && njetbins[1],"pfmet_resup>300" && njetbins[1]};
  vector<NamedFunc> resdown_metbins_boosted = {"pfmet_resdown>125&&pfmet_resdown<=300" && njetbins[0],"pfmet_resdown>300" && njetbins[0],"pfmet_resdown>125&&pfmet_resdown<=300" && njetbins[1],"pfmet_resdown>300" && njetbins[1]};

  vector<NamedFunc> denominators = {denominator,denominator,denominator,denominator};
  vector<NamedFunc> jup_denominators = {jup_denominator,jup_denominator,jup_denominator,jup_denominator};
  vector<NamedFunc> jdown_denominators = {jdown_denominator,jdown_denominator,jdown_denominator,jdown_denominator};
  vector<NamedFunc> resup_denominators = {resup_denominator,resup_denominator,resup_denominator,resup_denominator};
  vector<NamedFunc> resdown_denominators = {resdown_denominator,resdown_denominator,resdown_denominator,resdown_denominator};

  vector<NamedFunc> numerators = {numerator,numerator,numerator,numerator};
  vector<NamedFunc> numerators_0H = {numerator&&deepAK8bins[0],numerator&&deepAK8bins[0],numerator&&deepAK8bins[0],numerator&&deepAK8bins[0]};
  vector<NamedFunc> jup_numerators = {jup_numerator,jup_numerator,jup_numerator,jup_numerator};
  vector<NamedFunc> jup_numerators_0H = {jup_numerator&&deepAK8bins[0],jup_numerator&&deepAK8bins[0],jup_numerator&&deepAK8bins[0],jup_numerator&&deepAK8bins[0]};
  vector<NamedFunc> jdown_numerators = {jdown_numerator,jdown_numerator,jdown_numerator,jdown_numerator};
  vector<NamedFunc> jdown_numerators_0H = {jdown_numerator&&deepAK8bins[0],jdown_numerator&&deepAK8bins[0],jdown_numerator&&deepAK8bins[0],jdown_numerator&&deepAK8bins[0]};
  vector<NamedFunc> resup_numerators = {resup_numerator,resup_numerator,resup_numerator,resup_numerator};
  vector<NamedFunc> resup_numerators_0H = {resup_numerator&&deepAK8bins[0],resup_numerator&&deepAK8bins[0],resup_numerator&&deepAK8bins[0],resup_numerator&&deepAK8bins[0]};
  vector<NamedFunc> resdown_numerators = {resdown_numerator,resdown_numerator,resdown_numerator,resdown_numerator};
  vector<NamedFunc> resdown_numerators_0H = {resdown_numerator&&deepAK8bins[0],resdown_numerator&&deepAK8bins[0],resdown_numerator&&deepAK8bins[0],resdown_numerator&&deepAK8bins[0]};

  vector<NamedFunc> numerators_boosted = {numerator,numerator,numerator,numerator};
  vector<NamedFunc> numerators_boosted_1H = {numerator&& deepAK8bins[1],numerator&& deepAK8bins[1],numerator&& deepAK8bins[1],numerator&& deepAK8bins[1]};
  vector<NamedFunc> jup_numerators_boosted = {jup_numerator,jup_numerator,jup_numerator,jup_numerator};
  vector<NamedFunc> jup_numerators_boosted_1H = {jup_numerator&&deepAK8bins[1],jup_numerator&&deepAK8bins[1],jup_numerator&&deepAK8bins[1],jup_numerator&&deepAK8bins[1]};
  vector<NamedFunc> jdown_numerators_boosted = {jdown_numerator,jdown_numerator,jdown_numerator,jdown_numerator};
  vector<NamedFunc> jdown_numerators_boosted_1H = {jdown_numerator&&deepAK8bins[1],jdown_numerator&&deepAK8bins[1],jdown_numerator&&deepAK8bins[1],jdown_numerator&&deepAK8bins[1]};
  vector<NamedFunc> resup_numerators_boosted = {resup_numerator,resup_numerator,resup_numerator,resup_numerator};
  vector<NamedFunc> resup_numerators_boosted_1H = {resup_numerator&&deepAK8bins[1],resup_numerator&&deepAK8bins[1],resup_numerator&&deepAK8bins[1],resup_numerator&&deepAK8bins[1]};
  vector<NamedFunc> resdown_numerators_boosted = {resdown_numerator,resdown_numerator,resdown_numerator,resdown_numerator};
  vector<NamedFunc> resdown_numerators_boosted_1H = {resdown_numerator&&deepAK8bins[1],resdown_numerator&&deepAK8bins[1],resdown_numerator&&deepAK8bins[1],resdown_numerator&&deepAK8bins[1]};

  // Makes a plot for each vector in plotcuts
  vector<oneplot> plotcuts({
    {true, "pfmet_SR", "(1)"      && njetbins[0], metbins,         denominators,    numerators},
    {true, "pfmet_SR", "(1)"      && njetbins[1], metbins,         denominators,    numerators},
    {true, "pfmet_SR_H0", "(1)"  && njetbins[0], metbins,          denominators,    numerators_0H}, //numerator and denominator are the same, except the higgs requirement
    {true, "pfmet_SR_H0", "(1)"  && njetbins[1], metbins,          denominators,    numerators_0H},
    {true, "pfmet_SR_H1", "(1)"  ,               metbins_boosted,  denominators,    numerators_boosted_1H},
  });

  vector<oneplot> jup_plotcuts({
    {true, "pfmet_SR", "(1)"      && jup_njetbins[0], jup_metbins,            jup_denominators,         jup_numerators},
    {true, "pfmet_SR", "(1)"      && jup_njetbins[1], jup_metbins,            jup_denominators,         jup_numerators},
    {true, "pfmet_SR_H0", "(1)"  && jup_njetbins[0], jup_metbins,            jup_denominators,           jup_numerators_0H},
    {true, "pfmet_SR_H0", "(1)"  && jup_njetbins[1], jup_metbins,            jup_denominators,           jup_numerators_0H},
    {true, "pfmet_SR_H1", "(1)"  ,                   jup_metbins_boosted,    jup_denominators,   jup_numerators_boosted_1H},
  });

  vector<oneplot> jdown_plotcuts({
    {true, "pfmet_SR", "(1)"      && jdown_njetbins[0], jdown_metbins,          jdown_denominators,         jdown_numerators},
    {true, "pfmet_SR", "(1)"      && jdown_njetbins[1], jdown_metbins,          jdown_denominators,         jdown_numerators},
    {true, "pfmet_SR_H0", "(1)"  && jdown_njetbins[0], jdown_metbins,          jdown_denominators,           jdown_numerators_0H},
    {true, "pfmet_SR_H0", "(1)"  && jdown_njetbins[1], jdown_metbins,          jdown_denominators,           jdown_numerators_0H},
    {true, "pfmet_SR_H1", "(1)"  ,                     jdown_metbins_boosted,  jdown_denominators,   jdown_numerators_boosted_1H},
  });

  vector<oneplot> resup_plotcuts({
    {true, "pfmet_SR", "(1)"      && njetbins[0], resup_metbins,          resup_denominators,       resup_numerators},
    {true, "pfmet_SR", "(1)"      && njetbins[1], resup_metbins,          resup_denominators,       resup_numerators},
    {true, "pfmet_SR_H0", "(1)"  && njetbins[0], resup_metbins,          resup_denominators,         resup_numerators_0H},
    {true, "pfmet_SR_H0", "(1)"  && njetbins[1], resup_metbins,          resup_denominators,         resup_numerators_0H},
    {true, "pfmet_SR_H1", "(1)"  ,               resup_metbins_boosted,  resup_denominators, resup_numerators_boosted_1H},
  });

  vector<oneplot> resdown_plotcuts({
    {true, "pfmet_SR", "(1)"      && njetbins[0], resdown_metbins,          resdown_denominators,       resdown_numerators},
    {true, "pfmet_SR", "(1)"      && njetbins[1], resdown_metbins,          resdown_denominators,       resdown_numerators},
    {true, "pfmet_SR_H0", "(1)"  && njetbins[0], resdown_metbins,          resdown_denominators,         resdown_numerators_0H},
    {true, "pfmet_SR_H0", "(1)"  && njetbins[1], resdown_metbins,          resdown_denominators,         resdown_numerators_0H},
    {true, "pfmet_SR_H1", "(1)"  ,               resdown_metbins_boosted,  resdown_denominators, resdown_numerators_boosted_1H},
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
    if(data_mode) tag += "data_";

  vector<NamedFunc> weights; size_t nsels; //vector<NamedFunc> numerators,denominators;
  vector<TString> leglabels;
  vector<string> cuts;
  /////// Systematic weight mode //////
  if(systematic_mode){ // Same numerator and denominator, vary weights
     weights= {  "weight * w_higgsSF" * yearWeight,
                 "weight * w_higgsSF * w_puUp" * yearWeight,
                 "weight * w_higgsSF * w_puDown" * yearWeight,
                 "weight * w_higgsSF* w_btagLFUp" * yearWeight,
                 "weight * w_higgsSF* w_btagLFDown" * yearWeight,
                 "weight * w_higgsSF* w_btagHFUp" * yearWeight,
                 "weight * w_higgsSF* w_btagHFDown" * yearWeight,
                 "weight * w_higgsSF" * yearWeight,
                 "weight * w_higgsSF" * yearWeight,
                 "weight * w_higgsSF" * yearWeight,
                 "weight * w_higgsSF" * yearWeight,
                 "weight * w_higgsSF" * yearWeight * (1+0.2*hasGenBs),
                 "weight * w_higgsSF" * yearWeight * (1-0.2*hasGenBs),
                 "weight * w_higgsSFUp" * yearWeight,
                 "weight * w_higgsSFDown" * yearWeight,
                 "weight * w_higgsSF" * VV_up * yearWeight,
                 "weight * w_higgsSF" * VV_down * yearWeight,
                  };
     tag+="wjets1_";
     leglabels = {"Nominal", "PU up", "PU up/down", "b-tag mistag up","b-tag mistag up/down","b-tag HF up","b-tag HF up/down", "JES up", "JES up/down", "MET res up", "MET res up/down", "true b up", "true b up/down", "Higgs up", "Higgs up/down", "VV up", "VV up/down"};

    // theory uncertainties (q2, alpha_s, PDF, ISR) tbd.

     cuts = {   "(1)",
                "(1)",
                "(1)",
                "(1)",
                "(1)",
                "(1)",
                "(1)",
                "jup",
                "jdown",
                "resup",
                "resdown",
                "(1)",
                "(1)",
                "(1)",
                "(1)",
                "(1)",
                "(1)",
            };

    nsels = 2*weights.size();

  } //systematic mode

  if(year_mode){ 
    cuts= { "1","year==2016","year==2017","year==2018"};
    leglabels = {"Full Run II","2016 only","2017 only","2018 only"};
   
    for(uint ivar=0;ivar<cuts.size();ivar++){
      weights.push_back("weight" * yearWeight);
    }
    nsels = 2*cuts.size();

  } //year mode

    if(data_mode){ 
    cuts= { "1","1"}; //data, mc
    leglabels = {"Data, 2016-2018","MC, 2016-2018"};
   
    for(uint ivar=0;ivar<cuts.size();ivar++){
      weights.push_back("weight" * yearWeight);
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
    Bkgs num = w;
    if(data_mode) num = w;

    if(data_mode && i%2==0) indices.push_back(vector<vector<int> >({{proc_options[data],2*i,-1},{proc_options[data],2*i+1,1}})); //data ratios are even indices
    else indices.push_back(vector<vector<int> >({{proc_options[num],2*i,-1},{proc_options[num],2*i+1,1}}));
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
          {proc_options[num],2*i,-1},{proc_options[num],2*i+1,1},
      {proc_options[num],0,1},{proc_options[num],1,-1} //Assume 0th is nominal.
    }));
      }
      else{
        indices_double_ratio.push_back(vector<vector<int> >({
          {proc_options[num],2*i,-1},{proc_options[num],2*i+1,1}
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
        if (cuts[ivar]=="jup"){
          std::cout << "in jup" << std::endl;
          NamedFunc totcut= jup_plotcuts[iplot].baseline && jup_plotcuts[iplot].bincuts[ibin] && jup_plotcuts[iplot].denSel[ibin];
          table_cuts_den.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
          allcuts[iplot][2*ivar].push_back(totcut);
          cout<<"Denominator: "<<totcut.Name()<<endl;
        }
        else if (cuts[ivar]=="jdown"){
          std::cout << "in jdown" << std::endl;
          NamedFunc totcut= jdown_plotcuts[iplot].baseline && jdown_plotcuts[iplot].bincuts[ibin] && jdown_plotcuts[iplot].denSel[ibin];
          table_cuts_den.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
          allcuts[iplot][2*ivar].push_back(totcut);
          cout<<"Denominator: "<<totcut.Name()<<endl;
        }
        else if (cuts[ivar]=="resup"){
          std::cout << "in resup" << std::endl;
          NamedFunc totcut= resup_plotcuts[iplot].baseline && resup_plotcuts[iplot].bincuts[ibin] && resup_plotcuts[iplot].denSel[ibin];
          table_cuts_den.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
          allcuts[iplot][2*ivar].push_back(totcut);
          cout<<"Denominator: "<<totcut.Name()<<endl;
        }
        else if (cuts[ivar]=="resdown"){
          std::cout << "in resdown" << std::endl;
          NamedFunc totcut= resdown_plotcuts[iplot].baseline && resdown_plotcuts[iplot].bincuts[ibin] && resdown_plotcuts[iplot].denSel[ibin];
          table_cuts_den.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
          allcuts[iplot][2*ivar].push_back(totcut);
          cout<<"Denominator: "<<totcut.Name()<<endl;
        }
        else{
          NamedFunc totcut= plotcuts[iplot].baseline && plotcuts[iplot].bincuts[ibin] && plotcuts[iplot].denSel[ibin] && cuts[ivar];
          table_cuts_den.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
          allcuts[iplot][2*ivar].push_back(totcut);
          cout<<"Denominator: "<<totcut.Name()<<endl;
        }

      }
      TString tname = "rw"; tname += iplot; tname += 2*ivar;
      pm.Push<Table>(tname.Data(),  table_cuts_den, all_procs, false, false);
      vector<TableRow> table_cuts_num;

      for(size_t ibin=0; ibin<plotcuts[iplot].bincuts.size(); ibin++){

        if (cuts[ivar]=="jup"){
          std::cout << "in jup" << std::endl;
          NamedFunc totcut= jup_plotcuts[iplot].baseline && jup_plotcuts[iplot].bincuts[ibin] && jup_plotcuts[iplot].numSel[ibin];
          table_cuts_num.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
          allcuts[iplot][2*ivar+1].push_back(totcut);
        }
        else if (cuts[ivar]=="jdown"){
          std::cout << "in jdown" << std::endl;
          NamedFunc totcut= jdown_plotcuts[iplot].baseline && jdown_plotcuts[iplot].bincuts[ibin] && jdown_plotcuts[iplot].numSel[ibin];
          table_cuts_num.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
          allcuts[iplot][2*ivar+1].push_back(totcut);
        }
        else if (cuts[ivar]=="resup"){
          std::cout << "in resup" << std::endl;
          NamedFunc totcut= resup_plotcuts[iplot].baseline && resup_plotcuts[iplot].bincuts[ibin] && resup_plotcuts[iplot].numSel[ibin];
          table_cuts_num.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
          allcuts[iplot][2*ivar+1].push_back(totcut);
        }
        else if (cuts[ivar]=="resdown"){
          std::cout << "in resdown" << std::endl;
          NamedFunc totcut= resdown_plotcuts[iplot].baseline && resdown_plotcuts[iplot].bincuts[ibin] && resdown_plotcuts[iplot].numSel[ibin];
          table_cuts_num.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
          allcuts[iplot][2*ivar+1].push_back(totcut);
        }
        else{
          NamedFunc totcut= plotcuts[iplot].baseline && plotcuts[iplot].bincuts[ibin] && plotcuts[iplot].numSel[ibin] && cuts[ivar];
          table_cuts_num.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[ivar]));
          allcuts[iplot][2*ivar+1].push_back(totcut);
        }
        // NamedFunc totcut=plotcuts[iplot].baseline && plotcuts[iplot].bincuts[ibin] && numerators[ivar];
        //NamedFunc totcut=plotcuts[iplot].baseline && plotcuts[iplot].bincuts[ibin] && plotcuts[iplot].numSel[ibin] && cuts[ivar];
       // TString totcut=plotcuts[iplot].baseline+" && "+plotcuts[iplot].bincuts[ibin]+" && "+numerators[ivar];
      } // Loop over bins
      tname = "rw"; tname += iplot; tname += 2*ivar+1; 
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
