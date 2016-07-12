/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/AndrzejTestAlgorithm.cc
 * 
 *  @brief  Implementation of the andrzejtest algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "larpandoracontent/LArHelpers/LArClusterHelper.h"

#include "workshopcontent/Algorithms/AndrzejTestAlgorithm.h"

using namespace pandora;

namespace workshop_content
{

AndrzejTestAlgorithm::AndrzejTestAlgorithm() : 
    m_myMandatoryString(), 
    m_myOptionalBool(false), 
    m_myOptionalUnsignedInt(5), 
    m_myMandatoryFloatVector() 
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
	PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetList(*this,"CaloHitList2D", pAlgoCaloHitList)); 

        CaloHitVector sortedCaloHits(pCaloHitList->begin(), pCaloHitList->end()); 
    	std::sort(sortedCaloHits.begin(), sortedCaloHits.end(), lar_content::LArClusterHelper::SortHitsByPosition); 

        CaloHitVector sortedAlgoCaloHits(pAlgoCaloHitList->begin(), pAlgoCaloHitList->end()); 
    	std::sort(sortedAlgoCaloHits.begin(), sortedAlgoCaloHits.end(), lar_content::LArClusterHelper::SortHitsByPosition); 

	//for (const CaloHit * const pCaloHit : sortedCaloHits) 
    	//{ 
        //	std::cout << "InputHit - HitType: " << pCaloHit->GetHitType() << ", " << pCaloHit->GetPositionVector() << std::endl; 
    	//} 
	for (const CaloHit * const pCaloHit : sortedAlgoCaloHits) 
    	{ 
        	std::cout << "InputHit - HitType: " << pCaloHit->GetHitType() << ", " << pCaloHit->GetPositionVector() << std::endl; 
    	} 


        std::cout << std::endl; 


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



    return STATUS_CODE_SUCCESS;
}

} // namespace workshop_content
