//
//  Node.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/20/21.
//

#ifndef Node_hpp
#define Node_hpp
#include <set>
#include <string>
#include <vector>

class Node {
    
public:
    Node(void);
    Node* getAncestor(void) {return ancestor;}
    int getIndex(void) {return index;}
    std::string getName(void) {return name;}
    bool getIsLeaf(void) {return isLeaf;}
    int getOffset(void) {return offset;}
    void setAncestor(Node* p) {ancestor = p;}
    void setIndex(int i) {index = i;}
    void setName(std::string n) {name = n;}
    void setIsLeaf(bool tf) {isLeaf = tf;}
    void setOffset(int o) {offset = o;}
    void addNeighbor(Node* p) {neighbors.insert(p);}
    void removeNeighbor(Node* p) {neighbors.erase(p);}
    void removeNeighbors(void) {neighbors.clear();}
    std::set<Node*>& getNeighbors(void) {return neighbors;}
    std::vector<Node*> getDescendants(void);
    std::vector<Node*> getNeighborsVector(void);
private:
    std::set<Node*> neighbors;
    Node* ancestor;
    int index;
    std::string name;
    bool isLeaf;
    int offset;
};


#endif /* Node_hpp */
