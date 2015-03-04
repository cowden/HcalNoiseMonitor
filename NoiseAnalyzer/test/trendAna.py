#!/usr/bin/env python

###########################################
# Execute with ./trendAna.py 99291 99563
# Output : http://home.fnal.gov/~jhirsch/HcalRocMonSummary_99291-99563/
###########################################

# 1] Tool for plotting trends of various quantities run-by-run.
# 2] Quantities are harvested from HcalRoc monitoring histograms.
# 3] Requires trendAnaUtils.py, trendAnaTemplate.html.
# 4] To add plot of simple quantity, user needs to add single entry to
#    histInfo dictionary in trendAnaUtils.py.

# Author: Jim Hirschauer, jhirsch@fnal.gov, July 2009

# Coming improvements:
# 1] Output root tuple.
# 2] New plots.
# 3] Add option to see what runs are available.

#######################
# Import Modules
#######################

import sys
import os
#import time
#print time.ctime(1270236000)

# Check for necessary files
for file in ["trendAnaUtils.py",
             "trendAnaTemplate.html",
             "trendAnaRunTemplate.html"]:
    if not os.path.isfile(file):
        print "Cannot find %s.  Exiting." % file
        sys.exit()

import ROOT
import optparse
import commands
import trendAnaUtils
import re

#######################
# Get and parse options
#######################

parser = optparse.OptionParser("usage: %prog [options]\
file1.txt file2.txt\n")

parser.add_option ('--m', dest='namemod', type='string',default = '-1',
                   help="Modifier for naming of output")
parser.add_option ('--p', dest='outpath', type='string',
                   default = '-1',
                   help="Path to dir for output gifs and eps")
parser.add_option ('--o', dest='outd', type='string',
                   default = '-1',
                   help="Directory for output gifs and eps")
parser.add_option ('--d', dest='indir', type='string',
                   default = '/uscms_data/d2/cmsroc_hcal/rocmon/Craft_v4/',
                   help="Directory for input root files")
parser.add_option ('--f', action="store_true",
                   dest="overwrite", default=False,
                   help="Force overwrite of output dir.")
parser.add_option ('--noTrig', action="store_false",
                   dest="doTrig", default=True,
                   help="Do not make trigger trend plots.")
parser.add_option ('--doRbr', action="store_true",
                   dest="doRunByRunPlots", default=False,
                   help="Make run-by-run plots.")
options, args = parser.parse_args()

if len(args) != 2:
    print "Please specify run range.  Exiting."
    sys.exit()

runStart  = args[0]
runStop   = args[1]
outd      = options.outd
outpath   = options.outpath
indir     = options.indir
namemod   = options.namemod
overwrite = options.overwrite
doRunByRunPlots = options.doRunByRunPlots
doTrig    = options.doTrig

################################
# Setup input/output directories
################################
indir = indir+"/"

if outpath == "-1" and outd == "-1" :
    outdir = "./HcalRocMonSummary_"+str(runStart)+"-"+str(runStop)
elif outpath == "-1" and outd != "-1" :
    outdir = outd
elif outpath != "-1" and outd == "-1" :
    outdir = outpath+"/HcalRocMonSummary_"+str(runStart)+"-"+str(runStop)
elif outpath != "-1" and outd != "-1" :
    outdir = outpath+"/"+outd

if namemod != "-1" :
    outdir = outdir+"_"+namemod+"/"
else :
    outdir = outdir+"/"

print "Output dir = %s" % outdir

if not os.path.isdir(indir):
    print "Input dir %s does not exist.  Exiting." % indir
    sys.exit()

if os.path.isdir(outdir) and overwrite:
    os.system("rm -rf "+outdir)
    print "Overwriting output dir %s." % outdir
elif os.path.isdir(outdir):
    print "Output dir %s already exists.  Exiting." % outdir
    sys.exit()

os.system("mkdir "+outdir)

if not os.path.isdir(outdir) :
    print "Could not create dir %s.  Exiting." % outdir
    sys.exit()

# Find MergeAndOverlayNoisePlots.C, if needed.
if (doRunByRunPlots and
    not os.path.isfile("../../Condor/MergeAndOverlayNoisePlots.C") and
    not os.path.isfile("MergeAndOverlayNoisePlots.C")):
    print "Can't find MergeAndOverlayNoisePlots.C.  Exiting."
    print "Please find in UserCode/HcalRocFnal/Analysis/Condor/"
    sys.exit()
elif doRunByRunPlots :
    if os.path.isfile("MergeAndOverlayNoisePlots.C"):
        noiseScript = "MergeAndOverlayNoisePlots.C"
    else :
        noiseScript = "../../Condor/MergeAndOverlayNoisePlots.C"

        
################################
# Setup ROOT
################################

