//
//  Tree.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/20/21.
//

#include "Tree.hpp"
#include "Node.hpp"
#include "Branch.hpp"
#include "Probability.hpp"
#include "RandomVariable.hpp"
#include "BranchFactory.hpp"
#include "Msg.hpp"
#include "TransitionProbabilities.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

#undef DEBUG_LOCAL

Tree::Tree(const Tree& t) {
    clone(t);
}

Tree& Tree::operator=(const Tree& t) {
    if (this != &t)
        clone(t);
    return *this;
}

Tree::Tree(std::vector<std::string> tn) {
    RandomVariable& rng = RandomVariable::randomVariableInstance();
    taxonNames = tn;
    // three species tree
    root = addNode();
    for (int i = 0; i < 3; i++) {
        Node* p = addNode();
        p->setIndex(i);
        p->setName(taxonNames[i]);
        p->setIsLeaf(true);
        root->addNeighbor(p);
        p->addNeighbor(root);
        p->setAncestor(root);
    }
    
    // add remaining species/tips randomly
    for (int i = 3; i < taxonNames.size(); i++) {
        Node* p = NULL;
        do {
            p = nodes[(int)(rng.uniformRv()*nodes.size())];
            //would rng.uniformRV() ever be 1? if so, would it not go out of bounds?
        } while(p == root); // redo loop if it randomly chooses root (i.e. 0)
        Node* pAnc = p->getAncestor();
        
        Node* newTip = addNode();
        newTip->setIndex(i);
        newTip->setName(taxonNames[i]);
        newTip->setIsLeaf(true);
        Node* newInt = addNode();
        
        p->removeNeighbor(pAnc);
        pAnc->removeNeighbor(p);
        
        p->addNeighbor(newInt);
        newInt->addNeighbor(p);
        
        pAnc->addNeighbor(newInt);
        newInt->addNeighbor(pAnc);
        
        newTip->addNeighbor(newInt);
        newInt->addNeighbor(newTip);
        
        p->setAncestor(newInt);
        newInt->setAncestor(pAnc);
        newTip->setAncestor(newInt);
    }
    
    // get the pre-order traversal sequence
    initializeDownPassSequence();
        
    // index interior nodes from n-1 ... to 2n-2
    int intIdx = (int)taxonNames.size();
    for (int i = 0; i < downPassSequence.size(); i++) {
        Node* p = downPassSequence[i];
        if (p->getIsLeaf() == false) {
            p->setIndex(intIdx++);
        }
    }
    
    // add branches to the tree
    for (int i = 0; i < downPassSequence.size(); i++) {
        Node* p = downPassSequence[i];
        Node* pAnc = p->getAncestor();
        if (pAnc != NULL){
            double v = Probability::Exponential::rv(&rng, 10.0);
            Branch* b = addBranch(p, pAnc, v);
        }
    }
}

Tree::Tree(double lambda, double mu, double t) {
    RandomVariable& rng = RandomVariable::randomVariableInstance();
    root = addNode();
    Node* p = addNode();
    p->addNeighbor(root);
    root->addNeighbor(p);
    p->setAncestor(root);
    std::vector<Node*> activeNodes;
    activeNodes.push_back(p);
    
    double currT = 0.0;
    while(currT < t && activeNodes.size() > 0) {
        double rate = (lambda+mu)*activeNodes.size();
        currT += Probability::Exponential::rv(&rng, rate);
        if (currT < t) {
            int whichNode = (int)(rng.uniformRv()*activeNodes.size());
            p = activeNodes[whichNode];
            if (rng.uniformRv() < lambda/(lambda+mu)) {
                Node* newOne = addNode();
                Node* newTwo = addNode();
                newOne->addNeighbor(p);
                newTwo->addNeighbor(p);
                p->addNeighbor(newOne);
                p->addNeighbor(newTwo);
                newOne->setAncestor(p);
                newTwo->setAncestor(p);
                activeNodes[whichNode] = newOne;
                activeNodes.push_back(newTwo);
            } else {
                activeNodes[whichNode] = activeNodes[activeNodes.size()-1];
                activeNodes.pop_back();
            }
        }
    }
    
    
}

