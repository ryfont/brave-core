/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/statement/statement_util.h"

#include "brave/components/brave_ads/core/internal/account/statement/statement_feature.h"
#include "brave/components/brave_ads/core/internal/account/transactions/transactions_test_util.h"
#include "brave/components/brave_ads/core/internal/common/unittest/unittest_base.h"
#include "brave/components/brave_ads/core/internal/common/unittest/unittest_profile_pref_value.h"
#include "brave/components/brave_ads/core/internal/common/unittest/unittest_time_converter_util.h"
#include "brave/components/brave_ads/core/internal/common/unittest/unittest_time_util.h"
#include "brave/components/brave_ads/core/public/prefs/pref_names.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

class BraveAdsStatementUtilTest : public UnitTestBase {};

TEST_F(BraveAdsStatementUtilTest, GetNextPaymentDate) {
  // Arrange
  AdvanceClockTo(TimeFromUTCString("31 January 2020"));

  const base::Time next_token_redemption_at =
      TimeFromUTCString("5 February 2020");
  SetProfileTimePrefValue(prefs::kNextTokenRedemptionAt,
                          next_token_redemption_at);

  // Act
  const base::Time next_payment_date = GetNextPaymentDate(/*transactions=*/{});

  // Assert
  EXPECT_EQ(TimeFromUTCString("7 March 2020 23:59:59.999"), next_payment_date);
}

TEST_F(BraveAdsStatementUtilTest, GetEstimatedEarningsForThisMonth) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020"));

  TransactionList transactions;

  const TransactionInfo transaction_1 = test::BuildUnreconciledTransaction(
      /*value=*/0.02, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_1);

  const TransactionInfo transaction_2 = test::BuildTransaction(
      /*value=*/0.01, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression, /*reconciled_at=*/Now(),
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_2);

  AdvanceClockTo(TimeFromString("25 December 2020"));

  const TransactionInfo transaction_3 = test::BuildUnreconciledTransaction(
      /*value=*/0.0, AdType::kNotificationAd, ConfirmationType::kClicked,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_3);

  const TransactionInfo transaction_4 = test::BuildUnreconciledTransaction(
      /*value=*/0.03, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_4);

  const TransactionInfo transaction_5 = test::BuildUnreconciledTransaction(
      /*value=*/0.02, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_5);

  TransactionInfo transaction_6 = test::BuildUnreconciledTransaction(
      /*value=*/0.02, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transaction_6.ad_type = AdType::kNewTabPageAd;
  transactions.push_back(transaction_6);

  // Act
  const auto [min_estimated_earnings, max_estimated_earnings] =
      GetEstimatedEarningsForThisMonth(transactions);

  // Assert
  EXPECT_DOUBLE_EQ(0.07 * kMinEstimatedEarningsMultiplier.Get(),
                   min_estimated_earnings);
  EXPECT_DOUBLE_EQ(0.09, max_estimated_earnings);
}

TEST_F(BraveAdsStatementUtilTest, GetEstimatedEarningsForLastMonth) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020"));

  TransactionList transactions;

  const TransactionInfo transaction_1 = test::BuildTransaction(
      /*value=*/0.02, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression, /*reconciled_at=*/Now(),
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_1);

  TransactionInfo transaction_2 = test::BuildTransaction(
      /*value=*/0.02, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression, /*reconciled_at=*/Now(),
      /*should_generate_random_uuids=*/true);
  transaction_2.ad_type = AdType::kNewTabPageAd;
  transactions.push_back(transaction_2);

  const TransactionInfo transaction_3 = test::BuildUnreconciledTransaction(
      /*value=*/0.01, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_3);

  AdvanceClockTo(TimeFromString("25 December 2020"));

  const TransactionInfo transaction_4 = test::BuildUnreconciledTransaction(
      /*value=*/0.0, AdType::kNotificationAd, ConfirmationType::kClicked,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_4);

  const TransactionInfo transaction_5 = test::BuildTransaction(
      /*value=*/0.03, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression, /*reconciled_at=*/Now(),
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_5);

  // Act
  const auto [min_estimated_earnings, max_estimated_earnings] =
      GetEstimatedEarningsForLastMonth(transactions);

  // Assert
  EXPECT_DOUBLE_EQ(0.02 * kMinEstimatedEarningsMultiplier.Get(),
                   min_estimated_earnings);
  EXPECT_DOUBLE_EQ(0.04, max_estimated_earnings);
}

TEST_F(BraveAdsStatementUtilTest, GetAdsReceivedThisMonth) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020"));

  TransactionList transactions;

  const TransactionInfo transaction_1 = test::BuildUnreconciledTransaction(
      /*value=*/0.01, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_1);

  AdvanceClockTo(TimeFromString("25 December 2020"));

  const TransactionInfo transaction_2 = test::BuildUnreconciledTransaction(
      /*value=*/0.0, AdType::kNotificationAd, ConfirmationType::kClicked,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_2);

  const TransactionInfo transaction_3 = test::BuildUnreconciledTransaction(
      /*value=*/0.03, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_3);

  const TransactionInfo transaction_4 = test::BuildUnreconciledTransaction(
      /*value=*/0.02, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_4);

  // Act & Assert
  EXPECT_EQ(2, GetAdsReceivedThisMonth(transactions));
}

TEST_F(BraveAdsStatementUtilTest, GetAdsSummaryThisMonth) {
  // Arrange
  AdvanceClockTo(TimeFromString("5 November 2020"));

  TransactionList transactions;

  const TransactionInfo transaction_1 = test::BuildUnreconciledTransaction(
      /*value=*/0.01, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_1);

  AdvanceClockTo(TimeFromString("25 December 2020"));

  const TransactionInfo transaction_2 = test::BuildUnreconciledTransaction(
      /*value=*/0.0, AdType::kNotificationAd, ConfirmationType::kClicked,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_2);

  const TransactionInfo transaction_3 = test::BuildUnreconciledTransaction(
      /*value=*/0.03, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_3);

  const TransactionInfo transaction_4 = test::BuildUnreconciledTransaction(
      /*value=*/0.02, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_4);

  // Act
  const base::flat_map<std::string, int32_t> ads_summary =
      GetAdsSummaryThisMonth(transactions);

  // Assert
  const base::flat_map<std::string, int32_t> expected_ads_summary = {
      {"ad_notification", 2}};
  EXPECT_EQ(expected_ads_summary, ads_summary);
}

}  // namespace brave_ads
