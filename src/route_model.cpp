#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
  
  int counter = 0;
  for(Model::Node node : this->Nodes()){
    auto n = RouteModel::Node(counter++, this, node);
    m_Nodes.push_back(n);
  }
  CreateNodeToRoadHashmap();
}
  
void RouteModel::CreateNodeToRoadHashmap() {

  for (const Model::Road& road : Roads()){
    
    if(road.type != Model::Road::Type::Footway){
      for(int node_idx: Ways()[road.way].nodes){
        if(node_to_road.find(node_idx) == node_to_road.end()){
          
           node_to_road[node_idx] = std::vector<const Model::Road*>();
        }
        node_to_road[node_idx].push_back(&road);
      }
    }
  }
  return;
}

const RouteModel::Node* RouteModel::Node::FindNeighbors(std::vector<int> node_indices){
  //return nullptr;

  RouteModel::Node const* bestNode = nullptr;
  float bestDist = std::numeric_limits<float>::max();

  for(int index: node_indices){
    Node n = parent_model->SNodes()[index];
    float dist = RouteModel::Node::distance(n);
    if(!n.visited && dist != 0.0){
      if(dist < bestDist){
        bestDist = dist;
        bestNode = &parent_model->SNodes()[index]; 
      }
    }
  }
  return bestNode;
}

RouteModel::Node& RouteModel::FindClosestNode(float x, float y){

   RouteModel::Node n;
  n.x = x;
  n.y = y;

  float min_dist = std::numeric_limits<float>::max();
  int best_index = -1;

  for(auto road: Roads()){

    if(road.type != Model::Road::Type::Footway){

      for(int index : Ways()[road.way].nodes){
        float dist = n.distance(SNodes()[index]);

        if(dist < min_dist){
          min_dist = dist;
          best_index = index;
        }
      }
    }
  }
  return SNodes()[best_index];
}