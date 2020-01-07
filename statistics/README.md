wh_draw
========

Scripts for processing datacards into workspaces, executing combine, and plotting the results.

#### Prerequisites
First, make datacards using wh_draw/run/wh/make_datacards_scan.exe. This will store the datacards for all mass points in the scan in a subdirectory of statistics/ according the analysis_tag and binning options specified (the "variation folder").

#### Setup Combine tool
Then, checkout recent version of CMS and set up combine, following instructions here: http://cms-analysis.github.io/HiggsAnalysis-CombinedLimit/

Make sure your statistics/setup.sh file points to the location of your CMSSW release with combine.

#### Calculate the limits:

	source setup.sh
	./scan.sh  <name_of_variation_folder>

```scan.sh``` will first convert all datacards into workspaces, and then call ```scan_point.py``` to generate and execute the appropriate combine commands. The outputs will be stored in a file called limits.txt inside the particular variation folder. There is the option to skip the workspace-making step by passing "0" after the variation folder name.


#### Plot the results:
	
	source setup_plot.sh
	./compile.sh
	./run/limit_scan.exe -f <path_to_limits_file> -s <number of smoothings to perform>

The plotting script does not work in the most recent C++ version used by CMSSW_10, so it is necessary to check out an older version (in this case, CMSSW_8_0_20). Currently, performing any smoothings of the limit contour seems to cause strange artifacts near the diagonal (to be investigated). For now, it is recommended to use -s 0.




