#ifndef _HCALNOISEMON_NOISEANALYZER_HCALNOISEINFOANALYZER_H_
#define _HCALNOISEMON_NOISEANALYZER_HCALNOISEINFOANALYZER_H_

//
// HcalNoiseInfoAnalyzer.h
//
//    description: HcalNoiseMon version of /RecoMET/METAnalyzers/interface/HcalNoiseInfoAnalyzer.h
//
//    author: J.P. Chou, Brown
//
//	Modified 2012Aug18 by C M Jenkins 
//			University of South Alabama
//			Inlcude the ieta vs iph RBX energy wieghted plots.
//
//

// system include files
#include <memory>
#include <string>
#include <vector>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//cmj2011Jul15 #include "HcalNoiseMon/NoiseAnalyzer/interface/PrintInfo.h"

//cmj2012Aug18 ---------------------------------------------------------------------------------------
//cmj2012Jul3 ----- Include rechit and occupancy headers
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
//cmj2012Jul3 ----- Include rechit and occupancy headers
//cmj2012Aug18 ---------------------------------------------------------------------------------------

// forward declarations
class TH1D;
class TH2D;
class TFile;

namespace reco {

  //
  // class declaration
  //

  class HcalNoiseInfoAnalyzer : public edm::EDAnalyzer {
  public:
    explicit HcalNoiseInfoAnalyzer(const edm::ParameterSet&);
    ~HcalNoiseInfoAnalyzer();
  
  
  private:
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();
    
    edm::InputTag metTag_;
    edm::Service<TFileService> fs;

//cmj2011Jul15    PrintInfo* ePrint;    
  
    // root file/histograms
    TFile* rootfile_;

    TH1D* hLumiBlock_;         // Luminosity Block

    TH1D* hMaxZeros_;         // maximum # of zeros in an RBX channel
    TH1D* hTotalZeros_;       // total # of zeros in an RBX
    TH1D* hE2ts_;             // E(2ts) for the highest energy digi in an HPD
    TH1D* hE10ts_;            // E(10ts) for the highest energy digi in an HPD
    TH1D* hE2tsOverE10ts_;    // E(t2s)/E(10ts) for the highest energy digi in an HPD
    TH1D* h5E2ts_;             // E(2ts) for the highest 5 energy digi in an HPD
    TH1D* h5E10ts_;            // E(10ts) for the highest 5 energy digi in an HPD
    TH1D* h5E2tsOverE10ts_;    // E(t2s)/E(10ts) for the highest 5 energy digi in an HPD
    TH1D* hRBXE2ts_;          // Sum RBX E(2ts)
    TH1D* hRBXE10ts_;         // Sum RBX E(10ts)
    TH1D* hRBXE2tsOverE10ts_; // Sum RBXE(t2s)/E(10ts)
    TH1D* hHPDNHits_;         // Number of Hits with E>1.5 GeV in an HPD
    TH1D* hHPDNHits2_;        // Copy of Number of Hits with E>1.5 GeV in an HPD tro superimpose with RBX

    TH1D* hFailures_;         // code designating which cut the event failed (if any)
    TH1D* hBeforeRBXEnergy_;  // Total RecHit Energy in RBX before cuts
    TH1D* hAfterRBXEnergy_;   // Total RecHit Energy in RBX after cuts
    TH1D* hBeforeRBXEnergyRate_;  // Total RecHit Energy in RBX before cuts (Rate)
    TH1D* hAfterRBXEnergyRate_;   // Total RecHit Energy in RBX after cuts (Rate)
    TH1D* hMet_;
    TH1D* hMetRate_;

    TH1D* hNRBXHits_;
    TH1D* hNRBXobjects_;              // Total number of RBX objects
    TH1D* hNRBXobjectsE20GeV_;        // Total number of RBX objects with E>20 GeV
    TH1D* hHCalNoiseCategory_;        // Hcal noise category (1: RBX noise, 
                                      //                      2: HPD noise ion feedback, 
                                      //                      3: HPD noise discharge)
    TH1D* hRBXEnergy_RBXnoise_;       // Total RBX RecHit Energy for RBZX noise
    TH1D* hRBXEnergyRate_RBXnoise_;   // Total RBX RecHit Energy for RBZX noise (Rate)
    TH1D* hRBXEnergy_HPDionfeed_;     // Total RBX RecHit Energy for HPD ion feedback
    TH1D* hRBXEnergyRate_HPDionfeed_; // Total RBX RecHit Energy for HPD ion feedback (Rate)
    TH1D* hRBXEnergy_HPDdischg_;      // Total RBX RecHit Energy for HPD discharge
    TH1D* hRBXEnergyRate_HPDdischg_;  // Total RBX RecHit Energy for HPD discharge (Rate)

