#! /usr/bin/env python

import argparse
import os
import shutil
import re
import tempfile
import subprocess
import ROOT
import sys

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Runs combine on workspace for single mass point")
    parser.add_argument("-i", "--infile", default="",
                        help="Workspace file to process")
    parser.add_argument("-c", "--channelMasks", default="",
                        help="Which channels to mask?")

    args = parser.parse_args()
    ## get directory and masses for this signal point
    base_dir = os.path.dirname(args.infile)
    wspace_name = os.path.basename(args.infile)
    
    initial_dir = os.getcwd()

    mchi = int(wspace_name.split("mChi-")[1].split("_")[0])
    mlsp = int(wspace_name.split("mLSP-")[1].split("_")[0])
    
    f_sig_xsec = ROOT.TFile("xsec_tchi_13TeV.root","read")
    h_sig_xsec = f_sig_xsec.Get("h_xsec_c1n2")

    xsec = h_sig_xsec.GetBinContent(h_sig_xsec.FindBin(mchi));
    exsec = h_sig_xsec.GetBinError(h_sig_xsec.FindBin(mchi));

    temp_dir = "%s/tmp_%i_%i" %(base_dir,mchi,mlsp)
    if not os.path.exists(temp_dir): os.mkdir(temp_dir)

    os.chdir(temp_dir)
    # tmp_exe_filename = "run_combine_%i_%i.sh" % (mchi,mlsp)
    # tmp_exe = open(tmp_exe_filename,"w")

    ###expected###
   # combine_cmd = ["combine", "-M AsymptoticLimits", "%s/%s"% (base_dir,wspace_name), "--noFitAsimov", "-n Pre"]
    # combine_cmd = "combine -M AsymptoticLimits %s/%s --noFitAsimov -n Pre -t -1" % (base_dir,wspace_name)
    combine_cmd = "combine -M AsymptoticLimits %s/%s --name Pre" % (base_dir,wspace_name)
    if args.channelMasks:
        combine_cmd += " --setParameters %s"%args.channelMasks
    print combine_cmd
    #print os.getcwd()
    #os.system(combine_cmd)
    stream = os.popen(combine_cmd)
    output=stream.read()
    print output


    obs = float(output.split("Observed Limit: r <")[1].split("\n")[0])
    exp = float(output.split("Expected 50.0%: r <")[1].split("\n")[0])
    exp_p1 = float(output.split("Expected 84.0%: r <")[1].split("\n")[0])
    exp_m1 = float(output.split("Expected 16.0%: r <")[1].split("\n")[0])
    print "Expected is",exp,exp_p1,exp_m1

    # ###observed###
    # combine_cmd = "combine -M AsymptoticLimits %s/%s --name obs" % (base_dir,wspace_name)
    # print combine_cmd
    # #print os.getcwd()
    # #os.system(combine_cmd)
    # stream = os.popen(combine_cmd)
    # output=stream.read()
    # print output

    # ### hack for now, this is not correct variation for observed limit.
    # obs_p1 = float(output.split("Expected 84.0%: r <")[1].split("\n")[0])
    # obs_m1 = float(output.split("Expected 16.0%: r <")[1].split("\n")[0])
    # print obs

    os.chdir(initial_dir)
    #print "rm -rf %s"%temp_dir
    os.system("rm -rf %s"%temp_dir)

    scan_summary_name = base_dir.split("workspaces")[0] + "limits.txt"
    scan_summary =open(scan_summary_name,"a")
    # xsec = 1
    # obs = 999
    obs_m1= obs * (xsec - exsec)/xsec
    obs_p1 = obs * (xsec + exsec)/xsec
    sigobs=0
    sigexp=0

    #iss >> pmx >> pmy >> pxsec >> pobs >> pobsup >> pobsdown >> pexp >> pup >> pdown >> sigobs >> sigexp;
    scan_summary.write(" ".join([str(mchi),str(mlsp),str(xsec),str(obs),str(obs_m1),str(obs_p1),str(exp),str(exp_m1),str(exp_p1),str(sigobs),str(sigexp),"\n"]))
    scan_summary.close()
 