Tree::Tree(std::string newickString, std::vector<std::string> tn, double lam) {
    std::vector<std::string> tokens = tokenize(newickString);
    for (int i = 0; i < tokens.size(); i ++)
        std::cout << tokens[i] << std::endl;
    
    Node* p = NULL;
    
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "(") {
            
            Node* newNode = addNode();
            if (p == NULL)
                root = newNode;
            else {
                p->addNeighbor(newNode);
                newNode->addNeighbor(p);
                newNode->setAncestor(p);
            }
            p = newNode;
            
        } else if (tokens[i] == "," || tokens[i] == ")") {
            
            if (p->getAncestor() == NULL)
                Msg::error("Expecting ancestor to be there!");
            
            p = p->getAncestor();
                 
        } else if (tokens[i] == ";") {
            
            if (p != root)
                Msg::error("Expecting to end at the root!");
        
        } else {
            
            Node* newNode = addNode();
            newNode->setIsLeaf(true);
            std::string tName = "";
            int j = 0;
            for (j = 0; j < tn.size(); j++) {
                if (tokens[i] == tn[j]) {
                    tName = tokens[i];
                    break;
                }
            }
            
            if (tName == "")
                Msg::error("Expecting taxon name!");
            
            newNode->setName(tName);
            newNode->setIndex(j);
            newNode->addNeighbor(p);
            p->addNeighbor(newNode);
            newNode->setAncestor(p);
            p = newNode;
            
        }
    }
    
    initializeDownPassSequence();
    int intIdx = (int)tn.size();
    RandomVariable& rng = RandomVariable::randomVariableInstance();


    for (int i = 0; i < downPassSequence.size(); i++) {
        Node* p = downPassSequence[i];
        if (p->getIsLeaf() == false)
            p->setIndex(intIdx++);
        if (p->getAncestor() != NULL) {
            double v = Probability::Exponential::rv(&rng, lam);
            addBranch(p, p->getAncestor(), v);
        }
    }
    
}

Tree::~Tree(void) {
    removeAllBranches();
    deleteAllNodes();
}

Node* Tree::addNode(void) {
    Node* newNode = new Node;
    newNode->setOffset((int)nodes.size());
    nodes.push_back(newNode);
    return newNode;
}

// pre-order traversal
void Tree::initializeDownPassSequence(void) {
    downPassSequence.clear();
    passDown(root, root);
}

void Tree::passDown(Node* p, Node* from) {
    if (p != NULL) {
        std::set<Node*>& desc = p->getNeighbors();
        for (Node* n : desc) {
            if (n != from)
                passDown(n, p);
        }
        downPassSequence.push_back(p);
        if (p != from)
            p->setAncestor(from);
        else
            p->setAncestor(NULL);
    }

}

void Tree::print(void) {
    listNode(root, 0);
}

void Tree::print(std::string h) {
    std::cout << h << std::endl;
    print();
}

void Tree::listNode(Node* p, int indent) {
    
    if (p != NULL) {
        
        std::set<Node*>& desc = p->getNeighbors();
        Node* pAnc = p->getAncestor();
        Branch* b = findBranch(p, pAnc);
        
        for (int i = 0; i < indent; i++)
            std::cout << " ";
        std::cout << p->getIndex() << " ( ";
        for (Node* n : desc) {
            if (n == p->getAncestor())
                std::cout << "a_";
            std::cout << n->getIndex() << " ";
        }
        std::cout << ") ";
        
        std::cout << std::fixed << std::setprecision(5);
        if (b != NULL)
            std::cout << b->getLength() << " ";
        
        std::cout << p->getName() << " ";
        if (p == root)
            std::cout << " <- Root";
        std::cout << std::endl;
        
        for (Node* n : desc) {
            if (n != p->getAncestor())
                listNode(n, indent+3);
        }
    }
    
}

