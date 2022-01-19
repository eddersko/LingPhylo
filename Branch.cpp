//
//  Branch.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/28/21.
//

#include "Branch.hpp"
#include "Msg.hpp"
#include "Node.hpp"

Branch::Branch(void) {
    end1 = NULL;
    end2 = NULL;
    length = 0.0;
}

Branch::Branch(Node* e1, Node* e2) {
    setEnds(e1, e2);
    length = 0.0;
}

Branch::Branch(Node* e1, Node* e2, double len) {
    setEnds(e1, e2);
    length = len;
}

void Branch::setEnds(Node* e1, Node* e2) {
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

Node* Branch::getAncestor(void) {
    if (end1->getAncestor()==end2)
        return end2;
    else if (end2->getAncestor()==end1)
        return end1;
    return NULL;
}

Node* Branch::getDescendent(void) {
    if (end1->getAncestor()==end2)
        return end1;
    else if (end2->getAncestor()==end1)
        return end2;
    return NULL;
}

void Branch::clean(void) {
    end1 = NULL;
    end2 = NULL;
    length = 0.0;
}
