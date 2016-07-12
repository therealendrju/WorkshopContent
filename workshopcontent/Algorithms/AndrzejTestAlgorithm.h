/**
 *  @file   WorkshopContent/workshopcontent/Algorithms/AndrzejTestAlgorithm.h
 * 
 *  @brief  Header file for the andrzejtest algorithm class.
 * 
 *  $Log: $
 */
#ifndef WORKSHOP_ANDRZEJTEST_ALGORITHM_H
#define WORKSHOP_ANDRZEJTEST_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace workshop_content
{

/**
 *  @brief  AndrzejTestAlgorithm class
 */
class AndrzejTestAlgorithm : public pandora::Algorithm
{
public:
    /**
     *  @brief  Factory class for instantiating algorithm
     */
    class Factory : public pandora::AlgorithmFactory
    {
    public:
        pandora::Algorithm *CreateAlgorithm() const;
    };
    /**
    * @brief Default Constructor
    */
    AndrzejTestAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    // Member variables here
    std::string            m_myMandatoryString;  ///< A mandatory string	
    bool                   m_myOptionalBool;     ///< An optional Boolean
    unsigned int          m_myOptionalUnsignedInt;    ///< An optional unsigned int
    pandora::FloatVector    m_myMandatoryFloatVector;   ///< A mandatory vector of floats
    std::string     m_outputClusterListName;    ///< The output cluster list name
    unsigned int     m_nHitsPerCluster;          ///< The number of hits to add to each dummy cluster
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *AndrzejTestAlgorithm::Factory::CreateAlgorithm() const
{
    return new AndrzejTestAlgorithm();
}

} // namespace workshop_content

#endif // #ifndef WORKSHOP_ANDRZEJTEST_ALGORITHM_H
