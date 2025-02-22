// -*- C++ -*-
//
// Package:    FastTimeNumberingTester
// Class:      FastTimeNumberingTester
//
/**\class FastTimeNumberingTester FastTimeNumberingTester.cc
 test/FastTimeNumberingTester.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Sunanda Banerjee
//         Created:  Mon 2014/04/24
// $Id: FastTimeNumberingTester.cc,v 1.0 2014/04/24 14:06:07 sunanda Exp $
//
//

//#define EDM_ML_DEBUG

// system include files
#include <fstream>
#include <iostream>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/ForwardDetId/interface/FastTimeDetId.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDExpandedView.h"
#include "DetectorDescription/Core/interface/DDSpecifics.h"
#include "Geometry/HGCalCommonData/interface/FastTimeDDDConstants.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

class FastTimeNumberingTester : public edm::one::EDAnalyzer<> {
public:
  explicit FastTimeNumberingTester(const edm::ParameterSet&);
  ~FastTimeNumberingTester() override = default;

  void beginJob() override {}
  void analyze(edm::Event const& iEvent, edm::EventSetup const&) override;
  void endJob() override {}

private:
  const edm::ESGetToken<FastTimeDDDConstants, IdealGeometryRecord> token_;
};

FastTimeNumberingTester::FastTimeNumberingTester(const edm::ParameterSet&)
    : token_{esConsumes<FastTimeDDDConstants, IdealGeometryRecord>(edm::ESInputTag{})} {}

// ------------ method called to produce the data  ------------
void FastTimeNumberingTester::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  const FastTimeDDDConstants& fTnDC = iSetup.getData(token_);
  edm::LogVerbatim("HGCalGeom") << "Fast timing device with " << fTnDC.getCells(1) << ":" << fTnDC.getCells(2)
                                << " cells for barrel and endcap";
  for (int type = 1; type <= 2; ++type) {
    for (int ix = 0; ix < 400; ++ix) {
      for (int iy = 0; iy < 400; ++iy) {
        if (fTnDC.isValidXY(type, ix, iy)) {
          FastTimeDetId id1(type, ix, iy, 1), id2(type, ix, iy, -1);
          edm::LogVerbatim("HGCalGeom") << "Valid ID " << id1 << " and " << id2;
        } else {
#ifdef EDM_ML_DEBUG
          edm::LogVerbatim("HGCalGeom") << "ix = " << ix << ", iy = " << iy << " is not valid for "
                                        << "FastTime type " << type;
#endif
        }
        iy += 9;
      }
      ix += 9;
    }
  }
}

// define this as a plug-in
DEFINE_FWK_MODULE(FastTimeNumberingTester);
