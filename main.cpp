#include <iostream>
#include "glass_control.h"
#include <getopt.h> // C标准库中用于解析命令行参数的头文件

void print_usage() {
    std::cout << "Usage: ./glass_control [OPTIONS]" << std::endl
              << "Available options:" << std::endl
              << "  -m|--display-mode [MODE]   Set display mode: 1 for 2D, 2 for 3D" << std::endl
              << "  -b|--brightness [VALUE]    Set display brightness (0-100)" << std::endl
              << "  -h|--help                  Show this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    int c;

    int display_mode = 0;
    int brightness = -1;

    /* 命令行参数解析 */
    static struct option long_options[] = {
        {"display-mode", required_argument, 0, 'm'},
        {"brightness", required_argument, 0, 'b'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int option_index = 0;

    while ((c = getopt_long(argc, argv, "m:b:h", long_options, &option_index)) != -1) {
        switch (c) {
        case 'm':
            display_mode = atoi(optarg);
            break;

        case 'b':
            brightness = atoi(optarg);
            break;

        case 'h':
            print_usage();
            return 0;

        default:
            print_usage();
            return 1;
        }
    }

    if (display_mode != 0 || brightness != -1) {
        std::cout << "Setting display mode to " << display_mode << " (1: 2D mode, 2: 3D mode)" << std::endl;
        std::cout << "Setting brightness to " << brightness << std::endl;
    }

    GlassControl glass(1234, 5679,1); // Replace with actual VID and PID

    if (display_mode != -1) {
        glass.GlassSetDisplayMode(display_mode);
        std::cout << "Display mode: " << (int)glass.GlassGetDisplayMode() << std::endl;
    }

    if (brightness != -1) {
        glass.SetBrightness(0, brightness);
        std::cout << "Brightness: " << (int)glass.GlassGetBrightness(0) << std::endl;
    }

    std::cout << "Serial number: " << glass.GlassGetSerialNumber() << std::endl;
    std::cout << "Hardware version: " << glass.GlassGetHardwareVersion() << std::endl;
    std::cout << "Optical ID: " << glass.GlassGetOpticalID() << std::endl;

    return 0;
}
