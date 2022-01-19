//
//  NodePair.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/28/21.
//

#ifndef NodePair_hpp
#define NodePair_hpp
class Node;
class NodePair {
public:
    NodePair(void);
    NodePair(Node* e1, Node* e2);
    Node* getEnd1(void) const {return end1;}
    Node* getEnd2(void) const {return end2;}
    void setEnds(Node* e1, Node* e2);
    
private:
    Node* end1;
    Node* end2;
};

struct CompNodePair {
    bool operator()(const NodePair& np1, const NodePair& np2) const {
        
        if (np1.getEnd1() < np2.getEnd1())
            return true;
        else if (np1.getEnd1() == np2.getEnd1()) {
            if (np1.getEnd2() < np2.getEnd2())
                return true;
        }
        
        return false;
    }
    
    
};

#endif /* NodePair_hpp */
