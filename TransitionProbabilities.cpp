//
//  TransitionProbabilities.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 11/10/21.
//

#include "TransitionProbabilities.hpp"
#include "Tree.hpp"
#include "Node.hpp"
#include "Branch.hpp"
#include <cmath>
#include <iostream>
#include <iomanip>

TransitionProbabilities::TransitionProbabilities(void) {
    isInitialized = false;
}

void TransitionProbabilities::initialize(int nb) {
    if (isInitialized == true)
        return;
    numBranches = nb;
    activeVals = 0;
    for (int a = 0; a < 2; a++) {
        trProbs[a].resize(numBranches);
        branchLengths.resize(numBranches);
        for (int b = 0; b < numBranches; b++) {
            branchLengths[b] = 0.0;
            trProbs[a][b] = new double*[2];
            trProbs[a][b][0] = new double[4];
            trProbs[a][b][1] = trProbs[a][b][0] + 2;
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    trProbs[a][b][i][j] = 0.0;
        }
    }
    isInitialized = true;
}

TransitionProbabilities::~TransitionProbabilities(void) {
    for (int a = 0; a < 2; a ++) {
        for (int b = 0; b < numBranches; b++) {
            delete [] trProbs[a][b][0];
            delete [] trProbs[a][b];
        }
    }
}

void TransitionProbabilities::setTransProbs(Tree* t) {
    std::vector<Node*>& dpseq = t->getDownPassSequence();
    for (int n = 0; n < dpseq.size(); n++) {
        Node* p = dpseq[n];
        Node* pAncs = dpseq[n]->getAncestor();
        if (pAncs != NULL) {
            Branch* b = t->findBranch(p, pAncs);
            double v = b->getLength();
            branchLengths[p->getIndex()] = v;
            
            double pChange =   0.5 - 0.5*exp(-2.0*v);
            double pNoChange = 0.5 + 0.5*exp(-2.0*v);
            
            for (int i = 0; i < 2; i ++) {
                for (int j = 0; j < 2; j ++) {
                    if (i == j)
                        trProbs[activeVals][p->getIndex()][i][j] = pNoChange;
                    else
                        trProbs[activeVals][p->getIndex()][i][j] = pChange;
                }
            }
        }
    }
}

void TransitionProbabilities::print(void) {
    
    std::cout << std::fixed << std::setprecision(8);
    
    for (int n = 0; n < numBranches; n++) {
        std::cout << "Branch(" << n << ","  << branchLengths[n] << ")" << std::endl;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++)
                std::cout << trProbs[0][n][i][j] << " ";
            for (int j = 0; j < 2; j++)
                std::cout << trProbs[1][n][i][j] << " ";
            std::cout << std::endl;
        }
    }
}
 
void TransitionProbabilities::updateTransProb(int nodeIdx, double v) {
    
    branchLengths[nodeIdx] = v;
    
    double pChange =   0.5 - 0.5*exp(-2.0*v);
    double pNoChange = 0.5 + 0.5*exp(-2.0*v);
    
    for (int i = 0; i < 2; i ++) {
        for (int j = 0; j < 2; j ++) {
            if (i == j)
                trProbs[activeVals][nodeIdx][i][j] = pNoChange;
            else
                trProbs[activeVals][nodeIdx][i][j] = pChange;
        }
    }
    
    
}

void TransitionProbabilities::flipActiveVal(void) {
    if (activeVals == 0)
        activeVals = 1;
    else
        activeVals = 0;
}
