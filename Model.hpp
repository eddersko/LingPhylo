//
//  Model.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 11/3/21.
//

#ifndef Model_hpp
#define Model_hpp

class CharMatrix;
class ParameterTree;
class ConditionalLikelihoods;

class Model {
    
    public:
        Model(void);
        ~Model(void);
        double lnLikelihood(void);
        double lnPriorProbability(void);
        double update(void);
        void accept(void);
        void reject(void);
    private:
        CharMatrix* data;
        ParameterTree* tree;
        ConditionalLikelihoods* condlikes;
};

#endif /* Model_hpp */

