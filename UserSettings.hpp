//
//  UserSettings.hpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/1/21.
//

#ifndef UserSettings_hpp
#define UserSettings_hpp

#include <string>

class UserSettings{
public:
    UserSettings(UserSettings& s) = delete;
    void initialize(int argc, char* argv[]);
    static UserSettings& userSettings(void) {
        static UserSettings us;
        return us;
    }
    
    std::string getFileName(void);
    int getChainLength(void) {return chainLength;}
    void print(void);
    
private:
    UserSettings(void);
    std::string filename;
    int chainLength;
    bool isInitialized;
};

#endif /* UserSettings_hpp */
