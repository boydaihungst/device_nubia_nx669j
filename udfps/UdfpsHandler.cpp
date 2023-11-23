/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "UdfpsHander.nubia"

#include "UdfpsHandler.h"

#include <android-base/logging.h>
#include <fcntl.h>
#include <poll.h>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <stdint.h>
// #include <vendor/goodix/hardware/biometrics/fingerprint/2.1/IGoodixFingerprintDaemon.h>
#include <vendor/nubia/ifaa/1.0/IIfaa.h>

#define NOTIFY_FINGER_DOWN 1536
#define NOTIFY_FINGER_UP 1537

// #define hbm_feedback "/sys/kernel/lcd_enhance/hbm_feedback"

using ::android::sp;
using ::android::hardware::hidl_vec;
// using ::vendor::goodix::hardware::biometrics::fingerprint::V2_1::IGoodixFingerprintDaemon;
using ::vendor::nubia::ifaa::V1_0::IIfaa;

// static bool readBool(int fd) {
//     char c;
//     int rc;

//     rc = lseek(fd, 0, SEEK_SET);
//     if (rc) {
//         LOG(ERROR) << "failed to seek fd, err: " << rc;
//         return false;
//     }

//     rc = read(fd, &c, sizeof(char));
//     if (rc != 1) {
//         LOG(ERROR) << "failed to read bool from fd, err: " << rc;
//         return false;
//     }

//     return c != '0';
// }

class NubiadfpsHander : public UdfpsHandler {
  public:
    void init(fingerprint_device_t *device) {
        mDevice = device;

        mIfaa = IIfaa::getService();
            if (!mIfaa) {
                LOG(ERROR) << "No ifaa service" << mIfaa;
                return;
            }
        // std::thread([this]() {
        //     int fd = open(hbm_feedback, O_RDONLY);
        //     if (fd < 0) {
        //         LOG(ERROR) << "failed to open fd, err: " << fd;
        //         return;
        //     }

        //     struct pollfd fodUiPoll = {
        //             .fd = fd,
        //             .events = POLLERR | POLLPRI,
        //             .revents = 0,
        //     };

        //     while (true) {
        //         int rc = poll(&fodUiPoll, 1, -1);
        //         if (rc < 0) {
        //             LOG(ERROR) << "failed to poll fd, err: " << rc;
        //             continue;
        //         }

                // mGoodixFingerprintDaemon->sendCommand(readBool(fd) ? NOTIFY_FINGER_DOWN
                //     : NOTIFY_FINGER_UP, {}, [](int, const hidl_vec<signed char>&) {});
        //     }
        // }).detach();
    }

    void onFingerDown(uint32_t /*x*/, uint32_t /*y*/, float /*minor*/, float /*major*/) {
        // nothing
        mIfaa->processCmd(13, 0, 0, {}, 0, [](const hidl_vec<signed char>&) {});
        mIfaa->processCmd(14, 0, 0, {}, 0, [](const hidl_vec<signed char>&) {});
    }

    void onFingerUp() {
        mIfaa->processCmd(15, 0, 0, {}, 0, [](const hidl_vec<signed char>&) {});
        // nothing
    }

    void onAcquired(int32_t /*result*/, int32_t /*vendorCode*/) {
        // nothing
    }

    void cancel() {
        mIfaa->processCmd(15, 0, 0, {}, 0, [](const hidl_vec<signed char>&) {});
        // nothing
    }

  private:
    fingerprint_device_t *mDevice;
    sp<IIfaa> mIfaa;
};

static UdfpsHandler* create() {
    return new NubiadfpsHander();
}

static void destroy(UdfpsHandler* handler) {
    delete handler;
}

extern "C" UdfpsHandlerFactory UDFPS_HANDLER_FACTORY = {
    .create = create,
    .destroy = destroy,
};
