//
//  TransitionProbabilities.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 11/10/21.
//

#ifndef TransitionProbabilities_hpp
#define TransitionProbabilities_hpp

#include <vector>

class Tree;

class TransitionProbabilities {

public:
    static TransitionProbabilities&  transitionProbabilitiesInstance(void)
                                {
                                static TransitionProbabilities trProbInstance;
                                return trProbInstance;
                                }
    TransitionProbabilities(int nb);
    ~TransitionProbabilities(void);
    void initialize(int nb);
    std::vector<double**>& getTransProbs(void) { return trProbs[activeVals]; }
    void setTransProbs(Tree* t);
    void print(void);
    void updateTransProb(int nodeIdx, double v);
    void flipActiveVal(void);
private:
    TransitionProbabilities(void);
    int numBranches;
    int activeVals;
    bool isInitialized;
    std::vector<double**> trProbs[2];
    std::vector<double> branchLengths;
};


#endif /* TransitionProbabilities_hpp */
