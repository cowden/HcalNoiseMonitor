import ROOT
import glob
import os

#########################################################
# Makes dictionary of necessary info for each trendplot
#########################################################

# Format:
# histInfo[i]  = ("Detailed name of trend plot", nybins for TP,
#                 ybinlo for TP, ybinhi for TP,
#                 charted quantity, 
#                 histogram directory in TFile, histogram name in TFile,
#                 index method, flag for special drawing, marker color)

def getHistInfo(doTrig) :
    histInfo = {}
    trigHistInfo = {}
    noiseHistInfo = {}
    noise = "hcalnoiseinfoanalyzer/Noise/"
    trig  = "HcalTrig/HLT/"

    # "Mean" hists 
    noiseHistInfo[0]  = ("Mean RBX E for Ion feedback vs. Run"  , 100, 0, 200,
                    "Mean", noise,
                    "hRBXEnergy_HPDionfeed", "runnumber", "None", 2)
    noiseHistInfo[1]  = ("Mean RBX E for HPD discharge vs. Run" , 100, 0, 200,
                    "Mean", noise,
                    "hRBXEnergy_HPDdischg" , "runnumber", "None", 2)
    noiseHistInfo[2]  = ("Mean RBX E for RBX noise vs. Run"     , 100, 0, 200,
                    "Mean", noise,
                    "hRBXEnergy_RBXnoise"  , "runnumber", "None", 2)
    # "RMS" hists
    noiseHistInfo[3]  = ("RMS RBX E for Ion feedback vs. Run"   , 100, 0, 100,
                    "Rms" , noise,
                    "hRBXEnergy_HPDionfeed", "runnumber", "None", 2)
    noiseHistInfo[4]  = ("RMS RBX E for HPD discharge vs. Run"  , 100, 0, 100,
                    "Rms" , noise,
                    "hRBXEnergy_HPDdischg" , "runnumber", "None", 2)
    noiseHistInfo[5]  = ("RMS RBX E for RBX noise vs. Run"      , 100, 0, 100,
                    "Rms" , noise,
                    "hRBXEnergy_RBXnoise"  , "runnumber", "None", 2)
    # Average rate hists
    noiseHistInfo[6]  = ("HPD Ionfeedback Rate (Hz)"        , 100, 0, 10,
                    "Rate", noise,
                    "hLuminBlockNumberRate_HPDionfeed_20" , "runnumber",
                    "ifrate", 1)
    noiseHistInfo[7]  = ("HPD Ionfeedback Rate (Hz) 50 GeV" , 100, 0, 10,
                    "Rate", noise,
                    "hLuminBlockNumberRate_HPDionfeed_50" , "runnumber",
                    "ifrate", 2)
    noiseHistInfo[8]  = ("HPD Ionfeedback Rate (Hz) 100 GeV", 100, 0, 10,
                    "Rate", noise,
                    "hLuminBlockNumberRate_HPDionfeed_100", "runnumber",
                    "ifrate", 4)
    noiseHistInfo[9]  = ("HPD Ionfeedback Rate (Hz) 500 GeV", 100, 0, 10,
                    "Rate", noise,
                    "hLuminBlockNumberRate_HPDionfeed_500", "runnumber",
                    "ifrate", 6)
    noiseHistInfo[10] = ("HPD Discharge Rate (Hz)"          , 100, 0, 5,
                    "Rate", noise,
                    "hLuminBlockNumberRate_HPDdischg_20"  , "runnumber",
                    "hpdrate",1)
    noiseHistInfo[11] = ("HPD Discharge Rate (Hz) 50 GeV"   , 100, 0, 5,
                    "Rate", noise,
                    "hLuminBlockNumberRate_HPDdischg_50"  , "runnumber",
                    "hpdrate",2)
    noiseHistInfo[12] = ("HPD Discharge Rate (Hz) 100 GeV"  , 100, 0, 5,
                    "Rate", noise,
                    "hLuminBlockNumberRate_HPDdischg_100" , "runnumber",
                    "hpdrate",4)
    noiseHistInfo[13] = ("HPD Discharge Rate (Hz) 500 GeV"  , 100, 0, 5,
                    "Rate", noise,
                    "hLuminBlockNumberRate_HPDdischg_500" , "runnumber",
                    "hpdrate",6)
    noiseHistInfo[14] = ("RBXNoise Rate (Hz)"               , 100, 0, 20,
                    "Rate", noise,
                    "hLuminBlockNumberRate_RBXNoise_20"   , "runnumber",
                    "rbxrate",1)
    noiseHistInfo[15] = ("RBXNoise Rate (Hz) 50 GeV"        , 100, 0, 20,
                    "Rate", noise,
                    "hLuminBlockNumberRate_RBXNoise_50"   , "runnumber",
                    "rbxrate",2)
    noiseHistInfo[16] = ("RBXNoise Rate (Hz) 100 GeV"       , 100, 0, 20,
                    "Rate", noise,
                    "hLuminBlockNumberRate_RBXNoise_100"  , "runnumber",
                    "rbxrate",4)
    noiseHistInfo[17] = ("RBXNoise Rate (Hz) 500 GeV"       , 100, 0, 20,
                    "Rate", noise,
                    "hLuminBlockNumberRate_RBXNoise_500"  , "runnumber",
                    "rbxrate",6)
    # Number rate 2D hists
    nbins = 72
    lobin = 0.5
    hibin = 72.5
    noiseHistInfo[18] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] +HB : 20 GeV",
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HBpos_HPDnoise_20" ,"runindex","None",1)
    noiseHistInfo[19] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] +HB : 50 GeV",
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HBpos_HPDnoise_50" ,"runindex","None",1)
    noiseHistInfo[20] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] +HB : 100 GeV",
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HBpos_HPDnoise_100","runindex","None",1)
    noiseHistInfo[21] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] +HB : 500 GeV",
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HBpos_HPDnoise_500","runindex","None",1)
    noiseHistInfo[22] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] -HB : 20 GeV" ,
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HBneg_HPDnoise_20" ,"runindex","None",1)
    noiseHistInfo[23] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] -HB : 50 GeV" ,
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HBneg_HPDnoise_50" ,"runindex","None",1)
    noiseHistInfo[24] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] -HB : 100 GeV",
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HBneg_HPDnoise_100","runindex","None",1)
    noiseHistInfo[25] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] -HB : 500 GeV",
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HBneg_HPDnoise_500","runindex","None",1)
    noiseHistInfo[26] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] +HE : 20 GeV" ,
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HEpos_HPDnoise_20" ,"runindex","None",1)
    noiseHistInfo[27] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] +HE : 50 GeV" ,
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HEpos_HPDnoise_50" ,"runindex","None",1)
    noiseHistInfo[28] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] +HE : 100 GeV",
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HEpos_HPDnoise_100","runindex","None",1)
    noiseHistInfo[29] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] +HE : 500 GeV",
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HEpos_HPDnoise_500","runindex","None",1)
    noiseHistInfo[30] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] -HE : 20 GeV" ,
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HEneg_HPDnoise_20" ,"runindex","None",1)
    noiseHistInfo[31] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] -HE : 50 GeV" ,
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HEneg_HPDnoise_50" ,"runindex","None",1)
    noiseHistInfo[32] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] -HE : 100 GeV",
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HEneg_HPDnoise_100","runindex","None",1)
    noiseHistInfo[33] = ("Noise Rate (Hz) [z] in HPD [y] in Run [x] -HE : 500 GeV",
                    nbins, lobin, hibin,"2d",noise,
                    "hHPDNumberRate_HEneg_HPDnoise_500","runindex","None",1)
    noiseHistInfo[34] = ("Noise Rate (Hz) [z] in RBX [y] in Run [x] : 20 GeV",
                    nbins+6, lobin-1, hibin+5,"2d",noise,
                    "hRBXNumberRate_RBXnoise_20","runindex","None",1)
    noiseHistInfo[35] = ("Noise Rate (Hz) [z] in RBX [y] in Run [x] : 50 GeV",
                    nbins+6, lobin-1, hibin+5,"2d",noise,
                    "hRBXNumberRate_RBXnoise_50","runindex","None",1)
    noiseHistInfo[36] = ("Noise Rate (Hz) [z] in RBX [y] in Run [x] : 100 GeV",
                    nbins+6, lobin-1, hibin+5,"2d",noise,
                    "hRBXNumberRate_RBXnoise_100","runindex","None",1)
    noiseHistInfo[37] = ("Noise Rate (Hz) [z] in RBX [y] in Run [x] : 500 GeV",
                    nbins+6, lobin-1, hibin+5,"2d",noise,
                    "hRBXNumberRate_RBXnoise_500","runindex","None",1)

		    		    
    # Trigger plots
    trigHistInfo[0] = ("L1 Single Jet Rate (Hz)"   , 100, 0., 12.,"l1bin_16",
                       trig, "h_l1bitOn" ,"runnumber","jettrig",1)
    trigHistInfo[1] = ("L1 Single Jet10 Rate (Hz)" , 100, 0., 12.,"l1bin_17",
                       trig, "h_l1bitOn" ,"runnumber","jettrig",2)
    trigHistInfo[2] = ("L1 Single Jet20 Rate (Hz)" , 100, 0., 12.,"l1bin_18",
                       trig, "h_l1bitOn" ,"runnumber","jettrig",3)
    trigHistInfo[3] = ("L1 Single Jet30 Rate (Hz)" , 100, 0., 12.,"l1bin_19",
                       trig, "h_l1bitOn" ,"runnumber","jettrig",4)
    trigHistInfo[4] = ("L1 Single Jet40 Rate (Hz)" , 100, 0., 12.,"l1bin_20",
                       trig, "h_l1bitOn" ,"runnumber","jettrig",6)
    trigHistInfo[5] = ("L1 Single Jet50 Rate (Hz)" , 100, 0., 12.,"l1bin_21",
                       trig, "h_l1bitOn" ,"runnumber","jettrig",7)
    trigHistInfo[6] = ("L1 Single Jet60 Rate (Hz)" , 100, 0., 12.,"l1bin_22",
                       trig, "h_l1bitOn" ,"runnumber","jettrig",28)
    trigHistInfo[7] = ("L1 Single EG5 Rate (Hz)"      , 100, 0., 2. ,"l1bin_41",
                       trig, "h_l1bitOn" ,"runnumber","None",1)
    trigHistInfo[8] = ("L1 ETM 20 Rate (Hz)"          , 100, 0., 2. ,"l1bin_56",
                       trig, "h_l1bitOn" ,"runnumber","None",2)
    trigHistInfo[9] = ("L1 HTT 100 Rate (Hz)"         , 100, 0., 2. ,"l1bin_66",
                       trig, "h_l1bitOn" ,"runnumber","None",4)
    trigHistInfo[10] = ("L1 Single Mu Open Rate (Hz)"  , 100, 0., 0.1,"l1bin_71",
                        trig, "h_l1bitOn" ,"runnumber","None",2)

    nhist = 0
    for hist in noiseHistInfo.values():
        histInfo[nhist] = hist
        nhist = nhist+1

    if doTrig : 
        for hist in trigHistInfo.values():
            histInfo[nhist] = hist
            nhist = nhist+1
    
    return histInfo