#  ROOT initialization stuff here so that
#  my --help gets reported instead of ROOT --help.

ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch()
#ROOT.gStyle.SetOptStat(111111)
ROOT.gStyle.SetOptStat(0);
ROOT.gStyle.SetPalette(1)

ROOT.gStyle.SetNdivisions(405,"x");
#ROOT.gStyle.SetMarkerSize(0.8);
ROOT.gStyle.SetMarkerSize(1.5);
ROOT.gStyle.SetMarkerStyle(3);
ROOT.gStyle.SetMarkerColor(2);


#####################################
# Find files and load into dictionary
#####################################

fileDict = {}
nruns = 0
print 'Searching for runs %s to %s.' % (runStart, runStop)
for i in range(int(runStart), int(runStop)+1):
    if os.path.isfile(indir+"R"+str(i)+".root"):
        fileDict[(nruns,i)] = ROOT.TFile(indir+"R"+str(i)+".root")
        nruns = nruns+1

if nruns: 
    print "Found %i runs." % nruns
else:
    print "Found %i runs.  Exiting." % nruns
    sys.exit()

#####################################
# Copy html template to output dir
#####################################

os.system("cp trendAnaTemplate.html "+outdir+"index.html")
# Add run numbers
command_firstrun = "replace XXXXXX "+str(runStart)+" -- " \
                   +outdir+"index.html > /dev/null"
command_lastrun  = "replace YYYYYY "+str(runStop)+" -- "  \
                   +outdir+"index.html > /dev/null"
os.system(command_firstrun)    
os.system(command_lastrun)    


#####################################
# Declare trend plots
#####################################

# trendPlots is dictionary of form
# trendPlots[i] = (TH2F trend plot, path/hist to harvest, 
#                  quantity to harvest, index method,
#                  special plot flag, color)
trendPlots = trendAnaUtils.declareTrendPlots(runStart, runStop, nruns, doTrig)


#####################################
# Loop over files
#####################################

runlist = []
for i, file in sorted(fileDict.iteritems()):

    runindex = i[0]
    runnum   = i[1]

    print "Processing run %i." % runnum


    #####################################
    # Get number of events and lumi segments in run
    #####################################
    hLumiBlock = file.Get("hcalnoiseinfoanalyzer/Noise/hLumiBlock")
    # Sum number of non lumi segs
    nLumiSegs = 0.0
    nEvents   = -99.0
    nEvents = hLumiBlock.GetEntries()
    for bin in range(hLumiBlock.GetNbinsX()):
        if hLumiBlock.GetBinContent(int(bin)) > 0.0 :
            nLumiSegs = nLumiSegs+1
            
    #h_unixtime = file.Get("hcalnoiseinfoanalyzer/Noise/h_unixtime")
    h_rbxnoise_20 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_RBXNoise_20")
    h_rbxnoise_50 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_RBXNoise_50")
    h_rbxnoise_100 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_RBXNoise_100")
    h_rbxnoise_500 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_RBXNoise_500")
    
    h_hpdionfeed_20 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDionfeed_20")
    h_hpdionfeed_50 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDionfeed_50")
    h_hpdionfeed_100 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDionfeed_100")
    h_hpdionfeed_500 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDionfeed_500")
    
    h_hpddischg_20 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDdischg_20")
    h_hpddischg_50 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDdischg_50")
    h_hpddischg_100 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDdischg_100")
    h_hpddischg_500 = file.Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDdischg_500")

    rbx20 = trendAnaUtils.getAvgRate(h_rbxnoise_20, nLumiSegs)
    rbx50 = trendAnaUtils.getAvgRate(h_rbxnoise_50, nLumiSegs)
    rbx100 = trendAnaUtils.getAvgRate(h_rbxnoise_100, nLumiSegs)
    rbx500 = trendAnaUtils.getAvgRate(h_rbxnoise_500, nLumiSegs)
    
    ionfeed20 = trendAnaUtils.getAvgRate(h_hpdionfeed_20, nLumiSegs)
    ionfeed50 = trendAnaUtils.getAvgRate(h_hpdionfeed_50, nLumiSegs)
    ionfeed100 = trendAnaUtils.getAvgRate(h_hpdionfeed_100, nLumiSegs)
    ionfeed500 = trendAnaUtils.getAvgRate(h_hpdionfeed_500, nLumiSegs)
    
    dischg20 = trendAnaUtils.getAvgRate(h_hpddischg_20, nLumiSegs)
    dischg50 = trendAnaUtils.getAvgRate(h_hpddischg_50, nLumiSegs)
    dischg100 = trendAnaUtils.getAvgRate(h_hpddischg_100, nLumiSegs)
    dischg500 = trendAnaUtils.getAvgRate(h_hpddischg_500, nLumiSegs)

    # Add run index, run number, and # events to runlist
    runlist.append((runindex, runnum, nEvents, rbx20, rbx50, rbx100, rbx500, ionfeed20, ionfeed50, ionfeed100, ionfeed500, dischg20, dischg50, dischg100, dischg500))
            


	    
    #####################################
    # Loop over trendplots for each file
    #####################################
    for tp in trendPlots.values():

        # Harvest quantities
        hist = file.Get(tp[1]) 
        
        # For trigger rates tp[2] encodes trigger flag and bin number
        # Use regular expressions to get the info
        re_l1 = re.compile('l1bin*')
        re_hlt = re.compile('hltbin*')
        match_l1  = re_l1.match(tp[2])
        match_hlt = re_hlt.match(tp[2])
        
        if   tp[2] == "Mean" :
            value = trendAnaUtils.getHistMean(hist)
        elif tp[2] == "Rms"  :
            value = trendAnaUtils.getHistRms(hist)
        elif tp[2] == "Rate" :
            value = trendAnaUtils.getAvgRate(hist, nLumiSegs)
        # trendAnaUtils.fill2DPlot() harvests AND fills trendplots.
        elif tp[2] == "2d"   :
            trendAnaUtils.fill2DPlot(hist, tp[0], runindex, nLumiSegs)
        elif match_l1 or match_hlt :
            value = trendAnaUtils.getTrigRate(hist, tp[2], nLumiSegs)
        else:
            print 'Quantity to chart \"%s\" is undefined.  Exiting.' % tp[2]
            sys.exit()


        # Perform actual filling of trend plots
        # Note that "2d" trend plots were already filled above
        if   tp[3] == "runnumber" and tp[2] != "2d" :
            tp[0].Fill(float(runnum),  float(value))
        elif tp[3] == "runindex"  and tp[2] != "2d" :
            tp[0].Fill(float(runindex),float(value))
        elif tp[2] != "2d" :
            # If tp != 2d then we must have an undefined index method
            print 'Index method \"%s\" is undefined.  Exiting.' % tp[3]
            sys.exit()
        
        # Set color of histograms
        tp[0].SetMarkerColor(tp[5])


