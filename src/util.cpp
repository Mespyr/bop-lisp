#include "../include/util.h"

std::string read_ascii_file(const char* path) {

    // file
	std::fstream file;
	file.open(path, std::ios::in);
    
    std::string code;


	if (!file) {
		std::cout << "Error: No such file or directory '" << path << "'" << std::endl;
        exit(1);
	}
	else {
		std::string ch;
		
		while(getline(file, ch)){  
			code += ch + '\n';  
		}
	}
	file.close();
	return code+"\0";
}