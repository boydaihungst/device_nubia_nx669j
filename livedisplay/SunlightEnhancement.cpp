/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "SunlightEnhancementService"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include "SunlightEnhancement.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

static constexpr const char* kHbmModePath = "/sys/kernel/lcd_enhance/hbm_mode";
// Lock hbm mode
static constexpr const char* kLHbmModePath = "/sys/kernel/lcd_enhance/lhbm_mode";
// hbm value
static constexpr const char* kHbmStatePath = "/sys/kernel/lcd_enhance/hbm_state";

Return<bool> SunlightEnhancement::isEnabled() {
    std::string buf;
    if (!android::base::ReadFileToString(kHbmModePath, &buf)) {
        LOG(ERROR) << "Failed to read " << kHbmModePath;
        return false;
    }
      std::string buf2;

    if (!android::base::ReadFileToString(kHbmStatePath, &buf2)) {
        LOG(ERROR) << "Failed to read " << kHbmStatePath;
        return false;
    }
    return std::stoi(android::base::Trim(buf)) == 1 && std::stoi(android::base::Trim(buf2)) == 4080;
}

Return<bool> SunlightEnhancement::setEnabled(bool enabled) {
    // Lock brightness
    if (!android::base::WriteStringToFile((enabled ? "1" : "0"), kLHbmModePath)) {
        LOG(ERROR) << "Failed to write " << kLHbmModePath;
        return false;
    }
    if (enabled) {
      // enable hbm
      if (!android::base::WriteStringToFile("4080", kHbmStatePath)) {
          LOG(ERROR) << "Failed to write " << kHbmStatePath;
          return false;
      }
    }
    return true;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
