//
// HcalNoiseInfoProducer.cc
//
//   description: HcalNoiseMon version of /RecoMET/METAnalyzers/interface/HcalNoiseInfoAnalyzer.h
//
//   author: J.P. Chou, Brown
//
//	Modified by C M Jenkins
//			Department of Physics
//			University of South Alabama
//			Mobile, AL 36688
//		Change binnings for RBX and HPD energy threshold
//	Modified by C M Jenkins 2012Aug18
//			Universityu of South Alabama 
//			Add histograms for ieta vs iphi RBX energy weighted.
//
//

//cmj2011Jul Ordering may be important
//cmj2011Jul #include "HcalNoiseMon/NoiseAnalyzer/interface/HcalNoiseInfoAnalyzer.h"
#include "RecoMET/METAlgorithms/interface/HcalNoiseRBXArray.h"
#include "FWCore/Utilities/interface/EDMException.h"

//----------------CaloMET------------------------------
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
//----------------MET------------------------------
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

// cmj2011Jul15 - from Suichi
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
// cmj2012Apr25 #include "HcalNoiseMon/NoiseAnalyzer/interface/HcalNoiseInfoAnalyzer.h"
#include "HcalNoiseMonitor/NoiseAnalyzer/interface/HcalNoiseInfoAnalyzer.h"
// cmj2011Jul15 - from Suichi

#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"

using namespace reco;
using namespace edm;
using namespace std;
//
// constructors and destructor
//
  
HcalNoiseInfoAnalyzer::HcalNoiseInfoAnalyzer(const edm::ParameterSet& iConfig)
{
  // set parameters
  skDumpFlag=0;
  skDumpCounter=0;

  
  rbxCollName_    = iConfig.getParameter<std::string>("rbxCollName");
//cmj2011Jujl1  metTag_ = iConfig.getUntrackedParameter<edm::InputTag>("MetTag");
  
  ////
  skipLumiBlocks_ = iConfig.getParameter<double>("SkipLumiBlocks");
  numLumiBlocks_ = iConfig.getParameter<double>("NumLumiBlocks");
  ////

  truefirstLumi_ = firstLumiBlock_ = maxLumiBlock_ = -1;
  timeDone = false;

//cmj2011Jul15  ePrint = new PrintInfo(iConfig);
}
  
  
HcalNoiseInfoAnalyzer::~HcalNoiseInfoAnalyzer() {
//cmj2011Jul15  delete  ePrint;
}
  
// ------------ method called to for each event  ------------
void HcalNoiseInfoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  hRunNumber_->Fill(iEvent.id().run());
  
  skDumpFlag=0;

  // determine the lumi block 
  // and do some start of run, once a run stuff here
  int lumiBlock = iEvent.luminosityBlock();
  hLumiBlock_->Fill(lumiBlock);

	if(iEvent.bunchCrossing()>3450)return;

  hBunchCrossing_->Fill(iEvent.bunchCrossing());

  if(firstLumiBlock_<0){
    truefirstLumi_  = lumiBlock;
    firstLumiBlock_ = lumiBlock;

    // Get and store beginning and ending time of run
  }

  if(firstLumiBlock_ != int(iEvent.luminosityBlock()) ) {
    firstLumiBlock_ = iEvent.luminosityBlock();
  }


//cmj2011Jul15 - Drop time segment
/*
  if (!timeDone){
    edm::TimeValue_t bTime = iEvent.getRun().beginTime().value();
    edm::TimeValue_t eTime = iEvent.getRun().endTime().value();
    time_t sbtime = bTime >> 32;
    time_t setime = eTime >> 32;
    struct tm b_uptm;
    struct tm e_uptm;
    gmtime_r(&sbtime, &b_uptm);
    gmtime_r(&setime, &e_uptm);
    char b_datestring[256];
    char e_datestring[256];

    // 01 Sep 2009 
    // Per discussion with Liz Kennedy & Chris Jones: It
    // is a known problem that iEvent.getRun().endTime() and
    // iEvent.getRun().beginTime() do NOT work as expected.  The
    // reported time occurs sometime during the run, but not
    // necessarily at the beginning (or end) of the run.  When this is
    // fixed in framework we should make sure our code works...

    // "%c" gives full date/time (Tue Sep 01 11:25:58 2009)
    strftime(b_datestring, sizeof(b_datestring),"%c",&b_uptm);
    strftime(e_datestring, sizeof(e_datestring),"%c",&e_uptm);
    //     %x" gives date (08/01/09)
    //     strftime(b_datestring, sizeof(b_datestring),"%x",&b_uptm);
    //     strftime(e_datestring, sizeof(e_datestring),"%x",&e_uptm);
    hRunStartHolder_->SetTitle(b_datestring);
    hRunStopHolder_->SetTitle(e_datestring);
    timeDone = true;
  }
*/
//cmj2011Jul15 - Drop time segment

  /*
  bool found = false;
  for(int i=0; i!=int(lumi.size()); ++i){
    if(lumi.at(i) == iEvent.luminosityBlock()){
      found = true;
    }
  }
  if(!found) lumi.push_back(iEvent.luminosityBlock());

  if(lumi.size() < skipLumiBlocks_ || (lumi.size() > numLumiBlocks_+skipLumiBlocks_ && numLumiBlocks_ > 0)) {
    cout << "Lumi : " << iEvent.luminosityBlock() << " skipped." <<  endl;
    return;
  }
  */

  // get the MET collection
/*
  const CaloMET *met;
  Handle<CaloMETCollection> metHandle;
  iEvent.getByLabel(metTag_, metHandle);
  if(metHandle.isValid()) {
    const CaloMETCollection *metCol = metHandle.product();
    met = &(metCol->front());
    hMet_->Fill( met->pt());
    hMetRate_->Fill(met->pt());			
  } else {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find CaloMETCollection named " << metTag_ << ".\n";
    return;
  }  
*/
  

	// L1 trigger
  Handle <L1GlobalTriggerReadoutRecord> gtRecord_h;
  iEvent.getByLabel ("gtDigis",gtRecord_h); // assume only one L1 trigger record here
  const L1GlobalTriggerReadoutRecord* gtRecord = gtRecord_h.failedToGet () ? 0 : &*gtRecord_h;

  if (gtRecord) { // object is available
    
    //algo trigger bits
    for (int l1bit = 0; l1bit < 128; ++l1bit) {
      if (gtRecord->decisionWord() [l1bit]) h_l1_gtbit->Fill (l1bit);
    }
    
    //technical trigger bits
    for (int l1bit = 0; l1bit < 64; ++l1bit) {
      if (gtRecord->technicalTriggerWord() [l1bit]) h_l1_gtbit_techtrig->Fill (l1bit);
    	}
	}


  // get the RBX collection
  Handle<HcalNoiseRBXCollection> handle;
  iEvent.getByLabel(rbxCollName_,handle);
  if(!handle.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find HcalNoiseRBXCollection named " << rbxCollName_ << ".\n";
    return;
  }
//cmj2012Aug18 ---------- Add the hcal rechit collection ----------------------
   //hcal rechits
   Handle<HBHERecHitCollection> hbhe_hits_h;
   iEvent.getByLabel("hbhereco", hbhe_hits_h);
   const HBHERecHitCollection* hbhe_hits = hbhe_hits_h.failedToGet () ? 0 : &*hbhe_hits_h;
