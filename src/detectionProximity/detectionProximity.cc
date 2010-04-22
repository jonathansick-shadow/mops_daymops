// -*- LSST-C++ -*-
/* File: detectionProximity.cc
 * Author: Matthew Cleveland
 * Purpose: 
 */

#include "lsst/mops/daymops/detectionProximity/detectionProximity.h"


namespace lsst {
    namespace mops {

// prototypes not to be seen outside this file

void buildKDTree(const std::vector<Detection>, KDTree<unsigned int>&);

std::vector<std::pair<unsigned int, unsigned int> > 
getProximity(const std::vector<Detection>& queryPoints,
	     const KDTree<unsigned int>& searchTree, double,
	     double, double);



std::vector<std::pair <unsigned int, unsigned int> > detectionProximity(const std::vector<Detection>& queryPoints,
								   const std::vector<Detection>& dataPoints,
								   double distanceThreshold,
								   double brightnessThreshold,
								   double timeThreshold)
{
    std::vector<std::pair <unsigned int, unsigned int> > results;
    
    if(queryPoints.size() > 0 && dataPoints.size() > 0){
        
        //build KDTrees from detection vectors
        KDTree<unsigned int> dataTree;
        buildKDTree(dataPoints, dataTree);
        
        //get results
        results = getProximity(queryPoints, dataTree, distanceThreshold,
                               brightnessThreshold, timeThreshold);
    }

    return results;
}



/**********************************************************************
 * Populate a KDTree 'tree' from the Detections in vector 'points'
 ***********************************************************************/
void buildKDTree(const std::vector<Detection> points, KDTree<unsigned int> &tree)
{

  if(points.size() > 0){

    std::vector<PointAndValue<unsigned int> > vecPV;

    for(unsigned int i=0; i < points.size(); i++){
    
	PointAndValue<unsigned int> tempPV;
	std::vector<double> pairRADec;
	
	pairRADec.push_back(convertToStandardDegrees(points.at(i).getRA()));                    
	pairRADec.push_back(convertToStandardDegrees(points.at(i).getDec()));
        pairRADec.push_back(points.at(i).getMag());
        pairRADec.push_back(points.at(i).getEpochMJD());
	
	tempPV.setPoint(pairRADec);
	tempPV.setValue(i);
	vecPV.push_back(tempPV);
    }
    
    tree.buildFromData(vecPV, 4, 100);
  }
}
    
    

/*
 *
 */
std::vector<std::pair <unsigned int, unsigned int> > getProximity(const std::vector<Detection>& queryPoints,
								  const KDTree<unsigned int>& searchTree,
								  double maxDist, double maxBrightness,
								  double maxTime)
{
  std::vector<std::pair <unsigned int, unsigned int> > pairs;

  std::vector<GeometryType> myGeos;
  myGeos.push_back(RA_DEGREES); //RA
  myGeos.push_back(DEC_DEGREES); //Dec
  myGeos.push_back(EUCLIDEAN); //magnitude
  myGeos.push_back(EUCLIDEAN); //time

  
  for(unsigned int i=0; i<queryPoints.size(); i++){

      std::vector<double> RADecQueryPt;
      std::vector<double> otherDimsPt;
      std::vector<double> otherDimsTolerances;
      std::vector<PointAndValue<unsigned int> > queryResults;

      RADecQueryPt.push_back(convertToStandardDegrees(queryPoints.at(i).getRA()));
      RADecQueryPt.push_back(convertToStandardDegrees(queryPoints.at(i).getDec()));
    
      otherDimsPt.push_back(queryPoints.at(i).getMag());
      otherDimsPt.push_back(queryPoints.at(i).getEpochMJD());
      
      otherDimsTolerances.push_back(maxBrightness);
      otherDimsTolerances.push_back(maxTime);
      
      queryResults = searchTree.RADecRangeSearch(RADecQueryPt, maxDist, 
                                                 otherDimsPt, otherDimsTolerances, 
                                                 myGeos);
      
      for(unsigned int j=0; j<queryResults.size(); j++){
          pairs.push_back(std::make_pair(i, queryResults.at(j).getValue()));
      }
  }
  
  
  return pairs;
}


    }} // close lsst::mops
