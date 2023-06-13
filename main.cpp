#include <iostream>
#include "glass_control.h"

int main() {
    std::cout << "Hello World!" << std::endl;
    GlassControl glass(1234, 5679); // Replace with actual VID and PID
    glass.GlassSetDisplayMode(2); // 1: 2D mode, 2: 3D mode
    std::cout << "Display mode: " << (int)glass.GlassGetDisplayMode() << std::endl;
    std::cout << "Brightness: " << (int)glass.GlassGetBrightness(0) << std::endl;
    std::cout << "Serial number: " << glass.GlassGetSerialNumber() << std::endl;
    std::cout << "Hardware version: " << glass.GlassGetHardwareVersion() << std::endl;
    std::cout << "Optical ID: " << glass.GlassGetOpticalID() << std::endl;

    return 0;
}