//cmj2012Aug18 ----------- Add the hcal rechit collection ----------------------


  // total number of RBX objects
  hNRBXobjects_->Fill(handle->size());
  int nrbx20gev=0;

  // loop over the RBXs
  for(HcalNoiseRBXCollection::const_iterator rit=handle->begin(); rit!=handle->end(); ++rit) {

    // get the rbx
    HcalNoiseRBX rbx    = (*rit);
    
    // get the hpd with max signal
    HcalNoiseHPD maxhpd = (*rbx.maxHPD());
     
    // calculate some basic quantities
    int    numRBXhits = rbx.numRecHits(1.5);
    double rbxenergy  = rbx.recHitEnergy(1.5);
 
    // fill the energy in the RBX before we do anything else
    hBeforeRBXEnergy_->Fill(rbxenergy);
    hBeforeRBXEnergyRate_->Fill(rbxenergy);
    
    // count number of RBX objects with E>20 GeV
    if (rbxenergy>20.0) nrbx20gev++;

    // require at least 20 GeV in the RBX and at least one hit above 1.5 GeV
    // to even pursue rejecting as noise
    unsigned int failures         = 0;  // bits for "failures"
    double       hpdEnergyHighest = 0.; // Highest energy HPD's energy
    double       nHitsHighest     = 0.; // number of hits
    int          hpdNumberHighest = 0;  // ID number
    int          hpdSubdetHighest = 0;  // subdet
    int          hpdZsideHighest  = 0;  // side

    if(numRBXhits>0 && rbxenergy>10.0) {

      hMaxZeros_->Fill(rbx.maxZeros());
      hTotalZeros_->Fill(rbx.totalZeros());
      
      if(rbx.maxZeros()>3)   failures |= 0x1;
      if(rbx.totalZeros()>7) failures |= 0x2;
      
      double totale2ts=rbx.allChargeHighest2TS();
      double totale10ts=rbx.allChargeTotal();
      
      // loop over the HPDs in the RBX
      std::vector<HcalNoiseHPD> theHPDs = rbx.HPDs();
      
      for(std::vector<HcalNoiseHPD>::const_iterator hit=theHPDs.begin(); hit!=theHPDs.end(); ++hit) {
	HcalNoiseHPD hpd=(*hit);
	
	if ( hpd.recHitEnergy(1.5) > hpdEnergyHighest ) {
	  hpdEnergyHighest = hpd.recHitEnergy(1.5); // Highest energy HPD's energy
	  nHitsHighest     = hpd.numRecHits(1.5);   //                      number of hits
	  hpdNumberHighest = hpd.idnumber()+1;        //                      ID number
	  hpdSubdetHighest = HcalHPDRBXMap::subdetHPD(hpd.idnumber());    //                      subdet
	  hpdZsideHighest  = HcalHPDRBXMap::zsideHPD(hpd.idnumber());          //                      zside
	}

	// make sure we have at least 1 hit above 5 GeV
	if(hpd.numRecHits(5.0)<1) continue;
	
	double e2ts=hpd.bigChargeHighest2TS();
	double e10ts=hpd.bigChargeTotal();
	
	hE2ts_->Fill(e2ts);
	hE10ts_->Fill(e10ts);
	hE2tsOverE10ts_->Fill(e10ts ? e2ts/e10ts : -999);
		
	e2ts = hpd.big5ChargeHighest2TS();
	e10ts = hpd.big5ChargeTotal();

	h5E2ts_->Fill(e2ts);
	h5E10ts_->Fill(e10ts);
	h5E2tsOverE10ts_->Fill(e10ts ? e2ts/e10ts : -999);

	if(e10ts && !(e2ts/e10ts<0.95 && e2ts/e10ts>0.70)) failures |= 0x4;
		
	int numHPDhits = hpd.numRecHits(1.5);

	hHPDNHits_->Fill(numHPDhits);
	hHPDNHits2_->Fill(numHPDhits);
	if(numHPDhits>16) failures |= 0x8;

      }// theHPDs

      if(totale10ts && totale2ts/totale10ts<0.70) failures |= 0x10;

      //
      // Categorize noise
      int  noise_category = 0;
      bool isRBXnoise     = false;
      bool isHPDnoise     = false;
      bool isHPDionfeed   = false;
      bool isHPDdischg    = false;
      
      // if(failures){
      //Asli
      hHPDs_vs_RBX_Energy->Fill(hpdEnergyHighest/rbxenergy);
      if (hpdEnergyHighest/rbxenergy>0.98){                            // HPD noise
	isHPDnoise=true;
	skDumpFlag=0;       
	if (nHitsHighest>=9){ 
	  isHPDdischg=true;  
	  noise_category = 7;
	  skDumpFlag=0;       
	} // HPD noise discharge
	else{                 
	  
	  isHPDionfeed=true; 
	  skDumpFlag=0;       
	  noise_category = 8;
	} // HPD noise ion feedback
      }
      else{ 
	isRBXnoise=true; 
	noise_category = 1;
	if(totale2ts/totale10ts < 0.33)                    noise_category = 2;
	else if(totale2ts/totale10ts < 0.8)                noise_category = 3; 	
	else if(totale2ts/totale10ts>0.8 && numRBXhits>10) noise_category = 4; 	
	else if(totale2ts/totale10ts>0.8 && numRBXhits<10) noise_category = 5; 	
	skDumpFlag=1;       
      }                       // RBX noise
	
      hHCalNoiseCategory_->Fill(noise_category); 
      //        }
      
      //
      // Make plots based on noise categories
      if ( isRBXnoise ){          // RBX noise
//cmj2012Aug18 -----------------------------------------------------------
//cmj2012Aug18 ----------- Add the hcal rechit collection ----------------------
                double cmjRbxEnergy = -900.0;
                if (hbhe_hits) { // object is available 
                 for (HBHERecHitCollection::const_iterator hhit=hbhe_hits->begin(); hhit!=hbhe_hits->end(); hhit++) {
                 cmjRbxEnergy = hhit->energy();
//cmj2012Aug7
                     if(cmjRbxEnergy > 20.0) {   // 20 GeV cut
                     h_rechit_eta_RBX_energy_20 ->Fill((hhit->id()).ieta(),cmjRbxEnergy);
                     h_rechit_phi_RBX_energy_20 ->Fill((hhit->id()).iphi(),cmjRbxEnergy);
                     h_rechit_eta_phi_RBX_energy_20->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       if((hhit->id()).depth() == 1){
                       h_rechit_eta_phi_depth1_RBX_energy_20->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       }
                       if((hhit->id()).depth() == 2){
                       h_rechit_eta_phi_depth2_RBX_energy_20->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       }
                       if((hhit->id()).depth() == 3){
                       h_rechit_eta_phi_depth3_RBX_energy_20->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       }
                     }  // 20 GeV cut

                     if(cmjRbxEnergy > 100.0) {   // 100 GeV cut
                     h_rechit_eta_RBX_energy_100 ->Fill((hhit->id()).ieta(),cmjRbxEnergy);
                     h_rechit_phi_RBX_energy_100 ->Fill((hhit->id()).iphi(),cmjRbxEnergy);
                     h_rechit_eta_phi_RBX_energy_100->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       if((hhit->id()).depth() == 1){
                       h_rechit_eta_phi_depth1_RBX_energy_100->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       }
                       if((hhit->id()).depth() == 2){
                       h_rechit_eta_phi_depth2_RBX_energy_100->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       }
                       if((hhit->id()).depth() == 3){
                       h_rechit_eta_phi_depth3_RBX_energy_100->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       }
                     }  // 100 GeV cut

                     if(cmjRbxEnergy > 500.0) {   // 500 GeV cut
                     h_rechit_eta_RBX_energy_500 ->Fill((hhit->id()).ieta(),cmjRbxEnergy);
                     h_rechit_phi_RBX_energy_500 ->Fill((hhit->id()).iphi(),cmjRbxEnergy);
                     h_rechit_eta_phi_RBX_energy_500->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       if((hhit->id()).depth() == 1){
                       h_rechit_eta_phi_depth1_RBX_energy_500->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       }
                       if((hhit->id()).depth() == 2){
                       h_rechit_eta_phi_depth2_RBX_energy_500->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       }
                       if((hhit->id()).depth() == 3){
                       h_rechit_eta_phi_depth3_RBX_energy_500->Fill((hhit->id()).ieta(),(hhit->id()).iphi(),cmjRbxEnergy);
                       }
                     }  // 500 GeV cut

//cmj2012Aug8
                 }
                }
//cmj2012Aug18 ----------- Add the hcal rechit collection ----------------------
//cmj2012Aug18 -----------------------------------------------------------
	hRBXE2ts_->Fill(totale2ts);
	hRBXE10ts_->Fill(totale10ts);
	hRBXE2tsOverE10ts_->Fill(totale10ts ? totale2ts/totale10ts : -999);

	hRBXEnergy_RBXnoise_->Fill(rbxenergy);
	hRBXEnergyRate_RBXnoise_->Fill(rbxenergy);

	if(noise_category==2) {
	  hRBXEnergy_RBXnoisePedDrift1_->Fill(rbxenergy);
	  hRBXEnergyRate_RBXnoisePedDrift1_->Fill(rbxenergy);
	} else if(noise_category==3) {
	  hRBXEnergy_RBXnoisePedDrift2_->Fill(rbxenergy);
	  hRBXEnergyRate_RBXnoisePedDrift2_->Fill(rbxenergy);
	} else if(noise_category==4) {
	  hRBXEnergy_RBXnoiseFlash1_->Fill(rbxenergy);
	  hRBXEnergyRate_RBXnoiseFlash1_->Fill(rbxenergy);
	} else if(noise_category==5) {
	  hRBXEnergy_RBXnoiseFlash2_->Fill(rbxenergy);
	  hRBXEnergyRate_RBXnoiseFlash2_->Fill(rbxenergy);
	}

	hLuminBlockNumberRate_RBXNoise_10_->Fill(lumiBlock);
	hLuminBlockNumberRate_Noise_10_->Fill(lumiBlock);
	
	if(rbx.idnumber()<18){
	  hRBXNumberRate_RBXnoise_10_->Fill(rbx.idnumber()); 
	}
	else if(rbx.idnumber()>=18 && rbx.idnumber()< 36){
	  hRBXNumberRate_RBXnoise_10_->Fill(rbx.idnumber()+2);
	}
	else if(rbx.idnumber()>=36 && rbx.idnumber()<54){
	  hRBXNumberRate_RBXnoise_10_->Fill(rbx.idnumber()+4);
	}
	else if(rbx.idnumber()>=54 && rbx.idnumber()<= 72){
	  hRBXNumberRate_RBXnoise_10_->Fill(rbx.idnumber()+6);
	}

	if(noise_category==2) {
	  hLuminBlockNumberRate_RBXPedDrift1_10_->Fill(lumiBlock);
	  if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoisePedDrift1_10_->Fill(rbx.idnumber());
	  else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoisePedDrift1_10_->Fill(rbx.idnumber()+2);
	  else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoisePedDrift1_10_->Fill(rbx.idnumber()+4);
	  else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoisePedDrift1_10_->Fill(rbx.idnumber()+6);

	} else if(noise_category==3) {
	  hLuminBlockNumberRate_RBXPedDrift2_10_->Fill(lumiBlock);
	  if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoisePedDrift2_10_->Fill(rbx.idnumber());
	  else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoisePedDrift2_10_->Fill(rbx.idnumber()+2);
	  else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoisePedDrift2_10_->Fill(rbx.idnumber()+4);
	  else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoisePedDrift2_10_->Fill(rbx.idnumber()+6);

	} else if(noise_category==4) {
	  hLuminBlockNumberRate_RBXFlash1_10_->Fill(lumiBlock);
	  if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoiseFlash1_10_->Fill(rbx.idnumber());
	  else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoiseFlash1_10_->Fill(rbx.idnumber()+2);
	  else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoiseFlash1_10_->Fill(rbx.idnumber()+4);
	  else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoiseFlash1_10_->Fill(rbx.idnumber()+6);

	} else if(noise_category==5) {
	  hLuminBlockNumberRate_RBXFlash2_10_->Fill(lumiBlock);
	  if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoiseFlash2_10_->Fill(rbx.idnumber());
	  else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoiseFlash2_10_->Fill(rbx.idnumber()+2);
	  else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoiseFlash2_10_->Fill(rbx.idnumber()+4);
	  else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoiseFlash2_10_->Fill(rbx.idnumber()+6);
	}

        if(rbxenergy > 20){
	  hLuminBlockNumberRate_RBXNoise_20_->Fill(lumiBlock);
	  hLuminBlockNumberRate_Noise_20_->Fill(lumiBlock);
	  
	  if(rbx.idnumber()<18){
	    hRBXNumberRate_RBXnoise_20_->Fill(rbx.idnumber()); 
	  }
	  else if(rbx.idnumber()>=18 && rbx.idnumber()< 36){
	    hRBXNumberRate_RBXnoise_20_->Fill(rbx.idnumber()+2);
	  }
	  else if(rbx.idnumber()>=36 && rbx.idnumber()<54){
	    hRBXNumberRate_RBXnoise_20_->Fill(rbx.idnumber()+4);
	  }
	  else if(rbx.idnumber()>=54 && rbx.idnumber()<= 72){
	    hRBXNumberRate_RBXnoise_20_->Fill(rbx.idnumber()+6);
	  }

	  if(noise_category==2) {
	    hLuminBlockNumberRate_RBXPedDrift1_20_->Fill(lumiBlock);
	    if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoisePedDrift1_20_->Fill(rbx.idnumber());
	    else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoisePedDrift1_20_->Fill(rbx.idnumber()+2);
	    else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoisePedDrift1_20_->Fill(rbx.idnumber()+4);
	    else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoisePedDrift1_20_->Fill(rbx.idnumber()+6);
	    
	  } else if(noise_category==3) {
	    hLuminBlockNumberRate_RBXPedDrift2_20_->Fill(lumiBlock);
	    if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoisePedDrift2_20_->Fill(rbx.idnumber());
	    else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoisePedDrift2_20_->Fill(rbx.idnumber()+2);
	    else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoisePedDrift2_20_->Fill(rbx.idnumber()+4);
	    else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoisePedDrift2_20_->Fill(rbx.idnumber()+6);
	    
	  } else if(noise_category==4) {
	    hLuminBlockNumberRate_RBXFlash1_20_->Fill(lumiBlock);
	    if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoiseFlash1_20_->Fill(rbx.idnumber());
	    else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoiseFlash1_20_->Fill(rbx.idnumber()+2);
	    else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoiseFlash1_20_->Fill(rbx.idnumber()+4);
	    else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoiseFlash1_20_->Fill(rbx.idnumber()+6);
	    
	  } else if(noise_category==5) {
	    hLuminBlockNumberRate_RBXFlash2_20_->Fill(lumiBlock);
	    if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoiseFlash2_20_->Fill(rbx.idnumber());
	    else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoiseFlash2_20_->Fill(rbx.idnumber()+2);
	    else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoiseFlash2_20_->Fill(rbx.idnumber()+4);
	    else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoiseFlash2_20_->Fill(rbx.idnumber()+6);
	  }
	}

        if(rbxenergy > 50){
	  if(rbx.idnumber()<18){
	    hRBXNumberRate_RBXnoise_50_->Fill(rbx.idnumber()); 
	  }
	  else if(rbx.idnumber()>=18 && rbx.idnumber()< 36){
	    hRBXNumberRate_RBXnoise_50_->Fill(rbx.idnumber()+2);
	  }
	  else if(rbx.idnumber()>=36 && rbx.idnumber()<54){
	    hRBXNumberRate_RBXnoise_50_->Fill(rbx.idnumber()+4);
	  }
	  else if(rbx.idnumber()>=54 && rbx.idnumber()<= 72){
	    hRBXNumberRate_RBXnoise_50_->Fill(rbx.idnumber()+6);
	  }
	  hLuminBlockNumberRate_RBXNoise_50_->Fill(lumiBlock);
	  hLuminBlockNumberRate_Noise_50_->Fill(lumiBlock);

	  if(noise_category==2) {
	    hLuminBlockNumberRate_RBXPedDrift1_50_->Fill(lumiBlock);
	    if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoisePedDrift1_50_->Fill(rbx.idnumber());
	    else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoisePedDrift1_50_->Fill(rbx.idnumber()+2);
	    else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoisePedDrift1_50_->Fill(rbx.idnumber()+4);
	    else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoisePedDrift1_50_->Fill(rbx.idnumber()+6);
	    
	  } else if(noise_category==3) {
	    hLuminBlockNumberRate_RBXPedDrift2_50_->Fill(lumiBlock);
	    if(rbx.idnumber()<=18)                          hRBXNumberRate_RBXnoisePedDrift2_50_->Fill(rbx.idnumber());
	    else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoisePedDrift2_50_->Fill(rbx.idnumber()+2);
	    else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoisePedDrift2_50_->Fill(rbx.idnumber()+4);
	    else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoisePedDrift2_50_->Fill(rbx.idnumber()+6);
	    
	  } else if(noise_category==4) {
	    hLuminBlockNumberRate_RBXFlash1_50_->Fill(lumiBlock);
	    if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoiseFlash1_50_->Fill(rbx.idnumber());
	    else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoiseFlash1_50_->Fill(rbx.idnumber()+2);
	    else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoiseFlash1_50_->Fill(rbx.idnumber()+4);
	    else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoiseFlash1_50_->Fill(rbx.idnumber()+6);
	    
	  } else if(noise_category==5) {
	    hLuminBlockNumberRate_RBXFlash2_50_->Fill(lumiBlock);
	    if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoiseFlash2_50_->Fill(rbx.idnumber());
	    else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoiseFlash2_50_->Fill(rbx.idnumber()+2);
	    else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoiseFlash2_50_->Fill(rbx.idnumber()+4);
	    else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoiseFlash2_50_->Fill(rbx.idnumber()+6);
	  }

 
          if(rbxenergy > 100){
	    if(rbx.idnumber()<18){
	      hRBXNumberRate_RBXnoise_100_->Fill(rbx.idnumber()); 
	    }
	    else if(rbx.idnumber()>=18 && rbx.idnumber()< 36){
	      hRBXNumberRate_RBXnoise_100_->Fill(rbx.idnumber()+2);
	    }
	    else if(rbx.idnumber()>=36 && rbx.idnumber()<54){
	      hRBXNumberRate_RBXnoise_100_->Fill(rbx.idnumber()+4);
	    }
	    else if(rbx.idnumber()>=54 && rbx.idnumber()<= 72){
	      hRBXNumberRate_RBXnoise_100_->Fill(rbx.idnumber()+6);
	    }
            hLuminBlockNumberRate_RBXNoise_100_->Fill(lumiBlock);
	    hLuminBlockNumberRate_Noise_100_->Fill(lumiBlock);
	    
            if(rbxenergy > 500){
	      if(rbx.idnumber()<18){
		hRBXNumberRate_RBXnoise_500_->Fill(rbx.idnumber()); 
	      }
	      else if(rbx.idnumber()>=18 && rbx.idnumber()< 36){
	        hRBXNumberRate_RBXnoise_500_->Fill(rbx.idnumber()+2);
	      }
	      else if(rbx.idnumber()>=36 && rbx.idnumber()<54){
		hRBXNumberRate_RBXnoise_500_->Fill(rbx.idnumber()+4);
	      }
	      else if(rbx.idnumber()>=54 && rbx.idnumber()<= 72){
		hRBXNumberRate_RBXnoise_500_->Fill(rbx.idnumber()+6);
	      }
	      hLuminBlockNumberRate_RBXNoise_500_->Fill(lumiBlock);
	      hLuminBlockNumberRate_Noise_500_->Fill(lumiBlock);

	      if(noise_category==2) {
		hLuminBlockNumberRate_RBXPedDrift1_500_->Fill(lumiBlock);
		if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoisePedDrift1_500_->Fill(rbx.idnumber());
		else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoisePedDrift1_500_->Fill(rbx.idnumber()+2);
		else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoisePedDrift1_500_->Fill(rbx.idnumber()+4);
		else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoisePedDrift1_500_->Fill(rbx.idnumber()+6);
		
	      } else if(noise_category==3) {
		hLuminBlockNumberRate_RBXPedDrift2_500_->Fill(lumiBlock);
		if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoisePedDrift2_500_->Fill(rbx.idnumber());
		else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoisePedDrift2_500_->Fill(rbx.idnumber()+2);
		else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoisePedDrift2_500_->Fill(rbx.idnumber()+4);
		else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoisePedDrift2_500_->Fill(rbx.idnumber()+6);
		
	      } else if(noise_category==4) {
		hLuminBlockNumberRate_RBXFlash1_500_->Fill(lumiBlock);
		if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoiseFlash1_500_->Fill(rbx.idnumber());
		else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoiseFlash1_500_->Fill(rbx.idnumber()+2);
		else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoiseFlash1_500_->Fill(rbx.idnumber()+4);
		else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoiseFlash1_500_->Fill(rbx.idnumber()+6);
		
	      } else if(noise_category==5) {
		hLuminBlockNumberRate_RBXFlash2_500_->Fill(lumiBlock);
		if(rbx.idnumber()<18)                          hRBXNumberRate_RBXnoiseFlash2_500_->Fill(rbx.idnumber());
		else if(rbx.idnumber()>=18 && rbx.idnumber()<36) hRBXNumberRate_RBXnoiseFlash2_500_->Fill(rbx.idnumber()+2);
		else if(rbx.idnumber()>=36 && rbx.idnumber()<54) hRBXNumberRate_RBXnoiseFlash2_500_->Fill(rbx.idnumber()+4);
		else if(rbx.idnumber()>=54 && rbx.idnumber()<=72) hRBXNumberRate_RBXnoiseFlash2_500_->Fill(rbx.idnumber()+6);
	      }

	    }
	  }
	}
	
      } else if ( isHPDnoise ) {
	
	int hpdnumber = hpdNumberHighest;
	if (hpdSubdetHighest==1 && hpdZsideHighest==-1){hpdnumber -= 72;}
 	if (hpdSubdetHighest==2 && hpdZsideHighest== 1){hpdnumber -= 144;}
	if (hpdSubdetHighest==2 && hpdZsideHighest==-1){hpdnumber -= 216;}

	if ( isHPDionfeed ) {  // HPD noise ion feedback

	if(rbxenergy > 10){
	    hRBXEnergy_HPDionfeed_->Fill(rbxenergy);
	    hRBXEnergyRate_HPDionfeed_->Fill(rbxenergy);
	}

	  if(rbxenergy > 20){
	    hLuminBlockNumberRate_Noise_20_->Fill(lumiBlock);
	    hLuminBlockNumberRate_HPDNoise_20_->Fill(lumiBlock);
	    hLuminBlockNumberRate_HPDionfeed_20_->Fill(lumiBlock);
	    
	    if (hpdSubdetHighest==1 && hpdZsideHighest== 1){
	      hHPDNumberRate_HBpos_HPDnoise_20_->Fill(hpdnumber);
	      hHPDNumberRate_HBpos_HPDionfeed_20_->Fill(hpdnumber);
	      
	    } else if (hpdSubdetHighest==1 && hpdZsideHighest==-1){
	      hHPDNumberRate_HBneg_HPDnoise_20_->Fill(hpdnumber);
	      hHPDNumberRate_HBneg_HPDionfeed_20_->Fill(hpdnumber);
	      
	    } else if (hpdSubdetHighest==2 && hpdZsideHighest== 1){
	      hHPDNumberRate_HEpos_HPDnoise_20_->Fill(hpdnumber);
	      hHPDNumberRate_HEpos_HPDionfeed_20_->Fill(hpdnumber);
	      
	    } else if (hpdSubdetHighest==2 && hpdZsideHighest==-1){
	      hHPDNumberRate_HEneg_HPDnoise_20_->Fill(hpdnumber);
	      hHPDNumberRate_HEneg_HPDionfeed_20_->Fill(hpdnumber);
	      
	    }
	  }
	  if(rbxenergy > 50){
	    hLuminBlockNumberRate_Noise_50_->Fill(lumiBlock);
	    hLuminBlockNumberRate_HPDNoise_50_->Fill(lumiBlock);
	    hLuminBlockNumberRate_HPDionfeed_50_->Fill(lumiBlock);
	    if (hpdSubdetHighest==1 && hpdZsideHighest== 1){
	      hHPDNumberRate_HBpos_HPDnoise_50_->Fill(hpdnumber);
	      hHPDNumberRate_HBpos_HPDionfeed_50_->Fill(hpdnumber);
	      
	    } else if (hpdSubdetHighest==1 && hpdZsideHighest==-1){
	      hHPDNumberRate_HBneg_HPDnoise_50_->Fill(hpdnumber);
	      hHPDNumberRate_HBneg_HPDionfeed_50_->Fill(hpdnumber);
	      
	    } else if (hpdSubdetHighest==2 && hpdZsideHighest== 1){
	      hHPDNumberRate_HEpos_HPDnoise_50_->Fill(hpdnumber);
	      hHPDNumberRate_HEpos_HPDionfeed_50_->Fill(hpdnumber);

	    } else if (hpdSubdetHighest==2 && hpdZsideHighest==-1){
	      hHPDNumberRate_HEneg_HPDnoise_50_->Fill(hpdnumber);
	      hHPDNumberRate_HEneg_HPDionfeed_50_->Fill(hpdnumber);
	      
	    }
	    if(rbxenergy > 100){
	      hLuminBlockNumberRate_Noise_100_->Fill(lumiBlock);
	      hLuminBlockNumberRate_HPDNoise_100_->Fill(lumiBlock);
	      hLuminBlockNumberRate_HPDionfeed_100_->Fill(lumiBlock);
	      if (hpdSubdetHighest==1 && hpdZsideHighest== 1){
		hHPDNumberRate_HBpos_HPDnoise_100_->Fill(hpdnumber);
		hHPDNumberRate_HBpos_HPDionfeed_100_->Fill(hpdnumber);
		
	      } else if (hpdSubdetHighest==1 && hpdZsideHighest==-1){
		hHPDNumberRate_HBneg_HPDnoise_100_->Fill(hpdnumber);
		hHPDNumberRate_HBneg_HPDionfeed_100_->Fill(hpdnumber);
		
	      } else if (hpdSubdetHighest==2 && hpdZsideHighest== 1){
		hHPDNumberRate_HEpos_HPDnoise_100_->Fill(hpdnumber);
		hHPDNumberRate_HEpos_HPDionfeed_100_->Fill(hpdnumber);

	      } else if (hpdSubdetHighest==2 && hpdZsideHighest==-1){
		hHPDNumberRate_HEneg_HPDnoise_100_->Fill(hpdnumber);
		hHPDNumberRate_HEneg_HPDionfeed_100_->Fill(hpdnumber);
		
	      }
	      if(rbxenergy > 500){
		hLuminBlockNumberRate_Noise_500_->Fill(lumiBlock);
		hLuminBlockNumberRate_HPDNoise_500_->Fill(lumiBlock);
		hLuminBlockNumberRate_HPDionfeed_500_->Fill(lumiBlock);
		if (hpdSubdetHighest==1 && hpdZsideHighest== 1){
		  hHPDNumberRate_HBpos_HPDnoise_500_->Fill(hpdnumber);
		  hHPDNumberRate_HBpos_HPDionfeed_500_->Fill(hpdnumber);
		  
		} else if (hpdSubdetHighest==1 && hpdZsideHighest==-1){
		  hHPDNumberRate_HBneg_HPDnoise_500_->Fill(hpdnumber);
		  hHPDNumberRate_HBneg_HPDionfeed_500_->Fill(hpdnumber);
		  
		} else if (hpdSubdetHighest==2 && hpdZsideHighest== 1){
		  hHPDNumberRate_HEpos_HPDnoise_500_->Fill(hpdnumber);
		  hHPDNumberRate_HEpos_HPDionfeed_500_->Fill(hpdnumber);

		} else if (hpdSubdetHighest==2 && hpdZsideHighest==-1){
		  hHPDNumberRate_HEneg_HPDnoise_500_->Fill(hpdnumber);
		  hHPDNumberRate_HEneg_HPDionfeed_500_->Fill(hpdnumber);
		  
		}
		
	      }// if rbxenergy > 500
	    }// if rbxenergy > 100
	  }// if rbxenergy > 50

	} else if ( isHPDdischg ) {  // HPD noise discharge

	  if(rbxenergy > 10){
	    hRBXEnergy_HPDdischg_->Fill(rbxenergy);
	    hRBXEnergyRate_HPDdischg_->Fill(rbxenergy);
	}
	  if(rbxenergy > 20){

	    hLuminBlockNumberRate_Noise_20_->Fill(lumiBlock);
	    hLuminBlockNumberRate_HPDNoise_20_->Fill(lumiBlock);
	    hLuminBlockNumberRate_HPDdischg_20_->Fill(lumiBlock);
	    
	    if (hpdSubdetHighest==1 && hpdZsideHighest== 1){
	      hHPDNumberRate_HBpos_HPDnoise_20_->Fill(hpdnumber);
	      hHPDNumberRate_HBpos_HPDdischg_20_->Fill(hpdnumber);
	      
	    } else if (hpdSubdetHighest==1 && hpdZsideHighest==-1){
	      hHPDNumberRate_HBneg_HPDnoise_20_->Fill(hpdnumber);
	      hHPDNumberRate_HBneg_HPDdischg_20_->Fill(hpdnumber);
	      
	    } else if (hpdSubdetHighest==2 && hpdZsideHighest== 1){
	      hHPDNumberRate_HEpos_HPDnoise_20_->Fill(hpdnumber);
	      hHPDNumberRate_HEpos_HPDdischg_20_->Fill(hpdnumber);
	      
	    } else if (hpdSubdetHighest==2 && hpdZsideHighest==-1){
	      hHPDNumberRate_HEneg_HPDnoise_20_->Fill(hpdnumber);
	      hHPDNumberRate_HEneg_HPDdischg_20_->Fill(hpdnumber);
	      
	    }
	  }
	  if(rbxenergy > 50){
	    hLuminBlockNumberRate_Noise_50_->Fill(lumiBlock);
	    hLuminBlockNumberRate_HPDNoise_50_->Fill(lumiBlock);
	    hLuminBlockNumberRate_HPDdischg_50_->Fill(lumiBlock);
	    if (hpdSubdetHighest==1 && hpdZsideHighest== 1){
	      hHPDNumberRate_HBpos_HPDnoise_50_->Fill(hpdnumber);
	      hHPDNumberRate_HBpos_HPDdischg_50_->Fill(hpdnumber);
	      
	    } else if (hpdSubdetHighest==1 && hpdZsideHighest==-1){
	      hHPDNumberRate_HBneg_HPDnoise_50_->Fill(hpdnumber);
	      hHPDNumberRate_HBneg_HPDdischg_50_->Fill(hpdnumber);
	      
	    } else if (hpdSubdetHighest==2 && hpdZsideHighest== 1){
	      hHPDNumberRate_HEpos_HPDnoise_50_->Fill(hpdnumber);
	      hHPDNumberRate_HEpos_HPDdischg_50_->Fill(hpdnumber);

	    } else if (hpdSubdetHighest==2 && hpdZsideHighest==-1){
	      hHPDNumberRate_HEneg_HPDnoise_50_->Fill(hpdnumber);
	      hHPDNumberRate_HEneg_HPDdischg_50_->Fill(hpdnumber);
	      
	    }
	    if(rbxenergy > 100){
	      hLuminBlockNumberRate_Noise_100_->Fill(lumiBlock);
	      hLuminBlockNumberRate_HPDNoise_100_->Fill(lumiBlock);
	      hLuminBlockNumberRate_HPDdischg_100_->Fill(lumiBlock);
	      if (hpdSubdetHighest==1 && hpdZsideHighest== 1){
		hHPDNumberRate_HBpos_HPDnoise_100_->Fill(hpdnumber);
		hHPDNumberRate_HBpos_HPDdischg_100_->Fill(hpdnumber);
		
	      } else if (hpdSubdetHighest==1 && hpdZsideHighest==-1){
		hHPDNumberRate_HBneg_HPDnoise_100_->Fill(hpdnumber);
		hHPDNumberRate_HBneg_HPDdischg_100_->Fill(hpdnumber);
		
	      } else if (hpdSubdetHighest==2 && hpdZsideHighest== 1){
		hHPDNumberRate_HEpos_HPDnoise_100_->Fill(hpdnumber);
		hHPDNumberRate_HEpos_HPDdischg_100_->Fill(hpdnumber);

	      } else if (hpdSubdetHighest==2 && hpdZsideHighest==-1){
		hHPDNumberRate_HEneg_HPDnoise_100_->Fill(hpdnumber);
		hHPDNumberRate_HEneg_HPDdischg_100_->Fill(hpdnumber);
		
	      }
	      if(rbxenergy > 500){
		hLuminBlockNumberRate_Noise_500_->Fill(lumiBlock);
		hLuminBlockNumberRate_HPDNoise_500_->Fill(lumiBlock);
		hLuminBlockNumberRate_HPDdischg_500_->Fill(lumiBlock);
		if (hpdSubdetHighest==1 && hpdZsideHighest== 1){
		  hHPDNumberRate_HBpos_HPDnoise_500_->Fill(hpdnumber);
		  hHPDNumberRate_HBpos_HPDdischg_500_->Fill(hpdnumber);
		  
		} else if (hpdSubdetHighest==1 && hpdZsideHighest==-1){
		  hHPDNumberRate_HBneg_HPDnoise_500_->Fill(hpdnumber);
		  hHPDNumberRate_HBneg_HPDdischg_500_->Fill(hpdnumber);
		  
		} else if (hpdSubdetHighest==2 && hpdZsideHighest== 1){
		  hHPDNumberRate_HEpos_HPDnoise_500_->Fill(hpdnumber);
		  hHPDNumberRate_HEpos_HPDdischg_500_->Fill(hpdnumber);

		} else if (hpdSubdetHighest==2 && hpdZsideHighest==-1){
		  hHPDNumberRate_HEneg_HPDnoise_500_->Fill(hpdnumber);
		  hHPDNumberRate_HEneg_HPDdischg_500_->Fill(hpdnumber);
		  
		}
		
	      }// if rbxenergy > 500
	    }// if rbxenergy > 100
	  }// if rbxenergy > 50
	  
	} // noise_category=2(ion feedback) or 3(discharge)

      }   // noise_category=1(RBX noise) or (HPD noise)

      // Asli && Warren

      hNRBXHits_->Fill(numRBXhits);
      hAfterRBXEnergy_->Fill(rbxenergy);
      hAfterRBXEnergyRate_->Fill(rbxenergy);
    } // if (numRBXhits>0 && rbxenergy>20.)

    hFailures_->Fill(failures);
    //    if(failures==0) {
    //      hAfterRBXEnergy_->Fill(rbxenergy);
    //      hAfterRBXEnergyRate_->Fill(rbxenergy);
    //    }


// cmj2011Jul15
    if(skDumpFlag==1 && skDumpCounter<10) {
      cout<<" calling PrintInfo..."<<endl;
      skDumpCounter++ ;
      cout<<endl;
      cout<<"sk-dump "<<skDumpCounter<<endl;
 //     ePrint->analyze(iEvent, iSetup);
    }
// cmj2011Jul15
  
  }  // Loop over RBX boxes ends
 
  hNRBXobjectsE20GeV_->Fill(nrbx20gev);
  


  return;
}