// deep copy tree
void Tree::clone(const Tree& t) {
    
    if (nodes.size() != t.nodes.size()) {
        deleteAllNodes();
        for (int i = 0; i < t.nodes.size(); i++)
            addNode();
    }
    
    taxonNames = t.taxonNames;
    root = nodes[t.root->getOffset()];
    
    for (int i = 0; i < t.nodes.size(); i++) {
        Node* lft = nodes[i];
        Node* rht = t.nodes[i];
        if (rht->getAncestor()!= NULL)
            lft->setAncestor(nodes[rht->getAncestor()->getOffset()]);
        else
            lft->setAncestor(NULL);
        lft->setIndex(rht->getIndex());
        lft->setName(rht->getName());
        lft->setIsLeaf(rht->getIsLeaf());
        lft->removeNeighbors();
        std::set<Node*>& rhtNeighbors = rht->getNeighbors();
        for (Node* n : rhtNeighbors)
            lft->addNeighbor(nodes[n->getOffset()]);
    }
    downPassSequence.resize(t.downPassSequence.size());
    for (int i = 0; i < t.downPassSequence.size(); i++)
        downPassSequence[i] = nodes[t.downPassSequence[i]->getOffset()];
    
    // Copy branches
    removeAllBranches();
    const std::map<NodePair, Branch*, CompNodePair>& rhtBranches = t.getBranches();
    for (std::map<NodePair, Branch*, CompNodePair>::const_iterator it = rhtBranches.begin(); it != rhtBranches.end(); it++) {
        Node* e1 = nodes[it->first.getEnd1()->getOffset()];
        Node* e2 = nodes[it->first.getEnd2()->getOffset()];
        double v = it->second->getLength();
        addBranch(e1, e2, v);
    }
}

void Tree::deleteAllNodes(void) {
    
    for (int i = 0; i < nodes.size(); i++)
        delete nodes[i];
    nodes.clear();
}

Branch* Tree::findBranch(Node* e1, Node* e2) {
    
    if (e1 == NULL || e2 == NULL)
        return NULL;
    
    key.setEnds(e1, e2);
    std::map<NodePair, Branch*, CompNodePair>::iterator it = branches.find(key);
    
    if (it != branches.end())
        return it->second;
        
    return NULL;
    
}

Branch* Tree::addBranch(Node* e1, Node* e2) {
    BranchFactory& bf = BranchFactory::branchFactoryInstance();
    Branch* b = bf.getBranch();
    b->setEnds(e1, e2);
    b->setLength(0.0);
    branches.insert(std::make_pair(NodePair(e1, e2), b));
    return b;
}

Branch* Tree::addBranch(Node* e1, Node* e2, double len) {
    BranchFactory& bf = BranchFactory::branchFactoryInstance();
    Branch* b = bf.getBranch();
    b->setEnds(e1, e2);
    b->setLength(len);
    branches.insert(std::make_pair(NodePair(e1, e2), b));
    return b;
}

void Tree::removeBranch(Branch* b) {
    BranchFactory& bf = BranchFactory::branchFactoryInstance();
    for (std::map<NodePair, Branch*, CompNodePair>::iterator it = branches.begin(); it != branches.end(); it++) {
        if (it->second == b) {
            bf.returnToPool(b);
            branches.erase(it);
            return;
        }
    }
}

void Tree::removeBranch(Node* e1, Node* e2) {
    key.setEnds(e1, e2);
    std::map<NodePair, Branch*, CompNodePair>::iterator it = branches.find(key);
    
    if (it != branches.end()) {
        BranchFactory& bf = BranchFactory::branchFactoryInstance();
        bf.returnToPool(it->second);
        branches.erase(it);
    }
    
}

void Tree::removeAllBranches(void) {
    BranchFactory& bf = BranchFactory::branchFactoryInstance();
    for (std::map<NodePair, Branch*, CompNodePair>::iterator it = branches.begin(); it != branches.end(); it++) {
        bf.returnToPool(it->second);
    }
    branches.clear();
}

double Tree::updateBrLen(void) {
    
    double tuning = log(4.0);
    
    RandomVariable& rng = RandomVariable::randomVariableInstance();
    TransitionProbabilities& tp = TransitionProbabilities::transitionProbabilitiesInstance();
    
    // pick a branch at random
    int whichBranch = (int)(rng.uniformRv() * branches.size());
    int i = 0;
    Branch* b = NULL;
    for (std::map<NodePair, Branch*, CompNodePair>::iterator it = branches.begin(); it != branches.end(); it++) {
        if (i == whichBranch) {
            b = it->second;
            break;
        }
        i++;
    }
    if (b == NULL)
        Msg::error("Branch wasn't randomly chosen!");
    
    // change the length of the branch
    double oldV = b->getLength();
    double newV = oldV*exp((rng.uniformRv()-0.5)*tuning); // proposal mechansim constrain to positive value
    b->setLength(newV);
    
    // update transition probability
    tp.flipActiveVal();
    tp.setTransProbs(this);
    
    return log(newV)-log(oldV); // Hastings ratio
}