    TH1D* hRBXEnergy_RBXnoisePedDrift1_;     // Total RBX RecHit Energy for RBZX noise
    TH1D* hRBXEnergyRate_RBXnoisePedDrift1_; // Total RBX RecHit Energy for RBZX noise (Rate)
    TH1D* hRBXEnergy_RBXnoisePedDrift2_;     // Total RBX RecHit Energy for RBZX noise
    TH1D* hRBXEnergyRate_RBXnoisePedDrift2_; // Total RBX RecHit Energy for RBZX noise (Rate)
    TH1D* hRBXEnergy_RBXnoiseFlash1_;        // Total RBX RecHit Energy for RBZX noise
    TH1D* hRBXEnergyRate_RBXnoiseFlash1_;    // Total RBX RecHit Energy for RBZX noise (Rate)
    TH1D* hRBXEnergy_RBXnoiseFlash2_;        // Total RBX RecHit Energy for RBZX noise
    TH1D* hRBXEnergyRate_RBXnoiseFlash2_;    // Total RBX RecHit Energy for RBZX noise (Rate)

    TH1D* hRBXNumberRate_RBXnoise_10_;
    TH1D* hRBXNumberRate_RBXnoise_20_;  // RBX number ( 1-18: Barrel/positive-eta)
    TH1D* hRBXNumberRate_RBXnoise_50_;  //            (19-36: Barrel/negative-eta)
    TH1D* hRBXNumberRate_RBXnoise_100_;  //            (37-54: Endcap/positive-eta)
    TH1D* hRBXNumberRate_RBXnoise_500_;  //            (55-72: Endcap/negative-eta)

    TH1D* hRBXNumberRate_RBXnoisePedDrift1_10_;
    TH1D* hRBXNumberRate_RBXnoisePedDrift1_20_;  // RBX number ( 1-18: Barrel/positive-eta)
    TH1D* hRBXNumberRate_RBXnoisePedDrift1_50_;  //            (19-36: Barrel/negative-eta)
    TH1D* hRBXNumberRate_RBXnoisePedDrift1_100_;  //            (37-54: Endcap/positive-eta)
    TH1D* hRBXNumberRate_RBXnoisePedDrift1_500_;  //            (55-72: Endcap/negative-eta)

    TH1D* hRBXNumberRate_RBXnoisePedDrift2_10_;
    TH1D* hRBXNumberRate_RBXnoisePedDrift2_20_;  // RBX number ( 1-18: Barrel/positive-eta)
    TH1D* hRBXNumberRate_RBXnoisePedDrift2_50_;  //            (19-36: Barrel/negative-eta)
    TH1D* hRBXNumberRate_RBXnoisePedDrift2_100_;  //            (37-54: Endcap/positive-eta)
    TH1D* hRBXNumberRate_RBXnoisePedDrift2_500_;  //            (55-72: Endcap/negative-eta)

    TH1D* hRBXNumberRate_RBXnoiseFlash1_10_;
    TH1D* hRBXNumberRate_RBXnoiseFlash1_20_;  // RBX number ( 1-18: Barrel/positive-eta)
    TH1D* hRBXNumberRate_RBXnoiseFlash1_50_;  //            (19-36: Barrel/negative-eta)
    TH1D* hRBXNumberRate_RBXnoiseFlash1_100_;  //            (37-54: Endcap/positive-eta)
    TH1D* hRBXNumberRate_RBXnoiseFlash1_500_;  //            (55-72: Endcap/negative-eta)

    TH1D* hRBXNumberRate_RBXnoiseFlash2_10_;
    TH1D* hRBXNumberRate_RBXnoiseFlash2_20_;  // RBX number ( 1-18: Barrel/positive-eta)
    TH1D* hRBXNumberRate_RBXnoiseFlash2_50_;  //            (19-36: Barrel/negative-eta)
    TH1D* hRBXNumberRate_RBXnoiseFlash2_100_;  //            (37-54: Endcap/positive-eta)
    TH1D* hRBXNumberRate_RBXnoiseFlash2_500_;  //            (55-72: Endcap/negative-eta)

