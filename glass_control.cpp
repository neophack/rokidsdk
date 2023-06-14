#include "glass_control.h"


GlassControl::GlassControl(uint16_t vid, uint16_t pid, uint8_t interface_num) {
    handle = NULL;
    memset(&desc, 0, sizeof(desc));
    libusb_init(NULL);
    //    libusb_set_debug(NULL, 1);
    mutex_lock = new std::mutex();
    this->vid = vid;
    this->pid = pid;
    this->interface_num = interface_num;
}


GlassControl::~GlassControl() {
    if (handle != NULL) {
        libusb_close(handle);
        handle = NULL;
    }
    libusb_exit(NULL);
    delete mutex_lock;
}

int GlassControl::GlassRequest(unsigned char request_type, unsigned short value, unsigned short index, unsigned char *data, unsigned long length) {
    mutex_lock->lock();
    int ret = 0;

    // 打开设备
    handle = libusb_open_device_with_vid_pid(NULL, vid, pid);
    if (handle == NULL) {
        fprintf(stderr, "Failed to open device\n");
        libusb_exit(NULL);
        return EXIT_FAILURE;
    }

    ret = libusb_claim_interface(handle, interface_num);
    if (ret < 0) {
        // 处理错误
    }
    ret = libusb_control_transfer(handle, request_type & 0x80 | 0x40, request_type, index, value, data, length, 1000);
    if (ret < 0) {
        libusb_close(handle);
        handle = NULL;
        desc.idProduct = 0;
    }
    mutex_lock->unlock();
    return ret;
}


void GlassControl::GlassSetDisplayMode(int mode) {
    unsigned char data[4];
    data[0] = (unsigned char)mode;
    GlassRequest(0x01, 1, (unsigned short)mode, data, 1);
}

void GlassControl::SetBrightness(int index, int brightness) {
    unsigned char data[4];
    memset(data, 0, sizeof(data));
    data[0] = (unsigned char)index;
    data[1] = (unsigned char)brightness;
    GlassRequest(0x02, index, brightness, data, 1);
}


void GlassControl::GlassReboot(int flag) {
    unsigned char data[16];
    memset(data, 0, sizeof(data));
    if (desc.idProduct == pid) {
        if (flag == 1) {
            memcpy(data, "tobtsaf", 8);
        } else {
            memcpy(data, "lamron", 6);
        }
        GlassRequest(0x03, 0, 0xb00, data, strlen((char *)data));
    }
}

unsigned char GlassControl::GlassGetDisplayMode() {
    unsigned char data[64];
    memset(data, 0, sizeof(data));
    GlassRequest(0x81, 1, 0, data, 64);
    return data[0];
}

unsigned char GlassControl::GlassGetBrightness(int index) {
    unsigned char data[4];
    memset(data, 0, sizeof(data));
    GlassRequest(0x82, 2, (unsigned short)index, data, 1);
    if (data[0] != 1) {
        data[0] = 0;
    }
    return data[0];
}

void GlassControl::GlassGetPermission() {
    unsigned char data[8];
    memset(data, 0, sizeof(data));
    GlassRequest(0x81, 0, 0x900, data, 8);
    strtol((char *)data, (char **)0x0, 0x10);
}

unsigned char *GlassControl::GlassGetSerialNumber() {
    static unsigned char serial[64] = {0};
    memset(serial, 0, sizeof(serial));
    GlassRequest(0x81, 0, 0x100, serial, 64);
    return serial;
}

unsigned char *GlassControl::GlassGetHardwareVersion() {
    static unsigned char version[16] = {0};
    memset(version, 0, sizeof(version));
    GlassRequest(0x81, 0, 0x800, version, 16);
    return version;
}

unsigned char *GlassControl::GlassGetOpticalID() {
    static unsigned char id[64] = {0};
    memset(id, 0, sizeof(id));
    GlassRequest(0x81, 0, 0x700, id, 64);
    return id;
}

int GlassControl::GlassHMDRequest(unsigned char request_type, unsigned short value, unsigned short index, unsigned char *data, unsigned long length, long long *start_time, long long *end_time) {
    mutex_lock->lock();
    int ret = 0;
    if (handle == NULL) {
        libusb_device **devs;
        ssize_t cnt = libusb_get_device_list(NULL, &devs);
        if (cnt < 0) {
            mutex_lock->unlock();
            return -1;
        }
        for (int i = 0; i < cnt; i++) {
            libusb_device *dev = devs[i];
            libusb_device_descriptor dev_desc;
            if (libusb_get_device_descriptor(dev, &dev_desc) < 0) {
                continue;
            }
            if (dev_desc.idVendor == vid && dev_desc.idProduct == pid) {
                if (libusb_open(dev, &handle) == 0) {
                    memcpy(&desc, &dev_desc, sizeof(dev_desc));
                    break;
                }
            }
        }
        libusb_free_device_list(devs, 1);
        if (handle == NULL) {
            mutex_lock->unlock();
            return -1;
        }
    }
    if (desc.idProduct == pid) {
        timespec start;
        clock_gettime(CLOCK_MONOTONIC, &start);
        *start_time = start.tv_sec * 1000000000LL + start.tv_nsec;
        ret = libusb_control_transfer(handle, request_type & 0x80 | 0x40, request_type, index, value, data, length, 1);
        if (ret >= 0) {
            timespec end;
            clock_gettime(CLOCK_MONOTONIC, &end);
            *end_time = end.tv_sec * 1000000000LL + end.tv_nsec;
        } else {
            desc.idProduct = 0;
        }
    } else {
        ret = -1;
    }
    mutex_lock->unlock();
    return ret;
}

