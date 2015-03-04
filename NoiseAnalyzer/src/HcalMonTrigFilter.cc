// -*- C++ -*-
//
// Package:    HcalMonTrigFilter
// Class:      HcalMonTrigFilter
// 
/**\class HcalMonTrigFilter HcalMonTrigFilter.cc HcalNoiseMon/NoiseAnalyzer/src/HcalMonTrigFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  "Jian Wang"
//         Created:  Fri Mar 26 12:23:46 CDT 2010
// $Id: HcalMonTrigFilter.cc,v 1.2 2010/06/01 02:52:19 wangjian Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include <iostream>
#include <string>

//
// class declaration
//

class HcalMonTrigFilter : public edm::EDFilter {
   public:
      explicit HcalMonTrigFilter(const edm::ParameterSet&);
      ~HcalMonTrigFilter();

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------

  std::vector<std::string> l1Paths_;
  std::vector<int> techPaths_;
  std::vector<std::string> hltPaths_;
  edm::TriggerNames hltNames_;
  edm::InputTag l1Tag_;
  edm::InputTag hltTag_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HcalMonTrigFilter::HcalMonTrigFilter(const edm::ParameterSet& iConfig) :
  l1Paths_(iConfig.getParameter< std::vector<std::string> >("l1Paths")),
  techPaths_(iConfig.getParameter< std::vector<int> >("techPaths")),
  hltPaths_(iConfig.getParameter< std::vector<std::string> >("hltPaths")),
  l1Tag_(iConfig.getParameter< edm::InputTag >("l1Tag")),
  hltTag_(iConfig.getParameter< edm::InputTag >("hltTag"))
{
   //now do what ever initialization is needed

}


HcalMonTrigFilter::~HcalMonTrigFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
HcalMonTrigFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   Handle<L1GlobalTriggerReadoutRecord> gtRecord;
   iEvent.getByLabel(l1Tag_, gtRecord);
   const DecisionWord dWord = gtRecord->decisionWord();
   const TechnicalTriggerWord tWord = gtRecord->technicalTriggerWord();

   Handle<L1GlobalTriggerObjectMapRecord> L1GTOMRec;
   iEvent.getByLabel("hltL1GtObjectMap",L1GTOMRec);
   const vector<L1GlobalTriggerObjectMap>& objMapVec = L1GTOMRec->gtObjectMap();

   Handle<TriggerResults> hltResults;
   iEvent.getByLabel(hltTag_, hltResults);
   const edm::TriggerNames & hltNames_ = iEvent.triggerNames(*hltResults); 
   //hltNames_.init(*hltResults);

   vector<string>::const_iterator l1Path;
   vector<int>::const_iterator techPath;
   vector<string>::const_iterator hltPath;

   for ( l1Path = l1Paths_.begin(); l1Path != l1Paths_.end(); ++l1Path ) {

     for (vector<L1GlobalTriggerObjectMap>::const_iterator itMap = objMapVec.begin(); itMap != objMapVec.end(); ++itMap) {

	string l1Name = (*itMap).algoName();
	if(l1Name == *l1Path&&dWord.at((*itMap).algoBitNumber())) {
	//cout<<l1Name<<endl;
	return true;
	}
	}
	}

   for ( techPath = techPaths_.begin(); techPath != techPaths_.end(); ++techPath ) {
	if(tWord.at(*techPath))
	return true;
	}

   for ( hltPath = hltPaths_.begin(); hltPath != hltPaths_.end(); ++hltPath ) {
 
	for(size_t itrig = 0; itrig != hltResults->size(); ++itrig){
	
	string hltName = hltNames_.triggerName(itrig);
	if(hltName==*hltPath&&hltResults->accept(itrig)) {
	//cout<<hltName<<endl;
	return true;
	}
	}
	}

   return false;
}

// ------------ method called once each job just before starting event loop  ------------
void 
HcalMonTrigFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HcalMonTrigFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HcalMonTrigFilter);
