/* Utils file to implement common functions for the two daemons
 * such as reading the configuration file*/

#include "../include/utils.hpp"


std::string uppercase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::toupper(c); } );
    return str;
}


/* Get the path from the config file
 * The expected format is basePath=/path/to/folder
 * If the config file cannot be opened or the path is not 
 * a directory, a negative return code will be returned.
 */ 
int getPathFromConfig(fs::path configFile, fs::path *cp){
    std::ifstream cfg (configFile);
    if(!cfg.is_open()){
        std::cerr << "Unable to open configuration file!\n";
        return -1;
    }

    std::string line;
    while(std::getline(cfg, line)){
        if(line[0] == '#' || line.empty())
            continue;
        auto dp = line.find("=");
        auto name = line.substr(0, dp);
        if (name == "basePath"){
            std::string value = line.substr(dp + 1);
            if (value.back()== '/')
                value.pop_back();
            cp->assign(value);
            fs::file_status s = fs::symlink_status(*cp);
            if(!fs::is_directory(s)){
                std::cerr << "Path is not a directory!\n";
                return -2;   
            }
            break;
        }
    }
    return 0;
}

int getHashModeFromConfig(fs::path configFile, int *mode){
    std::ifstream cfg (configFile);
    if(!cfg.is_open()){
        std::cerr << "Unable to open configuration file!\n";
        return -1;
    }

    std::string line;
    while(std::getline(cfg, line)){
        if(line[0] == '#' || line.empty())
            continue;
        auto dp = line.find("=");
        auto name = line.substr(0, dp);
        if (name == "hashMode"){
            std::string value = line.substr(dp + 1);
            value = uppercase(value);
            if(strcmp(value.c_str(),"BLAKE3")==0)
                *mode=0;
            else if(strcmp(value.c_str(),"SHA256")==0)
                *mode=1;
            else{
                *mode=0;
                return -2;
            }break;
        }
    }
    return 0;
}