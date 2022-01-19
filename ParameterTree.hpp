//
//  ParameterTree.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/28/21.
//

#ifndef ParameterTree_hpp
#define ParameterTree_hpp
#include <string>
class Tree;

class ParameterTree {
    
public:
    ParameterTree(void) = delete;
    ParameterTree(std::string n, std::vector<std::string> tn, double lam);
    ParameterTree(std::string n, std::string newickString, std::vector<std::string> tn, double lam);
    ~ParameterTree(void);
    void accept(void);
    void reject(void);
    double lnProbability(void);
    double update(void);
    void print(void);
    Tree* getActiveTree(void) { return trees[0]; }
private:
    std::string name;
    double lambda;
    Tree* trees[2];
    
    
    
};

#endif /* ParameterTree_hpp */
