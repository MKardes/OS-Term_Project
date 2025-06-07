#include <limits>
#include "CPU.h"

int main(int argc, char *argv[]) {
    int debug_level = 0;

    if (argc != 2 && argc != 4) {
        std::cout << "Usages: " << std::endl;
        std::cout << "  " << argv[0] << " <filename>" << std::endl;
        std::cout << "  " << argv[0] << " <filename> -D <debug_level>" << std::endl;
        return 1;
    }

    if (argc == 4) {
        if (std::string(argv[2]) == "-D") {
            debug_level = std::stoi(argv[3]);
        } else {
            std::cout << "Invalid argument: " << argv[2] << std::endl;
            return 1;
        }
        if (debug_level < 0 || debug_level > 3) {
            std::cout << "Invalid debug level: " << debug_level << std::endl;
            return 1;
        }
    } else {
        debug_level = -1;
    }

    std::string filename = argv[1];
    try {

        CPU myCpu(filename, debug_level);
        std::cout << "CPU initialized" << std::endl;

        int old_kernel_mode = myCpu.getKernelMode();
        std::cout << "Executing..." << std::endl;
        while (!myCpu.isHalted()){
            myCpu.execute();

            /* DEBUG 1 */
            if (debug_level == 1 || debug_level == 2) {
                myCpu.debug();
            }

            if (debug_level == 2 && !myCpu.isHalted()) {
                std::cout << "Press enter to execute next instruction:";
                std::cin.get();
            }

            if (debug_level == 3 && old_kernel_mode != myCpu.getKernelMode()) {
                myCpu.thread_table_debug();
                old_kernel_mode = myCpu.getKernelMode();
            }
        }

        if (debug_level == 0) {
            myCpu.debug();
        }

    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}