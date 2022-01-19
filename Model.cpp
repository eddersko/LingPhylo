//
//  Model.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 11/3/21.
//

#include "Msg.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Tree.hpp"
#include <vector>
#include "CharMatrix.hpp"
#include "NclReader.hpp"
#include "UserSettings.hpp"
#include "ParameterTree.hpp"
#include "ConditionalLikelihoods.hpp"
#include "TransitionProbabilities.hpp"
#include "RandomVariable.hpp"

Model::Model(void) {
    
    // Read the data
    UserSettings& settings = UserSettings::userSettings();
    NclReader reader;
    data = reader.readMatrix(settings.getFileName());
    data->print();
    
    // Set up tree
    //tree = new ParameterTree("tree", data->getTaxonNames(), 10.0);
    tree = new ParameterTree("tree", "(((((Lakota,Dakota),Assiniboine),Stoney),Mandan),Hidatsa,Crow);", data->getTaxonNames(), 10.0);
    tree->print();
    
    // Set up conditional likelihoods
    condlikes = new ConditionalLikelihoods(data);
    condlikes->print();
    
    // Set up transition probabilities
    TransitionProbabilities& tp = TransitionProbabilities::transitionProbabilitiesInstance();
    tp.initialize(data->getNumTaxa()*2-1);
    tp.setTransProbs(tree->getActiveTree());
    tp.print();
}

Model::~Model(void) {
    delete data;
    delete tree;
    delete condlikes;
}

double Model::lnLikelihood(void) {

# if 1
    
    Tree* t = tree->getActiveTree();
    std::vector<Node*>& dpSeq = t->getDownPassSequence();
    TransitionProbabilities& tpR = TransitionProbabilities::transitionProbabilitiesInstance();
    std::vector<double**>& tp = tpR.getTransProbs();
    int numChar = condlikes->getNumChar();
    
    for (int n = 0; n < dpSeq.size(); n++) {
        Node* p = dpSeq[n];
        if (p->getIsLeaf() == false) {
            std::vector<Node*> descendants = p->getDescendants();
            if (descendants.size() == 2) {
                double* clL = condlikes->getCls(descendants[0]->getIndex());
                double* clR = condlikes->getCls(descendants[1]->getIndex());
                double* clP = condlikes->getCls(p->getIndex());
                double** tpL = tp[descendants[0]->getIndex()];
                double** tpR = tp[descendants[1]->getIndex()];
                
                for (int c = 0; c < numChar; c++) {
                    for (int i = 0; i < 2; i ++) {
                        double sumL = 0.0, sumR = 0.0;
                        for (int j = 0; j < 2; j++) {
                            sumL += clL[j] * tpL[i][j];
                            sumR += clR[j] * tpR[i][j];
                        }
                        clP[i] = sumL * sumR;
                    }
                    clP += 2;
                    clL += 2;
                    clR += 2;
                }
                
            } else if (descendants.size() == 3) {
                
                double* cl0 = condlikes->getCls(descendants[0]->getIndex());
                double* cl1 = condlikes->getCls(descendants[1]->getIndex());
                double* cl2 = condlikes->getCls(descendants[2]->getIndex());
                double* clP = condlikes->getCls(p->getIndex());
                double** tp0 = tp[descendants[0]->getIndex()];
                double** tp1 = tp[descendants[1]->getIndex()];
                double** tp2 = tp[descendants[2]->getIndex()];
                
                for (int c = 0; c < numChar; c++) {
                    for (int i = 0; i < 2; i ++) {
                        double sum0 = 0.0, sum1 = 0.0, sum2 = 0.0;
                        for (int j = 0; j < 2; j++) {
                            sum0 += cl0[j] * tp0[i][j];
                            sum1 += cl1[j] * tp1[i][j];
                            sum2 += cl2[j] * tp2[i][j];
                        }
                        clP[i] = sum0 * sum1 * sum2;
                    }
                    clP += 2;
                    cl0 += 2;
                    cl1 += 2;
                    cl2 += 2;
                }
                
            } else {
                Msg::error("Expecting two or three descendents.");
            }
        }
    }
    
    // Average probabilities at root
    double* clR = condlikes->getCls(t->getRoot()->getIndex());
    double lnL = 0.0;
    for (int c = 0; c < numChar; c++) {
        double prob = 0.0;
        for (int i = 0; i < 2; i++)
            prob += clR[i]*0.5;
        lnL += log(prob);
    }
    
    return lnL;
    
# else
    
    Tree* t = tree->getActiveTree();
    std::vector<Node*>& dpSeq = t->getDownPassSequence();
    TransitionProbabilities& tpR = TransitionProbabilities::transitionProbabilitiesInstance();
    std::vector<double**>& tp = tpR.getTransProbs();
    int numChar = condlikes->getNumChar();
        
    // Felsenstein pruning algorithm moving probabilities from tips to root
    for (int n = 0; n < dpSeq.size(); n++) {
        Node* p = dpSeq[n];
        if (p->getIsLeaf() == false) {
            std::vector<Node*> descendants = p->getDescendants();
            std::vector<double*> clD(descendants.size());
            std::vector<double**> tpD(descendants.size());
            for (int i = 0; i < descendants.size(); i++) {
                clD[i] = condlikes->getCls(descendants[i]->getIndex());
                tpD[i] = tp[descendants[i]->getIndex()];
            }
            double* clP = condlikes->getCls(p->getIndex());
            
            for (int c = 0; c < numChar; c++) {
                for (int i = 0; i < 2; i++)
                    clP[i] = 1.0;
                for (int d = 0; d < descendants.size(); d++) {
                    for (int i = 0; i < 2; i ++) {
                        double sum = 0.0;
                        for (int j = 0; j < 2; j++)
                            sum += clD[d][j]*tpD[d][i][j];
                        clP[i] *= sum;
                        clD[d] += 2;
                    }
                }
                clP += 2;
            }
        }
    }
    
    // Average probabilities at root
    double* clR = condlikes->getCls(t->getRoot()->getIndex());
    double lnL = 0.0;
    for (int c = 0; c < numChar; c++) {
        double prob = 0.0;
        for (int i = 0; i < 2; i++)
            prob += clR[i]*0.5;
        lnL += log(prob);
    }
    
    return lnL;

# endif
}

double Model::update(void) {
    return tree->update();
}

void Model::accept(void) {
    tree->accept();
}

void Model::reject(void) {
    tree->reject();
    TransitionProbabilities& tp = TransitionProbabilities::transitionProbabilitiesInstance();
    tp.flipActiveVal();
}

double Model::lnPriorProbability(void) {
    return tree->getActiveTree()->lnProbability();
}