#####################################
# Sort and write runlist to text file
#####################################

runlist.sort()
os.system("touch "+outdir+"runlist.html")
frunlist = open(outdir+"runlist.html","w")

# Print header
print >> frunlist, "<h1> Run list : %s-%s </h1><hr>" \
      % (str(runStart), str(runStop))
print >> frunlist, "<table width=\"600\" >"

print >> frunlist, "<tr><td align=\"right\"> Run Index </td><td align=\"right\"> # Events </td><td align=\"right\"> Run Number </td></tr>"
# Print line for each run
for i in range(len(runlist)) :
    link = '<a href=\"r%s\"> %s </a><br>' \
           % (runlist[i][1],runlist[i][1]) 
    print >> frunlist, "<tr><td align=\"right\"> %s </td><td align=\"right\"> %s </td><td align=\"right\"> %s </td></tr>" \
          % (runlist[i][0], runlist[i][2], link)

print >> frunlist, "</table>"
frunlist.close()     


#####################################
# rbx noise
#####################################
    
runlist.sort()
os.system("touch "+outdir+"rbxnoise.html")
frbxnoise = open(outdir+"rbxnoise.html","w")

# Print header
print >> frbxnoise, "<h1> RBX noise rates [Hz] for different energy thresholds </h1><hr>" 
#      % (str(runStart), str(runStop))
print >> frbxnoise, "<table width=\"800\" >"

print >> frbxnoise, "<tr><td align=\"right\"> Run Index </td><td align=\"right\"> Run Number </td><td align=\"right\"> 20 GeV </td><td align=\"right\"> 50 GeV </td><td align=\"right\"> 100 GeV </td><td align=\"right\"> 500 GeV </td></tr>"
# Print line for each run
for i in range(len(runlist)) :
    link = '<a href=\"r%s\"> %s </a><br>' \
           % (runlist[i][1],runlist[i][1])
    print >> frbxnoise, "<tr><td align=\"right\"> %s </td><td align=\"right\"> %s </td><td align=\"right\"> %.5s </td><td align=\"right\"> %.5s </td><td align=\"right\"> %.5s </td><td align=\"right\"> %.5s </td></tr>" \
          % (runlist[i][0], link , runlist[i][3] , runlist[i][4] , runlist[i][5] , runlist[i][6])

print >> frbxnoise, "</table>"
frbxnoise.close()     

#####################################
# hpd ionfeed
#####################################
    
runlist.sort()
os.system("touch "+outdir+"hpdionfeed.html")
fhpdionfeed = open(outdir+"hpdionfeed.html","w")

# Print header
print >> fhpdionfeed, "<h1> HPD ion feedback noise rates [Hz] for different energy thresholds </h1><hr>" 
#      % (str(runStart), str(runStop))
print >> fhpdionfeed, "<table width=\"800\" >"

