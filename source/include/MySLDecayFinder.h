#ifndef MySLDecayFinder_h
#define MySLDecayFinder_h 1

#include "EVENT/LCStrVec.h"
#include "IMPL/LCCollectionVec.h"
#include "marlin/Processor.h"
#include "lcio.h"
#include <string>
#include <vector>
#include <math.h>
//#include "CalibrationHelper.h"

#include <set>
#include <vector>

class TFile;
class TH1F;
class TTree;

using namespace lcio ;
using namespace marlin ;


/**  Example processor for marlin.
 *
 *  If compiled with MARLIN_USE_AIDA
 *  it creates a histogram (cloud) of the MCParticle energies.
 *
 *  <h4>Input - Prerequisites</h4>
 *  Needs the collection of MCParticles.
 *
 *  <h4>Output</h4>
 *  A histogram.
 *
 * @param CollectionName Name of the MCParticle collection
 *
 * @author F. Gaede, DESY
 * @version $Id: MySLDecayFinder.h,v 1.4 2005-10-11 12:57:39 gaede Exp $
 */

class MySLDecayFinder : public Processor {

 public:

  virtual Processor*  newProcessor() { return new MySLDecayFinder ; }


  MySLDecayFinder() ;

  /** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
  virtual void init() ;

  /** Called for every run.
   */
  virtual void processRunHeader( lcio::LCRunHeader *pLCRunHeader ) ;

  /** Called for every event - the working horse.
   */
  virtual void processEvent( EVENT::LCEvent *pLCEvent ) ;


  virtual void check( EVENT::LCEvent *pLCEvent ) ;

private:

  void Clear();

  void ExtractCollections(EVENT::LCEvent *pLCEvent);

  void FindSLDecays(EVENT::LCEvent *pLCEvent);

  /** Called after data processing for clean up.
   */
  virtual void end() ;


 protected:

  /** Input collection name.
   */
  std::string _colName{} ;

  int				m_nRun{} ;
  int				m_nEvt{} ;
  int				m_nRunSum{};
  int				m_nEvtSum{};

  std::string			m_mcParticleCollection{};
  std::string		    m_outcolSLDecays{};
  LCCollectionVec*      m_col_SLDecays{};

  int				m_printing;
  std::string         m_rootFile{};                           ///<
  int				m_lookForQuarksWithMotherZ;
/*
  ParticleVector		m_pfoVector{};
  MCParticleVector		m_pfoTargetVector{};
*/
  int				m_nPfosTotal;                           ///<
  int				m_nPfosNeutralHadrons;                  ///<
  int				m_nPfosPhotons;                         ///<
  int				m_nPfosTracks;                          ///<
  float				m_pfoEnergyTotal;                       ///<
  float				m_pfoEnergyNeutralHadrons;              ///<
  float				m_pfoEnergyPhotons;                     ///<
  float				m_pfoEnergyTracks;                      ///<
  float				m_pfoMassTotal;                         ///<
  std::vector<int>		m_pfoPdgCodes{};                        ///<

  int				m_nMcpsTotal;                           ///<
  int				m_nMcpsNeutralHadrons;                  ///<
  int				m_nMcpsPhotons;                         ///<
  int				m_nMcpsTracks;                          ///<
  float				m_mcpEnergyTotal;                       ///<
  float				m_mcpEnergyNeutralHadrons;              ///<
  float				m_mcpEnergyPhotons;                     ///<
  float				m_mcpEnergyTracks;                      ///<
  float				m_mcpMassTotal;                         ///<
  std::vector<int>		m_mcpPdgCodes{};                        ///<

  float				m_mcEnergyENu;                          ///<
  float				m_mcEnergyFwd;                          ///<
  float				m_eQQ;                                  ///<
  float				m_eQ1;                                  ///<
  float				m_eQ2;                                  ///<
  float				m_costQQ;                               ///<
  float				m_costQ1;                               ///<
  float				m_costQ2;                               ///<
  float				m_mQQ;                                  ///<
  float				m_thrust;                               ///<
  int				m_qPdg;                                 ///<

  int				m_nSLDecayTotal;                           ///<
  int				m_nSLDecayBHad;                           ///<
  int				m_nSLDecayCHad;                           ///<

  TFile              *m_pTFile{};                             ///<
  TTree              *m_pTTree{};                             ///<
  TH1F               *m_hPfoEnergySum{};                      ///<
  TH1F               *m_hPfoEnergySumL7A{};                   ///<


} ;

#endif
