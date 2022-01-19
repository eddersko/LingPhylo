//
//  MCMC.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 11/11/21.
//

#include "MCMC.hpp"
#include "Model.hpp"
#include "RandomVariable.hpp"
#include <cmath>
#include <iostream>
#include <iomanip>
#include "UserSettings.hpp"

MCMC::MCMC(Model* m) {
    UserSettings& settings = UserSettings::userSettings();
    
    mymodel = m;
    chainLength = settings.getChainLength();
    printFreq = 1;
    sampleFreq = 10;
}

void MCMC::run(void) {
    
    RandomVariable& rng = RandomVariable::randomVariableInstance();
    double currLnL = mymodel->lnLikelihood();
    double currLnP = mymodel->lnPriorProbability();
    
    for (int n = 1; n <= chainLength; n++) {
        
        // propose a new state for the chain
        double lnProposalProbability = mymodel->update();
        
        // calculate the probability of accepting the proposed state
        double newLnL = mymodel->lnLikelihood();
        double lnLikelihoodRatio = newLnL - currLnL;
        double newLnP = mymodel->lnPriorProbability();
        double lnPriorRatio = newLnP - currLnP;
        double lnR = lnLikelihoodRatio + lnPriorRatio + lnProposalProbability;
        
        // accept or reject the proposed state
        bool accept = false;
        if (log(rng.uniformRv()) < lnR)
            accept = true;
        
        // print state to screen
        if (n % printFreq == 0) {
            std::cout << std::fixed << std::setprecision(2);
            std::cout << n << " -- " << currLnL << " -> " << newLnL << " " << accept << std::endl;
        }
        
        // update the parameters to reflect the (potentially) new state
        if (accept == true) {
            mymodel->accept();
            currLnP = newLnP;
            currLnL = newLnL;
        } else {
            mymodel->reject();
            
        }
        
    }
}
