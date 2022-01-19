//
//  MCMC.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 11/11/21.
//

#ifndef MCMC_hpp
#define MCMC_hpp

class Model;

class MCMC {
    public:
        MCMC(void) = delete;
        MCMC(Model* m);
        void run(void);
    private:
        Model* mymodel;
        int chainLength;
        int printFreq;
        int sampleFreq;
};

#endif /* MCMC_hpp */
