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
  bool systematic_mode=true;
  bool debug = true;
  float lumi=137.;

  enum Bkgs {bkg, top, w, other};

  struct oneplot{
    TString name;
    NamedFunc baseline;
    vector<NamedFunc> bincuts;
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
        entries.push_back(vector<float>());
        weights.push_back(vector<float>());
        entries.back().push_back(allyields[ibkg][iabcd][ibin].NEffective());
        weights.back().push_back(allyields[ibkg][iabcd][ibin].Weight());
      } // Loop over indices

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

  float minx = 0.5, maxx = nbins+0.5, miny = minr*0.55, maxy = maxr*1.35;
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
  TLegend leg(legX, legY-legH, legX+legW, legY);
//  leg.SetTextSize(opts.LegendEntryHeight()); 
  leg.SetFillColor(0);
  // leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(42);
  leg.SetNColumns(2/*ngraphs*/);

  Palette colors("txt/colors.txt", "default");
  //vector<int> mcolors({kRed, kGreen+1, 4, kMagenta+2});
  vector<int> mcolors({1,kRed,kRed, kGreen-3,kGreen-3, kCyan-3,kCyan-3, kMagenta+2,kMagenta+2});
  vector<int> styles({20, 22, 23,22,23,22,23,22,23,22,23,22,23});
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
  cmslabel.DrawLatex(opts.LeftMargin()+0.005, 1-opts.TopMargin()+0.015,"#font[62]{CMS} #scale[0.8]{#font[52]{Simulation}}");
  cmslabel.SetTextAlign(31);
  cmslabel.DrawLatex(1-opts.RightMargin()-0.005, 1-opts.TopMargin()+0.015,"#font[42]{13 TeV}");

  line.SetLineStyle(3); line.SetLineWidth(1);
  line.DrawLine(minx, 1, maxx, 1);
  string syst_tag = "";
  if(systematic_mode) syst_tag = "syst_";
  string fname="plots/ratio_"+syst_tag+CodeToPlainText(tag.Data())+"_"+CodeToPlainText(ytitle.Data())+"_"+plotdef.name.Data()+"_"+CodeToPlainText(plotdef.baseline.Name())+".pdf";
  can.SaveAs(fname.c_str());
  cout<<endl<<" open "<<fname<<endl;

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


  Palette colors("txt/colors.txt", "default");

  // Cuts in baseline speed up the yield finding
  string baseline="";
  NamedFunc baselinef = "pass&&nvetoleps>=1&&ngoodjets>=2 && ngoodjets<=3 &&pfmet>125&&mt_met_lep>150 && ngoodbtags==2 && nWHLeptons>=1";
  TString single_lep ="nvetoleps==1&&PassTrackVeto&&PassTauVeto&&nWHLeptons==1";
  TString dilep="nvetoleps==2";

  string mc2016_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/s16v3/";
  string mc2016_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/s16v3/";

  string mc2017_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/f17v2/";
  string mc2017_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_05/f17v2/";

  string mc2018_dir = "/home/users/rheller/wh_babies/babies_v31_2_2019_10_03/a18v1/";
  string mc2018_dir_ttmet = "/home/users/rheller/wh_babies/babies_v30_9_2019_10_07/a18v1/";


  auto all_top = {mc2016_dir+"*TTJets_1lep_top_s16v3*.root",mc2016_dir+"*TTJets_1lep_tbar_s16v3*",mc2016_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2017_dir+"*TTJets_1lep_top_f17v2*.root",mc2017_dir+"*TTJets_1lep_tbar_f17v2*",mc2017_dir_ttmet+"*TTJets_1lep_*met150*.root",mc2018_dir+"*TTJets_1lep_top_a18v1*.root",mc2018_dir+"*TTJets_1lep_tbar_a18v1*",mc2018_dir_ttmet+"*TTJets_1lep_*met80*.root",mc2016_dir+"*_ST_*.root",mc2017_dir+"*_ST_*.root",mc2018_dir+"*_ST_*.root",mc2016_dir+"*TTJets_2lep_s16v3*.root", mc2016_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2017_dir+"*TTJets_2lep_f17v2*.root", mc2017_dir_ttmet+"*TTJets_2lep_*met150*.root",mc2018_dir+"*TTJets_2lep_a18v1*.root",mc2018_dir_ttmet+"*TTJets_2lep_*met80*.root"};


  auto all_other = {mc2016_dir+"*WW*.root", mc2016_dir+"*WZ*.root",mc2016_dir+"*ZZ*.root",mc2017_dir+"*WW*.root", mc2017_dir+"*WZ*.root",mc2017_dir+"*ZZ*.root",mc2018_dir+"*WW*.root", mc2018_dir+"*WZ*.root",mc2018_dir+"*ZZ*.root",mc2016_dir+"*_TTWJets*.root", mc2016_dir+"*_TTZ*.root",mc2017_dir+"*_TTWJets*.root", mc2017_dir+"*_TTZ*.root",mc2018_dir+"*_TTWJets*.root", mc2018_dir+"*_TTZ*.root"};

  //// Contributions
  auto proc_wjets = Process::MakeShared<Baby_full>("W+jets 2016-2018", Process::Type::background, kCyan-3, {mc2016_dir+"*slim_W*JetsToLNu_s16v3*",mc2016_dir+"*W*Jets_NuPt200_s16v*.root",mc2017_dir+"*slim_W*JetsToLNu_f17v2*",mc2017_dir+"*W*Jets_NuPt200_f17v2*.root",mc2018_dir+"*slim_W*JetsToLNu_a18v1*",mc2018_dir+"*W*Jets_NuPt200_a18v1*.root"},"stitch&&evt!=74125994"&&baselinef);
  auto proc_top = Process::MakeShared<Baby_full>("top 2016-2018", Process::Type::background, kRed,all_top,"stitch"&&baselinef);

  auto proc_other = Process::MakeShared<Baby_full>("TTV and VV 2016-2018", Process::Type::background, kRed,all_other,baselinef);



  vector<shared_ptr<Process> > all_procs = {proc_top,proc_wjets,proc_other};

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////// Defining cuts ///////////////////////////////////////////////
  // baseline defined above

  // Makes a plot for each vector in plotcuts
  vector<oneplot> plotcuts({{"ngoodjets", "pfmet>200", {"ngoodjets==2", "ngoodjets==3"&&LeadingNonBJetPt_med<100.}},
  {"pfmet", "ngoodjets==2", {"pfmet>125 && pfmet<=200","pfmet>200 && pfmet<=300","pfmet>300"}},
	{"pfmet", "ngoodjets==3"&&LeadingNonBJetPt_med<100., {"pfmet>125 && pfmet<=200","pfmet>200 && pfmet<=300","pfmet>300"}}});

  vector<NamedFunc> weights; size_t nsels; vector<TString> numerators,denominators;
  vector<TString> leglabels;

  /////// Systematic weight mode //////
  if(systematic_mode){ // Same numerator and denominator, vary weights
    weights= { "weight * w_pu" * yearWeight,
                "weight * w_pu" * yearWeight * ST_up,
                "weight * w_pu" * yearWeight * ST_down,
                "weight * w_pu * w_pu" * yearWeight,
                "weight" * yearWeight,
                "weight * w_pu" * yearWeight * ttbar_genmet_fix,
                "weight * w_pu" * yearWeight * ttbar_genmet_antifix,
                "weight * w_pu" * yearWeight * fake_up,
                "weight * w_pu" * yearWeight * fake_down
                 };

    leglabels = {"Nominal"/* 1-lepton m_{bb} on higgs"*/,"Single top x1.5","Single top x0.5","PU weights squared","PU weights off","Reweight genMET to 2016","Reweight genMET away from 2016","b-tag fake rate x1.5","b-tag fake rate x0.5"};

    nsels = 2*weights.size();

    TString numerator = single_lep+"&&mct>200&&mbb>90&&mbb<150";
    TString denominator = single_lep+"&&mct>150&&mct<=200&&mbb>90&&mbb<150";


    for(uint ivar=0;ivar<weights.size();ivar++){
      numerators.push_back(numerator);
      denominators.push_back(denominator);
    }
  } //systematic mode

  /////// Selection variation mode //////
  else{ // same weights, vary numerator/denominator selections
    numerators = {single_lep+"&&mct>200",
                  single_lep+"&&mct>200&&mbb>90&&mbb<150",
                  dilep+"&&mct>200",
                  dilep+"&&mct>200&&mbb>90&&mbb<150"};

    denominators = {single_lep+"&&mct>150&&mct<=200",
                    single_lep+"&&mct>150&&mct<=200&&mbb>90&&mbb<150",
                    dilep+"&&mct>150&&mct<=200",
                    dilep+"&&mct>150&&mct<=200&&mbb>90&&mbb<150"};

    leglabels = {"1-lepton inclusive m_{bb}","1-lepton m_{bb} on higgs","2-lepton inclusive m_{bb}","2-lepton m_{bb} on higgs"};

    for(uint ivar=0;ivar<numerators.size();ivar++){
        weights.push_back("weight * w_pu" * yearWeight);

    nsels = 2*weights.size();
    }
  }//selection mode


  vector<int> proc_options = {0,1};//allbkg, top only
  vector<TString> tags = {"allbkg","top"};

  vector<vector<vector<int> > > indices;
  vector<vector<vector<int> > > indices_double_ratio;

  for(int i=0;i<static_cast<int>(denominators.size());i++){
    //numerator is always top only; W predicted separately.
    indices.push_back(vector<vector<int> >({{proc_options[bkg],2*i,-1},{proc_options[top],2*i+1,1}}));
    if(i>0){
    indices_double_ratio.push_back(vector<vector<int> >({
      {proc_options[bkg],2*i,-1},{proc_options[top],2*i+1,1},
      {proc_options[bkg],0,1},{proc_options[top],1,-1} //Assume 0th is nominal.
    }));
    }
    else{
      indices_double_ratio.push_back(vector<vector<int> >({
        {proc_options[bkg],2*i,-1},{proc_options[top],2*i+1,1}
      }));
    }

  }


  PlotMaker pm;
  vector<vector<vector<NamedFunc> > > allcuts(plotcuts.size(), vector<vector<NamedFunc> > (nsels));
  //vector<vector<vector<TString> > > allcuts(plotcuts.size(), vector<vector<TString> > (nsels));
  for(size_t iplot=0; iplot<plotcuts.size(); iplot++){
    for(size_t idens=0; idens<denominators.size(); idens++){
      vector<TableRow> table_cuts_den;
      for(size_t ibin=0; ibin<plotcuts[iplot].bincuts.size(); ibin++){
        //TString totcut=plotcuts[iplot].baseline+" && "+plotcuts[iplot].bincuts[ibin]+" && "+denominators[idens];
        NamedFunc totcut=plotcuts[iplot].baseline && plotcuts[iplot].bincuts[ibin] && denominators[idens];
        table_cuts_den.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[idens]));
        allcuts[iplot][2*idens].push_back(totcut);
      }
      TString tname = "rmct"; tname += iplot; tname += 2*idens;
      pm.Push<Table>(tname.Data(),  table_cuts_den, all_procs, false, false);
      vector<TableRow> table_cuts_num;
      for(size_t ibin=0; ibin<plotcuts[iplot].bincuts.size(); ibin++){

        NamedFunc totcut=plotcuts[iplot].baseline && plotcuts[iplot].bincuts[ibin] && numerators[idens];
       // TString totcut=plotcuts[iplot].baseline+" && "+plotcuts[iplot].bincuts[ibin]+" && "+numerators[idens];
        table_cuts_num.push_back(TableRow("", totcut/*.Data()*/,0,0,weights[idens]));
        allcuts[iplot][2*idens+1].push_back(totcut);
      } // Loop over bins
      tname = "rmct"; tname += iplot; tname += 2*idens+1; 
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
    // allyields: [0] All bkg, [1] tt1l, [2] tt2l, [3] other
    vector<vector<vector<GammaParams> > > allyields(4,vector<vector<GammaParams> >(nsels));
    for(size_t idens=0; idens<denominators.size(); idens++){
      Table * yield_table = static_cast<Table*>(pm.Figures()[iplot*nsels+2*idens].get());
      allyields[bkg]  [2*idens] = yield_table->BackgroundYield(lumi);
      //cout<<"allyields "<<allyields[bkg]  [idens]<<endl;
      allyields[top] [2*idens] = yield_table->Yield(proc_top.get(), lumi);
      allyields[w] [2*idens] = yield_table->Yield(proc_wjets.get(), lumi);
      allyields[other] [2*idens] = yield_table->Yield(proc_other.get(), lumi);

      yield_table = static_cast<Table*>(pm.Figures()[iplot*nsels+2*idens+1].get());
      allyields[bkg]  [2*idens+1] = yield_table->BackgroundYield(lumi);
      //cout<<"allyields "<<allyields[bkg]  [2*idens+1]<<endl;
      allyields[top] [2*idens+1] = yield_table->Yield(proc_top.get(), lumi);
      allyields[w] [2*idens+1] = yield_table->Yield(proc_wjets.get(), lumi);
      allyields[other] [2*idens+1] = yield_table->Yield(proc_other.get(), lumi);
      // allyields[other][iabcd] = yield_table->Yield(proc_other.get(), lumi);
    } // Loop over ABCD cuts

    //// Print MC/Data yields, cuts applied, kappas, preds
    if(debug) printDebug(allcuts[iplot], allyields, baseline);


    plotRatio(allyields, plotcuts[iplot], indices, leglabels,systematic_mode,"allproc");
    if(systematic_mode) plotRatio(allyields, plotcuts[iplot], indices_double_ratio, leglabels,systematic_mode,"double_allproc");

   vector<vector<vector<int> > > indices_top = indices;
   vector<vector<vector<int> > > indices_top_double_ratio = indices_double_ratio;
   for(uint il=0;il<indices_top.size();il++){
    for(uint ij=0;ij<indices_top[il].size();ij++){
      indices_top[il][ij][0]=1; //convert "bkg" to "top"
      indices_top_double_ratio[il][ij][0]=1; //convert "bkg" to "top"
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
	  <<" other: "<<setw(9)<<RoundNumber(allyields[other][iabcd][ibin].Yield(), digits)
	  <<"  - "<< allcuts[iabcd][ibin].Name()<<endl;
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
