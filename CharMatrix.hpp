//
//  CharMatrix.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/15/21.
//

#ifndef CharMatrix_hpp
#define CharMatrix_hpp

#include <vector>
#include <string>

class CharMatrix {
    
    public:
        CharMatrix(int nt, int nc, unsigned** m, std::string s, std::vector<std::string> tn);
        ~CharMatrix(void);
        void print();
        std::vector<std::string> getTaxonNames(void) {return taxonNames;}
        int getNumTaxa(void) {return numTaxa;}
        int getNumChar(void) {return numChar;}
        unsigned getEntry(int taxonIdx, int charIdx) { return matrix[taxonIdx][charIdx]; }
    private:
        int numTaxa;
        int numChar;
        unsigned** matrix;
        std::string symbols;
        std::vector<std::string> taxonNames;
        
};

#endif /* CharMatrix_hpp */
