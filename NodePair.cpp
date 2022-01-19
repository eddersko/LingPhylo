//
//  NodePair.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/28/21.
//

#include "NodePair.hpp"
#include <iostream>
#include "Msg.hpp"

NodePair::NodePair(void) {
    end1 = NULL;
    end2 = NULL;
}

NodePair::NodePair(Node* e1, Node* e2) {
    setEnds(e1, e2);
}

void NodePair::setEnds(Node* e1, Node* e2) {
    if (e1 == NULL || e2 == NULL)
        Msg::error("You need two defined nodes!");
    if (e1 == e2)
        Msg::error("You need two different nodes!");
    
    if (e1 < e2) {
        end1 = e1;
        end2 = e2;
    } else {
        end1 = e2;
        end2 = e1;
    }
}