################################################################
# Declare trendplots using info grom getHistInfo()
################################################################

def declareTrendPlots (runStart, runStop, nruns, doTrig):

    # Set up x binning by run number
    nRuns      = int(runStop)-int(runStart)+1
    plotmargin = int(nRuns)*0.1
    loBin      = float(runStart)-float(plotmargin)-0.5
    hiBin      = float(runStop) +float(plotmargin)+0.5
    nBins      = nRuns+2*plotmargin
    
    # Set up x binning by run index
    loBin2 = -0.5
    hiBin2 = float(nruns)+0.5
    nBins2 = nruns+1

    histInfo = getHistInfo(doTrig)
    trendPlots = {}    
    # trendPlots is dictionary of form
    # trendPlots[i] = (TH2F trend plot, path/hist to harvest, 
    #                  quantity to harvest, index method, special plot
    #                  flag, color)
    
    nTP = 0
    for histI in histInfo.values() :
        if histI[7] == "runnumber" :
            trendPlots[nTP] = makeSingleTrendPlot(histI, nBins, loBin, hiBin)
        elif histI[7] == "runindex" :
            trendPlots[nTP] = makeSingleTrendPlot(histI, nBins2, loBin2, hiBin2)
        else :
            print "%s is bad method for run indexing.  Exiting." % histI[7]
            sys.exit()
        nTP = nTP+1
    return trendPlots