print >> fhpdionfeed, "<tr><td align=\"right\"> Run Index </td><td align=\"right\"> Run Number </td><td align=\"right\"> 20 GeV </td><td align=\"right\"> 50 GeV </td><td align=\"right\"> 100 GeV </td><td align=\"right\"> 500 GeV </td></tr>"
# Print line for each run
for i in range(len(runlist)) :
    link = '<a href=\"r%s\"> %s </a><br>' \
           % (runlist[i][1],runlist[i][1])
    print >> fhpdionfeed, "<tr><td align=\"right\"> %s </td><td align=\"right\"> %s </td><td align=\"right\"> %.5s </td><td align=\"right\"> %.5s </td><td align=\"right\"> %.5s </td><td align=\"right\"> %.5s </td></tr>" \
          % (runlist[i][0], link , runlist[i][7] , runlist[i][8] , runlist[i][9] , runlist[i][10])

print >> fhpdionfeed, "</table>"
fhpdionfeed.close()     

#####################################
# hpd discharge
#####################################
    
runlist.sort()
os.system("touch "+outdir+"hpddischg.html")
fhpddischg = open(outdir+"hpddischg.html","w")

# Print header
print >> fhpddischg, "<h1> HPD discharge noise rates [Hz] for different energy thresholds </h1><hr>" 
#      % (str(runStart), str(runStop))
print >> fhpddischg, "<table width=\"800\" >"

print >> fhpddischg, "<tr><td align=\"right\"> Run Index </td><td align=\"right\"> Run Number </td><td align=\"right\"> 20 GeV </td><td align=\"right\"> 50 GeV </td><td align=\"right\"> 100 GeV </td><td align=\"right\"> 500 GeV </td></tr>"
# Print line for each run
for i in range(len(runlist)) :
    link = '<a href=\"r%s\"> %s </a><br>' \
           % (runlist[i][1],runlist[i][1])
    print >> fhpddischg, "<tr><td align=\"right\"> %s </td><td align=\"right\"> %s </td><td align=\"right\"> %.5s </td><td align=\"right\"> %.5s </td><td align=\"right\"> %.5s </td><td align=\"right\"> %.5s </td></tr>" \
          % (runlist[i][0], link , runlist[i][11] , runlist[i][12] , runlist[i][13] , runlist[i][14])

print >> fhpddischg, "</table>"
fhpddischg.close()     

########################################
# Draw Run-by-run Plots
########################################

if (doRunByRunPlots):
    for runnum in runlist:
        trendAnaUtils.makeRunByRunPlots(indir, outdir, noiseScript, str(runnum[1]))



#####################################
# Plot trend plots on canvases.
#####################################

# Note that some plots have special flags so that they are plotted on
# the same canvas and/or have text written on canvas.  So get plots in
# several groups:

singleplots  = trendAnaUtils.getSinglePlots(trendPlots)
ifrateplots  = trendAnaUtils.getPlotFamily(trendPlots,"ifrate")
hpdrateplots = trendAnaUtils.getPlotFamily(trendPlots,"hpdrate")
rbxrateplots = trendAnaUtils.getPlotFamily(trendPlots,"rbxrate") 
jettrigplots = trendAnaUtils.getPlotFamily(trendPlots,"jettrig")

# Prepare dictionary of canvases
canvases = {}
ncanv = 0

#####################################
# Draw simple plots one to a canvas
#####################################

# each "plot" in "singleplots" is tuple (histogram, type)
for plot in singleplots.values() :
    name = plot[0].GetName()
    canvases[ncanv] = ROOT.TCanvas(name, name, 300, 300)
    if plot[1] == "2d" :
        plot[0].GetZaxis().SetLabelSize(0.02)
        plot[0].Draw("colz")
    else : plot[0].Draw()
    canvases[ncanv].SaveAs(outdir+name+".eps")
    canvases[ncanv].SaveAs(outdir+name+".gif")
    ncanv = ncanv+1

#####################################
# Draw special plots on canvases
#####################################

# Make text to print on canvases
ratetext = trendAnaUtils.makeRateText(runStart)
jettext = trendAnaUtils.makeJetText(runStart)

trendAnaUtils.drawOverlaidPlots(canvases, ncanv, outdir,
                                "ionfeedback_rate",ifrateplots,ratetext)
trendAnaUtils.drawOverlaidPlots(canvases, ncanv, outdir,
                                "hpd_discharge_rate",hpdrateplots,trendAnaUtils.makeRateText2(runStart))
trendAnaUtils.drawOverlaidPlots(canvases, ncanv, outdir,
                                "rbxnoise_rate",rbxrateplots,trendAnaUtils.makeRateText3(runStart))
trendAnaUtils.drawOverlaidPlots(canvases, ncanv, outdir,
                                "singlejet_rate",jettrigplots,jettext)



sys.exit()

