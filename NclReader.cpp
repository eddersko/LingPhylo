//
//  NclReader.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/13/21.
//
#include <iostream>
#include "NclReader.hpp"
#include "CharMatrix.hpp"

CharMatrix* NclReader::readMatrix(std::string fn) {
    
    nexusReader.ReadFilepath(fn.c_str(), MultiFormatReader::NEXUS_FORMAT);
    int numTaxaBlocks = nexusReader.GetNumTaxaBlocks();
    std::cout << "numTaxaBlocks = " << numTaxaBlocks << std::endl;
    CharMatrix* cm = NULL;
    
    for (int tBlock=0; tBlock<numTaxaBlocks; tBlock++)
        {
        NxsTaxaBlock* taxaBlock = nexusReader.GetTaxaBlock(tBlock);
        int numCharBlocks = nexusReader.GetNumCharactersBlocks(taxaBlock);
        for (int cBlock=0; cBlock<numCharBlocks; cBlock++)
            {
            NxsCharactersBlock* charBlock = nexusReader.GetCharactersBlock(taxaBlock, cBlock);
            const NxsDiscreteDatatypeMapper* mapper = charBlock->GetDatatypeMapperForChar(0);
            int numTaxa = charBlock->GetNTax();
            int numChar = charBlock->GetNChar();
            int numStates = mapper->GetNumStates();
            std::string sym = charBlock->GetSymbols(); // get possible character states
                
            // allocate a matrix
            unsigned** m = new unsigned*[numTaxa];
            m[0] = new unsigned[numTaxa * numChar];
            for (int i=1; i<numTaxa; i++)
                m[i] = m[i-1] + numChar;
            for (int i=0; i<numTaxa; i++)
                for (int j=0; j<numChar; j++)
                    m[i][j] = 0;
                
                std::vector<std::string> taxonNames;
            // iterates each taxon
            for (int i=0; i<numTaxa; i++)
                {
                NxsString tLabel = charBlock->GetTaxonLabel(i);
                std::string taxonName = NxsString::GetEscaped(tLabel).c_str();
                taxonNames.push_back(taxonName);
                // iterates each character
                for (int j=0; j<numChar; j++)
                    {
                        if (charBlock->IsGapState(i,j) == true || charBlock->IsMissingState(i, j) == true)
                        {
                            for (int s=0; s<numStates; s++)
                            {
                                std::string st = "";
                                st += sym[s];
                                int x = atoi(st.c_str());
                                m[i][j] += (unsigned)pow(2.0,(double)x);
                            }
                        }
                        else
                        {
                            std::string st = std::string(1, charBlock->GetState(i, j, 0) );
                            int x = atoi(st.c_str());
                            m[i][j] = (unsigned)pow(2.0,(double)x);
                            
                            for (int s=1; s<charBlock->GetNumStates(i, j); s++)
                            {
                                st = std::string(1, charBlock->GetState(i, j, s) );
                                x = atoi(st.c_str());
                                m[i][j] += (unsigned)pow(2.0,(double)x);
                                
                            }
                            
                        }
                    }
                    

                    
                }
                                
                cm = new CharMatrix(numTaxa, numChar, m, sym, taxonNames);
                
            }
        }
    return cm;
}