    TH1D* hHPDNumberRate_HBpos_HPDnoise_20_;  // HPD number for HPD noise    (Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDnoise_20_;  // HPD number [ion feed+dischg](Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDnoise_20_;  // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDnoise_20_;  // HPD number                  (EndCap/negative-eta)
    TH1D* hHPDNumberRate_HBpos_HPDionfeed_20_; // HPD number for HPD ion feed (Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDionfeed_20_; // HPD number                  (Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDionfeed_20_; // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDionfeed_20_; // HPD number                  (EndCap/negative-eta)
    TH1D* hHPDNumberRate_HBpos_HPDdischg_20_;  // HPD number for HPD discharge(Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDdischg_20_;  // HPD number                  (Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDdischg_20_;  // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDdischg_20_;  // HPD number                  (EndCap/negative-eta)

    TH1D* hHPDNumberRate_HBpos_HPDnoise_50_;  // HPD number for HPD noise    (Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDnoise_50_;  // HPD number [ion feed+dischg](Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDnoise_50_;  // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDnoise_50_;  // HPD number                  (EndCap/negative-eta)
    TH1D* hHPDNumberRate_HBpos_HPDionfeed_50_; // HPD number for HPD ion feed (Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDionfeed_50_; // HPD number                  (Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDionfeed_50_; // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDionfeed_50_; // HPD number                  (EndCap/negative-eta)
    TH1D* hHPDNumberRate_HBpos_HPDdischg_50_;  // HPD number for HPD discharge(Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDdischg_50_;  // HPD number                  (Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDdischg_50_;  // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDdischg_50_;  // HPD number                  (EndCap/negative-eta)

    TH1D* hHPDNumberRate_HBpos_HPDnoise_100_;  // HPD number for HPD noise    (Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDnoise_100_;  // HPD number [ion feed+dischg](Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDnoise_100_;  // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDnoise_100_;  // HPD number                  (EndCap/negative-eta)
    TH1D* hHPDNumberRate_HBpos_HPDionfeed_100_; // HPD number for HPD ion feed (Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDionfeed_100_; // HPD number                  (Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDionfeed_100_; // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDionfeed_100_; // HPD number                  (EndCap/negative-eta)
    TH1D* hHPDNumberRate_HBpos_HPDdischg_100_;  // HPD number for HPD discharge(Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDdischg_100_;  // HPD number                  (Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDdischg_100_;  // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDdischg_100_;  // HPD number                  (EndCap/negative-eta)

    TH1D* hHPDNumberRate_HBpos_HPDnoise_500_;  // HPD number for HPD noise    (Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDnoise_500_;  // HPD number [ion feed+dischg](Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDnoise_500_;  // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDnoise_500_;  // HPD number                  (EndCap/negative-eta)
    TH1D* hHPDNumberRate_HBpos_HPDionfeed_500_; // HPD number for HPD ion feed (Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDionfeed_500_; // HPD number                  (Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDionfeed_500_; // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDionfeed_500_; // HPD number                  (EndCap/negative-eta)
    TH1D* hHPDNumberRate_HBpos_HPDdischg_500_;  // HPD number for HPD discharge(Barrel/positive-eta)
    TH1D* hHPDNumberRate_HBneg_HPDdischg_500_;  // HPD number                  (Barrel/negative-eta)
    TH1D* hHPDNumberRate_HEpos_HPDdischg_500_;  // HPD number                  (EndCap/positive-eta)
    TH1D* hHPDNumberRate_HEneg_HPDdischg_500_;  // HPD number                  (EndCap/negative-eta)

    TH1D* hLuminBlockNumberRate_Noise_10_;
    TH1D* hLuminBlockNumberRate_Noise_20_;
    TH1D* hLuminBlockNumberRate_Noise_50_;
    TH1D* hLuminBlockNumberRate_Noise_100_;
    TH1D* hLuminBlockNumberRate_Noise_500_;

    TH1D* hLuminBlockNumberRate_RBXNoise_10_;
    TH1D* hLuminBlockNumberRate_RBXNoise_20_;
    TH1D* hLuminBlockNumberRate_RBXNoise_50_;
    TH1D* hLuminBlockNumberRate_RBXNoise_100_;
    TH1D* hLuminBlockNumberRate_RBXNoise_500_;

    TH1D* hLuminBlockNumberRate_RBXPedDrift1_10_;
    TH1D* hLuminBlockNumberRate_RBXPedDrift1_20_;
    TH1D* hLuminBlockNumberRate_RBXPedDrift1_50_;
    TH1D* hLuminBlockNumberRate_RBXPedDrift1_100_;
    TH1D* hLuminBlockNumberRate_RBXPedDrift1_500_;