def makeSingleTrendPlot(histI, nBins, loBin, hiBin):

    trendplot = (ROOT.TH2F(histI[4]+"_"+histI[6], histI[0],
                           int(nBins),
                           float(loBin), float(hiBin), 
                           int(histI[1]), float(histI[2]),
                           float(histI[3])), 
                 histI[5]+"/"+histI[6], histI[4], histI[7],
                 histI[8], histI[9])
    return trendplot

################################################################
# Get sub-dictionaries of plots from full dictionary of trendplots
################################################################

def getSinglePlots(trendPlots) :
    singleplots = {}
    nsingleplots = 0
    for tp in trendPlots.values() :
        if tp[4] == "None" :
            # Return histogram and type for drawing purposes
            singleplots[nsingleplots] = (tp[0], tp[2])
            nsingleplots = nsingleplots+1
    return singleplots

def getPlotFamily(trendPlots, familyname):
    plots = {}
    nplots = 0
    for tp in trendPlots.values() :
        if tp[4] == familyname : 
            plots[nplots] = tp[0]
            nplots = nplots+1
    return plots
        
########################################################
# Methods for harvesting info from run-by-run histograms
########################################################

def getHistMean(hist):
    if hist.GetEntries() == 0:
        return -99
    return hist.GetMean()

def getHistRms(hist):
    if hist.GetEntries() == 0: 
        return -99
    return hist.GetRMS()