double Tree::updateLocal(void) {
    
#   ifdef DEBUG_LOCAL
    print("before local: ");
#   endif
    
    RandomVariable& rng = RandomVariable::randomVariableInstance();
    double tuning = log(2.0);
    
    // choose the backbone of move
    std::vector<Node*> backboneNodes;
    std::vector<Branch*> backboneBranches;
    chooseBackbone(&rng, backboneNodes, backboneBranches);
    
    // calculate length of backbone
    double m = 0.0;
    for (int i = 0; i < backboneBranches.size(); i++)
        m += backboneBranches[i]->getLength();
    double mPrime = m*exp(tuning*(rng.uniformRv()-0.5));
    
    // update backbone branch lengths
    double factor = mPrime / m;
    for (int i = 0; i < backboneBranches.size(); i++) {
        double v = backboneBranches[i]->getLength();
        v *= factor;
        backboneBranches[i]->setLength(v);
    }

#   ifdef DEBUG_LOCAL
    for (int i = 0; i < backboneNodes.size(); i++) {
        std::cout << "backboneNodes[" << i << "]: " << backboneNodes[i]->getIndex() << std::endl;
    }
    
    for (int i = 0; i < backboneBranches.size(); i++) {
        std::cout << "backboneBranches[" << i << "]: " << backboneBranches[i]->getLength() << std::endl;
    }
    
    std::cout << "m = " << m << ", mPrime = " << mPrime << std::endl;
#   endif
    
    // choose one or two branches contiguous to move
    
    if (rng.uniformRv() < 0.5) {
        
        //Node* a = backboneNodes[1];
        //std::vector<Node*> pNeighbors = a->getNeighborsVector();
        //Node* b = NULL;
        //do {
            //    b = pNeighbors[(int)(rng.uniformRv()*pNeighbors.size())];
        //} while (b == backboneNodes[0] || b == backboneNodes[2]);
        
        // remove branch
        
        double v = backboneBranches[0]->getLength()+backboneBranches[1]->getLength();
        removeBranch(backboneBranches[0]);
        removeBranch(backboneBranches[1]);
        Branch* newBranch = addBranch(backboneNodes[0], backboneNodes[2], v);
        backboneNodes[0]->removeNeighbor(backboneNodes[1]);
        backboneNodes[1]->removeNeighbor(backboneNodes[0]);
        backboneNodes[1]->removeNeighbor(backboneNodes[2]);
        backboneNodes[2]->removeNeighbor(backboneNodes[1]);
        backboneNodes[0]->addNeighbor(backboneNodes[2]);
        backboneNodes[2]->addNeighbor(backboneNodes[0]);
        
        // move the branch
        
        double u = rng.uniformRv() * mPrime;
        if (u < v) {
            removeBranch(newBranch);
            backboneNodes[0]->removeNeighbor(backboneNodes[2]);
            backboneNodes[2]->removeNeighbor(backboneNodes[0]);
            backboneNodes[0]->addNeighbor(backboneNodes[1]);
            backboneNodes[1]->addNeighbor(backboneNodes[0]);
            backboneNodes[2]->addNeighbor(backboneNodes[1]);
            backboneNodes[1]->addNeighbor(backboneNodes[2]);
            addBranch(backboneNodes[0], backboneNodes[1], u);
            addBranch(backboneNodes[1], backboneNodes[2], v-u);
        } else {
            removeBranch(backboneNodes[2], backboneNodes[3]);
            backboneNodes[2]->removeNeighbor(backboneNodes[3]);
            backboneNodes[3]->removeNeighbor(backboneNodes[2]);
            backboneNodes[2]->addNeighbor(backboneNodes[1]);
            backboneNodes[1]->addNeighbor(backboneNodes[2]);
            backboneNodes[3]->addNeighbor(backboneNodes[1]);
            backboneNodes[1]->addNeighbor(backboneNodes[3]);
            addBranch(backboneNodes[2], backboneNodes[1], u-v);
            addBranch(backboneNodes[3], backboneNodes[1], mPrime-u);
        }
        
    } else {
        
        // remove branch
        
        double v = backboneBranches[1]->getLength()+backboneBranches[2]->getLength();
        removeBranch(backboneBranches[1]);
        removeBranch(backboneBranches[2]);
        Branch* newBranch = addBranch(backboneNodes[1], backboneNodes[3], v);
        backboneNodes[1]->removeNeighbor(backboneNodes[2]);
        backboneNodes[2]->removeNeighbor(backboneNodes[1]);
        backboneNodes[2]->removeNeighbor(backboneNodes[3]);
        backboneNodes[3]->removeNeighbor(backboneNodes[2]);
        backboneNodes[1]->addNeighbor(backboneNodes[3]);
        backboneNodes[3]->addNeighbor(backboneNodes[1]);
        
        // move the branch
        
        double u = rng.uniformRv() * mPrime;
        if (u < v) {
            removeBranch(newBranch);
            backboneNodes[1]->removeNeighbor(backboneNodes[3]);
            backboneNodes[3]->removeNeighbor(backboneNodes[1]);
            backboneNodes[1]->addNeighbor(backboneNodes[2]);
            backboneNodes[2]->addNeighbor(backboneNodes[1]);
            backboneNodes[2]->addNeighbor(backboneNodes[3]);
            backboneNodes[3]->addNeighbor(backboneNodes[2]);
            addBranch(backboneNodes[1], backboneNodes[2], u);
            addBranch(backboneNodes[2], backboneNodes[3], v-u);
        } else {
            removeBranch(backboneNodes[0], backboneNodes[1]);
            backboneNodes[0]->removeNeighbor(backboneNodes[1]);
            backboneNodes[1]->removeNeighbor(backboneNodes[0]);
            backboneNodes[0]->addNeighbor(backboneNodes[2]);
            backboneNodes[2]->addNeighbor(backboneNodes[0]);
            backboneNodes[1]->addNeighbor(backboneNodes[2]);
            backboneNodes[2]->addNeighbor(backboneNodes[1]);
            addBranch(backboneNodes[0], backboneNodes[2], u-v);
            addBranch(backboneNodes[2], backboneNodes[1], mPrime-u);
        }
    }
    // next time we'll move that branch
    
    initializeDownPassSequence();
    
    TransitionProbabilities& tp = TransitionProbabilities::transitionProbabilitiesInstance();
    
    // update transition probability
    tp.flipActiveVal();
    tp.setTransProbs(this);

#   ifdef DEBUG_LOCAL
    print("after local: ");
    getchar();
#   endif
    
    return 3.0 * log(factor);
}

