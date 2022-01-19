//
//  main.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/1/21.
//

#include <iostream>
#include "UserSettings.hpp"
#include "Model.hpp"
#include "MCMC.hpp"

int main(int argc, char * argv[]) {
    
    // Get the settings from the command line
    UserSettings& settings = UserSettings::userSettings(); 
    settings.initialize(argc, argv);
    settings.print();
    
    // Set up the phylogenetic model
    Model model;
    
    MCMC mcmc(&model);
    mcmc.run();
    
    std::cout << model.lnLikelihood() << std::endl;
    
    return 0;
}
