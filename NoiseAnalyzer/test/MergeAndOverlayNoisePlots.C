#include <string>
#include <sstream>
#include <iostream>

#include "TFile.h"
#include "TH1D.h"

using namespace std;

//void MergeAndOverlayNoisePlots(int Run, bool makeEps=true){
void MergeAndOverlayNoisePlots(int Run){
  bool makeEps=true;


  stringstream filen;
  filen <<"R"<< Run<<".root";
  TFile* iF = new TFile(filen.str().c_str());
  iF->cd("hcalnoiseinfoanalyzer/Noise");

  string runno = "";
  TH1I* hRunNumber_= (TH1I*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRunNumber");
  stringstream runno_ss;
  if(hRunNumber_->GetMaximum() == hRunNumber_->GetEntries()){
    stringstream runno_ss;
//    runno_ss << hRunNumber_->GetMaximumBin() + 107999;
    runno_ss << hRunNumber_->GetMean();
    runno = runno_ss.str(); 
  }
  else {
    cout << "More than 1 run number in data file." << endl;
    cout << "Using run Number give: " << Run << endl;
    stringstream runno_ss;
    runno_ss << Run;
    runno = runno_ss.str();
  }

  TH1D* hLumiBlock_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLumiBlock");
  int nLumiBlocks=0;
  for(int ibin=0; ibin<hLumiBlock_->GetNbinsX(); ibin++){
    if( hLumiBlock_->GetBinContent(ibin) ) nLumiBlocks++;
      //std::cout << "ibin " << ibin << " " << hLumiBlock_->GetBinContent(ibin) << std::endl;
  }
  std::cout << "Number of Lumi Blocks " << nLumiBlocks << std::endl;
  double totalRunTime = nLumiBlocks*23.31; //93.0
  int prescale = 80;
  double rate         = prescale/totalRunTime;
  

  TH1D* hNRBXHits_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/NumRBXHits");

  TH1D* hAfterRBXEnergy_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hAfterRBXEnergy");
  hAfterRBXEnergy_->SetTitle((string("R")+runno+string(" Noise")).c_str());
  TH1D* hAfterRBXEnergyRate_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hAfterRBXEnergyRate");
  hAfterRBXEnergyRate_->SetTitle((string("R")+runno+string(" HBHE Noise Rate")).c_str());
  hAfterRBXEnergyRate_->Scale(rate);

  TH1D* hRBXEnergy_RBXnoise_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXEnergy_RBXnoise");
  TH1D* hRBXEnergyRate_RBXnoise_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXEnergyRate_RBXnoise");
  hRBXEnergyRate_RBXnoise_->Scale(rate);
  TH1D* hRBXEnergy_HPDionfeed_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXEnergy_HPDionfeed");
  TH1D* hRBXEnergyRate_HPDionfeed_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXEnergyRate_HPDionfeed");
  hRBXEnergyRate_HPDionfeed_->Scale(rate);
  TH1D* hRBXEnergy_HPDdischg_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXEnergy_HPDdischg");
  TH1D* hRBXEnergyRate_HPDdischg_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXEnergyRate_HPDdischg");
  hRBXEnergyRate_HPDdischg_->Scale(rate);

  TH1D* hRBXNumberRate_RBXnoise_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXNumberRate_RBXnoise_20");
  hRBXNumberRate_RBXnoise_20_->SetTitle((string("R")+runno+string(" RBXNoise (1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)")).c_str());
  hRBXNumberRate_RBXnoise_20_->Scale(rate);
  TH1D* hRBXNumberRate_RBXnoise_50_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXNumberRate_RBXnoise_50");
  hRBXNumberRate_RBXnoise_50_->Scale(rate);
  TH1D* hRBXNumberRate_RBXnoise_100_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXNumberRate_RBXnoise_100");
  hRBXNumberRate_RBXnoise_100_->Scale(rate);
  TH1D* hRBXNumberRate_RBXnoise_500_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXNumberRate_RBXnoise_500");
  hRBXNumberRate_RBXnoise_500_->Scale(rate);

  TH1D* hHPDNumberRate_HBpos_HPDnoise_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDnoise_20");
  hHPDNumberRate_HBpos_HPDnoise_20_->SetTitle((string("R")+runno+string(" HBpos Noise")).c_str());
  hHPDNumberRate_HBpos_HPDnoise_20_->Scale(rate);
  TH1D* hHPDNumberRate_HBneg_HPDnoise_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDnoise_20");
  hHPDNumberRate_HBneg_HPDnoise_20_->SetTitle((string("R")+runno+string(" HBneg Noise")).c_str());
  hHPDNumberRate_HBneg_HPDnoise_20_->Scale(rate);
  TH1D* hHPDNumberRate_HEpos_HPDnoise_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDnoise_20");
  hHPDNumberRate_HEpos_HPDnoise_20_->SetTitle((string("R")+runno+string(" HEpos Noise")).c_str());
  hHPDNumberRate_HEpos_HPDnoise_20_->Scale(rate);
  TH1D* hHPDNumberRate_HEneg_HPDnoise_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDnoise_20");
  hHPDNumberRate_HEneg_HPDnoise_20_->SetTitle((string("R")+runno+string(" HEneg Noise")).c_str());
  hHPDNumberRate_HEneg_HPDnoise_20_->Scale(rate);

  TH1D* hHPDNumberRate_HBpos_HPDionfeed_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDionfeed_20");
  hHPDNumberRate_HBpos_HPDionfeed_20_->SetTitle((string("R")+runno+string(" HBpos IonFeed")).c_str());
  hHPDNumberRate_HBpos_HPDionfeed_20_->Scale(rate);
  TH1D* hHPDNumberRate_HBneg_HPDionfeed_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDionfeed_20");
  hHPDNumberRate_HBneg_HPDionfeed_20_->SetTitle((string("R")+runno+string(" HBneg IonFeed")).c_str());
  hHPDNumberRate_HBneg_HPDionfeed_20_->Scale(rate);
  TH1D* hHPDNumberRate_HEpos_HPDionfeed_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDionfeed_20");
  hHPDNumberRate_HEpos_HPDionfeed_20_->SetTitle((string("R")+runno+string(" HEpos IonFeed")).c_str());
  hHPDNumberRate_HEpos_HPDionfeed_20_->Scale(rate);
  TH1D* hHPDNumberRate_HEneg_HPDionfeed_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDionfeed_20");
  hHPDNumberRate_HEneg_HPDionfeed_20_->SetTitle((string("R")+runno+string(" HEneg IonFeed")).c_str());
  hHPDNumberRate_HEneg_HPDionfeed_20_->Scale(rate);

  TH1D* hHPDNumberRate_HBpos_HPDdischg_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDdischg_20");
  hHPDNumberRate_HBpos_HPDdischg_20_->SetTitle((string("R")+runno+string(" HBpos Discharge")).c_str());
  hHPDNumberRate_HBpos_HPDdischg_20_->Scale(rate);
  TH1D* hHPDNumberRate_HBneg_HPDdischg_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDdischg_20");
  hHPDNumberRate_HBneg_HPDdischg_20_->SetTitle((string("R")+runno+string(" HBneg Discharge")).c_str());
  hHPDNumberRate_HBneg_HPDdischg_20_->Scale(rate);
  TH1D* hHPDNumberRate_HEpos_HPDdischg_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDdischg_20");
  hHPDNumberRate_HEpos_HPDdischg_20_->SetTitle((string("R")+runno+string(" HEpos Discharge")).c_str());
  hHPDNumberRate_HEpos_HPDdischg_20_->Scale(rate);
  TH1D* hHPDNumberRate_HEneg_HPDdischg_20_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDdischg_20");
  hHPDNumberRate_HEneg_HPDdischg_20_->SetTitle((string("R")+runno+string(" HEneg Discharge")).c_str());
  hHPDNumberRate_HEneg_HPDdischg_20_->Scale(rate);

  TH1D* hHPDNumberRate_HBpos_HPDnoise_50_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDnoise_50");
  hHPDNumberRate_HBpos_HPDnoise_50_->Scale(rate);  
  TH1D* hHPDNumberRate_HBneg_HPDnoise_50_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDnoise_50");
  hHPDNumberRate_HBneg_HPDnoise_50_->Scale(rate);  
  TH1D* hHPDNumberRate_HEpos_HPDnoise_50_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDnoise_50");
  hHPDNumberRate_HEpos_HPDnoise_50_->Scale(rate);  
  TH1D* hHPDNumberRate_HEneg_HPDnoise_50_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDnoise_50");
  hHPDNumberRate_HEneg_HPDnoise_50_->Scale(rate);  
  TH1D* hHPDNumberRate_HBpos_HPDionfeed_50_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDionfeed_50");
  hHPDNumberRate_HBpos_HPDionfeed_50_->Scale(rate);
  TH1D* hHPDNumberRate_HBneg_HPDionfeed_50_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDionfeed_50");
  hHPDNumberRate_HBneg_HPDionfeed_50_->Scale(rate);
  TH1D* hHPDNumberRate_HEpos_HPDionfeed_50_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDionfeed_50");
  hHPDNumberRate_HEpos_HPDionfeed_50_->Scale(rate);
  TH1D* hHPDNumberRate_HEneg_HPDionfeed_50_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDionfeed_50");
  hHPDNumberRate_HEneg_HPDionfeed_50_->Scale(rate);
  TH1D* hHPDNumberRate_HBpos_HPDdischg_50_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDdischg_50");
  hHPDNumberRate_HBpos_HPDdischg_50_->Scale(rate); 
  TH1D* hHPDNumberRate_HBneg_HPDdischg_50_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDdischg_50");
  hHPDNumberRate_HBneg_HPDdischg_50_->Scale(rate); 
  TH1D* hHPDNumberRate_HEpos_HPDdischg_50_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDdischg_50");
  hHPDNumberRate_HEpos_HPDdischg_50_->Scale(rate); 
  TH1D* hHPDNumberRate_HEneg_HPDdischg_50_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDdischg_50");
  hHPDNumberRate_HEneg_HPDdischg_50_->Scale(rate); 

  TH1D* hHPDNumberRate_HBpos_HPDnoise_100_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDnoise_100");
  hHPDNumberRate_HBpos_HPDnoise_100_->Scale(rate);  
  TH1D* hHPDNumberRate_HBneg_HPDnoise_100_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDnoise_100");
  hHPDNumberRate_HBneg_HPDnoise_100_->Scale(rate);  
  TH1D* hHPDNumberRate_HEpos_HPDnoise_100_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDnoise_100");
  hHPDNumberRate_HEpos_HPDnoise_100_->Scale(rate);  
  TH1D* hHPDNumberRate_HEneg_HPDnoise_100_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDnoise_100");
  hHPDNumberRate_HEneg_HPDnoise_100_->Scale(rate);  
  TH1D* hHPDNumberRate_HBpos_HPDionfeed_100_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDionfeed_100");
  hHPDNumberRate_HBpos_HPDionfeed_100_->Scale(rate);
  TH1D* hHPDNumberRate_HBneg_HPDionfeed_100_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDionfeed_100");
  hHPDNumberRate_HBneg_HPDionfeed_100_->Scale(rate);
  TH1D* hHPDNumberRate_HEpos_HPDionfeed_100_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDionfeed_100");
  hHPDNumberRate_HEpos_HPDionfeed_100_->Scale(rate);
  TH1D* hHPDNumberRate_HEneg_HPDionfeed_100_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDionfeed_100");
  hHPDNumberRate_HEneg_HPDionfeed_100_->Scale(rate);
  TH1D* hHPDNumberRate_HBpos_HPDdischg_100_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDdischg_100");
  hHPDNumberRate_HBpos_HPDdischg_100_->Scale(rate); 
  TH1D* hHPDNumberRate_HBneg_HPDdischg_100_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDdischg_100");
  hHPDNumberRate_HBneg_HPDdischg_100_->Scale(rate); 
  TH1D* hHPDNumberRate_HEpos_HPDdischg_100_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDdischg_100");
  hHPDNumberRate_HEpos_HPDdischg_100_->Scale(rate); 
  TH1D* hHPDNumberRate_HEneg_HPDdischg_100_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDdischg_100");
  hHPDNumberRate_HEneg_HPDdischg_100_->Scale(rate); 

  TH1D* hHPDNumberRate_HBpos_HPDnoise_500_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDnoise_500");
  hHPDNumberRate_HBpos_HPDnoise_500_->Scale(rate);  
  TH1D* hHPDNumberRate_HBneg_HPDnoise_500_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDnoise_500");
  hHPDNumberRate_HBneg_HPDnoise_500_->Scale(rate);  
  TH1D* hHPDNumberRate_HEpos_HPDnoise_500_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDnoise_500");
  hHPDNumberRate_HEpos_HPDnoise_500_->Scale(rate);  
  TH1D* hHPDNumberRate_HEneg_HPDnoise_500_  = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDnoise_500");
  hHPDNumberRate_HEneg_HPDnoise_500_->Scale(rate);  
  TH1D* hHPDNumberRate_HBpos_HPDionfeed_500_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDionfeed_500");
  hHPDNumberRate_HBpos_HPDionfeed_500_->Scale(rate);
  TH1D* hHPDNumberRate_HBneg_HPDionfeed_500_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDionfeed_500");
  hHPDNumberRate_HBneg_HPDionfeed_500_->Scale(rate);
  TH1D* hHPDNumberRate_HEpos_HPDionfeed_500_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDionfeed_500");
  hHPDNumberRate_HEpos_HPDionfeed_500_->Scale(rate);
  TH1D* hHPDNumberRate_HEneg_HPDionfeed_500_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDionfeed_500");
  hHPDNumberRate_HEneg_HPDionfeed_500_->Scale(rate);
  TH1D* hHPDNumberRate_HBpos_HPDdischg_500_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBpos_HPDdischg_500");
  hHPDNumberRate_HBpos_HPDdischg_500_->Scale(rate); 
  TH1D* hHPDNumberRate_HBneg_HPDdischg_500_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HBneg_HPDdischg_500");
  hHPDNumberRate_HBneg_HPDdischg_500_->Scale(rate); 
  TH1D* hHPDNumberRate_HEpos_HPDdischg_500_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEpos_HPDdischg_500");
  hHPDNumberRate_HEpos_HPDdischg_500_->Scale(rate); 
  TH1D* hHPDNumberRate_HEneg_HPDdischg_500_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hHPDNumberRate_HEneg_HPDdischg_500");
  hHPDNumberRate_HEneg_HPDdischg_500_->Scale(rate); 

  TH1D* hLuminBlockNumberRate_Noise_20_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_Noise_20");
  hLuminBlockNumberRate_Noise_20_->SetTitle((string("R")+runno+string(" LuminBlock Noise")).c_str());
  hLuminBlockNumberRate_Noise_20_->Scale(rate*nLumiBlocks); 
  TH1D* hLuminBlockNumberRate_Noise_50_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_Noise_50");
  hLuminBlockNumberRate_Noise_50_->Scale(rate*nLumiBlocks); 
  TH1D* hLuminBlockNumberRate_Noise_100_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_Noise_100");
  hLuminBlockNumberRate_Noise_100_->Scale(rate*nLumiBlocks);
  TH1D* hLuminBlockNumberRate_Noise_500_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_Noise_500");
  hLuminBlockNumberRate_Noise_500_->Scale(rate*nLumiBlocks);

  TH1D* hLuminBlockNumberRate_RBXNoise_20_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_RBXNoise_20");
  hLuminBlockNumberRate_RBXNoise_20_->SetTitle((string("R")+runno+string(" LuminBlock RBXNoise")).c_str());
  hLuminBlockNumberRate_RBXNoise_20_->Scale(rate*nLumiBlocks); 
  TH1D* hLuminBlockNumberRate_RBXNoise_50_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_RBXNoise_50");
  hLuminBlockNumberRate_RBXNoise_50_->Scale(rate*nLumiBlocks); 
  TH1D* hLuminBlockNumberRate_RBXNoise_100_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_RBXNoise_100");
  hLuminBlockNumberRate_RBXNoise_100_->Scale(rate*nLumiBlocks);
  TH1D* hLuminBlockNumberRate_RBXNoise_500_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_RBXNoise_500");
  hLuminBlockNumberRate_RBXNoise_500_->Scale(rate*nLumiBlocks);

  TH1D* hLuminBlockNumberRate_HPDNoise_20_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDNoise_20");
  hLuminBlockNumberRate_HPDNoise_20_->SetTitle((string("R")+runno+string(" LuminBlock HPDNoise")).c_str());
  hLuminBlockNumberRate_HPDNoise_20_->Scale(rate*nLumiBlocks);
  TH1D* hLuminBlockNumberRate_HPDNoise_50_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDNoise_50");
  hLuminBlockNumberRate_HPDNoise_50_->Scale(rate*nLumiBlocks); 
  TH1D* hLuminBlockNumberRate_HPDNoise_100_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDNoise_100");
  hLuminBlockNumberRate_HPDNoise_100_->Scale(rate*nLumiBlocks);
  TH1D* hLuminBlockNumberRate_HPDNoise_500_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDNoise_500");
  hLuminBlockNumberRate_HPDNoise_500_->Scale(rate*nLumiBlocks);

  TH1D* hLuminBlockNumberRate_HPDionfeed_20_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDionfeed_20");
  hLuminBlockNumberRate_HPDionfeed_20_->SetTitle((string("R")+runno+string(" LuminBlock IonFeed")).c_str());
  hLuminBlockNumberRate_HPDionfeed_20_->Scale(rate*nLumiBlocks); 
  TH1D* hLuminBlockNumberRate_HPDionfeed_50_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDionfeed_50");
  hLuminBlockNumberRate_HPDionfeed_50_->Scale(rate*nLumiBlocks); 
  TH1D* hLuminBlockNumberRate_HPDionfeed_100_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDionfeed_100");
  hLuminBlockNumberRate_HPDionfeed_100_->Scale(rate*nLumiBlocks);
  TH1D* hLuminBlockNumberRate_HPDionfeed_500_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDionfeed_500");
  hLuminBlockNumberRate_HPDionfeed_500_->Scale(rate*nLumiBlocks);

  TH1D* hLuminBlockNumberRate_HPDdischg_20_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDdischg_20");
  hLuminBlockNumberRate_HPDdischg_20_->SetTitle((string("R")+runno+string(" LuminBlock Discharge")).c_str());
  hLuminBlockNumberRate_HPDdischg_20_->Scale(rate*nLumiBlocks); 
  TH1D* hLuminBlockNumberRate_HPDdischg_50_ = (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDdischg_50");
  hLuminBlockNumberRate_HPDdischg_50_->Scale(rate*nLumiBlocks); 
  TH1D* hLuminBlockNumberRate_HPDdischg_100_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDdischg_100");
  hLuminBlockNumberRate_HPDdischg_100_->Scale(rate*nLumiBlocks);
  TH1D* hLuminBlockNumberRate_HPDdischg_500_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hLuminBlockNumberRate_HPDdischg_500");
  hLuminBlockNumberRate_HPDdischg_500_->Scale(rate*nLumiBlocks);
  
  TH1D* hE2ts_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hE2ts");
  TH1D* hE10ts_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hE10ts");
  TH1D* hE2tsOverE10ts_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hE2tsOverE10ts");
  
  TH1D* h5E2ts_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/h5E2ts");
  TH1D* h5E10ts_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/h5E10ts");
  TH1D* h5E2tsOverE10ts_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/h5E2tsOver5E10ts");

  TH1D* hRBXE2ts_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXE2ts");
  TH1D* hRBXE10ts_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXE10ts");
  TH1D* hRBXE2tsOverE10ts_= (TH1D*) iF->Get("hcalnoiseinfoanalyzer/Noise/hRBXE2tsOverE10ts");

  char cname[100];
  sprintf(cname, "%i_c1", Run);
  TCanvas* c1 = new TCanvas(cname, cname , 900, 600);
  //  TCanvas* c1 = new TCanvas("c1", "", 900, 600);
  c1->Divide(2,2);
  c1->cd(1);
  hHPDNumberRate_HBneg_HPDnoise_20_->Draw();
  hHPDNumberRate_HBneg_HPDnoise_50_->SetLineColor(kRed);
  hHPDNumberRate_HBneg_HPDnoise_50_->Draw("same");
  hHPDNumberRate_HBneg_HPDnoise_100_->SetLineColor(kGreen);
  hHPDNumberRate_HBneg_HPDnoise_100_->Draw("same");
  hHPDNumberRate_HBneg_HPDnoise_500_->SetLineColor(kBlue);
  hHPDNumberRate_HBneg_HPDnoise_500_->Draw("same");
  
  c1->cd(2);
  hHPDNumberRate_HBpos_HPDnoise_20_->Draw();
  hHPDNumberRate_HBpos_HPDnoise_50_->SetLineColor(kRed);
  hHPDNumberRate_HBpos_HPDnoise_50_->Draw("same");
  hHPDNumberRate_HBpos_HPDnoise_100_->SetLineColor(kGreen);
  hHPDNumberRate_HBpos_HPDnoise_100_->Draw("same");
  hHPDNumberRate_HBpos_HPDnoise_500_->SetLineColor(kBlue);
  hHPDNumberRate_HBpos_HPDnoise_500_->Draw("same");

  c1->cd(3);
   hHPDNumberRate_HEneg_HPDnoise_20_->Draw();
   hHPDNumberRate_HEneg_HPDnoise_50_->SetLineColor(kRed);
   hHPDNumberRate_HEneg_HPDnoise_50_->Draw("same");
   hHPDNumberRate_HEneg_HPDnoise_100_->SetLineColor(kGreen);
   hHPDNumberRate_HEneg_HPDnoise_100_->Draw("same");
   hHPDNumberRate_HEneg_HPDnoise_500_->SetLineColor(kBlue);
   hHPDNumberRate_HEneg_HPDnoise_500_->Draw("same");

   c1->cd(4);
   hHPDNumberRate_HEpos_HPDnoise_20_->Draw();
   hHPDNumberRate_HEpos_HPDnoise_50_->SetLineColor(kRed);
   hHPDNumberRate_HEpos_HPDnoise_50_->Draw("same");
   hHPDNumberRate_HEpos_HPDnoise_100_->SetLineColor(kGreen);
   hHPDNumberRate_HEpos_HPDnoise_100_->Draw("same");
   hHPDNumberRate_HEpos_HPDnoise_500_->SetLineColor(kBlue);
   hHPDNumberRate_HEpos_HPDnoise_500_->Draw("same");

   if(makeEps)
     c1->SaveAs(( string("KbookPlots/R")+runno+string("_HPD_Noise")+string(".eps") ).c_str());
    // c1->SaveAs(( string("R")+runno+string("/HPD_Noise")+string(".gif") ).c_str());

  sprintf(cname, "%i_c2", Run);
  TCanvas* c2 = new TCanvas(cname, cname , 900, 600);
  //TCanvas* c2 = new TCanvas("c2", "", 900, 600);
   c2->Divide(2,2);
   c2->cd(1);
   hHPDNumberRate_HBneg_HPDionfeed_20_->Draw();
   hHPDNumberRate_HBneg_HPDionfeed_50_->SetLineColor(kRed);
   hHPDNumberRate_HBneg_HPDionfeed_50_->Draw("same");
   hHPDNumberRate_HBneg_HPDionfeed_100_->SetLineColor(kGreen);
   hHPDNumberRate_HBneg_HPDionfeed_100_->Draw("same");
   hHPDNumberRate_HBneg_HPDionfeed_500_->SetLineColor(kBlue);
   hHPDNumberRate_HBneg_HPDionfeed_500_->Draw("same");

   c2->cd(2);
   hHPDNumberRate_HBpos_HPDionfeed_20_->Draw();
   hHPDNumberRate_HBpos_HPDionfeed_50_->SetLineColor(kRed);
   hHPDNumberRate_HBpos_HPDionfeed_50_->Draw("same");
   hHPDNumberRate_HBpos_HPDionfeed_100_->SetLineColor(kGreen);
   hHPDNumberRate_HBpos_HPDionfeed_100_->Draw("same");
   hHPDNumberRate_HBpos_HPDionfeed_500_->SetLineColor(kBlue);
   hHPDNumberRate_HBpos_HPDionfeed_500_->Draw("same");

   c2->cd(3);
   hHPDNumberRate_HEneg_HPDionfeed_20_->Draw();
   hHPDNumberRate_HEneg_HPDionfeed_50_->SetLineColor(kRed);
   hHPDNumberRate_HEneg_HPDionfeed_50_->Draw("same");
   hHPDNumberRate_HEneg_HPDionfeed_100_->SetLineColor(kGreen);
   hHPDNumberRate_HEneg_HPDionfeed_100_->Draw("same");
   hHPDNumberRate_HEneg_HPDionfeed_500_->SetLineColor(kBlue);
   hHPDNumberRate_HEneg_HPDionfeed_500_->Draw("same");

   c2->cd(4);
   hHPDNumberRate_HEpos_HPDionfeed_20_->Draw();
   hHPDNumberRate_HEpos_HPDionfeed_50_->SetLineColor(kRed);
   hHPDNumberRate_HEpos_HPDionfeed_50_->Draw("same");
   hHPDNumberRate_HEpos_HPDionfeed_100_->SetLineColor(kGreen);
   hHPDNumberRate_HEpos_HPDionfeed_100_->Draw("same");
   hHPDNumberRate_HEpos_HPDionfeed_500_->SetLineColor(kBlue);
   hHPDNumberRate_HEpos_HPDionfeed_500_->Draw("same");

   if(makeEps)
   c2->SaveAs( (string("KbookPlots/R")+runno+string("_HPDionfeed_Noise")+string(".eps")).c_str());
   //c2->SaveAs(( string("R")+runno+string("/HPDionfeed_Noise")+string(".gif") ).c_str()); 
 
  sprintf(cname, "%i_c3", Run);
  TCanvas* c3 = new TCanvas(cname, cname , 900, 600);
  //TCanvas* c3 = new TCanvas("c3", "", 900, 600);
   c3->Divide(2,2);
   c3->cd(1);
   hHPDNumberRate_HBneg_HPDdischg_20_->Draw();
   hHPDNumberRate_HBneg_HPDdischg_50_->SetLineColor(kRed);
   hHPDNumberRate_HBneg_HPDdischg_50_->Draw("same");
   hHPDNumberRate_HBneg_HPDdischg_100_->SetLineColor(kGreen);
   hHPDNumberRate_HBneg_HPDdischg_100_->Draw("same");
   hHPDNumberRate_HBneg_HPDdischg_500_->SetLineColor(kBlue);
   hHPDNumberRate_HBneg_HPDdischg_500_->Draw("same");
  
   c3->cd(2);
   hHPDNumberRate_HBpos_HPDdischg_20_->Draw();
   hHPDNumberRate_HBpos_HPDdischg_50_->SetLineColor(kRed);
   hHPDNumberRate_HBpos_HPDdischg_50_->Draw("same");
   hHPDNumberRate_HBpos_HPDdischg_100_->SetLineColor(kGreen);
   hHPDNumberRate_HBpos_HPDdischg_100_->Draw("same");
   hHPDNumberRate_HBpos_HPDdischg_500_->SetLineColor(kBlue);
   hHPDNumberRate_HBpos_HPDdischg_500_->Draw("same");

   c3->cd(3);
   hHPDNumberRate_HEneg_HPDdischg_20_->Draw();
   hHPDNumberRate_HEneg_HPDdischg_50_->SetLineColor(kRed);
   hHPDNumberRate_HEneg_HPDdischg_50_->Draw("same");
   hHPDNumberRate_HEneg_HPDdischg_100_->SetLineColor(kGreen);
   hHPDNumberRate_HEneg_HPDdischg_100_->Draw("same");
   hHPDNumberRate_HEneg_HPDdischg_500_->SetLineColor(kBlue);
   hHPDNumberRate_HEneg_HPDdischg_500_->Draw("same");

   c3->cd(4);
   hHPDNumberRate_HEpos_HPDdischg_20_->Draw();
   hHPDNumberRate_HEpos_HPDdischg_50_->SetLineColor(kRed);
   hHPDNumberRate_HEpos_HPDdischg_50_->Draw("same");
   hHPDNumberRate_HEpos_HPDdischg_100_->SetLineColor(kGreen);
   hHPDNumberRate_HEpos_HPDdischg_100_->Draw("same");
   hHPDNumberRate_HEpos_HPDdischg_500_->SetLineColor(kBlue);
   hHPDNumberRate_HEpos_HPDdischg_500_->Draw("same");

   if(makeEps)
   c3->SaveAs(( string("KbookPlots/R")+runno+string("_HPDdischg_Noise")+string(".eps")).c_str());
  // c3->SaveAs(( string("R")+runno+string("/HPDdischg_Noise")+string(".gif") ).c_str());

  sprintf(cname, "%i_c4", Run);
  TCanvas* c4 = new TCanvas(cname, cname , 900, 600);
  //TCanvas* c4 = new TCanvas("c4", "", 900, 450);
   c4->Divide(2,1);
   c4->cd(1);
   hRBXNumberRate_RBXnoise_20_->Draw();
   hRBXNumberRate_RBXnoise_50_->SetLineColor(kRed);
   hRBXNumberRate_RBXnoise_50_->Draw("same");
   hRBXNumberRate_RBXnoise_100_->SetLineColor(kGreen);
   hRBXNumberRate_RBXnoise_100_->Draw("same");
   hRBXNumberRate_RBXnoise_500_->SetLineColor(kBlue);
   hRBXNumberRate_RBXnoise_500_->Draw("same");
   c4->cd(2);
	hNRBXHits_->Draw();
   c4->GetPad(2)->SetLogy();

   if(makeEps)
   c4->SaveAs((string("KbookPlots/R")+runno+string("_RBX_Noise")+string(".eps")).c_str());
  // c4->SaveAs(( string("R")+runno+string("/RBX_Noise")+string(".gif") ).c_str());

  sprintf(cname, "%i_c5", Run);
  TCanvas* c5 = new TCanvas(cname, cname , 900, 600);
  //TCanvas* c5 = new TCanvas("c5", "", 900, 450);
   /*
   c5->Divide(2,1);
   c5->cd(1);
   hAfterRBXEnergy_->Draw();
   hRBXEnergy_RBXnoise_->SetLineColor(kGreen);
   hRBXEnergy_RBXnoise_->Draw("same");
   hRBXEnergy_HPDionfeed_->SetLineColor(kRed);
   hRBXEnergy_HPDionfeed_->Draw("same");
   hRBXEnergy_HPDdischg_->SetLineColor(kBlue);
   hRBXEnergy_HPDdischg_->Draw("same");

   c5->cd(2);
   //hAfterRBXEnergyRate_->SetAxisRange(0.01, 100, "Y");
   */
   
   hAfterRBXEnergyRate_->SetXTitle("RBX energy threshold (GeV)");
   hAfterRBXEnergyRate_->SetYTitle("Rate (Hz)");
   hAfterRBXEnergyRate_->SetStats(0);
   hAfterRBXEnergyRate_->SetLineWidth(2);
   hAfterRBXEnergyRate_->SetAxisRange(0.1, 100, "Y");
   hAfterRBXEnergyRate_->Draw();

   c5->SetLogy();
   hRBXEnergyRate_RBXnoise_->SetLineWidth(2);
   hRBXEnergyRate_HPDionfeed_->SetLineWidth(2);
   hRBXEnergyRate_HPDdischg_->SetLineWidth(2);
   hRBXEnergyRate_RBXnoise_->SetLineColor(6);
   hRBXEnergyRate_RBXnoise_->Draw("same");
   hRBXEnergyRate_HPDionfeed_->SetLineColor(kRed);
   hRBXEnergyRate_HPDionfeed_->Draw("same");
   hRBXEnergyRate_HPDdischg_->SetLineColor(kBlue);
   hRBXEnergyRate_HPDdischg_->Draw("same");

   if(makeEps)
   c5->SaveAs((string("KbookPlots/R")+runno+string("_HBHE_Noise")+string(".eps")).c_str());
  // c5->SaveAs(( string("R")+runno+string("/Noise_Rate")+string(".gif") ).c_str());  
 
  sprintf(cname, "%i_c6", Run);
  TCanvas* c6 = new TCanvas(cname, cname , 900, 600);
  //TCanvas* c6 = new TCanvas("c5", "", 1200, 900);
   c6->Divide(2,3);

 //  bool max200 = true;
 //  if(hLuminBlockNumberRate_Noise_20_->Integral(200, 500) != 0 )
 //    max200 = false;
   
   int max;

   if(hLuminBlockNumberRate_Noise_20_->Integral(1000, 3000) != 0){
    max = 3000;
   }
   else if(hLuminBlockNumberRate_Noise_20_->Integral(300, 1000) != 0){
    max = 1000;
   }else
    max = 300;


   c6->cd(1);
   hLuminBlockNumberRate_Noise_20_->Draw();
   hLuminBlockNumberRate_Noise_20_->GetXaxis()->SetRange(0, max);
   hLuminBlockNumberRate_Noise_50_->SetLineColor(kRed);
   hLuminBlockNumberRate_Noise_50_->Draw("same");
   hLuminBlockNumberRate_Noise_100_->SetLineColor(kGreen);
   hLuminBlockNumberRate_Noise_100_->Draw("same");
   hLuminBlockNumberRate_Noise_500_->SetLineColor(kBlue);
   hLuminBlockNumberRate_Noise_500_->Draw("same");

   c6->cd(2);
   hLuminBlockNumberRate_RBXNoise_20_->Draw();
   hLuminBlockNumberRate_RBXNoise_20_->GetXaxis()->SetRange(0, max);
   hLuminBlockNumberRate_RBXNoise_50_->SetLineColor(kRed);
   hLuminBlockNumberRate_RBXNoise_50_->Draw("same");
   hLuminBlockNumberRate_RBXNoise_100_->SetLineColor(kGreen);
   hLuminBlockNumberRate_RBXNoise_100_->Draw("same");
   hLuminBlockNumberRate_RBXNoise_500_->SetLineColor(kBlue);
   hLuminBlockNumberRate_RBXNoise_500_->Draw("same");

   c6->cd(3);
   hLuminBlockNumberRate_HPDNoise_20_->Draw();
   hLuminBlockNumberRate_HPDNoise_20_->GetXaxis()->SetRange(0, max);
   hLuminBlockNumberRate_HPDNoise_50_->SetLineColor(kRed);
   hLuminBlockNumberRate_HPDNoise_50_->Draw("same");
   hLuminBlockNumberRate_HPDNoise_100_->SetLineColor(kGreen);
   hLuminBlockNumberRate_HPDNoise_100_->Draw("same");
   hLuminBlockNumberRate_HPDNoise_500_->SetLineColor(kBlue);
   hLuminBlockNumberRate_HPDNoise_500_->Draw("same");

   c6->cd(4);
   hLuminBlockNumberRate_HPDionfeed_20_->Draw();
   hLuminBlockNumberRate_HPDionfeed_20_->GetXaxis()->SetRange(0, max);
   hLuminBlockNumberRate_HPDionfeed_50_->SetLineColor(kRed);
   hLuminBlockNumberRate_HPDionfeed_50_->Draw("same");
   hLuminBlockNumberRate_HPDionfeed_100_->SetLineColor(kGreen);
   hLuminBlockNumberRate_HPDionfeed_100_->Draw("same");
   hLuminBlockNumberRate_HPDionfeed_500_->SetLineColor(kBlue);
   hLuminBlockNumberRate_HPDionfeed_500_->Draw("same");

   c6->cd(5);
   hLuminBlockNumberRate_HPDdischg_20_->Draw();
   hLuminBlockNumberRate_HPDdischg_20_->GetXaxis()->SetRange(0, max);
   hLuminBlockNumberRate_HPDdischg_50_->SetLineColor(kRed);
   hLuminBlockNumberRate_HPDdischg_50_->Draw("same");
   hLuminBlockNumberRate_HPDdischg_100_->SetLineColor(kGreen);
   hLuminBlockNumberRate_HPDdischg_100_->Draw("same");
   hLuminBlockNumberRate_HPDdischg_500_->SetLineColor(kBlue);
   hLuminBlockNumberRate_HPDdischg_500_->Draw("same");
  
   c6->cd(6);
   hRunNumber_->Draw();

   if(makeEps)
   c6->SaveAs((string("KbookPlots/R")+runno+string("_LumiBlocks")+string(".eps")).c_str());
  // c6->SaveAs(( string("R")+runno+string("/LumiBlocks")+string(".gif") ).c_str());

  sprintf(cname, "%i_c7", Run);
  TCanvas* c7 = new TCanvas(cname, cname , 900, 600);
  //TCanvas* c7 = new TCanvas("c7", "", 1200, 900);
   c7->Divide(2, 3);
   c7->cd(1);
   hE2ts_->Draw();
   c7->cd(2);
   h5E2ts_->Draw();
   c7->cd(3);
   hE10ts_->Draw();
   c7->cd(4);
   h5E10ts_->Draw();
   c7->cd(5);
   hE2tsOverE10ts_->Draw();
   c7->cd(6);
   h5E2tsOverE10ts_->Draw();
   
   if(makeEps)
   c7->SaveAs((string("KbookPlots/R")+runno+string("_HPD_BigTS")+string(".eps")).c_str());
  // c7->SaveAs(( string("R")+runno+string("/HB_BigTS")+string(".gif") ).c_str());

  sprintf(cname, "%i_c8", Run);
  TCanvas* c8 = new TCanvas(cname, cname , 900, 600);
  //TCanvas* c8 = new TCanvas("c8", "", 1200, 600);
   c8->Divide(1, 3);
   c8->cd(1);
   hRBXE2ts_->Draw();
   c8->cd(2);
   hRBXE10ts_->Draw();
   c8->cd(3);
   hRBXE2tsOverE10ts_->Draw();

   if(makeEps)
   c8->SaveAs((string("KbookPlots/R")+runno+string("_RBX_BigTS")+string(".eps")).c_str());
  // c8->SaveAs(( string("R")+runno+string("/RBX_BigTS")+string(".gif") ).c_str());
   


}

