#ifndef ROUTE_MODEL_H
#define ROUTE_MODEL_H

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>

class RouteModel : public Model {

  public:
    class Node : public Model::Node {
      public:

        Node *parent = nullptr;
        float h_value = std::numeric_limits<float>::max();
        float g_value = 0.0;
        bool visited = false;
        std::vector<Node *> neighbors;

        Node(){}
        Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}

        // euclidean distance between two points
        float distance(Node n) const {
          return std::sqrt(std::pow(x-n.x, 2.0)+ std::pow(y-n.y, 2.0));
        }

      void FindNeighbors();

      private:
        int index;
        RouteModel * parent_model = nullptr;

        Node* FindNeighbor(std::vector<int> node_indices);
    };
    
    std::vector<Node> path;

    RouteModel(const std::vector<std::byte> &xml);  
    auto &SNodes() noexcept { return m_Nodes; }
    auto &GetNodeToRoadMap() { return node_to_road; }
    RouteModel::Node &FindClosestNode(float x, float y);
  
  private:
  
    std::vector<Node> m_Nodes;
    std::unordered_map<int, std::vector<const Model::Road *>> node_to_road;
  
  	void CreateNodeToRoadHashmap();
};

#endif