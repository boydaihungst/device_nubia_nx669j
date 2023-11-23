/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "GloveModeService"

#include "GloveMode.h"

#include <fstream>

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

const std::string kGloveModePath =
    "/sys/devices/platform/nubia_goodix_ts.0/gesture/sensitivity";

Return<bool> GloveMode::isEnabled() {
    std::ifstream file(kGloveModePath);
    int enabled;
    file >> enabled;

    return enabled == 1;
}

Return<bool> GloveMode::setEnabled(bool enabled) {
    std::ofstream file(kGloveModePath);
    file << (enabled ? "1" : "0");
    return !file.fail();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
