/**
 *  @file   WorkshopContent/examplecontent/ExampleAlgorithms/WriteTreeAlgorithm.cc
 * 
 *  @brief  Implementation of the write tree algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "examplecontent/ExampleAlgorithms/WriteTreeAlgorithm.h"

#include "examplecontent/ExampleHelpers/ExampleHelper.h"

using namespace pandora;

namespace example_content
{

WriteTreeAlgorithm::~WriteTreeAlgorithm()
{
    // Tree is written to file when the algorithm is deleted (i.e. when the parent Pandora instance is deleted)
    PANDORA_MONITORING_API(SaveTree(this->GetPandora(), m_treeName, m_fileName, "UPDATE"));
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode WriteTreeAlgorithm::Run()
{
    // Create a simple root tree containing, for each event, the number of clusters in the current list and a vector
    // of all the cluster hadronic energy estimators.
    const ClusterList *pClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));

    int nClusters(0);
    FloatVector clusterEnergies;

    ClusterVector clusterVector(pClusterList->begin(), pClusterList->end());
    std::sort(clusterVector.begin(), clusterVector.end(), ExampleHelper::ExampleClusterSort);

    for (const Cluster *const pCluster : clusterVector)
    {
        clusterEnergies.push_back(pCluster->GetHadronicEnergy());
        ++nClusters;
    }

    PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName, "nClusters", nClusters));
    PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName, "clusterEnergies", &clusterEnergies));
    PANDORA_MONITORING_API(FillTree(this->GetPandora(), m_treeName));

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode WriteTreeAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "FileName", m_fileName));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "TreeName", m_treeName));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content
