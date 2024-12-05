/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_SERVING_NEW_TAB_PAGE_AD_SERVING_DELEGATE_MOCK_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_SERVING_NEW_TAB_PAGE_AD_SERVING_DELEGATE_MOCK_H_

#include "brave/components/brave_ads/core/internal/serving/new_tab_page_ad_serving_delegate.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace brave_ads {

class NewTabPageAdServingDelegateMock : public NewTabPageAdServingDelegate {
 public:
  NewTabPageAdServingDelegateMock();

  NewTabPageAdServingDelegateMock(const NewTabPageAdServingDelegateMock&) =
      delete;
  NewTabPageAdServingDelegateMock& operator=(
      const NewTabPageAdServingDelegateMock&) = delete;

  ~NewTabPageAdServingDelegateMock() override;

  MOCK_METHOD(void, OnOpportunityAroseToServeNewTabPageAd, ());

  MOCK_METHOD(void, OnDidServeNewTabPageAd, (const NewTabPageAdInfo& ad));

  MOCK_METHOD(void, OnFailedToServeNewTabPageAd, ());
};

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_SERVING_NEW_TAB_PAGE_AD_SERVING_DELEGATE_MOCK_H_
