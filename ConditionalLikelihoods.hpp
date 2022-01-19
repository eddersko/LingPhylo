//
//  ConditionalLikelihoods.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 11/4/21.
//

#ifndef ConditionalLikelihoods_hpp
#define ConditionalLikelihoods_hpp

class CharMatrix;

class ConditionalLikelihoods {
    
    public:
        ConditionalLikelihoods(void) = delete;
        ConditionalLikelihoods(CharMatrix* cm);
        ~ConditionalLikelihoods(void);
        double* getCls(int ndeIdx) { return cls[activeVal][ndeIdx]; }
        void print(void);
        int getNumChar(void) { return numChar; }
    private:
        double* rawCls;
        double** cls[2];
        int activeVal;
        int numNodes;
        int numTaxa;
        int numChar;
        int numStates;
};

#endif /* ConditionalLikelihoods_hpp */
