#include <iostream>
#include "glass_control.h"

int main() {
    GlassControl glass(0x1234, 0x5678); // Replace with actual VID and PID
    glass.GlassSetDisplayMode(1);
    std::cout << "Display mode: " << (int)glass.GlassGetDisplayMode() << std::endl;
    std::cout << "Brightness: " << (int)glass.GlassGetBrightness(0) << std::endl;
    std::cout << "Serial number: " << glass.GlassGetSerialNumber() << std::endl;
    std::cout << "Hardware version: " << glass.GlassGetHardwareVersion() << std::endl;
    std::cout << "Optical ID: " << glass.GlassGetOpticalID() << std::endl;
    return 0;
}
