/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_COMMON_TEST_FILE_PATH_TEST_UTIL_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_COMMON_TEST_FILE_PATH_TEST_UTIL_H_

namespace base {
class FilePath;
}  // namespace base

namespace brave_ads::test {

base::FilePath DataPath();

base::FilePath ComponentResourcesDataPath();

base::FilePath DataResourcesPath();

}  // namespace brave_ads::test

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_COMMON_TEST_FILE_PATH_TEST_UTIL_H_
