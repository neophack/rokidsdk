#ifndef GLASS_CONTROL_H
#define GLASS_CONTROL_H

#include <libusb-1.0/libusb.h>
#include <mutex>
#include <sys/stat.h>
#include <cstring>

class GlassControl {
public:
    GlassControl(uint16_t vid, uint16_t pid, uint8_t interface_num);
    ~GlassControl();
    int GlassRequest(unsigned char request_type, unsigned short value, unsigned short index, unsigned char *data, unsigned long length);
    void GlassSetDisplayMode(int mode);
    void GlassReboot(int flag);
    unsigned char GlassGetDisplayMode();
    unsigned char GlassGetBrightness(int index);
    void GlassGetPermission();
    unsigned char *GlassGetSerialNumber();
    unsigned char *GlassGetHardwareVersion();
    unsigned char *GlassGetOpticalID();
    int GlassHMDRequest(unsigned char request_type, unsigned short value, unsigned short index, unsigned char *data, unsigned long length, long long *start_time, long long *end_time);
    void SetBrightness(int index, int brightness);

private:
    libusb_device_handle *handle;
    libusb_device_descriptor desc;
    std::mutex *mutex_lock;
    uint16_t vid;
    uint16_t pid;
    uint8_t interface_num;
};


#endif // GLASS_CONTROL_H