// ------------ method called once each job just before starting event loop  ------------
void 
HcalNoiseInfoAnalyzer::beginJob()
{
  // book histograms
//   rootfile_ = new TFile(rootHistFilename_.c_str(), "RECREATE");
//cmj2011Jun30
//	Change the Energy threshold bins
//	Original Values:
//	Int_t noiseBins = 100;
//	Float_t LowBin = 0.0;
//	Float_t HighBin = 200.0;
//
//cmj2011Jul7	Int_t noiseBins = 225;
//cmj2011Jul7	Float_t LowBin = 50.0;
//cmj2011Jul7	Float_t HighBin = 500.0;
//	New Bins
	Int_t noiseBins = 475;
	Float_t LowBin = 50.0;
	Float_t HighBin = 1000.0;
//cmj2011Jun30
  
  TFileDirectory NoiseDir = fs->mkdir("Noise" );
 // ePrint->beginJob(iSetup);



   hRunStartHolder_ =NoiseDir.make<TH1I>("hRunStartHolder", "Holder for start of run date and time", 10, 0, 10);
   hRunStopHolder_  =NoiseDir.make<TH1I>("hRunStopHolder", "Holder for end of run date and time", 10, 0, 10);


  hLumiBlock_ = NoiseDir.make<TH1D>("hLumiBlock","Luminosity Block",3000, 0, 3000);

  hMaxZeros_ = NoiseDir.make<TH1D>("hMaxZeros","Max # of zeros in an RBX",15,-0.5,14.5);
  hTotalZeros_ = NoiseDir.make<TH1D>("hTotalZeros","total # of zeros in an RBX",15,-0.5,14.5);
  hE2ts_ = NoiseDir.make<TH1D>("hE2ts","E(2ts) for the highest energy digi in an HPD",50,0,200);
  hE10ts_ = NoiseDir.make<TH1D>("hE10ts","E(10ts) for the highest energy digi in an HPD",50,0,200);
  hE2tsOverE10ts_ = NoiseDir.make<TH1D>("hE2tsOverE10ts","E(t2s)/E(10ts) for the highest energy digi in an HPD",100,0.0,2.0);
  h5E2ts_ = NoiseDir.make<TH1D>("h5E2ts","E(2ts) for the highest 5 energy digi in an HPD",50,0,200);
  h5E10ts_ = NoiseDir.make<TH1D>("h5E10ts","E(10ts) for the highest 5 energy digi in an HPD",50,0,200);
  h5E2tsOverE10ts_ = NoiseDir.make<TH1D>("h5E2tsOver5E10ts","E(t2s)/E(10ts) for the highest 5 energy digi in an HPD",100,0.0,2.0);
  hRBXE2ts_ = NoiseDir.make<TH1D>("hRBXE2ts","Sum RBX E(2ts)",50,0,200);
  hRBXE10ts_ = NoiseDir.make<TH1D>("hRBXE10ts","Sum RBX E(10ts)",50,0,200);
  hRBXE2tsOverE10ts_ = NoiseDir.make<TH1D>("hRBXE2tsOverE10ts","Sum RBX E(t2s)/E(10ts)",100,0.0,2.0);
  hHPDNHits_ = NoiseDir.make<TH1D>("hHPDNHits","Number of Hits with E>1.5 GeV in an HPD",19,-0.5,18.5);
  hHPDNHits2_ = NoiseDir.make<TH1D>("hHPDNHits2","Number of Hits with E>1.5 GeV in an HPD",73,-0.5,72.5);
  
  hFailures_ = NoiseDir.make<TH1D>("hFailures","code designating which cut the event failed (if any)",32,-0.5,31.5);
  hBeforeRBXEnergy_ = NoiseDir.make<TH1D>("hBeforeRBXEnergy","Total RecHit Energy in RBX before cuts",50,0,200);
  hAfterRBXEnergy_ = NoiseDir.make<TH1D>("hAfterRBXEnergy","Total RecHit Energy in RBX after cuts",50,0,200);

//cmj2011Jun30  hBeforeRBXEnergyRate_ = NoiseDir.make<TH1D>("hBeforeRBXEnergyRate","Total RecHit Energy in RBX before cuts (Rate)",50,0,200);
//cmj2011Jun30  hAfterRBXEnergyRate_ = NoiseDir.make<TH1D>("hAfterRBXEnergyRate","Total RecHit Energy in RBX after cuts (Rate)",50,0,200);
  hBeforeRBXEnergyRate_ = NoiseDir.make<TH1D>("hBeforeRBXEnergyRate","Total RecHit Energy in RBX before cuts (Rate)",50,LowBin,HighBin);
  hAfterRBXEnergyRate_ = NoiseDir.make<TH1D>("hAfterRBXEnergyRate","Total RecHit Energy in RBX after cuts (Rate)",50,LowBin,HighBin);

  hNRBXHits_=NoiseDir.make<TH1D>("NumRBXHits", "Number of Hits in RBX", 74, -0.5, 73.5);
  hNRBXobjects_= NoiseDir.make<TH1D>("hNRBXobjects","Total number of RBX objects",51,-0.5,50.5);

  hNRBXobjectsE20GeV_= NoiseDir.make<TH1D>("hNRBXobjectsE20GeV","Total number of RBX objects with E>20 GeV",51,-0.5,50.5);
  hHCalNoiseCategory_=NoiseDir.make<TH1D>("hHCalNoiseCategory","Hcal noise category",10,1.,10.);
  hHCalNoiseCategory_->GetXaxis()->SetBinLabel(1, "RBXNoise");
  hHCalNoiseCategory_->GetXaxis()->SetBinLabel(2, "RBXPedDrift-1");
  hHCalNoiseCategory_->GetXaxis()->SetBinLabel(3, "RBXPedDrift-2");
  hHCalNoiseCategory_->GetXaxis()->SetBinLabel(4, "RBXFlash-1");
  hHCalNoiseCategory_->GetXaxis()->SetBinLabel(5, "RBXFlash-2");
  hHCalNoiseCategory_->GetXaxis()->SetBinLabel(7, "HPDDischarge");
  hHCalNoiseCategory_->GetXaxis()->SetBinLabel(8, "HPDIonFeedBack");

//cmj2011Jun30  hRBXEnergy_RBXnoise_=NoiseDir.make<TH1D>("hRBXEnergy_RBXnoise","Total RBX RecHit Energy for RBX noise",100,0,200);
//cmj2011Jun30  hRBXEnergyRate_RBXnoise_=NoiseDir.make<TH1D>("hRBXEnergyRate_RBXnoise","Total RBX RecHit Energy for RBX noise (Rate)",100,0,200);
//cmj2011Jun30
  hRBXEnergy_RBXnoise_=NoiseDir.make<TH1D>("hRBXEnergy_RBXnoise","Total RBX RecHit Energy for RBX noise",noiseBins,LowBin,HighBin);
  hRBXEnergyRate_RBXnoise_=NoiseDir.make<TH1D>("hRBXEnergyRate_RBXnoise","Total RBX RecHit Energy for RBX noise (Rate)",noiseBins,LowBin,HighBin);
//cmj2011Jun30

  hRBXEnergy_RBXnoisePedDrift1_=NoiseDir.make<TH1D>("hRBXEnergy_RBXnoisePedDrift1","RBXEnergy:e2ts/e10ts<0.33",50,0,200);
  hRBXEnergyRate_RBXnoisePedDrift1_=NoiseDir.make<TH1D>("hRBXEnergyRate_RBXnoisePedDrift1","RBXEnergy(Rate):e2ts/e10ts<0.33",50,0,200);
  hRBXEnergy_RBXnoisePedDrift2_=NoiseDir.make<TH1D>("hRBXEnergy_RBXnoisePedDrift2","RBXEnergy:0.33e2ts/e10ts<0.8",50,0,200);
  hRBXEnergyRate_RBXnoisePedDrift2_=NoiseDir.make<TH1D>("hRBXEnergyRate_RBXnoisePedDrift2","RBXEnergy(Rate):0.33<2ts/e10ts<0.8",50,0,200);
  hRBXEnergy_RBXnoiseFlash1_=NoiseDir.make<TH1D>("hRBXEnergy_RBXnoiseFlash1","RBXEnergy:e2ts/e10ts>0.8 & NHits>10",50,0,200);
  hRBXEnergyRate_RBXnoiseFlash1_=NoiseDir.make<TH1D>("hRBXEnergyRate_RBXnoiseFlash1","RBXEnergy(Rate):e2ts/e10ts>0.8 & NHits>10",50,0,200);
  hRBXEnergy_RBXnoiseFlash2_=NoiseDir.make<TH1D>("hRBXEnergy_RBXnoiseFlash2","RBXEnergy:e2ts/e10ts>0.8 & NHits<10",50,0,200);
  hRBXEnergyRate_RBXnoiseFlash2_=NoiseDir.make<TH1D>("hRBXEnergyRate_RBXnoiseFlash2","RBXEnergy(Rate):e2ts/e10ts>0.8 & NHits<10",50,0,200);

//cmj2011Jun30  hRBXEnergy_HPDionfeed_=NoiseDir.make<TH1D>("hRBXEnergy_HPDionfeed","Total RBX RecHit Energy for HPD ion feedback",100,0,200);
//cmj2011Jun30  hRBXEnergyRate_HPDionfeed_=NoiseDir.make<TH1D>("hRBXEnergyRate_HPDionfeed","Total RBX RecHit Energy for HPD ion feedback (Rate)",100,0,200);
//cmj2011Jun30  hRBXEnergy_HPDdischg_=NoiseDir.make<TH1D>("hRBXEnergy_HPDdischg","Total RBX RecHit Energy for HPD discharge",100,0,200);
//cmj2011Jun30  hRBXEnergyRate_HPDdischg_=NoiseDir.make<TH1D>("hRBXEnergyRate_HPDdischg","Total RBX RecHit Energy for HPD discharge (Rate)",100,0,200);
  hRBXEnergy_HPDionfeed_=NoiseDir.make<TH1D>("hRBXEnergy_HPDionfeed","Total RBX RecHit Energy for HPD ion feedback",noiseBins,LowBin,HighBin);
  hRBXEnergyRate_HPDionfeed_=NoiseDir.make<TH1D>("hRBXEnergyRate_HPDionfeed","Total RBX RecHit Energy for HPD ion feedback (Rate)",noiseBins,LowBin,HighBin);
  hRBXEnergy_HPDdischg_=NoiseDir.make<TH1D>("hRBXEnergy_HPDdischg","Total RBX RecHit Energy for HPD discharge",noiseBins,LowBin,HighBin);
  hRBXEnergyRate_HPDdischg_=NoiseDir.make<TH1D>("hRBXEnergyRate_HPDdischg","Total RBX RecHit Energy for HPD discharge (Rate)",noiseBins,LowBin,HighBin);
//cmj2011Jun30

  hRBXNumberRate_RBXnoise_10_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoise_10","RBX number(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate > 10 GeV)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoise_20_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoise_20","RBX number(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate > 20 GeV)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoise_50_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoise_50","RBX number(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate > 50 GeV)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoise_100_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoise_100","RBX number(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate > 100 GeV)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoise_500_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoise_500","RBX number(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate > 500 GeV)",80,-0.5,79.5);

  hRBXNumberRate_RBXnoisePedDrift1_10_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoisePedDrift1_10",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>10GeV,e2ts/e10ts<0.33)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoisePedDrift1_20_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoisePedDrift1_20",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>20GeV,e2ts/e10ts<0.33)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoisePedDrift1_50_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoisePedDrift1_50",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>50GeV,e2ts/e10ts<0.33)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoisePedDrift1_100_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoisePedDrift1_100","RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>100GeV,e2ts/e10ts<0.33)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoisePedDrift1_500_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoisePedDrift1_500","RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>500GeV,e2ts/e10ts<0.33)",80,-0.5,79.5);

  hRBXNumberRate_RBXnoisePedDrift2_10_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoisePedDrift2_10",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>10GeV,0.33<e2ts/e10ts<0.8)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoisePedDrift2_20_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoisePedDrift2_20",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>20GeV,0.33<e2ts/e10ts<0.8)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoisePedDrift2_50_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoisePedDrift2_50",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>50GeV,0.33<e2ts/e10ts<0.8)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoisePedDrift2_100_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoisePedDrift2_100","RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>100GeV,0.33<e2ts/e10ts<0.8)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoisePedDrift2_500_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoisePedDrift2_500","RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>500GeV,0.33<e2ts/e10ts<0.8)",80,-0.5,79.5);

  hRBXNumberRate_RBXnoiseFlash1_10_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoiseFlash1_10",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>10GeV,e2ts/e10ts>0.8 && NHits>10)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoiseFlash1_20_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoiseFlash1_20",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>20GeV,e2ts/e10ts>0.8 && NHits>10)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoiseFlash1_50_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoiseFlash1_50",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>50GeV,e2ts/e10ts>0.8 && NHits>10)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoiseFlash1_100_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoiseFlash1_100","RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>100GeV,e2ts/e10ts>0.8 && NHits>10)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoiseFlash1_500_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoiseFlash1_500","RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>500GeV,e2ts/e10ts>0.8 && NHits>10)",80,-0.5,79.5);

  hRBXNumberRate_RBXnoiseFlash2_10_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoiseFlash2_10",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>10GeV,e2ts/e10ts>0.8 && NHits<10)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoiseFlash2_20_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoiseFlash2_20",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>20GeV,e2ts/e10ts>0.8 && NHits<10)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoiseFlash2_50_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoiseFlash2_50",  "RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>50GeV,e2ts/e10ts>0.8 && NHits<10)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoiseFlash2_100_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoiseFlash2_100","RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>100GeV,e2ts/e10ts>0.8 && NHits<10)",80,-0.5,79.5);
  hRBXNumberRate_RBXnoiseFlash2_500_=NoiseDir.make<TH1D>("hRBXNumberRate_RBXnoiseFlash2_500","RBXId(1-18:HB+,21-38:HB-,41-58:HE+,61-78:HE-)(Rate>500GeV,e2ts/e10ts>0.8 && NHits<10)",80,-0.5,79.5);

  hHPDNumberRate_HBpos_HPDnoise_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDnoise_20","Noise HPD number HB+ (Rate > 20 GeV)",73,-0.5,72.5);  
  hHPDNumberRate_HBneg_HPDnoise_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDnoise_20","Noise HPD number HB- (Rate > 20 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDnoise_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDnoise_20","Noise HPD number HE+ (Rate > 20 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDnoise_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDnoise_20","Noise HPD number HE- (Rate > 20 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBpos_HPDionfeed_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDionfeed_20","Noise (ion feedbck) HPD number HB+ (Rate > 20 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBneg_HPDionfeed_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDionfeed_20","Noise (ion feedbck) HPD number HB- (Rate > 20 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDionfeed_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDionfeed_20","Noise (ion feedbck) HPD number HE+ (Rate > 20 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDionfeed_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDionfeed_20","Noise (ion feedbck) HPD number HE- (Rate > 20 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBpos_HPDdischg_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDdischg_20","Noise (discharge) HPD number HB+ (Rate > 20 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBneg_HPDdischg_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDdischg_20","Noise (discharge) HPD number HB- (Rate > 20 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDdischg_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDdischg_20","Noise (discharge) HPD number HE+ (Rate > 20 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDdischg_20_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDdischg_20","Noise (discharge) HPD number HE- (Rate > 20 GeV)",73,-0.5,72.5);

  hHPDNumberRate_HBpos_HPDnoise_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDnoise_50","Noise HPD number HB+ (Rate > 50 GeV)",73,-0.5,72.5);  
  hHPDNumberRate_HBneg_HPDnoise_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDnoise_50","Noise HPD number HB- (Rate > 50 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDnoise_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDnoise_50","Noise HPD number HE+ (Rate > 50 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDnoise_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDnoise_50","Noise HPD number HE- (Rate > 50 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBpos_HPDionfeed_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDionfeed_50","Noise (ion feedbck) HPD number HB+ (Rate > 50 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBneg_HPDionfeed_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDionfeed_50","Noise (ion feedbck) HPD number HB- (Rate > 50 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDionfeed_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDionfeed_50","Noise (ion feedbck) HPD number HE+ (Rate > 50 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDionfeed_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDionfeed_50","Noise (ion feedbck) HPD number HE- (Rate > 50 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBpos_HPDdischg_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDdischg_50","Noise (discharge) HPD number HB+ (Rate > 50 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBneg_HPDdischg_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDdischg_50","Noise (discharge) HPD number HB- (Rate > 50 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDdischg_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDdischg_50","Noise (discharge) HPD number HE+ (Rate > 50 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDdischg_50_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDdischg_50","Noise (discharge) HPD number HE- (Rate > 50 GeV)",73,-0.5,72.5);

  hHPDNumberRate_HBpos_HPDnoise_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDnoise_100","Noise HPD number HB+ (Rate > 100 GeV)",73,-0.5,72.5);  
  hHPDNumberRate_HBneg_HPDnoise_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDnoise_100","Noise HPD number HB- (Rate > 100 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDnoise_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDnoise_100","Noise HPD number HE+ (Rate > 100 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDnoise_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDnoise_100","Noise HPD number HE- (Rate > 100 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBpos_HPDionfeed_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDionfeed_100","Noise (ion feedbck) HPD number HB+ (Rate > 100 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBneg_HPDionfeed_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDionfeed_100","Noise (ion feedbck) HPD number HB- (Rate > 100 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDionfeed_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDionfeed_100","Noise (ion feedbck) HPD number HE+ (Rate > 100 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDionfeed_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDionfeed_100","Noise (ion feedbck) HPD number HE- (Rate > 100 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBpos_HPDdischg_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDdischg_100","Noise (discharge) HPD number HB+ (Rate > 100 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBneg_HPDdischg_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDdischg_100","Noise (discharge) HPD number HB- (Rate > 100 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDdischg_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDdischg_100","Noise (discharge) HPD number HE+ (Rate > 100 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDdischg_100_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDdischg_100","Noise (discharge) HPD number HE- (Rate > 100 GeV)",73,-0.5,72.5);

  hHPDNumberRate_HBpos_HPDnoise_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDnoise_500","Noise HPD number HB+ (Rate > 500 GeV)",73,-0.5,72.5);  
  hHPDNumberRate_HBneg_HPDnoise_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDnoise_500","Noise HPD number HB- (Rate > 500 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDnoise_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDnoise_500","Noise HPD number HE+ (Rate > 500 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDnoise_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDnoise_500","Noise HPD number HE- (Rate > 500 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBpos_HPDionfeed_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDionfeed_500","Noise (ion feedbck) HPD number HB+ (Rate > 500 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBneg_HPDionfeed_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDionfeed_500","Noise (ion feedbck) HPD number HB- (Rate > 500 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDionfeed_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDionfeed_500","Noise (ion feedbck) HPD number HE+ (Rate > 500 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDionfeed_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDionfeed_500","Noise (ion feedbck) HPD number HE- (Rate > 500 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBpos_HPDdischg_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HBpos_HPDdischg_500","Noise (discharge) HPD number HB+ (Rate > 500 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HBneg_HPDdischg_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HBneg_HPDdischg_500","Noise (discharge) HPD number HB- (Rate > 500 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEpos_HPDdischg_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HEpos_HPDdischg_500","Noise (discharge) HPD number HE+ (Rate > 500 GeV)",73,-0.5,72.5);
  hHPDNumberRate_HEneg_HPDdischg_500_=NoiseDir.make<TH1D>("hHPDNumberRate_HEneg_HPDdischg_500","Noise (discharge) HPD number HE- (Rate > 500 GeV)",73,-0.5,72.5);

  hLuminBlockNumberRate_Noise_10_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_Noise_10", "Noise LuminBlock number (Rate > 10 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_Noise_20_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_Noise_20", "Noise LuminBlock number (Rate > 20 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_Noise_50_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_Noise_50", "Noise LuminBlock number (Rate > 50 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_Noise_100_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_Noise_100", "Noise LuminBlock number (Rate > 100 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_Noise_500_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_Noise_500", "Noise LuminBlock number (Rate > 500 GeV)", 3000, 0, 3000);

  hLuminBlockNumberRate_RBXNoise_10_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXNoise_10", "RBXNoise LuminBlock number (Rate > 10 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXNoise_20_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXNoise_20", "RBXNoise LuminBlock number (Rate > 20 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXNoise_50_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXNoise_50", "RBXNoise LuminBlock number (Rate > 50 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXNoise_100_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXNoise_100", "RBXNoise LuminBlock number (Rate > 100 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXNoise_500_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXNoise_500", "RBXNoise LuminBlock number (Rate > 500 GeV)", 3000, 0, 3000);

  hLuminBlockNumberRate_RBXPedDrift1_10_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXPedDrift1_10", "RBXPedDrift1 LuminBlock(Rate>10 GeV, e2ts/e10ts<0.33)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXPedDrift1_20_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXPedDrift1_20", "RBXPedDrift1 LuminBlock(Rate>20 GeV, e2ts/e10ts<0.33)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXPedDrift1_50_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXPedDrift1_50", "RBXPedDrift1 LuminBlock(Rate>50 GeV, e2ts/e10ts<0.33)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXPedDrift1_100_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXPedDrift1_100", "RBXPedDrift1 LuminBlock(Rate>100 GeV, e2ts/e10ts<0.33)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXPedDrift1_500_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXPedDrift1_500", "RBXPedDrift1 LuminBlock(Rate>500 GeV, e2ts/e10ts<0.33)", 3000, 0, 3000);

  hLuminBlockNumberRate_RBXPedDrift2_10_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXPedDrift2_10", "RBXPedDrift2 LuminBlock(Rate>10 GeV, 0.33<e2ts/e10ts<0.8)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXPedDrift2_20_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXPedDrift2_20", "RBXPedDrift2 LuminBlock(Rate>20 GeV, 0.33<e2ts/e10ts<0.8)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXPedDrift2_50_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXPedDrift2_50", "RBXPedDrift2 LuminBlock(Rate>50 GeV, 0.33<e2ts/e10ts<0.8)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXPedDrift2_100_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXPedDrift2_100", "RBXPedDrift2 LuminBlock(Rate>100 GeV, 0.33<e2ts/e10ts<0.8)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXPedDrift2_500_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXPedDrift2_500", "RBXPedDrift2 LuminBlock(Rate>500 GeV, 0.33<e2ts/e10ts<0.8)", 3000, 0, 3000);

  hLuminBlockNumberRate_RBXFlash1_10_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXFlash1_10", "RBXFlash1 LuminBlock(Rate>10 GeV, e2ts/e10ts>0.8 && NHits>10)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXFlash1_20_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXFlash1_20", "RBXFlash1 LuminBlock(Rate>20 GeV, e2ts/e10ts>0.8 && NHits>10)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXFlash1_50_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXFlash1_50", "RBXFlash1 LuminBlock(Rate>50 GeV, e2ts/e10ts>0.8 && NHits>10)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXFlash1_100_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXFlash1_100", "RBXFlash1 LuminBlock(Rate>100 GeV, e2ts/e10ts>0.8 && NHits>10)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXFlash1_500_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXFlash1_500", "RBXFlash1 LuminBlock(Rate>500 GeV, e2ts/e10ts>0.8 && NHits>10)", 3000, 0, 3000);


  hLuminBlockNumberRate_RBXFlash2_10_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXFlash2_10", "RBXFlash2 LuminBlock(Rate>10 GeV, e2ts/e10ts>0.8 && NHits<10)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXFlash2_20_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXFlash2_20", "RBXFlash2 LuminBlock(Rate>20 GeV, e2ts/e10ts>0.8 && NHits<10)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXFlash2_50_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXFlash2_50", "RBXFlash2 LuminBlock(Rate>50 GeV, e2ts/e10ts>0.8 && NHits<10)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXFlash2_100_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXFlash2_100", "RBXFlash2 LuminBlock(Rate>100 GeV, e2ts/e10ts>0.8 && NHits<10)", 3000, 0, 3000);
  hLuminBlockNumberRate_RBXFlash2_500_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_RBXFlash2_500", "RBXFlash2 LuminBlock(Rate>500 GeV, e2ts/e10ts>0.8 && NHits<10)", 3000, 0, 3000);


  hLuminBlockNumberRate_HPDNoise_20_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDNoise_20", "HPDNoise LuminBlock number (Rate > 20 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_HPDNoise_50_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDNoise_50", "HPDNoise LuminBlock number (Rate > 50 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_HPDNoise_100_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDNoise_100", "HPDNoise LuminBlock number (Rate > 100 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_HPDNoise_500_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDNoise_500", "HPDNoise LuminBlock number (Rate > 500 GeV)", 3000, 0, 3000);

  hLuminBlockNumberRate_HPDionfeed_20_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDionfeed_20", "Noise (ion feedback) LuminBlock number (Rate > 20 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_HPDionfeed_50_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDionfeed_50", "Noise (ion feedback) LuminBlock number (Rate > 50 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_HPDionfeed_100_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDionfeed_100", "Noise (ion feedback) LuminBlock number (Rate > 100 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_HPDionfeed_500_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDionfeed_500", "Noise (ion feedback) LuminBlock number (Rate > 500 GeV)", 3000, 0, 3000);

  hLuminBlockNumberRate_HPDdischg_20_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDdischg_20", "Noise (discharge) LuminBlock number (Rate > 20 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_HPDdischg_50_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDdischg_50", "Noise (discharge) LuminBlock number (Rate > 50 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_HPDdischg_100_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDdischg_100", "Noise (discharge) LuminBlock number (Rate > 100 GeV)", 3000, 0, 3000);
  hLuminBlockNumberRate_HPDdischg_500_=NoiseDir.make<TH1D>("hLuminBlockNumberRate_HPDdischg_500", "Noise (discharge) LuminBlock number (Rate > 500 GeV)", 3000, 0, 3000);


  hRunNumber_=NoiseDir.make<TH1I>("hRunNumber", "Run Numbers used in this file", 330000, 130000, 460000);
  //hRunNumber_->SetBit(TH1::kCanRebin);
  
  hBunchCrossing_=NoiseDir.make<TH1I>("hBunchCrossing", "Bunch Crossings used in this file", 4000, 0, 4000);

  hHPDs_vs_RBX_Energy = NoiseDir.make<TH1D>("HPD Energy vs RBX Energy", "", 120 , 0., 1.2); 

  hinvscale = NoiseDir.make<TH1D>("invscale", "scale factor (bx)", 3565, 0., 3565);

//-------------------------------MET------------------------------------------------------------------------------------ 
//cmj2011Jul15  hMet_=NoiseDir.make<TH1D>("hMet","MET",50,0,200);
//cmj2011JUl15  hMetRate_=NoiseDir.make<TH1D>("hMetRate","MET Rate",50,0,200);

  h_l1_gtbit = NoiseDir.make<TH1D>("h_l1_gtbit", "L1 Algo Trigger Bit", 128, -0.5, 127.5);
  h_l1_gtbit_techtrig = NoiseDir.make<TH1D>("h_l1_gtbit_techtrig", "L1 Technical Trigger Bit", 64, -0.5, 63.5);

//cmj2012Aug18 --------------------------------------------------------------------------------------------
// cmj2012Aug6
   Int_t neta = 60; Double_t etaLow=-30.0; Double_t etaHi = +30.0;
   Int_t nphi = 73; Double_t phiLow=0.0;   Double_t phiHi = 72.0;
   h_rechit_eta_phi_RBX_energy_20 = NoiseDir.make<TH2D>("h_rechit_eta_phi_RBX_energy_20","#eta vs. #phi (HBHE) RBX Energy Weighted > 20",neta,etaLow,etaHi,nphi,phiLow,phiHi);
   h_rechit_eta_RBX_energy_20 = NoiseDir.make<TH1D>("h_rechit_eta_RBX_energy_20","#eta (HBHE)- RBX Energy Weighted > 20",neta,etaLow,etaHi);
   h_rechit_phi_RBX_energy_20 = NoiseDir.make<TH1D>("h_rechit_phi_RBX_energy_20","#phi (HBHE)- RBX Energy Weighted > 20",nphi,phiLow,phiHi);
   h_rechit_eta_phi_depth1_RBX_energy_20 = NoiseDir.make<TH2D>("h_rechit_eta_phi_depth1_RBX_energy_20","#eta vs. #phi (HBHE) - depth1 - RBX Energy Weighted > 20",neta,etaLow,etaHi,nphi,phiLow,phiHi);
   h_rechit_eta_phi_depth2_RBX_energy_20 = NoiseDir.make<TH2D>("h_rechit_eta_phi_depth2_RBX_energy_20","#eta vs. #phi (HBHE) - depth2 - RBX Energy Weighted > 20",neta,etaLow,etaHi,nphi,phiLow,phiHi);
   h_rechit_eta_phi_depth3_RBX_energy_20 = NoiseDir.make<TH2D>("h_rechit_eta_phi_depth3_RBX_energy_20","#eta vs. #phi (HBHE) - depth2 - RBX Energy Weighted > 20",neta,etaLow,etaHi,nphi,phiLow,phiHi);

   h_rechit_eta_phi_RBX_energy_100 = NoiseDir.make<TH2D>("h_rechit_eta_phi_RBX_energy_100","#eta vs. #phi (HBHE) RBX Energy Weighted > 100",neta,etaLow,etaHi,nphi,phiLow,phiHi);
   h_rechit_eta_RBX_energy_100 = NoiseDir.make<TH1D>("h_rechit_eta_RBX_energy_100","#eta (HBHE)- RBX Energy Weighted > 100",neta,etaLow,etaHi);
   h_rechit_phi_RBX_energy_100 = NoiseDir.make<TH1D>("h_rechit_phi_RBX_energy_100","#phi (HBHE)- RBX Energy Weighted > 100",nphi,phiLow,phiHi);
   h_rechit_eta_phi_depth1_RBX_energy_100 = NoiseDir.make<TH2D>("h_rechit_eta_phi_depth1_RBX_energy_100","#eta vs. #phi (HBHE) - depth1 - RBX Energy Weighted > 100",neta,etaLow,etaHi,nphi,phiLow,phiHi);
   h_rechit_eta_phi_depth2_RBX_energy_100 = NoiseDir.make<TH2D>("h_rechit_eta_phi_depth2_RBX_energy_100","#eta vs. #phi (HBHE) - depth2 - RBX Energy Weighted > 100",neta,etaLow,etaHi,nphi,phiLow,phiHi);
   h_rechit_eta_phi_depth3_RBX_energy_100 = NoiseDir.make<TH2D>("h_rechit_eta_phi_depth3_RBX_energy_100","#eta vs. #phi (HBHE) - depth2 - RBX Energy Weighted > 100",neta,etaLow,etaHi,nphi,phiLow,phiHi);

   h_rechit_eta_phi_RBX_energy_500 = NoiseDir.make<TH2D>("h_rechit_eta_phi_RBX_energy_500","#eta vs. #phi (HBHE) RBX Energy Weighted > 500",neta,etaLow,etaHi,nphi,phiLow,phiHi);
   h_rechit_eta_RBX_energy_500 = NoiseDir.make<TH1D>("h_rechit_eta_RBX_energy_500","#eta (HBHE)- RBX Energy Weighted > 500",neta,etaLow,etaHi);
   h_rechit_phi_RBX_energy_500 = NoiseDir.make<TH1D>("h_rechit_phi_RBX_energy_500","#phi (HBHE)- RBX Energy Weighted > 500",nphi,phiLow,phiHi);
   h_rechit_eta_phi_depth1_RBX_energy_500 = NoiseDir.make<TH2D>("h_rechit_eta_phi_depth1_RBX_energy_500","#eta vs. #phi (HBHE) - depth1 - RBX Energy Weighted > 500",neta,etaLow,etaHi,nphi,phiLow,phiHi);
   h_rechit_eta_phi_depth2_RBX_energy_500 = NoiseDir.make<TH2D>("h_rechit_eta_phi_depth2_RBX_energy_500","#eta vs. #phi (HBHE) - depth2 - RBX Energy Weighted > 500",neta,etaLow,etaHi,nphi,phiLow,phiHi);
   h_rechit_eta_phi_depth3_RBX_energy_500 = NoiseDir.make<TH2D>("h_rechit_eta_phi_depth3_RBX_energy_500","#eta vs. #phi (HBHE) - depth2 - RBX Energy Weighted > 500",neta,etaLow,etaHi,nphi,phiLow,phiHi);
// cmj2012Aug6
//cmj2012Aug18 --------------------------------------------------------------------------------------------

  
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HcalNoiseInfoAnalyzer::endJob() {

  
  // Convert "number of events" to "Rate(Hz)"
  //   sort(lumi.begin(), lumi.end());
//   reverse(lumi.begin(), lumi.end());

  //scale and integrate to calculate the Rates
  //double invscale = (maxLumiBlock_-firstLumiBlock_+1)*93.;

  // scale size depends upon number of lumi blocks run over
  // 4 differnt cases
  double invscale;

  // if run over all lumi blocks after some skipped
  if(numLumiBlocks_ < 0) 
    invscale  = (lumi.size()-skipLumiBlocks_);

  // if run over numluniblocks after some skipped
  else if(lumi.size() > numLumiBlocks_+skipLumiBlocks_)
    invscale = numLumiBlocks_;

  // if run over numluniblocks but not enough blocks left in run after some skipped
  else if(lumi.size() > skipLumiBlocks_)
    invscale = (lumi.size() - skipLumiBlocks_);

  // if all events skipped
  else {
    cout << "No Lumi sections were proccessed asked to skip: "
	 << skipLumiBlocks_ << " but only " << lumi.size()
	 << " blocks in Run."<< endl;
    invscale = 1;
  }

  hinvscale->Fill(invscale);

/*

  hBeforeRBXEnergyRate_->Scale(1/invscale);
  hAfterRBXEnergyRate_->Scale(1/invscale);
  hMetRate_->Scale(1/invscale);
  hRBXEnergyRate_RBXnoise_->Scale(1/invscale);   
  hRBXEnergyRate_HPDionfeed_->Scale(1/invscale); 
  hRBXEnergyRate_HPDdischg_->Scale(1/invscale);  
*/

  for(int bin1=0; bin1<=hBeforeRBXEnergyRate_->GetNbinsX()+1; bin1++) {
    double integral=0.0;
    for(int bin2=bin1; bin2<=hBeforeRBXEnergyRate_->GetNbinsX()+1; bin2++)
      integral += hBeforeRBXEnergyRate_->GetBinContent(bin2);
    hBeforeRBXEnergyRate_->SetBinContent(bin1, integral);
  }

  for(int bin1=0; bin1<=hAfterRBXEnergyRate_->GetNbinsX()+1; bin1++) {
    double integral=0.0;
    for(int bin2=bin1; bin2<=hAfterRBXEnergyRate_->GetNbinsX()+1; bin2++)
      integral += hAfterRBXEnergyRate_->GetBinContent(bin2);
    hAfterRBXEnergyRate_->SetBinContent(bin1, integral);
  }


//cmj2011Jul15    
/*
  for(int bin1=0; bin1<=hMetRate_->GetNbinsX()+1; bin1++) {
    double integral=0.0;
    for(int bin2=bin1; bin2<=hMetRate_->GetNbinsX()+1; bin2++)
      integral += hMetRate_->GetBinContent(bin2);
    hMetRate_->SetBinContent(bin1, integral);
  }
*/
//cmj2011Jul15
  
  for(int bin1=0; bin1<=hRBXEnergyRate_RBXnoise_->GetNbinsX()+1; bin1++) {
    double integral=0.0;
    for(int bin2=bin1; bin2<=hRBXEnergyRate_RBXnoise_->GetNbinsX()+1; bin2++)
      integral += hRBXEnergyRate_RBXnoise_->GetBinContent(bin2);
    hRBXEnergyRate_RBXnoise_->SetBinContent(bin1, integral);
  }

  for(int bin1=0; bin1<=hRBXEnergyRate_HPDionfeed_->GetNbinsX()+1; bin1++) {
    double integral=0.0;
    for(int bin2=bin1; bin2<=hRBXEnergyRate_HPDionfeed_->GetNbinsX()+1; bin2++)
      integral += hRBXEnergyRate_HPDionfeed_->GetBinContent(bin2);
    hRBXEnergyRate_HPDionfeed_->SetBinContent(bin1, integral);
  }

  for(int bin1=0; bin1<=hRBXEnergyRate_HPDdischg_->GetNbinsX()+1; bin1++) {
    double integral=0.0;
    for(int bin2=bin1; bin2<=hRBXEnergyRate_HPDdischg_->GetNbinsX()+1; bin2++)
      integral += hRBXEnergyRate_HPDdischg_->GetBinContent(bin2);
    hRBXEnergyRate_HPDdischg_->SetBinContent(bin1, integral);
  }

//cmj2011Jul15  ePrint->endJob();
}


//define this as a plug-in
DEFINE_FWK_MODULE(HcalNoiseInfoAnalyzer);
