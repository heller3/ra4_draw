#! /usr/bin/env python

import argparse
import os
import shutil
import re
import tempfile
import subprocess
import ROOT
import sys

ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gStyle.SetLabelFont(42,"xyz")
ROOT.gStyle.SetLabelSize(0.05,"xyz")
#ROOT.gStyle.SetTitleFont(42)
ROOT.gStyle.SetTitleFont(42,"xyz")
ROOT.gStyle.SetTitleFont(42,"t")
#ROOT.gStyle.SetTitleSize(0.05)
ROOT.gStyle.SetTitleSize(0.06,"xyz")
ROOT.gStyle.SetTitleSize(0.06,"t")
ROOT.gStyle.SetPadBottomMargin(0.14)
ROOT.gStyle.SetPadLeftMargin(0.14)
ROOT.gStyle.SetTitleOffset(1,'y')
ROOT.gStyle.SetLegendTextSize(0.037)
ROOT.gStyle.SetGridStyle(3)
ROOT.gStyle.SetGridColor(12)
ROOT.gStyle.SetOptFit(1)
one = ROOT.TColor(2001,0.906,0.153,0.094)
two = ROOT.TColor(2002,0.906,0.533,0.094)
three = ROOT.TColor(2003,0.086,0.404,0.576)
four =ROOT.TColor(2004,0.071,0.694,0.18)
five =ROOT.TColor(2005,0.388,0.098,0.608)
six=ROOT.TColor(2006,0.906,0.878,0.094)
colors = [1,2001,2002,2003,2004,2005,2006,6,2,3,4,6,7,5,1,8,9,29,38,46,1,2001,2002,2003,2004,2005,2006]

plot_name = "overlay.pdf"
variation_names = ["original_36","original","nominal_4metbins","boosted_4metbins"]
variation_titles = ["2016 analysis with Re-Reco, 36 fb^{-1}","2016 analysis with Re-Reco, reloaded to 137 fb^{-1}","Run II selection re-optimization","Run II selection re-optimization + boosted tag"]
xparticle = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{2}}}";
yparticle = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";

def cosmetic_tgraph(graph,colorindex,central=True):
	graph.SetLineColor(colors[colorindex])
	graph.SetMarkerColor(colors[colorindex])
	graph.SetMarkerSize(0.75)
	graph.SetMarkerStyle(20)
	if not central:
		graph.SetLineWidth(1)
	# 	graph.SetMarkerSize(2.5)
	# 	graph.SetMarkerStyle(29)
	graph.SetTitle("; %s; %s" %(xparticle,yparticle) )

def get_curves(var_name):
	file = ROOT.TFile.Open("TChiWH_%s_limit_scan.root"%var_name)
	central = file.Get("TChiWHExpectedLimit")
	up = file.Get("TChiWHExpectedLimitUp")
	down = file.Get("TChiWHExpectedLimitDown")
	return central,up,down


c = ROOT.TCanvas("","",1000,800)
# c.SetGridy()
# c.SetGridx()
mgraph = ROOT.TMultiGraph()
leg = ROOT.TLegend(0.17,0.65,0.87,0.88)
leg.SetMargin(0.15)
h2 = ROOT.TH2F("h2","",10,0,900,10,0,600)


for i,name in enumerate(variation_names):
	central,up,down = get_curves(name)
	cosmetic_tgraph(central,i,True)
	cosmetic_tgraph(up,i,False)
	cosmetic_tgraph(down,i,False)
#	mgraph.GetHistogram().SetMaximum(600);

	mgraph.Add(central)
	mgraph.Add(up)
	mgraph.Add(down)

	leg.AddEntry(central, variation_titles[i],"L")

mgraph.SetTitle("; m_{%s} [GeV]; m_{%s} [GeV]" %(xparticle,yparticle) )
# mgraph.GetYaxis().SetRangeUser(0.,600.);
h2.Draw()
mgraph.SetMaximum(600);
mgraph.Draw("L same")
leg.Draw("same")
c.Print(plot_name)

