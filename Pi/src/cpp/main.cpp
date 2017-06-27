#include <string>
#include <iostream>

#include "comm/comm.h"
#include "gpio/GPIOPin.h"

int main() {
	initComms();

    int type;
    
    while (type != 3) {
        type = -1;
        
        std::cout << "Please enter 0 for a write operation or 1 for a read operation. Enter 3 to quit" << std::endl;
        while (!(std::cin >> type)) {
            std::cin.clear();
            std::cin.sync();
            std::cout << "Please enter another value. Your entry is incorrect." << std::endl;
        }
        
        int id = 0;
        int val = 0;
        if (type != 3) {
            if (type) {
                std::cout << "Read operation requested.";
                std::cout << "Enter the id of the sensor you would like to read." << std::endl;
                while (!(std::cin >> cmd)) {
                    std::cin.clear();
                    std::cin.sync();
                    std::cout << "Please enter another value. Your entry is incorrect." << std::endl;
                }
                read(id);
            } else {
                std::cout << "Write operation requested." << std::endl;
                std::cout << "Enter the id of the sensor you would like to write to. This must be between 0 and 14" << std::endl;
                while (!(std::cin >> cmd) || cmd > 14 || cmd < 0) {
                    std::cin.clear();
                    std::cin.sync();
                    std::cout << "Please enter another value. Your entry is incorrect." << std::endl;
                }
                std::cout << "Enter the magnitude you would like to write." << std::endl;
                while (!(std::cin >> val)) {
                    std::cin.clear();
                    std::cin.sync();
                    std::cout << "Please enter another value. Your entry is incorrect." << std::endl;
                }
                write(val, id);
            }
        }
    }
    
    std::cout << "Quitting" << std::endl;
    
	return 0;
}