void Tree::chooseBackbone(RandomVariable* rng, std::vector<Node*>& backboneNodes, std::vector<Branch*>& backboneBranches) {
        
    Node* p = NULL;
    do {
        p = nodes[(int)(rng->uniformRv()*nodes.size())];
    } while (p->getIsLeaf()==true || p == root);
    Node* pAncs = p->getAncestor();
        
    Node* q = NULL;
    std::vector<Node*> pNeighbors = p->getNeighborsVector();
        
    do {
        q = pNeighbors[(int)(rng->uniformRv()*pNeighbors.size())];
    } while (q==pAncs);
    
    Node* r = NULL;
    std::vector<Node*> pAncsNeighbors = pAncs->getNeighborsVector();
    do {
        r = pAncsNeighbors[(int)(rng->uniformRv()*pAncsNeighbors.size())];
    } while (r==p);
    
    Branch* b1 = findBranch(q, p);
    Branch* b2 = findBranch(p, pAncs);
    Branch* b3 = findBranch(pAncs, r);
    
    backboneNodes.push_back(q);
    backboneNodes.push_back(p);
    backboneNodes.push_back(pAncs);
    backboneNodes.push_back(r);
    
    backboneBranches.push_back(b1);
    backboneBranches.push_back(b2);
    backboneBranches.push_back(b3);
}


double Tree::lnProbability(void) {
    double lnP = 0.0;
    
    for (std::map<NodePair, Branch*, CompNodePair>::iterator it = branches.begin(); it != branches.end(); it++) {
        lnP += Probability::Exponential::lnPdf(10.0, it->second->getLength());
    }
    
    return lnP;
}

std::vector<std::string> Tree::tokenize(std::string str) {
    
    std::vector<std::string> tokens;
    std::string word = "";
    
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        if (c == '(' || c == ')' || c == ',' || c == ';') {
            if (word != "") {
                tokens.push_back(word);
                word = "";
            }
        }
        
        if (c == '(')
            tokens.push_back("(");
        else if (c == ')')
            tokens.push_back(")");
        else if (c == ',')
            tokens.push_back(",");
        else if (c == ';')
            tokens.push_back(";");
        else {
            word += c;
        }
            
    }
    
    return tokens;
}
