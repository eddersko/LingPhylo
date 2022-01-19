//
//  ConditionalLikelihoods.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 11/4/21.
//

#include "ConditionalLikelihoods.hpp"
#include "CharMatrix.hpp"
#include <cmath>
#include <iostream>
#include <iomanip>

ConditionalLikelihoods::ConditionalLikelihoods(CharMatrix* cm) {
    // initialize variables
    numTaxa = cm->getNumTaxa();
    numChar = cm->getNumChar();
    numNodes = 2*numTaxa-2;
    numStates = 2;
    activeVal = 0;
    
    // allocate array holding conditional probabilities
    int clsSize = numChar*numNodes*numStates*2;
    rawCls = new double[clsSize];
    for (int i = 0; i < clsSize; i++)
        rawCls[i] = 0.0;
    
    // allocate a matrix to be preposition ourselves an array above
    double* p = &rawCls[0];
    for (int i = 0; i < 2; i++) {
        cls[i] = new double*[numNodes];
        for (int j = 0; j < numNodes; j++) {
            cls[i][j] = p;
            p += numStates * numChar;
        }
    }
    
    // initialize tip conditional likelihoods
    for (int i = 0; i < numTaxa; i++) {
        
        double* p0 = cls[0][i];
        double* p1 = cls[1][i];
        
        for (int j = 0; j < numChar; j++) {
            unsigned charCode = cm->getEntry(i, j);
            unsigned mask = 1;
            for (int k = 0; k < numStates; k++) {
                if ((mask & charCode) != 0) {
                    p0[k] = 1.0;
                    p1[k] = 1.0;
                }
                mask = mask << 1;
            }
            p0 += numStates;
            p1 += numStates;
        }
    }
}

ConditionalLikelihoods::~ConditionalLikelihoods(void) {
    delete [] rawCls;
    for (int i = 0; i < 2; i++)
        delete [] cls[i];
}

void ConditionalLikelihoods::print(void) {
    
    std::cout << std::fixed << std::setprecision(0);
    
    for (int j = 0; j < numChar; j++) {
        
        std::cout << j << " -- ";
        for (int i = 0; i < numTaxa; i++) {
            
            double* p0 = cls[0][i];
            p0 += numStates * j;
            for (int s = 0; s < numStates; s++)
                std::cout << p0[s];
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}
