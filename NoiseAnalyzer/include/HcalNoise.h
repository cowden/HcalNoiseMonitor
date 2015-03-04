#ifndef HCALNOISE_H
#define HCALNOISE_H

#include "RecoMET/METAlgorithms/interface/HcalNoiseRBXArray.h"
#include "FWCore/Utilities/interface/EDMException.h"

using namespace reco;
using namespace edm;

int HcalNoise(const Event& iEvent, int Algo)
{

   /*
      if Algo=1, Hcal Noise Id Algorithems are chosen (both loose and tight cuts included).
      Reference twiki page: TBA

      Then the return value can be 0, 1 or 2.
      0 means the event can survive cuts.
      1 means the event can be rejected by loose cuts.
      2 means the event can NOT be rejected by loose cuts, but can be rejected by tight cuts.

      if Algo=2, Hcal RecHit status flag cuts are chosen. (Bit 0 and 1 are used here)
      https://twiki.cern.ch/twiki/bin/view/CMS/HcalRecHitFlagAssignments

      Then the return value would be 0 or 1.
      0 means the event survives the cuts.
      1 means the event is rejected.
    */

   //The following cut values are valid for CMSSW3XX (not CMSSW2XX)
   double const SUME2E10_LOOSE = 0.7; //cut on sum of charge in highest 2 ts of every hit in RBX over that in 10 ts
   double const SUME2E10_TIGHT = 0.8;
   double const MINRECHITTIME_LOOSE = -7.; //cut on rechit timing
   double const MAXRECHITTIME_LOOSE = 6.;
   double const MINRECHITTIME_TIGHT = -5.;
   double const MAXRECHITTIME_TIGHT = 4.;
   int const HPDNHITS = 16; //cut on # of hits in a HPD
   int const TOTALZEROS = 7; //cut on total # of ADC zeros in RBX

   int flag = 0;

   if( Algo==1 ){

      double minSumE2E10 = 10.;
      double minRecHitTime = 1000.;
      double maxRecHitTime = -1000.;
      int maxHPDNHits = 0;
      int maxTotalZeros = 0;

      Handle<HcalNoiseRBXCollection> handle;
      iEvent.getByLabel( "hcalnoise", handle );

      // loop over the RBXs
      for( HcalNoiseRBXCollection::const_iterator rit=handle->begin(); rit!=handle->end(); ++rit ) {

         HcalNoiseRBX const& rbx = *rit;

         double rbxenergy = rbx.recHitEnergy( 1.5 );
         int numRBXhits = rbx.numRecHits( 1.5 );

         // require at least 20 GeV in the RBX and at least one hit above 1.5 GeV
         if( numRBXhits > 0  &&  rbxenergy > 20. ) {

            if( rbx.totalZeros() > maxTotalZeros )
               maxTotalZeros = rbx.totalZeros();

            double totale2ts = rbx.allChargeHighest2TS();
            double totale10ts = rbx.allChargeTotal();
            if( totale10ts  &&  totale2ts/totale10ts < minSumE2E10 )
               minSumE2E10 = totale2ts/totale10ts;

            // loop over the HPDs in the RBX
            std::vector<HcalNoiseHPD> const& theHPDs = rbx.HPDs();

            for( std::vector<HcalNoiseHPD>::const_iterator hit = theHPDs.begin(); hit != theHPDs.end(); ++hit ) {

               HcalNoiseHPD const& hpd = *hit;

               // make sure we have at least 1 hit above 5 GeV
               int numHPDhits = hpd.numRecHits( 5.0 );
               if( numHPDhits<1 ) continue;

               // cut on RecHit Time for only RecHit with E > 20GeV

               if( hpd.maxRecHitTime(20.) > maxRecHitTime )
                  maxRecHitTime = hpd.maxRecHitTime(20.);
               if( hpd.minRecHitTime(20.) < minRecHitTime )
                  minRecHitTime = hpd.minRecHitTime(20.);

               if( numHPDhits>maxHPDNHits )
                  maxHPDNHits=numHPDhits;

            } // theHPDs

         } // if (numRBXhits>0 && rbxenergy>20.)

      }  // Loop over RBX boxes ends

      if( 
         maxTotalZeros > TOTALZEROS
         || minSumE2E10 < SUME2E10_LOOSE
         || minRecHitTime < MINRECHITTIME_LOOSE
         || maxRecHitTime > MAXRECHITTIME_LOOSE
         )
         flag=1; 
      else if(
         maxTotalZeros > TOTALZEROS
         || minSumE2E10 < SUME2E10_TIGHT
         || minRecHitTime < MINRECHITTIME_TIGHT
         || maxRecHitTime > MAXRECHITTIME_TIGHT
         || maxHPDNHits > HPDNHITS
         )
         flag=2; 

   }

   if( Algo==2 ){

      Handle<HBHERecHitCollection> hbherecoHandle;
      iEvent.getByLabel( "hbhereco", hbherecoHandle );
      for(
            HBHERecHitCollection::const_iterator itHBHE=hbherecoHandle->begin(); 
            itHBHE != hbherecoHandle->end();
            ++itHBHE )
      {
         int rechitflags = itHBHE->flags();
         if( rechitflags & 0x3 != 0 )
            flag=1;
      }

   }

   return flag;

}

#endif HCALNOISE_H

