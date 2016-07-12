/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/AndrzejTestAlgorithm.cc
 * 
 *  @brief  Implementation of the andrzejtest algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larpandoracontent/LArHelpers/LArClusterHelper.h"
#include "larpandoracontent/LArHelpers/LArMCParticleHelper.h"

#include "workshopcontent/Algorithms/AndrzejTestAlgorithm.h"

using namespace pandora;

namespace workshop_content
{

AndrzejTestAlgorithm::AndrzejTestAlgorithm() : 
    m_myMandatoryString(), 
    m_myOptionalBool(false), 
    m_myOptionalUnsignedInt(5), 
    m_myMandatoryFloatVector(),
    m_outputClusterListName(), 
    m_nHitsPerCluster(10)  
	{ 
	} 


StatusCode AndrzejTestAlgorithm::Run()
{
    // Algorithm code here
        std::cout << "-m_myMandatoryString: "  << m_myMandatoryString << std::endl 
              << "-m_myOptionalBool: "  << m_myOptionalBool << std::endl 
              << "-m_myOptionalUnsignedInt: " << m_myOptionalUnsignedInt << std::endl 
              << "-m_myMandatoryString: "; 
        for (const auto  value: m_myMandatoryFloatVector) 
             std::cout << value << " "; 


	const  CaloHitList *pCaloHitList(nullptr); 
	PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCaloHitList)); 


        const  CaloHitList *pAlgoCaloHitList(nullptr); 
	PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetList(*this,"CaloHitListW", pAlgoCaloHitList)); 

       //Create temporary list

        const  ClusterList *pTemporaryList(nullptr); 
	
        std::string temporaryListName; 

	PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::CreateTemporaryListAndSetCurrent(*this, pTemporaryList, temporaryListName)); 

        CaloHitVector sortedCaloHits(pCaloHitList->begin(), pCaloHitList->end()); 
    	std::sort(sortedCaloHits.begin(), sortedCaloHits.end(), lar_content::LArClusterHelper::SortHitsByPosition); 

        CaloHitVector sortedAlgoCaloHits(pAlgoCaloHitList->begin(), pAlgoCaloHitList->end()); 
    	std::sort(sortedAlgoCaloHits.begin(), sortedAlgoCaloHits.end(), lar_content::LArClusterHelper::SortHitsByPosition); 

        const Cluster *pCluster(nullptr); 

        //visualisation
	const bool showDetectorGaps(true); 

	PandoraMonitoringApi::SetEveDisplayParameters(this->GetPandora(), showDetectorGaps, DETECTOR_VIEW_XZ, -1.f, -1.f, 1.f); 
	PandoraMonitoringApi::VisualizeCaloHits(this->GetPandora(), pAlgoCaloHitList, "CurrentCaloHits", BLUE); 

	//for (const CaloHit * const pCaloHit : sortedCaloHits) 
    	//{ 
        //	std::cout << "InputHit - HitType: " << pCaloHit->GetHitType() << ", " << pCaloHit->GetPositionVector() << std::endl; 
    	//} 
	for (const CaloHit * const pCaloHit : sortedAlgoCaloHits) 
    	{ 
        	std::cout << "InputHit - HitType: " << pCaloHit->GetHitType() << ", " << pCaloHit->GetPositionVector() << std::endl; 
		if (!PandoraContentApi::IsAvailable(*this, pCaloHit)) continue; 

		if (!pCluster || (pCluster->GetNCaloHits() >= m_nHitsPerCluster)) 
		        { 
		            PandoraContentApi::Cluster::Parameters parameters; 
		            parameters.m_caloHitList.insert(pCaloHit); 
		            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Cluster::Create(*this, parameters, pCluster)); 
		        } 
		else
		        { 
		            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::AddToCluster(*this, pCluster, pCaloHit)); 
		        } 

    	} 

        // MCParticles
	const  MCParticleList *pMCParticleList(nullptr); 
	PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pMCParticleList)); 
  
        MCParticleVector sortedMCParticles(pMCParticleList->begin(), pMCParticleList->end()); 
    	std::sort(sortedMCParticles.begin(), sortedMCParticles.end(), lar_content::LArMCParticleHelper::SortBySource); 

	for
 	(const MCParticle * const pMCParticle : sortedMCParticles) 
	    { 
        	std::cout << "InputMCParticle - PDG: " << pMCParticle->GetParticleId() << ", nParents " << pMCParticle->GetParentList().size() 
                  << ", nDaughters " << pMCParticle->GetDaughterList().size() << std::endl; 
	    } 
        

        std::cout << std::endl; 

        PandoraMonitoringApi::VisualizeMCParticles(this->GetPandora(), pMCParticleList, "CurrentMCParticles", RED); 
	PandoraMonitoringApi::ViewEvent(this->GetPandora()); 

    if(!pTemporaryList->empty()) 
	    { 
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList<Cluster>(*this, m_outputClusterListName)); 
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ReplaceCurrentList<Cluster>(*this, m_outputClusterListName)); 
	    } 

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode AndrzejTestAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    // Read settings from xml file here
     PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,"MyMandatoryString",m_myMandatoryString)); 
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle, 
"MyOptionalBool", m_myOptionalBool)); 
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle, "MyOptionalUnsignedInt", m_myOptionalUnsignedInt)); 
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadVectorOfValues(xmlHandle, "MyMandatoryFloatVector", m_myMandatoryFloatVector)); 

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle, "OutputClusterListName", m_outputClusterListName)); 
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle, "NHitsPerCluster", m_nHitsPerCluster)); 


    return STATUS_CODE_SUCCESS;
}

} // namespace workshop_content
