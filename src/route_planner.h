#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"


class RoutePlanner {
  public:
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
	
    float GetDistance() { return distance; }  
    std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node* current_node);
    void AStarSearch();
    
  private:

    RouteModel &m_Model;
    RouteModel::Node* start_node;
	  RouteModel::Node* end_node;
  	float distance;
    std::vector<RouteModel::Node*> open_list;

    RouteModel::Node* NextNode();
    float CalculateHValue(const RouteModel::Node* node);
    void AddNeighbors(RouteModel::Node* node);
};

#endif
