//
//  NclReader.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/13/21.
//

#ifndef NclReader_hpp
#define NclReader_hpp

#include <string>
#include "ncl.h"
#include "nxsmultiformat.h"

class CharMatrix;

class NclReader {
        
    public:
        CharMatrix* readMatrix(std::string fn);
        
    private:
        MultiFormatReader nexusReader;
    
};

#endif /* NclReader_hpp */
