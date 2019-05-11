#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
	start_x *= 0.01;
  	start_y *= 0.01;
  	end_x *= 0.01;
  	end_y *= 0.01;
	start_node = &m_Model.FindClosestNode(start_x, start_y);
	end_node = &m_Model.FindClosestNode(end_x, end_y);
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node* current_node) {

	std::vector<RouteModel::Node> path;

	while(current_node != nullptr){
		path.push_back(*current_node);

		this->distance += current_node->distance(*(current_node->parent));
		current_node = current_node->parent;
	}
	m_Model.MetricScale();
	return path;
}

void RoutePlanner::AStarSearch() {
	start_node->visited = true;
	open_list.push_back(start_node);

	RouteModel::Node* current_node = nullptr;

	while(!open_list.empty()){
		current_node = NextNode();

		if (current_node->distance(*end_node) == 0.0) {
			m_Model.path = ConstructFinalPath(current_node);
			return;
		}
		AddNeighbors(current_node);
	}
}

float RoutePlanner::CalculateHValue(const RouteModel::Node* node) {
	return node->distance(*end_node);
}


RouteModel::Node* RoutePlanner::NextNode() {

	std::sort(open_list.begin(), open_list.end(), [](const auto& a, const auto& b){
		return a->h_value + b->g_value < a->h_value + b->g_value; 
	}); 

	RouteModel::Node* node = open_list.front();
	open_list.erase(open_list.begin());
	
	return node;

}

void RoutePlanner::AddNeighbors(RouteModel::Node* node){
	node->FindNeighbors();

	for(auto neighbor: node->neighbors){
		
		neighbor->parent = node;
		neighbor->g_value = node->g_value + node->distance(*neighbor);
		neighbor->h_value = CalculateHValue(node);
		neighbor->visited = true;
		
		this->open_list.push_back(neighbor);
	}

}