#include "CPU.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    try {
        CPU myCpu(filename);
        std::cout << "CPU initialized" << std::endl;
        
        std::cout << "Executing..." << std::endl;
        while (!myCpu.isHalted()){
            myCpu.execute(); 
        }
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}