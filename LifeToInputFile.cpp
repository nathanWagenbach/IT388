//Converter for Life 1.05 files to our file format. Ignores # lines and only works if all lines are the same length.
//Intended to allow for use of the designs in Stephan A Silver's Life Lexicon https://conwaylife.com/ref/lexicon/lex_home.htm
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int main(int argc, char* argv[]){
    if (argc!=3){
        std::cout<<"Usage:"<<argv[0]<<" [inputfile] [outputfile]\n";
        return 0;
    }
    std::ifstream file(argv[1]);
    std::vector<std::string> lines;
    std::string line;
    if(!file.good()){//This is only here for the error message, the while loop will evaluate while(0) if the file can't be read.
        std::cerr<<argv[1]<<" could not be read or does not exist."<<std::endl;
        return 0;
    }
    
    while(getline(file,line)){
        if (line.empty() || line[0]=='#') continue;
        std::stringstream encoded;
       
        for(auto i:line){
            if (i==' ');
            else if (i=='*'||i=='O'){
                encoded<<1;
            }
            else if (i=='.'){
                encoded<<0;
            }
        }
        lines.push_back(encoded.str());
    }
    std::ofstream outFile(argv[2]);
    for(auto i:lines){
        outFile<<i<<'\n';
    }
}
