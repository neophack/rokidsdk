#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

int main(int argc, char **argv) {
    libusb_device_handle *handle;
    int r;

    // Initialize libusb
    r = libusb_init(NULL);
    if (r < 0) {
        fprintf(stderr, "libusb_init error: %s\n", libusb_error_name(r));
        return 1;
    }

    // Open the device
    handle = libusb_open_device_with_vid_pid(NULL, 0x1234, 0x5678);
    if (handle == NULL) {
        fprintf(stderr, "Failed to open device\n");
        libusb_exit(NULL);
        return 1;
    }

    // Claim the interface
    r = libusb_claim_interface(handle, 0);
    if (r < 0) {
        fprintf(stderr, "Failed to claim interface: %s\n", libusb_error_name(r));
        libusb_close(handle);
        libusb_exit(NULL);
        return 1;
    }

    // Send a control transfer
    uint8_t request_type = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT;
    uint8_t request = 0x01;
    uint16_t value = 0x0000;
    uint16_t index = 0x0000;
    unsigned char data[] = {0x00, 0x01, 0x02, 0x03};
    int length = sizeof(data);
    int timeout = 1000;
    int transferred;
    r = libusb_control_transfer(handle, request_type, request, value, index, data, length, timeout);
    if (r < 0) {
        fprintf(stderr, "Control transfer error: %s\n", libusb_error_name(r));
        libusb_release_interface(handle, 0);
        libusb_close(handle);
        libusb_exit(NULL);
        return 1;
    }

    // Release the interface and close the device
    libusb_release_interface(handle, 0);
    libusb_close(handle);

    // Deinitialize libusb
    libusb_exit(NULL);

    return 0;
}

