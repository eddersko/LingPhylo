//
//  Tree.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/20/21.
//

#ifndef Tree_hpp
#define Tree_hpp
#include <vector>
#include <string>
#include <map>
#include "NodePair.hpp"

class RandomVariable;
class Node;
class Branch;

class Tree {
    
public:
    Tree(void) = delete;
    Tree(const Tree& t);
    Tree(std::vector<std::string> tn);
    Tree(double lambda, double mu, double t);
    Tree(std::string newickString, std::vector<std::string> tn, double lam);
    ~Tree(void);
    Tree& operator=(const Tree& t);
    void initializeDownPassSequence(void);
    void print(void);
    void print(std::string header);
    Branch* findBranch(Node* e1, Node* e2);
    const std::map<NodePair, Branch*, CompNodePair>& getBranches(void) const { return branches; }
    std::vector<Node*>& getDownPassSequence(void) { return downPassSequence; }
    Node* getRoot(void) { return root; }
    double updateBrLen(void);
    double updateLocal(void);
    double lnProbability(void);
private:
    std::vector<Node*> nodes;
    std::vector<std::string> taxonNames;
    std::vector<Node*> downPassSequence;
    Node* root;
    Node* addNode(void);
    void passDown(Node* p, Node* from);
    void listNode(Node* p, int indent);
    void clone(const Tree& t);
    void deleteAllNodes(void);
    std::map<NodePair, Branch*, CompNodePair> branches;
    NodePair key;
    Branch* addBranch(Node* e1, Node* e2);
    Branch* addBranch(Node* e1, Node* e2, double len);
    void removeBranch(Branch* b);
    void removeBranch(Node* e1, Node* e2);
    void removeAllBranches(void);
    void chooseBackbone(RandomVariable* rng, std::vector<Node*>& backboneNodes, std::vector<Branch*>& backboneBranches);
    std::vector<std::string> tokenize(std::string str);
};


#endif /* Tree_hpp */
