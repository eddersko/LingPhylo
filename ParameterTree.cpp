//
//  ParameterTree.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/28/21.
//

#include "ParameterTree.hpp"
#include "Tree.hpp"
#include "RandomVariable.hpp"

ParameterTree::ParameterTree(std::string n, std::vector<std::string> tn, double lam) {
    name = n;
    lambda = lam;
    trees[0] = new Tree(tn);
    trees[1] = new Tree(*trees[0]);
}

ParameterTree::ParameterTree(std::string n, std::string newickString, std::vector<std::string> tn, double lam) {
    name = n;
    lambda = lam;
    trees[0] = new Tree(newickString, tn, lambda);
    trees[1] = new Tree(*trees[0]);
}

ParameterTree::~ParameterTree(void) {
    delete trees[0];
    delete trees[1];
}

void ParameterTree::accept(void) {
    *trees[1] = *trees[0];
}

void ParameterTree::reject(void) {
    *trees[0] = *trees[1];
}

// still need to implement
double ParameterTree::lnProbability(void) {
    
    return 0.0;
}

double ParameterTree::update(void) {
    
    Tree* tree = trees[0];
    RandomVariable& rng = RandomVariable::randomVariableInstance();
    double u = rng.uniformRv();
    if (u < 0.1)
        return tree->updateBrLen();
    else
        return tree->updateLocal();
    
    return 0.0;
}

void ParameterTree::print(void) {    
    trees[0]->print();
}