def getAvgRate(hist, nLumiSegs) :
    if hist.GetEntries() == 0 : 
        return 0
    #nbins = 0.0
    #for bin in range(hist.GetNbinsX()):
    #    if hist.GetBinContent(bin) != 0: nbins=nbins+1
    # 1 bin = 1 luminosity segment = 93 seconds
    avg = hist.Integral()*80.0/(nLumiSegs*23.3)
    #print hist.GetName()
    #print avg
    return avg

def getTrigRate(hist, trig_bin, nLumiSegs) :
    # Get the bin number from the trig_bin string
    bin = int(trig_bin.split("_")[1])
    if hist.GetEntries() == 0 : 
        return -99
    # Return number of hits/divided by number of seconds in run.
    #    return float(hist.GetBinContent(int(bin)))/(93.0*float(nLumiSegs))
    return float(hist.GetBinContent(int(bin)))

# This actually harvests AND fills 2D plots
def fill2DPlot(hist, tp, runindex, nLumiSegs) :
#    print hist.GetName()
    if hist.GetEntries() == 0 : 
        #        print "Empty histogram", hist.GetName()
        return
    # +1 on range needed b/c GetNbinsX() counts underflow bin as 0. 
    for bin in range(1, hist.GetNbinsX()+1): #
        vbin = 0.0
        vbin = hist.GetBinContent(int(bin))
        # float(bin)-1.0 to convert from bin index to hpd number
        # (0.5 no longer needed to fill at center of bin)
        tp.Fill(float(runindex),
                float(bin)-1.0,
                float(vbin)*80/(23.3*float(nLumiSegs)))

######################################
# Format text to draw on plots
######################################
 
def makeRateText(runStart) :
    tplaces = [9., 8.5, 8., 7.5]
    tcolors = [1, 2, 4, 6]
    tnames = [ROOT.TString(" 20 GeV threshold"),
              ROOT.TString(" 50 GeV threshold"),
              ROOT.TString("100 GeV threshold"),
              ROOT.TString("500 GeV threshold")]
    ttholds = {}
    for i in range(4) :
        ttholds[i] = ROOT.TText(float(runStart),
                                float(tplaces[i]), str(tnames[i]))
        ttholds[i].SetTextColor(tcolors[i])
        ttholds[i].SetTextSize(0.05)
    return ttholds

