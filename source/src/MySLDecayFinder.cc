#include "MySLDecayFinder.h"
#include <iostream>

#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>

// ----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"

#ifdef MARLIN_USE_AIDA
#include <marlin/AIDAProcessor.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/ICloud1D.h>
//#include <AIDA/IHistogram1D.h>
#endif // MARLIN_USE_AIDA

#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"

using namespace lcio ;
using namespace marlin ;


MySLDecayFinder aMySLDecayFinder ;


MySLDecayFinder::MySLDecayFinder() :
	Processor("MySLDecayFinder"),
	m_nRun(0),
     m_nEvt(0),
     m_nRunSum(0),
     m_nEvtSum(0),
     m_printing(0),
     m_lookForQuarksWithMotherZ(0),
	m_nPfosTotal(0),
     m_nPfosNeutralHadrons(0),
     m_nPfosPhotons(0),
     m_nPfosTracks(0),
     m_pfoEnergyTotal(0.f),
     m_pfoEnergyNeutralHadrons(0.f),
     m_pfoEnergyPhotons(0.f),
     m_pfoEnergyTracks(0.f),
	m_pfoMassTotal(0.f),
	m_nMcpsTotal(0),
     m_nMcpsNeutralHadrons(0),
     m_nMcpsPhotons(0),
     m_nMcpsTracks(0),
     m_mcpEnergyTotal(0.f),
     m_mcpEnergyNeutralHadrons(0.f),
     m_mcpEnergyPhotons(0.f),
     m_mcpEnergyTracks(0.f),
	m_mcpMassTotal(0.f),

	m_mcEnergyENu(0.f),
     m_mcEnergyFwd(0.f),
     m_eQQ(-99.f),
     m_eQ1(-99.f),
     m_eQ2(-99.f),
     m_costQQ(-99.f),
     m_costQ1(-99.f),
     m_costQ2(-99.f),
     m_mQQ(-99.f),
     m_thrust(-99.f),
     m_qPdg(-99.f),

	m_nSLDecayTotal(0),
	m_nSLDecayBHad(0),
	m_nSLDecayCHad(0),

	m_pTFile(NULL)


{

    // modify processor description
    _description = "MySLDecayFinder finds semi-leptonic decays inside jets" ;


    // register steering parameters: name, description, class-variable, default value
    registerInputCollection( LCIO::MCPARTICLE,
          				"MCParticleCollection" ,
          				"Name of the MCParticle collection"  ,
          				m_mcParticleCollection,
          				std::string("MCParticle")
    						);
    registerProcessorParameter("RootFile",
						"Name of the output root file",
						m_rootFile,
						std::string("MySLDecayFinder.root")
						);

}



void MySLDecayFinder::init()
{

    streamlog_out(DEBUG) << "   init called  " << std::endl ;

    // usually a good idea to
    printParameters() ;

    m_nRun = 0 ;
    m_nEvt = 0 ;
    m_nRunSum = 0;
    m_nEvtSum = 0;
    this->Clear();

	m_pTFile = new TFile(m_rootFile.c_str(), "recreate");
	m_pTTree = new TTree("PfoAnalysisTree", "PfoAnalysisTree");
	m_pTTree->SetDirectory(m_pTFile);
	m_pTTree->Branch("run", &m_nRun, "run/I");
	m_pTTree->Branch("event", &m_nEvt, "event/I");
	m_pTTree->Branch("nBHadSLDecay", &m_nSLDecayBHad, "nBHadSLDecay/I");
	m_pTTree->Branch("nCHadSLDecay", &m_nSLDecayCHad, "nCHadSLDecay/I");
	m_pTTree->Branch("nSLDecayTotal", &m_nSLDecayTotal, "nSLDecayTotal/I");
}


void MySLDecayFinder::processRunHeader( LCRunHeader *pLCRunHeader)
{
	m_nRun = 0;
     m_nEvt = 0;
     ++m_nRunSum;

//    m_nRun++ ;
}



void MySLDecayFinder::processEvent( LCEvent *pLCEvent )
{

	m_nRun = pLCEvent->getRunNumber();
     m_nEvt = pLCEvent->getEventNumber();
     ++m_nEvtSum;

	if ((m_nEvtSum % 100) == 0)
         std::cout << " processed events: " << m_nEvtSum << std::endl;


	this->Clear();
	this->ExtractCollections(pLCEvent);
	this->FindSLDecays(pLCEvent);
	m_pTTree->Fill();
}



