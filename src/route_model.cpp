#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
  
  int counter = 0;
  std::vector<Model::Node> modelNodes = this->Nodes();
  for(int i = 0; i < modelNodes.size(); ++i){
    m_Nodes.push_back(RouteModel::Node(counter, this, modelNodes[i]));
    counter++;
  }
  CreateNodeToRoadHashmap();
}
  
void RouteModel::CreateNodeToRoadHashmap() {

  for (const Model::Road& road : Roads()){
    
    if(road.type != Model::Road::Type::Footway){
      for(int node_idx : Ways()[road.way].nodes){

        if(node_to_road.find(node_idx) == node_to_road.end()){
           node_to_road[node_idx] = std::vector<const Model::Road*>();
        }
        node_to_road[node_idx].push_back(&road);
      }
    }
  }
  return;
}

RouteModel::Node* RouteModel::Node::FindNeighbor(std::vector<int> node_indices){

  RouteModel::Node* bestNode = nullptr;
  float bestDist = std::numeric_limits<float>::max();

  for(int index : node_indices){
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

void RouteModel::Node::FindNeighbors() {

  for(auto& road : parent_model->node_to_road[this->index]){
    RouteModel::Node* n = this->FindNeighbor(parent_model->Ways()[road->way].nodes);
    if (n) {
      this->neighbors.push_back(n);
    }
  }
}
