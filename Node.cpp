//
//  Node.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/20/21.
//

#include "Node.hpp"

Node::Node(void) {
    ancestor = NULL;
    index = 0;
    isLeaf = false;
    name = "";
    offset = 0;
}

std::vector<Node*> Node::getDescendants(void) {
    
    std::vector<Node*> descendants;
    for (Node* n : neighbors) {
        if (n != ancestor)
            descendants.push_back(n);
    }
    return descendants;
}

std::vector<Node*> Node::getNeighborsVector(void) {
    
    std::vector<Node*> neighborsVector;
    
    for (Node* n : neighbors)
        neighborsVector.push_back(n);
    
    return neighborsVector;
}