void MySLDecayFinder::check( LCEvent *pLCEvent )
{
    // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void MySLDecayFinder::end()
{

	m_pTFile->cd();
     m_pTTree->Write();

    //   std::cout << "MySLDecayFinder::end()  " << name()
    // 	    << " processed " << _nEvt << " events in " << _nRun << " runs "
    // 	    << std::endl ;

}

void MySLDecayFinder::Clear()
{
	m_nPfosTotal = 0;
     m_nPfosNeutralHadrons = 0;
     m_nPfosPhotons = 0;
     m_nPfosTracks = 0;
     m_pfoEnergyTotal = 0.f;
     m_pfoEnergyNeutralHadrons = 0.f;
     m_pfoEnergyPhotons = 0.f;
     m_pfoEnergyTracks = 0.f;
	m_pfoMassTotal = 0.f;

	m_nMcpsTotal = 0;
     m_nMcpsNeutralHadrons = 0;
     m_nMcpsPhotons = 0;
     m_nMcpsTracks = 0;
     m_mcpEnergyTotal = 0.f;
     m_mcpEnergyNeutralHadrons = 0.f;
     m_mcpEnergyPhotons = 0.f;
     m_mcpEnergyTracks = 0.f;
	m_mcpMassTotal = 0.f;

	m_mcEnergyENu = 0.f;
     m_mcEnergyFwd = 0.f;
     m_eQQ = -99.f;
     m_eQ1 = -99.f;
     m_eQ2 = -99.f;
     m_costQQ = -99.f;
     m_costQ1 = -99.f;
     m_costQ2 = -99.f;
     m_mQQ = -99.f;
     m_thrust = -99.f;
     m_qPdg = -99;

	m_nSLDecayTotal = 0;
	m_nSLDecayBHad = 0;
	m_nSLDecayCHad = 0;

}

void MySLDecayFinder::ExtractCollections(EVENT::LCEvent *pLCEvent)
{
	try
     {
     	const EVENT::LCCollection *pLCCollection = pLCEvent->getCollection(m_mcParticleCollection);

     	for (unsigned int i = 0, nElements = pLCCollection->getNumberOfElements(); i < nElements; ++i)
		{
			const EVENT::MCParticle *pMCParticle = dynamic_cast<EVENT::MCParticle*>(pLCCollection->getElementAt(i));

			if (NULL == pMCParticle)
			throw EVENT::Exception("Collection type mismatch");

			if (!pMCParticle->getParents().empty())
				continue;

//             this->ApplyPfoSelectionRules(pMCParticle, mcPfoCandidates);
		}
	}
     catch (...)
     {
		streamlog_out(WARNING) << "Could not extract mc particle collection " << m_mcParticleCollection << std::endl;
     }
}

void MySLDecayFinder::FindSLDecays(EVENT::LCEvent *pLCEvent)
{
	try
	{
		const EVENT::LCCollection *pLCCollection = pLCEvent->getCollection(m_mcParticleCollection);

		for (unsigned int i = 0, nElements = pLCCollection->getNumberOfElements(); i < nElements; ++i)
		{
			const EVENT::MCParticle *pMCParticle = dynamic_cast<EVENT::MCParticle*>(pLCCollection->getElementAt(i));

			if (NULL == pMCParticle)
				throw EVENT::Exception("Collection type mismatch");

			const int absPdgCode(std::abs(pMCParticle->getPDG()));
			if ((absPdgCode == 12) || (absPdgCode == 14) || (absPdgCode == 16))
			{
				if (!(pMCParticle->getParents().empty()))
				{
					for (unsigned int p = 0; p < (pMCParticle->getParents()).size(); ++p)
					{
						const int absParPdgCode(std::abs((pMCParticle->getParents())[p]->getPDG()));
						if ((floor(absParPdgCode/100)==5) || (floor(absParPdgCode/1000)==5))
						{
							for (unsigned int d = 0; d < (((pMCParticle->getParents())[p])->getDaughters()).size(); ++d)
							{
								const int absDauPdgCode(std::abs(((pMCParticle->getParents())[p]->getDaughters())[d]->getPDG()));
								if ((absDauPdgCode == 11) || (absDauPdgCode == 13) || (absDauPdgCode == 15))
								{
									++m_nSLDecayTotal;
									++m_nSLDecayBHad;
									streamlog_out(DEBUG) << "One Semi-Leptonic decay of B-Hadron was found" << std::endl;
								}
							}
						}
						if ((floor(absParPdgCode/100)==4) || (floor(absParPdgCode/1000)==4))
						{
							for (unsigned int d = 0; d < (((pMCParticle->getParents())[p])->getDaughters()).size(); ++d)
							{
								const int absDauPdgCode(std::abs(((pMCParticle->getParents())[p]->getDaughters())[d]->getPDG()));
								if ((absDauPdgCode == 11) || (absDauPdgCode == 13) || (absDauPdgCode == 15))
								{
									++m_nSLDecayTotal;
									++m_nSLDecayCHad;
									streamlog_out(DEBUG) << "One Semi-Leptonic decay of Charmed-Hadron was found" << std::endl;
								}
							}
						}
					}
				}
			}
		}
		streamlog_out(DEBUG) << "Number of Semi-Leptonic decay of B-Hadron: " << m_nSLDecayBHad << std::endl;
		streamlog_out(DEBUG) << "Number of Semi-Leptonic decay of C-Hadron: " << m_nSLDecayCHad << std::endl;
		streamlog_out(DEBUG) << "Total Number of Semi-Leptonic decays: " << m_nSLDecayTotal << std::endl;
	}
	catch (...)
     {
         streamlog_out(WARNING) << "Could not extract Semi-Leptonic decay " << std::endl;
     }
}
