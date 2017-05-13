#include "Helper.h"
#include <string>
#include <vector>

using namespace std;

vector<string> Helper::split_cmd(const string& cmd){
    vector<string> cmd_strs;
    size_t spos=0, epos;
    while(spos<cmd.size()){
        spos=cmd.find_first_not_of(" \t",spos); // skip spaces
        if(spos==string::npos) break; // nothing left
        if(cmd[spos]=='"'){
            epos = cmd.find('"',++spos);
        } else {
            epos = cmd.find(' ',spos);
        }
        if(epos==string::npos) epos=cmd.size();
        cmd_strs.push_back(cmd.substr(spos,epos-spos));
        spos = epos+1;
    }
    return cmd_strs;
}