def makeRateText2(runStart) :
    tplaces2 = [4.5, 4.25, 4., 3.75]
    tcolors2 = [1, 2, 4, 6]
    tnames2 = [ROOT.TString(" 20 GeV threshold"),
              ROOT.TString(" 50 GeV threshold"),
              ROOT.TString("100 GeV threshold"),
              ROOT.TString("500 GeV threshold")]
    ttholds2 = {}
    for i in range(4) :
        ttholds2[i] = ROOT.TText(float(runStart),
                                float(tplaces2[i]), str(tnames2[i]))
        ttholds2[i].SetTextColor(tcolors2[i])
        ttholds2[i].SetTextSize(0.05)
    return ttholds2

def makeRateText3(runStart) :
    tplaces3 = [18., 17., 16., 15.]
    tcolors3 = [1, 2, 4, 6]
    tnames3 = [ROOT.TString(" 20 GeV threshold"),
              ROOT.TString(" 50 GeV threshold"),
              ROOT.TString("100 GeV threshold"),
              ROOT.TString("500 GeV threshold")]
    ttholds3 = {}
    for i in range(4) :
        ttholds3[i] = ROOT.TText(float(runStart),
                                float(tplaces3[i]), str(tnames3[i]))
        ttholds3[i].SetTextColor(tcolors3[i])
        ttholds3[i].SetTextSize(0.05)
    return ttholds3

def makeJetText(runStart) :
    tplaces = [12.0, 11.5, 11., 10.5, 10., 9.5, 9.]
    tcolors = [1, 2, 3, 4, 6, 7, 28]
    tnames = [ROOT.TString("Jet6"),
              ROOT.TString("Jet10"),
              ROOT.TString("Jet20"),
              ROOT.TString("Jet30"),
              ROOT.TString("Jet40"),
              ROOT.TString("Jet50"),
              ROOT.TString("Jet60")]
    ttholds = {}
    for i in range(len(tnames)) :
        ttholds[i] = ROOT.TText(float(runStart),
                                float(tplaces[i]), str(tnames[i]))
        ttholds[i].SetTextColor(tcolors[i])
        ttholds[i].SetTextSize(0.04)
    return ttholds



######################################
# Make Run-by-Run Plots
######################################

def makeRunByRunPlots(indir, outdir, noiseScript, runnum):

    print "Making run-by-run plots for run %s" % runnum
    # Make directory for run-by-run plots
    os.system ("mkdir "+outdir+"r"+runnum)
    os.system ("ln -s "+outdir+"r"+runnum+" KbookPlots")
    os.system ("cp "+indir+"R"+runnum+".root .")
    
    # Execute root script that makes noise plots
    rootcommand = 'root -l -b -q \"'+noiseScript+'('+runnum+')\" > /dev/null'

    os.system(rootcommand)
    os.system("cp trendAnaRunTemplate.html KbookPlots/index.html")
    command_replace = "replace XXXXXX "+runnum+\
                      " -- KbookPlots/index.html >& /dev/null"
    os.system(command_replace)    

    # Make small gifs for html display
    files = glob.glob(outdir+"r"+runnum+"/*eps")
    for file in files:
        base = os.path.splitext(file)[0]
        command = "convert %s.eps -resize 300x300 %s_small.gif" % (base, base)
        os.system(command)    

    # Clean up
    os.system("rm -f KbookPlots")
    os.system("rm -f R"+runnum+".root")
    os.system("rm -rf R"+runnum)

################################################
# Method for drawing multiple plots on one canv
################################################

def drawOverlaidPlots(canvases, ncanv,  outdir, name, plotdict, textdict):
    if len(plotdict) == 0 : return
    first = True
    canvases[ncanv] = ROOT.TCanvas(name, name, 300, 300)
    
#    if name == "singlejet_rate":
#        pad=canvases[ncanv].GetPad(0)
#        pad.SetLogy()
    
    for plot in plotdict.values() : 
        if first :
            plot.Draw()
            first = False
        else :
            plot.Draw("same")

        for text in textdict.values() : text.Draw()

    name = canvases[ncanv].GetName()
    canvases[ncanv].SaveAs(outdir+name+".eps")
    canvases[ncanv].SaveAs(outdir+name+".gif")
    ncanv = ncanv+1