    TH1D* hLuminBlockNumberRate_RBXPedDrift2_10_;
    TH1D* hLuminBlockNumberRate_RBXPedDrift2_20_;
    TH1D* hLuminBlockNumberRate_RBXPedDrift2_50_;
    TH1D* hLuminBlockNumberRate_RBXPedDrift2_100_;
    TH1D* hLuminBlockNumberRate_RBXPedDrift2_500_;

    TH1D* hLuminBlockNumberRate_RBXFlash1_10_;
    TH1D* hLuminBlockNumberRate_RBXFlash1_20_;
    TH1D* hLuminBlockNumberRate_RBXFlash1_50_;
    TH1D* hLuminBlockNumberRate_RBXFlash1_100_;
    TH1D* hLuminBlockNumberRate_RBXFlash1_500_;

    TH1D* hLuminBlockNumberRate_RBXFlash2_10_;
    TH1D* hLuminBlockNumberRate_RBXFlash2_20_;
    TH1D* hLuminBlockNumberRate_RBXFlash2_50_;
    TH1D* hLuminBlockNumberRate_RBXFlash2_100_;
    TH1D* hLuminBlockNumberRate_RBXFlash2_500_;


    TH1D* hLuminBlockNumberRate_HPDNoise_20_;
    TH1D* hLuminBlockNumberRate_HPDNoise_50_;
    TH1D* hLuminBlockNumberRate_HPDNoise_100_;
    TH1D* hLuminBlockNumberRate_HPDNoise_500_;

    TH1D* hLuminBlockNumberRate_HPDionfeed_20_;
    TH1D* hLuminBlockNumberRate_HPDionfeed_50_;
    TH1D* hLuminBlockNumberRate_HPDionfeed_100_;
    TH1D* hLuminBlockNumberRate_HPDionfeed_500_;

    TH1D* hLuminBlockNumberRate_HPDdischg_20_;
    TH1D* hLuminBlockNumberRate_HPDdischg_50_;
    TH1D* hLuminBlockNumberRate_HPDdischg_100_;
    TH1D* hLuminBlockNumberRate_HPDdischg_500_;

    TH1I* hRunNumber_;
    TH1I* hBunchCrossing_;
    TH1D* h_l1_gtbit;
    TH1D* h_l1_gtbit_techtrig;

    TH1D* hHPDs_vs_RBX_Energy;

    TH1D* hinvscale;

    TH1I* hRunStartHolder_;
    TH1I* hRunStopHolder_;
//cmj2012Aug18 ------------------------------------------------------------------
// cmj2012Aug6
    TH1D *h_rechit_eta_RBX_energy_20;
    TH1D *h_rechit_phi_RBX_energy_20;
    TH2D *h_rechit_eta_phi_RBX_energy_20;
    TH2D *h_rechit_eta_phi_depth1_RBX_energy_20;
    TH2D *h_rechit_eta_phi_depth2_RBX_energy_20;
    TH2D *h_rechit_eta_phi_depth3_RBX_energy_20;
    TH1D *h_rechit_eta_RBX_energy_100;
    TH1D *h_rechit_phi_RBX_energy_100;
    TH2D *h_rechit_eta_phi_RBX_energy_100;
    TH2D *h_rechit_eta_phi_depth1_RBX_energy_100;
    TH2D *h_rechit_eta_phi_depth2_RBX_energy_100;
    TH2D *h_rechit_eta_phi_depth3_RBX_energy_100;
    TH1D *h_rechit_eta_RBX_energy_500;
    TH1D *h_rechit_phi_RBX_energy_500;
    TH2D *h_rechit_eta_phi_RBX_energy_500;
    TH2D *h_rechit_eta_phi_depth1_RBX_energy_500;
    TH2D *h_rechit_eta_phi_depth2_RBX_energy_500;
    TH2D *h_rechit_eta_phi_depth3_RBX_energy_500;
// cmj2012Aug6

//cmj2012Aug18 ------------------------------------------------------------------
    int skDumpFlag;
    int skDumpCounter;

    // parameters
    std::string rbxCollName_;      // label for the HcalNoiseRBXCollection
    std::string rootHistFilename_; // name of the histogram file

    std::string m_trigName;
    bool m_isFirstTrig;
    unsigned m_trigIndex;
    edm::InputTag m_hltResults;

    double numLumiBlocks_;
    double skipLumiBlocks_;
    /////////////////   
    int truefirstLumi_;      // first Luminosity block
    int firstLumiBlock_;      // first Luminosity block
    int maxLumiBlock_;        // maximum Luminosity block
    /////////////////////////   
    bool timeDone;
    std::vector<long> lumi;
  };

} // end of namespace

#endif
