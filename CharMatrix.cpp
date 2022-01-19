//
//  CharMatrix.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/15/21.
//

#include "CharMatrix.hpp"
#include <iostream>

CharMatrix::CharMatrix(int nt, int nc, unsigned** m, std::string s, std::vector<std::string> tn) {
    
    numTaxa = nt;
    numChar = nc;
    matrix = m;
    symbols = s;
    taxonNames = tn;
    
}

CharMatrix::~CharMatrix(void) {
    delete [] matrix;
    delete [] matrix[0];
}

//void CharMatrix::print(void) {
    
//    for (int i = 0; i < numTaxa; i++) {
        
//      std::cout << taxonNames[i] << "  ";
        
//      for (int j = 0; j < numChar; j++) {
//          std::cout << matrix[i][j];
//      }
//        std::cout << std::endl;
//    }
    
//}

void CharMatrix::print(void) {
    
    for (int i = 0; i < numChar; i++) {
        
        std::cout << i << " -- ";
        
        for (int j = 0; j < numTaxa; j++) {
            std::cout << matrix[j][i];
        }
        std::cout << std::endl;
        
    }
}
