#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
  
  int counter = 0;
  for(Model::Node node : this->Nodes()){
    auto n = RouteModel::Node(counter++, this, node);
    m_Nodes.push_back(n);
  }
  
  void RouteModel::CreateNodeToRoadHashmap() {
    for(auto road: Roads()){

      if(road.type != Model::Road::Type::Footway){
       for(int node_idx: Ways()[road.way].nodes){
         
         if(node_to_road.find(node_idx) == node_to_road.end()){
            node_to_road[node_idx] = std::vector<const Model::Road *> lst;
         }
           node_to_road[node_idx].push_back(road);
       } 
      }
    }
    return;
  }
}