//
//  Branch.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/28/21.
//

#ifndef Branch_hpp
#define Branch_hpp

class Node;

class Branch {
    
public:
    Branch(void);
    Branch(Node* e1, Node* e2);
    Branch(Node* e1, Node* e2, double len);
    void setEnd1(Node* e1) {end1 = e1;}
    Node* getEnd1(void) {return end1;}
    void setEnd2(Node* e2) {end2 = e2;}
    Node* getEnd2(void) {return end2;}
    void setLength(double len) {length = len;}
    double getLength(void) {return length;}
    void setEnds(Node* e1, Node* e2);
    Node* getAncestor(void);
    Node* getDescendent(void);
    void clean(void);
    
private:
    Node* end1;
    Node* end2;
    double length;
    
};

#endif /* Branch_hpp */
