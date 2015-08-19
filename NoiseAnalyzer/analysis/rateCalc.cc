///////////////////////////////////////////////////////////
// C S Cowden  
// Simple program to extract Hcal noise rate from histogram
// file produced by the HcalNoiseMonitor package.
// -------------------------------------------------------
// This analysis makes some assumptions which affect the accuracy
// of the rate calculation.  As the development matures,
// some of these assumptions perhaps could be removed or options
// added which will improve the accuracy when said assumptions are
// not valid.  The assumptions are listed here in no particular order:
// *) Data input is RAW file
// *) Any lumi section encountered by the CMSSW module is completely covered 
//    by the data processed.
// --------------------------------------------------------
////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <unistd.h>

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

// -- namespaces and typedefs
using namespace std;

//
// global variables
static const double g_LStime = 23.25; // seconds/LS
static const int nRBX = 80;  // number of RBX in HBHE


// 
// print program usage
void printUsage() {
  cout << "rateCalc [-r runNumber] [-s preScale]  histogramFile.root" << endl;
  return;
}


// --
// Main function of program
int main ( int argc, char ** argv ) {

  char * fileName = NULL;
  unsigned runNumber = 0;
  double preScale = 10.;
  // parse command line options

  int c;
  opterr = 0;
  while ( (c= getopt(argc,argv,"s:r:h")) != -1 )
    switch (c) {
      case 'r':
	runNumber = atoi(optarg);
	break;
      case 's':	
	preScale = atof(optarg);
	break;
      case 'h':
	printUsage();
	return 0;
    }

  if ( optind != argc ) fileName = argv[optind];
  

  if ( !fileName ) {
    cerr << "A file name must be provided!! Bailing out...." << endl;
    printUsage();
    return 1;
  }
  // --------------------------
  // open the root file
  TFile theFile(fileName);

  char name[100];
  const char *baseName = "hcalnoiseinfoanalyzer/Noise";
 

  // find total number of lumi-sections in this file.
  // retrieve lumi-section plot
  // scan and count non-zero bins in LS histogram
  sprintf(name,"%s/hLumiBlock",baseName);
  TH1D *h_lumiblock = (TH1D*)theFile.Get(name);

  unsigned numLS = 0;

  const unsigned nLSBins = h_lumiblock->GetNbinsX();
  // ignore underflows and overflows of the histogram
  for ( unsigned i=1; i != nLSBins; i++ ) {
    const double content = h_lumiblock->GetBinContent(i);
    if ( content > 0. ) numLS++;
  }

  // scale factor to convert rate to Hz
  const double factor = numLS*g_LStime/preScale;

  // -------------------------
  // Determine rates of noise
  // Mean of Noise Energy Distributions
  // RMS of Noise Energy Distributions

  // Average noise rate for several thresholds
  //  - HPD ion feedback (20, 50, 100, 500 GeV)
  sprintf(name,"%s/hLuminBlockNumberRate_HPDionfeed_20",baseName);
  TH1D *h_HPDionfeedback_20 = (TH1D*)theFile.Get(name);
  const double HPDionfeedback_20 = h_HPDionfeedback_20->Integral();

  sprintf(name,"%s/hLuminBlockNumberRate_HPDionfeed_50",baseName);
  TH1D *h_HPDionfeedback_50 = (TH1D*)theFile.Get(name);
  const double HPDionfeedback_50 = h_HPDionfeedback_50->Integral();

  sprintf(name,"%s/hLuminBlockNumberRate_HPDionfeed_100",baseName);
  TH1D *h_HPDionfeedback_100 = (TH1D*)theFile.Get(name);
  const double HPDionfeedback_100 = h_HPDionfeedback_100->Integral();

  sprintf(name,"%s/hLuminBlockNumberRate_HPDionfeed_500",baseName);
  TH1D *h_HPDionfeedback_500 = (TH1D*)theFile.Get(name);
  const double HPDionfeedback_500 = h_HPDionfeedback_500->Integral();

  //  - HPD discharge (20, 50, 100, 500 GeV)
  sprintf(name,"%s/hLuminBlockNumberRate_HPDdischg_20",baseName);
  TH1D *h_HPDdischarge_20 = (TH1D*)theFile.Get(name);
  const double HPDdischarge_20 = h_HPDdischarge_20->Integral();

  sprintf(name,"%s/hLuminBlockNumberRate_HPDdischg_50",baseName);
  TH1D *h_HPDdischarge_50 = (TH1D*)theFile.Get(name);
  const double HPDdischarge_50 = h_HPDdischarge_50->Integral();

  sprintf(name,"%s/hLuminBlockNumberRate_HPDdischg_100",baseName);
  TH1D *h_HPDdischarge_100 = (TH1D*)theFile.Get(name);
  const double HPDdischarge_100 = h_HPDdischarge_100->Integral();

  sprintf(name,"%s/hLuminBlockNumberRate_HPDdischg_500",baseName);
  TH1D *h_HPDdischarge_500 = (TH1D*)theFile.Get(name);
  const double HPDdischarge_500 = h_HPDdischarge_500->Integral();

  //  - RBX noise (20 50 100 500 GeV) 
  sprintf(name,"%s/hLuminBlockNumberRate_RBXNoise_20",baseName);
  TH1D *h_RBXNoise_20 = (TH1D*)theFile.Get(name);
  const double RBXNoise_20 = h_RBXNoise_20->Integral();

  sprintf(name,"%s/hLuminBlockNumberRate_RBXNoise_50",baseName);
  TH1D *h_RBXNoise_50 = (TH1D*)theFile.Get(name);
  const double RBXNoise_50 = h_RBXNoise_50->Integral();

  sprintf(name,"%s/hLuminBlockNumberRate_RBXNoise_100",baseName);
  TH1D *h_RBXNoise_100 = (TH1D*)theFile.Get(name);
  const double RBXNoise_100 = h_RBXNoise_100->Integral();

  sprintf(name,"%s/hLuminBlockNumberRate_RBXNoise_500",baseName);
  TH1D *h_RBXNoise_500 = (TH1D*)theFile.Get(name);
  const double RBXNoise_500 = h_RBXNoise_500->Integral();

  // ----------------------------------------
  // dump the noise rates to stdout for storage in a log file
  // or immediate processing
  cout << "Dumping noise rates for run number " << runNumber << endl;
  cout << "Total Number of LS covered: " << numLS << endl;
  cout << "Prescale:          " << preScale << endl;
  cout << "Conversion factor: " << factor << endl;
  cout << "------------------------------------------" << endl;
  cout << "BGLB" << endl;
  cout << "threshold HPD_ion HPD_discharge RBX_Noise" << endl;
  printf("20  %13f%13f%13f\n",HPDionfeedback_20/factor,HPDdischarge_20/factor,RBXNoise_20/factor);
  printf("50  %13f%13f%13f\n",HPDionfeedback_50/factor,HPDdischarge_50/factor,RBXNoise_50/factor);
  printf("100 %13f%13f%13f\n",HPDionfeedback_100/factor,HPDdischarge_100/factor,RBXNoise_100/factor);
  printf("500 %13f%13f%13f\n",HPDionfeedback_500/factor,HPDdischarge_500/factor,RBXNoise_500/factor);
  cout << "EGLB" << endl;


  cout << endl;

 

  //-----------------------------------------
  // RBX noise by RBX box number
  sprintf(name,"%s/hRBXNumberRate_RBXnoise_20",baseName);
  TH1D *h_RBXNumber_20 = (TH1D*)theFile.Get(name);

  sprintf(name,"%s/hRBXNumberRate_RBXnoise_50",baseName);
  TH1D *h_RBXNumber_50 = (TH1D*)theFile.Get(name);

  sprintf(name,"%s/hRBXNumberRate_RBXnoise_100",baseName);
  TH1D *h_RBXNumber_100 = (TH1D*)theFile.Get(name);

  sprintf(name,"%s/hRBXNumberRate_RBXnoise_500",baseName);
  TH1D *h_RBXNumber_500 = (TH1D*)theFile.Get(name);

  cout << "\nRBX Noise by RBX Number" << endl;
  cout << "Threshold: 20 GeV" << endl;
  cout << "BTH20" << endl;
  for ( int bin=1; bin <= nRBX; bin++ ) {
    cout << runNumber << " " << bin << " " << h_RBXNumber_20->GetBinContent(bin)/factor << endl;
  } 
  cout << "ETH20" << endl;

  cout << "Threshold: 50 GeV" << endl;
  cout << "BTH50" << endl;
  for ( int bin=1; bin <= nRBX; bin++ ) {
    cout << runNumber << " " << bin << " " << h_RBXNumber_50->GetBinContent(bin)/factor << endl;
  } 
  cout << "ETH50" << endl;
  
  cout << "Threshold: 100 GeV" << endl;
  cout << "BTH100" << endl;
  for ( int bin=1; bin <= nRBX; bin++ ) {
    cout << runNumber << " " << bin << " " << h_RBXNumber_100->GetBinContent(bin)/factor << endl;
  } 
  cout << "ETH100" << endl;


  cout << "Threshold: 500 GeV" << endl;
  cout << "BTH500" << endl;
  for ( int bin=1; bin <= nRBX; bin++ ) {
    cout << runNumber << " " << bin << " " << h_RBXNumber_500->GetBinContent(bin)/factor << endl;
  } 
  cout << "ETH500" << endl;


  return 0;
}
