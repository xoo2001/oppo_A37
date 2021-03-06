/*
 * Copyright 2017 The LineageOS Project
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

#define LOG_TAG "android.hardware.light@2.0-service.A37"

/* dev-harsh1998: set page size to 32Kb for our hal */
#include <hwbinder/ProcessState.h>
#include <cutils/properties.h>

#include <hidl/HidlTransportSupport.h>

#include "Light.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using android::hardware::light::V2_0::ILight;
using android::hardware::light::V2_0::implementation::Light;

using android::OK;
using android::sp;
using android::status_t;

#define DEFAULT_LGTHAL_HW_BINDER_SIZE_KB 32
size_t getHWBinderMmapSize() {
    size_t value = 0;
    value = property_get_int32("persist.vendor.A37.lighthal.hw.binder.size", DEFAULT_LGTHAL_HW_BINDER_SIZE_KB);
    if (!value) value = DEFAULT_LGTHAL_HW_BINDER_SIZE_KB; // deafult to 1 page of 32 Kb
     return 1024 * value;
}

int main() {
    /* default to 32Kb */
    android::hardware::ProcessState::initWithMmapSize(getHWBinderMmapSize());
    android::sp<ILight> service = new Light();

    configureRpcThreadpool(1, true);

    status_t status = service->registerAsService();
    if (status != OK) {
        ALOGE("Cannot register Light HAL service.");
        return 1;
    }

    ALOGI("Light HAL service ready.");

    joinRpcThreadpool();

    ALOGI("Light HAL service failed to join thread pool.");
    return 1;
}
