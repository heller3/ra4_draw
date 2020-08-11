
#include <cstdlib>
#include <iostream>
#include <fstream>
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
  

  /*double lumi2016 = 35.9;
  double lumi2017 = 41.6;
  double lumi2018 = 59.7;*/
  double lumicomb = 137.2;

  string signal_dir = "/home/users/dspitzba/wh_babies/babies_v33_4_2020_07_09/";

  /*string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname,"compute-")){
    signal_dir = "/net/cms29";
  }*/
 
  Palette colors("txt/colors.txt", "default");

  //signals

  /*auto signal_comb_800_1 = Process::MakeShared<Baby_full>("2016-2018 Signal (800,1)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_SMS_TChiWH*.root"},"pass&&mass_stop==800&&mass_lsp==1");
  auto signal_comb_400_150 = Process::MakeShared<Baby_full>("2016-2018 Signal (450,150)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_SMS_TChiWH*.root"},"pass&&mass_stop==400&&mass_lsp==150");
  auto signal_comb_225_75 = Process::MakeShared<Baby_full>("2016-2018 Signal (225,75)", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_SMS_TChiWH*.root"},"pass&&mass_stop==225&&mass_lsp==75");
*/

  //FIND ALL SIGNAL MASS POINTS
  TChain sig_tree("t");
  sig_tree.Add("/home/users/dspitzba/wh_babies/babies_v33_4_2020_07_09/slim_SMS_TChiWH_s16v3_0.root");

  long nentries(sig_tree.GetEntries());
  cout<<"Got "<<nentries<<" entries."<<endl;
  // TString outfolder = outpath;
  // outfolder.Remove(outfolder.Last('/')+1, outfolder.Length());
  // if(outfolder!="") gSystem->mkdir(outfolder, kTRUE);

  //Project sig_tree into mass plane to find points for skim
  TH2F * mass_plane = new TH2F("mchi_vs_mlsp","mchi_vs_mlsp",1000,-0.5,999.5,1000,-0.5,999.5);
  sig_tree.Project("mchi_vs_mlsp","mass_stop:mass_lsp","","colz");

  int ini_x = mass_plane->FindFirstBinAbove(0,1);
  int last_x = mass_plane->FindLastBinAbove(0,1);
  int ini_y = mass_plane->FindFirstBinAbove(0,2);
  int last_y = mass_plane->FindLastBinAbove(0,2);

  //load all pairs as cuts into vector
  vector<TString> pair_cuts;
  vector<TString> mass_tag;
  vector<float> v_mchi;
  vector<float> v_mlsp;

  int Npoints=0;
  for(int iy=ini_y; iy<=last_y; iy++){
    for(int ix=ini_x; ix<=last_x; ix++){
      if(mass_plane->GetBinContent(ix,iy) > 0){
        int mchi = static_cast<int>(mass_plane->GetYaxis()->GetBinCenter(iy));
        int mlsp = static_cast<int>(mass_plane->GetXaxis()->GetBinCenter(ix));
        //if(mchi!=175) continue;
        pair_cuts.push_back(Form("mass_stop==%i&&mass_lsp==%i",mchi,mlsp));
        mass_tag.push_back(Form("mChi-%i_mLSP-%i_",mchi,mlsp));
        v_mchi.push_back(mchi);
        v_mlsp.push_back(mlsp);
        cout<<"Found mass point "<<mass_tag.back()<<endl;
  Npoints++;
      }
    }
  }

  //DEFINE A PROCESS FOR EACH SIGNAL MASS POINT FOUND
  vector<shared_ptr<Process> > sample_list_comb;
  for(unsigned iSig(0); iSig<mass_tag.size(); iSig++){

    auto signal_var = Process::MakeShared<Baby_full>("2016-2018 Signal", Process::Type::signal, colors("t1tttt"),{signal_dir+"slim_SMS_TChiWH*.root"},"pass&&"+pair_cuts[iSig]);
    sample_list_comb.push_back(signal_var);

  }//close for loop over mass_tag vector
  
  PlotOpt log_lumi("txt/plot_styles.txt", "CMSPaper");
  log_lumi.Title(TitleType::preliminary)
    .Bottom(BottomType::ratio)
    .YAxis(YAxisType::log)
    .Stack(StackType::data_norm);
  PlotOpt lin_lumi = log_lumi().YAxis(YAxisType::linear);
  PlotOpt log_shapes = log_lumi().Stack(StackType::shapes)
    .ShowBackgroundError(false);
  PlotOpt lin_shapes = log_shapes().YAxis(YAxisType::linear);
  PlotOpt log_lumi_info = log_lumi().Title(TitleType::info);
  PlotOpt lin_lumi_info = lin_lumi().Title(TitleType::info);
  PlotOpt log_shapes_info = log_shapes().Title(TitleType::info);
  PlotOpt lin_shapes_info = lin_shapes().Title(TitleType::info);
  vector<PlotOpt> all_plot_types = {log_lumi, lin_lumi, log_shapes, lin_shapes,
                                    log_lumi_info, lin_lumi_info, log_shapes_info, lin_shapes_info};
  PlotOpt style2D("txt/plot_styles.txt", "Scatter");
  vector<PlotOpt> bkg_hist = {style2D().Stack(StackType::data_norm).Title(TitleType::preliminary)};
  vector<PlotOpt> bkg_pts = {style2D().Stack(StackType::lumi_shapes).Title(TitleType::info)};
  


  PlotMaker * pmcomb = new PlotMaker();


    Table & signalRegion_125to200_resolved_cutflow_comb = pmcomb->Push<Table>("signalRegion_125to200_resolved_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jup>125&&pfmet_jup<200&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jdown>125&&pfmet_jdown<200&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jerup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jerdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&genmet>125&&genmet<200&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&genPt_ak8_higgsTagger==0.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&reduced_MET>125&&reduced_MET<200&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);



    Table & signalRegion_125to300_boosted_cutflow_comb = pmcomb->Push<Table>("signalRegion_125to300_boosted_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jup>125&&pfmet_jup<300&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jdown>125&&pfmet_jdown<300&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jerup_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jerdown_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&genmet>125&&genmet<300&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&genPt_ak8_higgsTagger==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&reduced_MET>125&&reduced_MET<300&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_200to300_resolved_cutflow_comb = pmcomb->Push<Table>("signalRegion_200to300_resolved_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jup>200&&pfmet_jup<300&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jdown>200&&pfmet_jdown<300&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jerup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jerdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&genmet>200&&genmet<300&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&genPt_ak8_higgsTagger==0.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&reduced_MET>200&&reduced_MET<300&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_300plus_boosted_cutflow_comb = pmcomb->Push<Table>("signalRegion_300plus_boosted_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jup>300&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jdown>300&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jerup_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jerdown_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&genmet>300&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&genPt_ak8_higgsTagger==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&reduced_MET>300&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_300to400_resolved_cutflow_comb = pmcomb->Push<Table>("signalRegion_300to400_resolved_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jup>300&&pfmet_jup<400&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jdown>300&&pfmet_jdown<400&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jerup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jerdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&genmet>300&&genmet<400&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&genPt_ak8_higgsTagger==0.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&reduced_MET>300&&reduced_MET<400&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_400plus_resolved_cutflow_comb = pmcomb->Push<Table>("signalRegion_400plus_resolved_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jup>400&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet_jdown>400&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jerup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jerdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&genmet>400&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&genPt_ak8_higgsTagger==0.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&reduced_MET>400&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==2&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_3jet_125to200_resolved_cutflow_comb = pmcomb->Push<Table>("signalRegion_3jet_125to200_resolved_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jup>125&&pfmet_jup<200&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jdown>125&&pfmet_jdown<200&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jerup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jerdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&genmet>125&&genmet<200&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&genPt_ak8_higgsTagger==0.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&reduced_MET>125&&reduced_MET<200&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<200&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);



    Table & signalRegion_3jet_125to300_boosted_cutflow_comb = pmcomb->Push<Table>("signalRegion_3jet_125to300_boosted_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jup>125&&pfmet_jup<300&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jdown>125&&pfmet_jdown<300&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jerup_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jerdown_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&genmet>125&&genmet<300&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&genPt_ak8_higgsTagger==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&reduced_MET>125&&reduced_MET<300&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>125&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_3jet_200to300_resolved_cutflow_comb = pmcomb->Push<Table>("signalRegion_3jet_200to300_resolved_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jup>200&&pfmet_jup<300&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jdown>200&&pfmet_jdown<300&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jerup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jerdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&genmet>200&&genmet<300&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&genPt_ak8_higgsTagger==0.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&reduced_MET>200&&reduced_MET<300&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>200&&pfmet<300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_3jet_300plus_boosted_cutflow_comb = pmcomb->Push<Table>("signalRegion_3jet_300plus_boosted_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jup>300&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jdown>300&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jerup_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jerdown_mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&genmet>300&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&genPt_ak8_higgsTagger==1.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&reduced_MET>300&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==1&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_3jet_300to400_resolved_cutflow_comb = pmcomb->Push<Table>("signalRegion_3jet_300to400_resolved_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jup>300&&pfmet_jup<400&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jdown>300&&pfmet_jdown<400&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jerup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jerdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&genmet>300&&genmet<400&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&genPt_ak8_higgsTagger==0.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&reduced_MET>300&&reduced_MET<400&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>300&&pfmet<400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);

    Table & signalRegion_3jet_400plus_resolved_cutflow_comb = pmcomb->Push<Table>("signalRegion_3jet_400plus_resolved_cutflow_comb", vector<TableRow>{
      TableRow("Nominal",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("lepSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFUp"*yearWeight),
      TableRow("lepSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_lepSFDown"*yearWeight),
      //TableRow("tauSFup",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFUp"*yearWeight),
      //TableRow("tauSFdown",     WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_tauSFDown"*yearWeight),
      TableRow("L1prefireUp",   WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireUp"*yearWeight),
      TableRow("L1prefireDown", WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_L1prefireDown"*yearWeight),
      TableRow("ISRUp",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRUp"*yearWeight),
      TableRow("ISRDown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_ISRnjets*w_ISRDown"*yearWeight),
      TableRow("q2Up",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Up"*yearWeight),
      TableRow("q2Down",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_q2Down"*yearWeight),
      TableRow("btagHFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFUp"*yearWeight),
      TableRow("btagHFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagHFDown"*yearWeight),
      TableRow("btagLFUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFUp"*yearWeight),
      TableRow("btagLFDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagLFDown"*yearWeight),
      TableRow("btagFSUp",      WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSUp"*yearWeight),
      TableRow("btagFSDown",    WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_btagFSDown"*yearWeight),
      TableRow("puUp",          WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puUp"*yearWeight),
      TableRow("puDown",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff*w_puDown"*yearWeight),
      TableRow("JESup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jup>400&&jup_mct>200&&mt_met_lep_jup>150&&jup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JESdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet_jdown>400&&jdown_mct>200&&mt_met_lep_jdown>150&&jdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERup",         WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&jerup_mct>200&&mt_met_lep>150&&jerup_mbb>90&&jup_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("JERdown",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&jerdown_mct>200&&mt_met_lep>150&&jerdown_mbb>90&&jdown_mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      //TableRow("genMET",        WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&genmet>400&&mct>200&&mt_genmet_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FatJet genPt",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&genPt_ak8_higgsTagger==0.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("FastSim Corr",  WHLeptons==1.&&LeadingNonBJetPt_med<300.&&reduced_MET>400&&reducedMET_mT>150&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&mct>200&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight*trig_eff"*yearWeight),
      TableRow("TrigEff",       WHLeptons==1.&&LeadingNonBJetPt_med<300.&&"pass&&nvetoleps==1&&PassTrackVeto&&PassTauVeto&&ngoodjets==3&&pfmet>400&&mct>200&&mt_met_lep>150&&mbb>90&&mbb<150&&ngoodbtags==2&&nHiggs==0&&hasNano",0,0,"weight"*yearWeight)

        }, sample_list_comb,true);


if(single_thread) pmcomb->multithreaded_ = false;
  pmcomb->MakePlots(lumicomb);


  //Efficiency division
  ofstream outFile;
  outFile.open("signalSystematics.txt");

  int nSignals = sample_list_comb.size();
  
  for(int iProc = 0; iProc < nSignals; iProc++){
    for(unsigned iTable(0); iTable < 12; iTable++){
      vector< vector<GammaParams>  > yields(nSignals,vector<GammaParams> (25));
      outFile << v_mchi[iProc] << " " << v_mlsp[iProc] << " ";
      for(unsigned iVar(0); iVar < 24; iVar++){
    
        double resultYield = 0;

        Table * yield_table = static_cast<Table*>(pmcomb->Figures()[iTable].get());
        yields[iProc] = yield_table->Yield(sample_list_comb[iProc].get(), lumicomb);
        resultYield = 1.0+((yields[iProc][iVar].Yield()-yields[iProc][0].Yield())/(yields[iProc][0].Yield()));

        if(resultYield>=2.0){
          resultYield = 2;
        }else if(resultYield<=0.0){
          resultYield = 0.01;
        }else if(resultYield!=resultYield){
          resultYield = 1;
        }

        outFile << resultYield;

        if(iVar!=23){
          outFile << " ";
        }

      }//for loop over variations/rows
      outFile << endl;
    }//for loop over 12 signal regions
    outFile << endl;
  }//for loop over signal mass points

  outFile.close();

  vector<GammaParams> yields_signalRegion_125to200_resolved_comb = signalRegion_125to200_resolved_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_125to200_resolved_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signalRegion_200to300_resolved_comb = signalRegion_200to300_resolved_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_200to300_resolved_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signalRegion_300to400_resolved_comb = signalRegion_300to400_resolved_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_300to400_resolved_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signalRegion_400plus_resolved_comb = signalRegion_400plus_resolved_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_400plus_resolved_comb){
    cout << yield << endl;
  }



  vector<GammaParams> yields_signalRegion_3jet_125to200_resolved_comb = signalRegion_3jet_125to200_resolved_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_3jet_125to200_resolved_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signalRegion_3jet_200to300_resolved_comb = signalRegion_3jet_200to300_resolved_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_3jet_200to300_resolved_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signalRegion_3jet_300to400_resolved_comb = signalRegion_3jet_300to400_resolved_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_3jet_300to400_resolved_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signalRegion_3jet_400plus_resolved_comb = signalRegion_3jet_400plus_resolved_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_3jet_400plus_resolved_comb){
    cout << yield << endl;
  }



  vector<GammaParams> yields_signalRegion_125to300_boosted_comb = signalRegion_125to300_boosted_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_125to300_boosted_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signalRegion_300plus_boosted_comb = signalRegion_300plus_boosted_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_300plus_boosted_comb){
    cout << yield << endl;
  }



  vector<GammaParams> yields_signalRegion_3jet_125to300_boosted_comb = signalRegion_3jet_125to300_boosted_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_3jet_125to300_boosted_comb){
    cout << yield << endl;
  }

  vector<GammaParams> yields_signalRegion_3jet_300plus_boosted_comb = signalRegion_3jet_300plus_boosted_cutflow_comb.BackgroundYield(lumicomb);
  for(const auto &yield: yields_signalRegion_3jet_300plus_boosted_comb){
    cout << yield << endl;
  }

  
}
