/* $Id: datetimetest.cpp,v 1.1.2.1.2.4 2012/10/09 13:00:08 ianbell Exp $
 *
 * OpenMAMA: The open middleware agnostic messaging API
 * Copyright (C) 2011 NYSE Technologies, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

/*  Description: These tests will check the creation and manipulation of
 *               the mamaDateTime format.
 */

#include "gtest/gtest.h"
#include "mama/mama.h"
#include "mama/timezone.h"
#include "mama/status.h"
#include "MainUnitTestC.h"
#include "mama/datetime.h"
#include <iostream>

/* Only for 32 bit unix / linux systsme */
#if defined(__i386__) && (defined(unix) || defined(__unix__) || defined(__unix))
    #define setMamaDateTimeExtendedValue(target, source_s, source_ns)          \
        mama_f64_t smdtev_f64 = (mama_f64_t)source_s + (((mama_f64_t)(source_ns / 1000)) / 1000000.0); \
        EXPECT_EQ(MAMA_STATUS_OK, mamaDateTime_setEpochTimeF64(target, smdtev_f64))

    #define getMamaDateTimeExtendedValue(source, target_s, target_ns)          \
        mama_f64_t gmdtev_f64;                                                 \
        EXPECT_EQ(MAMA_STATUS_OK, mamaDateTime_getEpochTimeSeconds(source, &gmdtev_f64)); \
        target_s = (int64_t) gmdtev_f64;                                       \
        target_ns = (long) ((gmdtev_f64 - (mama_f64_t)target_s) * 1000000.0);  \
        target_ns *= 1000;
    #define checkExtendedNanosecondEquality(expected, actual)                  \
        EXPECT_NEAR(expected, actual, 10000)
    #define checkExtendedMicrosecondEquality(expected, actual)                 \
        EXPECT_NEAR(expected, actual, 10)
#else
    #define setMamaDateTimeExtendedValue(target, source_s, source_ns)          \
        struct timespec smdtev_ts = { (time_t)source_s, source_ns };           \
        mamaDateTime_setFromStructTimeSpec(target, &smdtev_ts)

    #define getMamaDateTimeExtendedValue(source, target_s, target_ns)          \
        struct timespec gmdtev_ts;                                             \
        mamaDateTime_getStructTimeSpec(source, &gmdtev_ts);                    \
        target_s = gmdtev_ts.tv_sec;                                           \
        target_ns = gmdtev_ts.tv_nsec
    #define checkExtendedNanosecondEquality(expected, actual)                  \
        EXPECT_EQ(expected, actual)
    #define checkExtendedMicrosecondEquality(expected, actual)                 \
        EXPECT_EQ(expected, actual)

#endif


class MamaDateTimeTestC : public ::testing::Test
{
protected:

    MamaDateTimeTestC(void);
    virtual ~MamaDateTimeTestC(void);

    virtual void SetUp(void);
    virtual void TearDown(void);
    mamaBridge mBridge;
};

MamaDateTimeTestC::MamaDateTimeTestC(void)
{
}

MamaDateTimeTestC::~MamaDateTimeTestC(void)
{
}

void
MamaDateTimeTestC::SetUp(void)
{
    mama_loadBridge (&mBridge, getMiddleware());
    mama_open();
}

void
MamaDateTimeTestC::TearDown(void)
{
    mama_close();
}

// Test Create function with valid and NULL parameters
TEST_F (MamaDateTimeTestC, TestCreate)
{
    mamaDateTime t = NULL;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

// Test Destroy function with valid and NULL parameters
TEST_F (MamaDateTimeTestC, TestDestroy)
{
    mamaDateTime t, nullTime = NULL;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_destroy(nullTime) );
}

// Test Clear function with valid and NULL parameters
TEST_F (MamaDateTimeTestC, TestClear)
{
    mamaDateTime t, nullTime = NULL;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_clear(t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_clear(nullTime) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

// Test ClearDate function with valid and NULL parameters
TEST_F (MamaDateTimeTestC, TestClearDate)
{
    mamaDateTime t, nullTime = NULL;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_clearDate(t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_clearDate(nullTime) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

// Test ClearTime function with valid and NULL parameters
TEST_F (MamaDateTimeTestC, TestClearTime)
{
    mamaDateTime t, nullTime = NULL;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_clearTime(t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_clearTime(nullTime) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

// Test Copy function with valid and NULL parameters
TEST_F (MamaDateTimeTestC, TestCopy)
{
    mamaDateTime t,u, nullTime = NULL;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&u) );

    // Test will NULL parameters
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_copy(t, nullTime) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_copy(nullTime, t) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_copy(nullTime, nullTime) );

    // Test wil valid parameters
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_copy(t,u) );
    EXPECT_EQ ( 1, mamaDateTime_equal(t,u) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(u) );
}


// Test Empty function with valid and NULL parameters
TEST_F (MamaDateTimeTestC, TestEmpty)
{
    mamaDateTime t, nullTime = NULL;

    // Test with NULL paremeters
    EXPECT_EQ ( 1, mamaDateTime_empty(nullTime) );

    // Test with Valid parematers
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( 1, mamaDateTime_empty(t) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

// Test Equal function with valid and NULL parameters
TEST_F (MamaDateTimeTestC, TestEqual)
{
    mamaDateTime t, u, nullTime = NULL;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&u) );

    // Test with NULL paremeters
    EXPECT_EQ ( 1, mamaDateTime_equal(nullTime, nullTime) );
    EXPECT_EQ ( 0, mamaDateTime_equal(t, nullTime) );
    EXPECT_EQ ( 0, mamaDateTime_equal(nullTime, t) );
    EXPECT_EQ ( 1, mamaDateTime_equal(t, t) );

    // Test with Valid Parameters
    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_setToNow(u) );
    ASSERT_NE ( 1, mamaDateTime_equal(t,u) );
    ASSERT_NE ( 1, mamaDateTime_equal(u,t) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(u) );
}


// Test SetEpochTime with valid and invalid input parameters
TEST_F (MamaDateTimeTestC, TestSetEpochTime)
{
    mamaDateTime t, nullTime = NULL;
    mama_u32_t secs    = 43200,  negSecs    = secs * -1,
               micSecs = 500000,
               secsPerDay = (24 * 60 * 60),
               secsPer2Days = secsPerDay * 2;
    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setEpochTime(nullTime, secs, micSecs, precision) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setEpochTime(nullTime, negSecs, micSecs, precision) );

    // Test with valid arguments
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, negSecs, micSecs, precision) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secs, micSecs, precision) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secsPerDay, micSecs, precision) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secsPer2Days, micSecs, precision) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

// Test Compare method iwth valid and invalid input parameters
TEST_F (MamaDateTimeTestC, TestCompare)
{
    mamaDateTime t, u, nullTime = NULL;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&u) );

    EXPECT_EQ ( 0, mamaDateTime_compare(nullTime, nullTime) );
    EXPECT_EQ ( 0, mamaDateTime_compare(t, nullTime) );
    EXPECT_EQ ( 0, mamaDateTime_compare(nullTime, t) );

    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_setToNow(u) );
    // t is less than u

    EXPECT_EQ ( -1, mamaDateTime_compare(t,u) );
    EXPECT_EQ ( 1, mamaDateTime_compare(u,t) );
    EXPECT_EQ ( 0, mamaDateTime_compare(t, t) );
    EXPECT_EQ ( 0, mamaDateTime_compare(u, u) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(u) );
}

// Test Set Epoch Time F64 with valid and invalid parameters
TEST_F (MamaDateTimeTestC, TestSetEpochTimeF64)
{
    mamaDateTime t, nullTime = NULL;
    mama_f64_t secs    = 43200,  negSecs    = secs * -1,
               secsPerDay = (24 * 60 * 60),
               secsPer2Days = secsPerDay * 2;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setEpochTimeF64(nullTime, secs) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setEpochTimeF64(nullTime, negSecs) );

    // Test with valid input arguments
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeF64(t, secs) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeF64(t, negSecs) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeF64(t, secsPerDay) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeF64(t, secsPer2Days) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

// Test Set Epoch Time Millisecond with valid and invalid parameters
TEST_F (MamaDateTimeTestC, TestSetEpochTimeMilliseconds)
{
    mamaDateTime t, nullTime = NULL;
    mama_u64_t mSecs      = 43200 * 1000,  negMSecs = mSecs * -1,
               mSecsPerDay = (24 * 60 * 60) * 1000,
               mSecsPer2Days = mSecsPerDay * 2;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setEpochTimeMilliseconds(nullTime, mSecs) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setEpochTimeMilliseconds(nullTime, negMSecs) );

    // Test with valid input arguments
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeMilliseconds(t, mSecs) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeMilliseconds(t, negMSecs) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeMilliseconds(t, mSecsPerDay) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeMilliseconds(t, mSecsPer2Days) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

// Test Set Epoch Time Microseconds with valid and invalid parameters
TEST_F (MamaDateTimeTestC, TestSetEpochTimeMicroseconds)
{
    mamaDateTime t, nullTime = NULL;
    mama_u64_t m1 = 43200, m2 = 1000, m3 = 1000,
               mSecs       = m1 * m2 * m3,  negMSecs = mSecs * -1,
               mSecsPerDay = (24 * 60 * 60) * m2 * m3,
               mSecsPer2Days = mSecsPerDay * 2;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setEpochTimeMicroseconds(nullTime, mSecs) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setEpochTimeMilliseconds(nullTime, negMSecs) );

    // Test with valid input arguments
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeMicroseconds(t, mSecs) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeMicroseconds(t, negMSecs) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeMicroseconds(t, mSecsPerDay) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeMicroseconds(t, mSecsPer2Days) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

// Test Set With Hints with valid and invalid parameters
TEST_F (MamaDateTimeTestC, TestSetWithHints)
{
    mamaDateTime t, nullTime = NULL;
    mama_u64_t  secs = 43200, negSecs = secs * -1,
               micSecs = 500 * 1000,
               secsPerDay = (24 * 60 * 60);
    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;
    mamaDateTimeHints hints =  MAMA_DATE_TIME_HAS_DATE;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setWithHints(nullTime, secs, micSecs, precision, hints) );

    // Test with valid input arguments
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setWithHints(t, secs, micSecs, precision, hints) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setWithHints(t, negSecs, micSecs, precision, hints) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setWithHints(t, secsPerDay, micSecs, precision, hints) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

// Test Set Precsiosn with valid and invalid parameters
TEST_F (MamaDateTimeTestC, TestSetPrecision)
{
    mamaDateTime t, nullTime = NULL;
    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setPrecision(nullTime, precision) );

    // Test with valid arguments
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setPrecision(t, precision) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

// Test Set To Now with valid and invalid parameters
TEST_F (MamaDateTimeTestC, TestSetToNow)
{
    mamaDateTime t, nullTime = NULL;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setToNow(nullTime) );

    // Test with valid arguments
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}


TEST_F (MamaDateTimeTestC, TestSetToMidnightToday)
{
    mamaDateTime t = NULL, nullTime = NULL;
    mamaTimeZone z = NULL, nullTimeZone = NULL;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setToMidnightToday(nullTime, nullTimeZone) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setToMidnightToday(nullTime, z) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToMidnightToday(t, z) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToMidnightToday(t, nullTimeZone) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestSetWithPrecisionAndTzTest)
{
    mamaDateTime t, nullTime = NULL;
    mamaTimeZone tz  = mamaTimeZone_utc(), nullTz = NULL;
    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;
    // These are all valid values
    mama_u32_t year = 2013, month = 1, day = 7, hour = 9, minute = 21, second = 12, microsecond = 500;

   // These are valid values which will get converted by the underlying implementation.
    mama_u32_t invyear = -2013, invmonth = 13, invday = 47, invhour = 29, invminute = 121, invsecond = 712;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setWithPrecisionAndTz(nullTime, year, month, day, hour, minute,
                                                                            second, microsecond, precision, nullTz) );

    // Test with valid argumetns
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setWithPrecisionAndTz(t, invyear, invmonth, invday, invhour, invminute,
                                                                            invsecond, microsecond, precision, tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setWithPrecisionAndTz(t, year, month, day, hour, minute,
                                                                   second, microsecond, precision, tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestSetTimeTest)
{
    mamaDateTime t, nullTime = NULL;
    // These are all valid values
    mama_u32_t hour = 9, minute = 21, second = 12, microsecond = 500;

   // Larger values, which are converted by the implemenation.
    mama_u32_t invhour = 29, invminute = 121, invsecond = 712;


    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setTime(nullTime, hour, minute, second, microsecond) );

    // Test with valid arguments
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setTime(t, hour, minute, invsecond, microsecond) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setTime(t, hour, invminute, second, microsecond) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setTime(t, invhour, minute, second, microsecond) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setTime(t, hour, minute, second, microsecond) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}


TEST_F (MamaDateTimeTestC, TestSetTimeWithPrecisionTzTest)
{
    mamaDateTime t, nullTime = NULL;
    mamaTimeZone tz  = mamaTimeZone_utc(), nullTz = NULL;
    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;
    // These are all valid values
    mama_u32_t hour = 9, minute = 21, second = 12, microsecond = 500;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setTimeWithPrecisionAndTz(nullTime, hour, minute, second, microsecond, precision, tz) );

    // Test with valid arguments
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setTimeWithPrecisionAndTz(t, hour, minute, second, microsecond, precision, nullTz) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setTimeWithPrecisionAndTz(t, hour, minute, second, microsecond, precision, tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestSetDate)
{
    mamaDateTime t, nullTime = NULL;
    mama_u32_t year = 2013,         month = 6,     day = 18,
               invYear = year * -1, invMonth = 15, invDay = 35;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    // Test with invalid arguments
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setDate(nullTime, year, month, day) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setDate(t, year, month, invDay) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setDate(t, year, invMonth, day) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setDate(t, invYear, month, day) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setDate(t, year, month, day) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestCopyTime)
{
    mamaDateTime t1, t2, nullTime = NULL;
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t2) );

    mama_u32_t hour = 10, minute = 6, second = 18, microsecond = 123;

    // Test with valid parameters
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_copyTime(nullTime,nullTime) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_copyTime(t1,nullTime) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_copyTime(nullTime,t2) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t2) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setTime(t2, hour, minute, second, microsecond) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_copyTime(t1,t2) );
    EXPECT_EQ ( 1, mamaDateTime_equal(t1,t2) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t2) );
}

TEST_F (MamaDateTimeTestC, TestCopyDate)
{
    mamaDateTime t1, t2, nullTime = NULL;
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t2) );
    mama_u32_t year = 2009, month = 6, day = 18;

    // Test with valid parameters
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_copyDate(nullTime,nullTime) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_copyDate(t1,nullTime) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_copyDate(nullTime,t2) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_copy(t2,t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setDate(t2, year, month, day) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_copyDate(t1,t2) );
    EXPECT_EQ ( 1, mamaDateTime_equal(t1,t2) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t2) );
}

TEST_F (MamaDateTimeTestC, TestHasTime)
{
    mamaDateTime t, nullTime = NULL;
    mama_bool_t b;
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_hasTime(nullTime, &b) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_hasTime(t, &b) );
    EXPECT_TRUE ( b );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestHasDate)
{
    mamaDateTime t, nullTime = NULL;
    mama_bool_t b;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_hasDate(nullTime, &b) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_hasDate(t, &b) );
    EXPECT_TRUE ( b );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestAddSeconds)
{
    mamaDateTime t, nullTime = NULL;
    mama_f64_t hour = 3600, minute = 60, second = 1, lessThanOneMin = 20, greaterThanOneMin = 520;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_addSeconds(nullTime, hour) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addSeconds(t, hour) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addSeconds(t, minute) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addSeconds(t, second) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addSeconds(t, lessThanOneMin) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addSeconds(t, greaterThanOneMin) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestAddWholeSeconds)
{
    mamaDateTime t, nullTime = NULL;
    mama_i32_t hour = 3600, minute = 60, second = 1, lessThanOneMin = 20, greaterThanOneMin = 520;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_addWholeSeconds(nullTime, hour) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addWholeSeconds(t, hour) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addWholeSeconds(t, minute) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addWholeSeconds(t, second) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addWholeSeconds(t, lessThanOneMin) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addWholeSeconds(t, greaterThanOneMin) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestAddMicroSeconds)
{
    mamaDateTime t, nullTime = NULL;
    mama_i64_t mic = 1, milli = mic * 1000, sec = milli * 1000, minute = sec * 60, hour = minute * 60, day = hour * 24;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_addMicroseconds(nullTime, mic) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addMicroseconds(t, mic) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addMicroseconds(t, milli) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addMicroseconds(t, sec) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addMicroseconds(t, minute) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addMicroseconds(t, hour) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_addMicroseconds(t, day) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestSetFromString)
{
    mamaDateTime t, nullTime = NULL;

    // Format a string using today's date and a time,
    // 4th July
    std::string szGood = "2013-07-04 10:03:21.123";
    // 31st June
    std::string szInvalid = "2013-06-31 10:03:21.123";
    // Garbage
    std::string szBad = "13th Aquarius 429 BC";
    // NULL String
    std::string szNull;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setFromString(nullTime, szGood.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szInvalid.c_str()) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szBad.c_str()) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szNull.c_str()) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szGood.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestSetFromStringWithTz)
{
    mamaDateTime t, nullTime = NULL;

    mamaTimeZone tz  = mamaTimeZone_utc(), nullTz = NULL;;

    // Format a string using today's date and a time,
    // 4th July
    std::string szGood = "2013-07-04 10:03:21.123";
    // 31st June
    std::string szInvalid = "2013-06-31 10:03:21.123";
    // Garbage
    std::string szBad = "13th Aquarius 429 BC";
    // NULL String
    std::string szNull;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setFromStringWithTz(nullTime, szGood.c_str(), tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringWithTz(t, szInvalid.c_str(), nullTz) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringWithTz(t, szBad.c_str(), nullTz) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringWithTz(t, szNull.c_str(), nullTz) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringWithTz(t, szGood.c_str(), nullTz) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringWithTz(t, szGood.c_str(), tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestSetFromStringBuffer)
{
    mamaDateTime t, nullTime = NULL;
    mama_size_t  bigBuff = 100, smallBuff = 1;

    // Format a string using today's date and a time,
    // 4th July
    std::string szGood = "2013-07-04 10:03:21.123";
    // 31st June
    std::string szInvalid = "2013-06-31 10:03:21.123";
    // Garbage
    std::string szBad = "13th Aquarius 429 BC";
    // NULL String
    std::string szNull;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setFromStringBuffer(nullTime, szGood.c_str(), bigBuff) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringBuffer(t, szInvalid.c_str(), bigBuff) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringBuffer(t, szBad.c_str(), bigBuff) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringBuffer(t, szNull.c_str(), bigBuff) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringBuffer(t, szGood.c_str(), bigBuff) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringBuffer(t, szGood.c_str(), smallBuff) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestSetFromStringBufferWIthTz)
{
    mamaDateTime t, nullTime = NULL;
    mama_size_t  bigBuff = 100, smallBuff = 1;
    mamaTimeZone tz  = mamaTimeZone_utc(), nullTz = NULL;;

    // Format a string using today's date and a time,
    // 4th July
    std::string szGood = "2013-07-04 10:03:21.123";
    // 31st June
    std::string szInvalid = "2013-06-31 10:03:21.123";
    // Garbage
    std::string szBad = "13th Aquarius 429 BC";
    // NULL String
    std::string szNull;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setFromStringBufferWithTz(nullTime, szGood.c_str(), bigBuff, nullTz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringBufferWithTz(t, szInvalid.c_str(), bigBuff, nullTz) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringBufferWithTz(t, szBad.c_str(), bigBuff, nullTz) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringBufferWithTz(t, szNull.c_str(), bigBuff, nullTz) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringBufferWithTz(t, szGood.c_str(), bigBuff, nullTz) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringBufferWithTz(t, szGood.c_str(), smallBuff, nullTz) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStringBufferWithTz(t, szGood.c_str(), bigBuff, tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTime)
{
    mamaDateTime t, nullTime = NULL;

    mama_u32_t secs    = 43200,  expectedSecs,
               micSecs = 500000, expectedMicSecs;

    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS,
                          expectedPrecision;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secs, micSecs, precision) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getEpochTime(nullTime, &expectedSecs, &expectedMicSecs, &expectedPrecision) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTime(t, &expectedSecs, &expectedMicSecs, &expectedPrecision) );
    EXPECT_EQ ( secs, expectedSecs );
    EXPECT_EQ ( micSecs, expectedMicSecs );
    EXPECT_EQ ( precision, expectedPrecision );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeWithTz)
{
    mamaDateTime t, nullTime = NULL;

    mama_u32_t secs    = 43200,  expectedSecs,
               micSecs = 500000, expectedMicSecs;

    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS,
                          expectedPrecision;

    mamaTimeZone tz  = mamaTimeZone_utc();

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secs, micSecs, precision) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getEpochTimeWithTz(nullTime, &expectedSecs, &expectedMicSecs, &expectedPrecision, tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTimeWithTz(t, &expectedSecs, &expectedMicSecs, &expectedPrecision, tz) );
    EXPECT_EQ ( secs, expectedSecs );
    EXPECT_EQ ( micSecs, expectedMicSecs );
    EXPECT_EQ ( precision, expectedPrecision );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeMicroseconds)
{
    mamaDateTime t, nullTime = NULL;

    mama_u32_t secs32    = 43219,  micSecs32 = 500000;
    mama_u64_t secs64    = secs32, micSecs64 = micSecs32,
               expectedMicSecs = (secs64 * 1000000) + micSecs64, actualMicSecs;

    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secs32, micSecs32, precision) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getEpochTimeMicroseconds(nullTime, &actualMicSecs) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTimeMicroseconds(t, &actualMicSecs) );
    EXPECT_EQ ( actualMicSecs, expectedMicSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeMicrosecondsWithTz)
{
    mamaDateTime t, nullTime = NULL;

    mamaTimeZone tz  = mamaTimeZone_utc();

    mama_u32_t secs32    = 43219,  milSecs32 = 123, micSecs32 = (milSecs32 * 1000) + 456;
    mama_u64_t secs64    = secs32, micSecs64 = micSecs32,
               expectedMicSecs = (secs64 * 1000000) + micSecs64, actualMicSecs;

    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secs32, micSecs32, precision) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getEpochTimeMicrosecondsWithTz(nullTime, &actualMicSecs, tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTimeMicrosecondsWithTz(t, &actualMicSecs, tz) );
    EXPECT_EQ ( actualMicSecs, expectedMicSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeMilliseconds)
{
    mamaDateTime t, nullTime = NULL;

    mama_u32_t secs32    = 43219,  milSecs32 = 123, micSecs32 = (milSecs32 * 1000) + 456;
    mama_u64_t secs64    = secs32, milSecs64 = milSecs32,
               expectedMilSecs = (secs64 * 1000) + milSecs64, actualMilSecs;

    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secs32, micSecs32, precision) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getEpochTimeMilliseconds(nullTime, &actualMilSecs) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTimeMilliseconds(t, &actualMilSecs) );
    EXPECT_EQ ( actualMilSecs, expectedMilSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeMillisecondsWithTz)
{
    mamaDateTime t, nullTime = NULL;

    mamaTimeZone tz  = mamaTimeZone_utc();

    mama_u32_t secs32    = 43219,  milSecs32 = 123, micSecs32 = (milSecs32 * 1000) + 456;
    mama_u64_t secs64    = secs32, milSecs64 = milSecs32,
               expectedMilSecs = (secs64 * 1000) + milSecs64, actualMilSecs;

    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secs32, micSecs32, precision) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getEpochTimeMillisecondsWithTz(nullTime, &actualMilSecs, tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTimeMillisecondsWithTz(t, &actualMilSecs, tz) );
    EXPECT_EQ ( actualMilSecs, expectedMilSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeSeconds)
{
    mamaDateTime t, nullTime = NULL;

    mama_u32_t secs32    = 43219,  milSecs32 = 123, micSecs32 = (milSecs32 * 1000) + 456;
    mama_f64_t expectedSecs, actualSecs;

    /* The returned value will also take into account the microseconds value, so
     * this should be factored into the expected value calculation.
     */
    expectedSecs = secs32 + (mama_f64_t)(micSecs32 / 1000000.0);

    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secs32, micSecs32, precision) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getEpochTimeSeconds(nullTime, &actualSecs) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTimeSeconds(t, &actualSecs) );
    EXPECT_EQ ( actualSecs, expectedSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeSecondsWithCheck)
{
    mamaDateTime t, nullTime = NULL;

    mama_u32_t secs32    = 432190,  milSecs32 = 123, micSecs32 = (milSecs32 * 1000) + 456;
    mama_f64_t expectedSecs, actualSecs;

    /* In this case, we pass in a value for the seconds which is greater than
     * those in single day. This ensures that the check for getting the time
     * doesn't try and add the current date, making our expected calculation
     * more straightforward.
     */
    expectedSecs = secs32 + (mama_f64_t)(micSecs32 / 1000000.0);

    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secs32, micSecs32, precision) );

    EXPECT_EQ ( MAMA_STATUS_NULL_ARG, mamaDateTime_getEpochTimeSecondsWithCheck(nullTime, &actualSecs) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTimeSecondsWithCheck(t, &actualSecs) );
    EXPECT_EQ ( actualSecs, expectedSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeSecondsWithTz)
{
    mamaDateTime t, nullTime = NULL;

    mamaTimeZone tz  = mamaTimeZone_utc();

    mama_u32_t secs32    = 43219,  milSecs32 = 123, micSecs32 = (milSecs32 * 1000) + 456;
    mama_f64_t expectedSecs, actualSecs;

    /* Since we're using the UTC timezone, we don't need to take into account
     * the offset at this point, though the test could be improved to consider
     * it.
     */
    expectedSecs = secs32 + (mama_f64_t)(micSecs32 / 1000000.0);

    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTime(t, secs32, micSecs32, precision) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getEpochTimeSecondsWithTz(nullTime, &actualSecs, tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTimeSecondsWithTz(t, &actualSecs, tz) );
    EXPECT_EQ ( actualSecs, expectedSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetWithHints)
{
    mamaDateTime t, nullTime = NULL;

    mamaDateTimePrecision actualPrecision = MAMA_DATE_TIME_PREC_MICROSECONDS, expectedPrecision;

    mama_u32_t actualSecs      = 43219,  expectedSecs,
               actualMilliSecs = 123,
               actualMicroSecs = (actualMilliSecs * 1000) + 456, expectedMicroSecs;

    mamaDateTimeHints actualHints = MAMA_DATE_TIME_HAS_DATE, expectedHints;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    // Test with valid input arguments
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setWithHints(t, actualSecs, actualMicroSecs, actualPrecision, actualHints) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getWithHints(nullTime, &expectedSecs, &expectedMicroSecs, &expectedPrecision, &expectedHints) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getWithHints(t, &expectedSecs, &expectedMicroSecs, &expectedPrecision, &expectedHints) );
    EXPECT_EQ ( actualSecs, expectedSecs );
    EXPECT_EQ ( actualMicroSecs, expectedMicroSecs );
    EXPECT_EQ ( actualPrecision, expectedPrecision );
    EXPECT_EQ ( actualHints, expectedHints );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetStructTimeVal)
{
    struct timeval tVal;

    mamaDateTime t, nullTime = NULL;

    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;

    mama_u32_t secs      = 43219, mSecs = 123, uSecs = (mSecs * 1000) + 456;

    mamaDateTimeHints hints =  MAMA_DATE_TIME_HAS_DATE;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setWithHints(t, secs, uSecs, precision, hints) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTimeVal(nullTime, NULL) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTimeVal(nullTime, &tVal) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTimeVal(t, NULL) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTimeVal(t, &tVal) );
    EXPECT_EQ ( tVal.tv_sec, secs );
    EXPECT_EQ ( tVal.tv_usec, uSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetStructTimeValWithTz)
{
    struct timeval tVal;

    mamaDateTime t, nullTime = NULL;

    mamaTimeZone tz  = mamaTimeZone_utc();

    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;

    mama_u32_t secs      = 43219, mSecs = 123, uSecs = (mSecs * 1000) + 456;

    mamaDateTimeHints hints =  MAMA_DATE_TIME_HAS_DATE;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setWithHints(t, secs, uSecs, precision, hints) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTimeValWithTz(nullTime, NULL, tz) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTimeValWithTz(nullTime, &tVal, tz) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTimeValWithTz(t, NULL, tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTimeValWithTz(t, &tVal, tz) );
    EXPECT_EQ ( tVal.tv_sec, secs );
    EXPECT_EQ ( tVal.tv_usec, uSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetStructTm)
{
    struct tm tM;

    mamaDateTime t, nullTime = NULL;

    int sec = 21, min = 3, hour = 10, mday = 4, mon = 6, year = 113, wday = 4, yday = 184;

    std::string szTime = "2013-07-04 10:03:21.123";

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTm(nullTime, NULL) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTm(nullTime, &tM) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTm(t, NULL) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTm(t, &tM) );
    EXPECT_EQ ( sec, tM.tm_sec );
    EXPECT_EQ ( min, tM.tm_min );
    EXPECT_EQ ( hour, tM.tm_hour );
    EXPECT_EQ ( mday, tM.tm_mday );
    EXPECT_EQ ( mon, tM.tm_mon );
    EXPECT_EQ ( year, tM.tm_year );
    EXPECT_EQ ( wday, tM.tm_wday );
    EXPECT_EQ ( yday, tM.tm_yday );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetStructTmWithTz)
{
    struct tm tM;

    mamaDateTime t, nullTime = NULL;

    mamaTimeZone tz  = mamaTimeZone_utc();

    int sec = 21, min = 3, hour = 10, mday = 4, mon = 6, year = 113, wday = 4, yday = 184;

    std::string szTime = "2013-07-04 10:03:21.123";

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTmWithTz(nullTime, NULL, tz) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTmWithTz(nullTime, &tM, tz) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTmWithTz(t, NULL, tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTmWithTz(t, &tM, tz) );
    EXPECT_EQ ( sec, tM.tm_sec );
    EXPECT_EQ ( min, tM.tm_min );
    EXPECT_EQ ( hour, tM.tm_hour );
    EXPECT_EQ ( mday, tM.tm_mday );
    EXPECT_EQ ( mon, tM.tm_mon );
    EXPECT_EQ ( year, tM.tm_year );
    EXPECT_EQ ( wday, tM.tm_wday );
    EXPECT_EQ ( yday, tM.tm_yday );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetAsString)
{
    mamaDateTime t, nullTime = NULL;

    std::string szActualTime = "2013-07-04 10:03:21.123";
    char szBuff[50];

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szActualTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getAsString(nullTime, szBuff, 50) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getAsString(t, szBuff, 50) );
    EXPECT_STREQ ( szActualTime.c_str(), szBuff );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetAsStringTimeOnly)
{
    mamaDateTime t = NULL;

    const char* stringTime = "10:03:21.123";
    char stringBuffer[50];

    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_create (&t));
    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_setFromString (t, stringTime));

    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_getAsString (t, stringBuffer, 50));
    EXPECT_STREQ (stringTime, stringBuffer);

    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_destroy (t));
}

TEST_F (MamaDateTimeTestC, TestGetAsStringDateOnly)
{
    mamaDateTime t = NULL;

    const char* stringTime = "2013-07-04";
    char stringBuffer[50];

    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_create (&t));
    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_setFromString (t, stringTime));

    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_getAsString (t, stringBuffer, 50));
    EXPECT_STREQ (stringTime, stringBuffer);

    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_destroy (t));
}

TEST_F (MamaDateTimeTestC, TestGetTimeAsString)
{
    mamaDateTime t, nullTime = NULL;

    std::string szActualTime = "2013-07-04 10:03:21.123",
                szExpectedTime = "10:03:21.123";
    char szBuff[50];

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szActualTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getTimeAsString(nullTime, szBuff, 50) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getTimeAsString(t, szBuff, 50) );
    EXPECT_STREQ ( szExpectedTime.c_str(), szBuff );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetDateAsString)
{
    mamaDateTime t, nullTime = NULL;

    std::string szActualTime = "2013-07-04 10:03:21.123",
                szExpectedDate = "2013-07-04";
    char szBuff[50];

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szActualTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getDateAsString(nullTime, szBuff, 50) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getDateAsString(t, szBuff, 50) );
    EXPECT_STREQ ( szExpectedDate.c_str(), szBuff );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetYear)
{
    mamaDateTime t, nullTime = NULL;

    std::string szActualTime = "2013-07-04 10:03:21.123";

    mama_u32_t act, expected = 2013;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szActualTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getYear(nullTime, &act) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getYear(t, &act) );
    EXPECT_EQ ( act, expected );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetMonth)
{
    mamaDateTime t, nullTime = NULL;

    std::string szActualTime = "2013-07-04 10:03:21.123";

    mama_u32_t act, expected = 7;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szActualTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getMonth(nullTime, &act) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getMonth(t, &act) );
    EXPECT_EQ ( act, expected );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetDay)
{
    mamaDateTime t, nullTime = NULL;

    std::string szActualTime = "2013-07-04 10:03:21.123";

    mama_u32_t act, expected = 4;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szActualTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getDay(nullTime, &act) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getDay(t, &act) );
    EXPECT_EQ ( act, expected );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetHour)
{
    mamaDateTime t, nullTime = NULL;

    std::string szActualTime = "2013-07-04 10:03:21.123";

    mama_u32_t act, expected = 10;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szActualTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getHour(nullTime, &act) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getHour(t, &act) );
    EXPECT_EQ ( act, expected );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetMinute)
{
    mamaDateTime t, nullTime = NULL;

    std::string szActualTime = "2013-07-04 10:03:21.123";

    mama_u32_t act, expected = 3;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szActualTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getMinute(nullTime, &act) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getMinute(t, &act) );
    EXPECT_EQ ( act, expected );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetSecond)
{
    mamaDateTime t, nullTime = NULL;

    std::string szActualTime = "2013-07-04 10:03:21.123";

    mama_u32_t act, expected = 21;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szActualTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getSecond(nullTime, &act) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getSecond(t, &act) );
    EXPECT_EQ ( act, expected );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetMicrosecond)
{
    mamaDateTime t, nullTime = NULL;

    std::string szActualTime = "2013-07-04 10:03:21.123";

    mama_u32_t act, expected = 123000;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szActualTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getMicrosecond(nullTime, &act) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getMicrosecond(t, &act) );
    EXPECT_EQ ( act, expected );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetDayOfWeek)
{
    mamaDateTime t, nullTime = NULL;

    std::string szActualTime = "2013-07-04 10:03:21.123";

    mamaDayOfWeek act, expected = Thursday;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t, szActualTime.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getDayOfWeek(nullTime, &act) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getDayOfWeek(t, &act) );
    EXPECT_EQ ( act, expected );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestDiffSeconds)
{
    mamaDateTime t1, t2, nullTime = NULL;

    std::string szTime1 = "2013-07-04 10:03:21.123",
                szTime2 = "2012-07-04 10:03:21.123";

    mama_f64_t act, expected =  60 * 60 * 24 * 365;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t2) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t1, szTime1.c_str()) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t2, szTime2.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffSeconds(nullTime, nullTime, NULL) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffSeconds(t1, nullTime, NULL) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffSeconds(t1, t2, NULL) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_diffSeconds(t1, t2, &act) );
    EXPECT_EQ ( act, expected );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t2) );
}

TEST_F (MamaDateTimeTestC, TestDiffSecondsSameDay)
{
    mamaDateTime t1, t2, nullTime = NULL;

    std::string szTime1 = "2013-07-04 10:03:21.123",
                szTime2 = "2013-07-04 12:03:21.123";

    mama_f64_t act, expected =  60 * 60 * 2;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t2) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t1, szTime1.c_str()) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t2, szTime2.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffSecondsSameDay(nullTime, nullTime, NULL) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffSecondsSameDay(t1, nullTime, NULL) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffSecondsSameDay(t1, t2, NULL) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_diffSecondsSameDay(t2, t1, &act) );
    EXPECT_EQ ( act, expected );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t2) );
}

TEST_F (MamaDateTimeTestC, TestDiffMicroseconds)
{
    mamaDateTime t1, t2, nullTime = NULL;

    std::string szTime1 = "2013-07-04 10:03:21.123",
                szTime2 = "2012-07-04 10:03:21.123";

    mama_i64_t mics = 1000000, dd = 365, hh = 24, mm = 60, ss = 60;
    mama_i64_t act, expected =  dd * hh * mm * ss * mics;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t2) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t1, szTime1.c_str()) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t2, szTime2.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffMicroseconds(nullTime, nullTime, NULL) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffMicroseconds(t1, nullTime, NULL) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffMicroseconds(t1, t2, NULL) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_diffMicroseconds(t1, t2, &act) );
    EXPECT_EQ ( act, expected );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t2) );
}

TEST_F (MamaDateTimeTestC, TestDiffMicrosecondsMixedValues)
{
    mamaDateTime t1, t2, nullTime = NULL;

    std::string szTime1 = "2013-07-04 10:03:21.123",
                szTime2 = "2012-07-04 10:03:21.123";

    mama_i64_t act;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t2) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t1, szTime1.c_str()) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t2, szTime2.c_str()) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffMicroseconds(nullTime, nullTime, NULL) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffMicroseconds(t1, nullTime, NULL) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_diffMicroseconds(t1, t2, NULL) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_diffMicroseconds(t1, t2, &act) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t1) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t2) );
}

/*  Description:     Get the current time since epoch by passing:
 *                       1) NULL date-time & valid seconds
 *                       2) Valid date-time & NULL seconds
 *                       3) NULL date-time & NULL seconds
 *                   to mamaDateTime_getEpochTimeSecondsWithCheck();
 *
 *  Expected Result: MAMA_STATUS_NULL_ARG
 */

TEST_F (MamaDateTimeTestC, NullArguments)
{
    mama_f64_t      seconds = 0;
    mamaDateTime    m_DateTime = NULL;

    /* Call with a NULL date time */
    EXPECT_EQ (MAMA_STATUS_NULL_ARG,
               mamaDateTime_getEpochTimeSecondsWithCheck (NULL, &seconds));

    /* NULL seconds */
    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_create (&m_DateTime));
    EXPECT_EQ (MAMA_STATUS_NULL_ARG,
               mamaDateTime_getEpochTimeSecondsWithCheck (m_DateTime, NULL));

    /* NULL for both */
    EXPECT_EQ (MAMA_STATUS_NULL_ARG,
               mamaDateTime_getEpochTimeSecondsWithCheck (NULL, NULL));

    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_destroy (m_DateTime));
}

/*  Description:     Get the number of seconds since epoch from:
 *                       1) A string representation of a time
 *                       2) A string representation of a time and the current
 *                          date
 *                   Compare these strings.
 *
 *  Expected Result: completeDateSeconds = timeSeconds
 */
TEST_F (MamaDateTimeTestC, CompareDates)
{
    mamaDateTime today                 = NULL;
    char         stringDate[200]       = "";
    const char*  time                  = "10:00:00.000000";
    char         completeDateTime[400] = "";
    mamaDateTime m_cDateTime           = NULL;
    mama_f64_t   completeDateSeconds   = 0;
    mama_f64_t   timeSeconds           = 0;

    /* Get todays date in a date time */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_create (&today));

    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_setToNow (today));

    /* Get the string representation of the data */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_getAsFormattedString (today, stringDate, 100, "%Y-%m-%d"));

    /* Destroy the date */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_destroy (today));

    /* Format a string using today's date and a time,
     * this should be as "2010-07-04 10:00:00.000" */
    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_create (&m_cDateTime));
    sprintf (completeDateTime, "%s %s", stringDate, time);

    /* Set the date from this string */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_setFromString (m_cDateTime, completeDateTime));

    /* Get the number of seconds */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_getEpochTimeSecondsWithCheck (m_cDateTime, &completeDateSeconds));

    /* Set the date using just the time string */
    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_clear (m_cDateTime));
    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_setFromString (m_cDateTime, time));

    /* Get the number of seconds from this */
    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_getEpochTimeSecondsWithCheck (m_cDateTime, &timeSeconds));

    /* These must be the same */
    EXPECT_EQ (completeDateSeconds, timeSeconds);
    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_destroy (m_cDateTime));
}

/*  Description:     Set the number of seconds since epoch from a timeval.
 *                   Compare these strings.
 *
 *  Expected Result: timeStr = stringBuffer
 */
TEST_F (MamaDateTimeTestC, TestSetStructTimeVal)
{
    struct timeval          tVal;
    mamaDateTime            t, nullTime = NULL;
    const char*             timeStr     = "2017-01-17 17:31:47.123000";
    char                    stringBuffer[50];
    mama_u32_t              secs        = 0;
    mama_u32_t              uSecs       = 0;
    mamaDateTimePrecision   precision;

    /* The following timeval represents the time - "2017-01-17 17:31:47.123" */
    tVal.tv_sec = 1484674307;
    tVal.tv_usec = 123000;


    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setFromStructTimeVal(nullTime, NULL) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setFromStructTimeVal(nullTime, &tVal) );
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setFromStructTimeVal(t, NULL) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStructTimeVal(t, &tVal) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getAsString (t, stringBuffer, 50) );
    EXPECT_STREQ (timeStr, stringBuffer);

    /* Precision was set to 6 dp so should be MAMA_DATE_TIME_PREC_MICROSECONDS */
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTime(t, &secs, &uSecs, &precision) );
    EXPECT_EQ ( precision, MAMA_DATE_TIME_PREC_MICROSECONDS );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, GetAsFormattedStringWithTzTest)
{
    mamaDateTime t1                    = NULL;
    char         stringDate[100]       = "";
    const char*  time                  = "2013-07-04 10:03:21.123000";
    char         completeDateTime[100] = "";

    /* Get todays date in a date time */
    EXPECT_EQ (MAMA_STATUS_OK, mamaDateTime_create (&t1));

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromString(t1, time) );

    /* Get the string representation of the data */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_getAsFormattedString (t1, stringDate, 100, "%Y-%m-%d %H:%M:%S"));

    /* Get the string representation of the data */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_getAsFormattedStringWithTz (t1, completeDateTime, 100, "%Y-%m-%d %H:%M:%S", mamaTimeZone_utc()));

    EXPECT_STREQ ( stringDate, completeDateTime );

    /* Test some of the seconds / sub-seconds options */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_getAsFormattedStringWithTz (t1, completeDateTime, 100, "%Y-%m-%d %H:%M:%S.%.", mamaTimeZone_utc()));

    EXPECT_STREQ ( time, completeDateTime );

    // %, should add the "." before the sub seconds
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_getAsFormattedStringWithTz (t1, completeDateTime, 100, "%Y-%m-%d %H:%M:%S%,", mamaTimeZone_utc()));

    EXPECT_STREQ ( time, completeDateTime );

    /* Destroy the date */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_destroy (t1));
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeExtended)
{
    mamaDateTimePrecision   precision;
    mamaDateTime            t           = NULL;
    mama_u32_t              secs        = 0;
    mama_u32_t              uSecs       = 0;
    int64_t                 inSecs      = 4294967296;
    long                    inNSecs     = 123000000;
    int64_t                 outSecs     = 0;
    long                    outNSecs    = 0;

    /* This test uses the time - "2106-02-07 06:28:16.123000"
        1 second more than a uint32_t can hold */
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );
    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);
    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    /* Should fail as you can't fit value into u32 */
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getEpochTime(t, &secs, &uSecs, &precision) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeExtendedNegativeValues)
{
    mamaDateTime            t           = NULL;
    mama_u32_t              secs        = 0;
    mama_u32_t              uSecs       = 0;
    mamaDateTimePrecision   precision;
    char                    stringBuffer[50];
    const char*             expectedDate = "1969-12-29 23:59:55";
    int64_t                 inSecs      = -172805; // 48 hours, 5 sec before epoch
    long                    inNSecs     = 0;
    int64_t                 outSecs     = 0;
    long                    outNSecs    = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    /* This will ensure date is set since it's not obvious from value */
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setToNow(t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    /* Unsigned values cannot represent negative so return error */
    EXPECT_EQ (MAMA_STATUS_INVALID_ARG, mamaDateTime_getEpochTime(t, &secs, &uSecs, &precision) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getAsString (t, stringBuffer, 50) );
    EXPECT_STREQ (expectedDate, stringBuffer);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetStructTimeSpecExtended)
{
    /* The following test represents the time - "4375-02-26 15:38:40.123000" */
    mamaDateTime            t           = NULL;
    int64_t                 inSecs      = 75899345920;
    long                    inNSecs     = 123000000;
    int64_t                 outSecs     = 0;
    long                    outNSecs    = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    /* float precision no good for this range on 32 bit */
    checkExtendedNanosecondEquality( inNSecs, outNSecs);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetStructTimeSpecExtendedNegativeValues)
{
    /* The following test represents the time - "1969-12-31 23:59:55" */
    char                    stringBuffer[50];
    mamaDateTime            t            = NULL;
    const char*             expectedDate = "1969-12-31 23:59:55";
    int64_t                 inSecs       = -5;
    long                    inNSecs      = 0;
    int64_t                 outSecs      = 0;
    long                    outNSecs     = 0;


    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getAsString (t, stringBuffer, 50) );
    EXPECT_STREQ (expectedDate, stringBuffer);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeWithTzExtended)
{
    /* This test represents the time - "2106-02-07 06:28:16.123000"
        1 second more than a uint32_t can hold */
    mamaDateTime            t           = NULL;
    mama_u32_t              secs        = 0;
    mama_u32_t              uSecs       = 0;
    mamaDateTimePrecision   precision;
    mamaTimeZone            tz          = mamaTimeZone_utc();
    int64_t                 inSecs      = 4294967296;
    long                    inNSecs     = 123000000;
    int64_t                 outSecs     = 0;
    long                    outNSecs    = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getEpochTimeWithTz(t, &secs, &uSecs, &precision, tz) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeMicrosecondsExtended)
{
    /* This test represents the time - "4375-02-26 15:38:40.123000" */
    mamaDateTime    t       = NULL;
    mama_u64_t      uSecs   = 0;
    int64_t         inSecs = 75899345920;
    long            inNSecs = 123000000;
    int64_t         outSecs     = 0;
    long            outNSecs    = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    checkExtendedNanosecondEquality( inNSecs, outNSecs);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTimeMicroseconds(t, &uSecs) );
    checkExtendedMicrosecondEquality (uSecs, (inSecs * 1000000) + (inNSecs / 1000));

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetWithHintsExtended)
{
    /* This test represents the time - "2106-02-07 06:28:16.123000"
        1 second more than a uint32_t can hold */
    mamaDateTime            t           = NULL;
    mama_u32_t              secs        = 0;
    mama_u32_t              uSecs       = 0;
    mamaDateTimePrecision   precision;
    mamaDateTimeHints       hints;
    int64_t                 inSecs      = 4294967296;
    long                    inNSecs     = 123000000;
    int64_t                 outSecs     = 0;
    long                    outNSecs    = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getWithHints(t, &secs, &uSecs, &precision, &hints) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}


TEST_F (MamaDateTimeTestC, TestSetTimeExtendedTest)
{
    mamaDateTime    t1   = NULL;
    mama_u32_t      hour = 9, minute = 21, second = 12, microsecond = 500;
    mama_u32_t      year = 2500, month = 12, day = 15;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t1) );

    // Add a date beyond the current limit, 2106
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setTime(t1, hour, minute, second, microsecond));

    if (sizeof(time_t) < sizeof(int64_t))
    {
        // Date cannot be parsed on 32 bit systems
        EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_setDate(t1, year, month, day));
    }
    else
    {
        char            dateTimeString[56];
        const char*     expectedResult = "2500-12-15 09:21:12.000";

        // Add a date beyond the current limit, 2106
        EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setDate(t1, year, month, day));

        // Get the date as a string for comparing
        EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getAsString (t1, dateTimeString, 56));

        EXPECT_STREQ ( dateTimeString, expectedResult );
    }

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t1) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimeMicrosecondsExtendedNegativeValues)
{
    /* The following test represents the time - "1969-12-31 23:59:55" */
    char            stringBuffer[50];
    const char*     expectedDate = "1969-12-31 23:59:55";
    mamaDateTime    t            = NULL;
    mama_u64_t      uSecs        = 0;
    int64_t         inSecs       = -5;
    long            inNSecs      = 0;
    int64_t         outSecs      = 0;
    long            outNSecs     = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTimeMicroseconds(t, &uSecs) );

    EXPECT_EQ ( uSecs, (outSecs * 1000000) + (outNSecs / 1000) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getAsString (t, stringBuffer, 50) );
    EXPECT_STREQ (expectedDate, stringBuffer);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetWithHintsExtendedNegativeValues)
{
    /* The following test represents the time - "1969-12-31 23:59:55" */
    mamaDateTime            t           = NULL;
    mama_u32_t              secs        = 0;
    mama_u32_t              uSecs       = 0;
    mamaDateTimePrecision   precision;
    mamaDateTimeHints       hints;
    char                    stringBuffer[50];
    const char*             expectedDate = "1969-12-31 23:59:55";
    int64_t                 inSecs      = -5;
    long                    inNSecs     = 0;
    int64_t                 outSecs     = 0;
    long                    outNSecs    = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    /* Range cannot be represented by u32 */
    EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getWithHints(t, &secs, &uSecs, &precision, &hints) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getAsString (t, stringBuffer, 50) );
    EXPECT_STREQ (expectedDate, stringBuffer);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetMicrosecondExtendedNegativeValues)
{
    /* The following test represents the time - "1969-12-31 23:59:55" */
    char            stringBuffer[50];
    const char*     expectedDate = "1969-12-31 23:59:55";
    mamaDateTime    t            = NULL;
    mama_u32_t      uSec         = 0;
    int64_t         inSecs       = -5;
    long            inNSecs      = 0;
    int64_t         outSecs      = 0;
    long            outNSecs     = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getMicrosecond(t, &uSec) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getAsString (t, stringBuffer, 50) );
    EXPECT_STREQ (expectedDate, stringBuffer);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestVeryLargeValues)
{
    struct timespec         tVal1, tVal2;
    mamaDateTime            t           = NULL;

    /* The following timeval represents the largest possible valule for an int64" */
    mama_i64_t int64_max = 9223372036854775807;
    tVal1.tv_sec = int64_max;
    tVal1.tv_nsec = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStructTimeSpec(t, &tVal1) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTimeSpec(t, &tVal2) );
    EXPECT_EQ ( tVal1.tv_sec, tVal2.tv_sec );
    EXPECT_EQ ( tVal1.tv_nsec, tVal2.tv_nsec );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetAsStringLargeExtendedNegativeValues)
{
    struct timespec         tVal1, tVal2;
    char                    stringBuffer[50];
    mamaDateTime            t            = NULL;
    const char*             expectedDate = "1969-12-02 00:00:00";

    /* The following timeval represents the time - "1969-12-02 00:00:00" */
    tVal1.tv_sec = -2592000;
    tVal1.tv_nsec = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStructTimeSpec(t, &tVal1) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTimeSpec(t, &tVal2) );
    EXPECT_EQ ( tVal1.tv_sec, tVal2.tv_sec );
    EXPECT_EQ ( tVal1.tv_nsec, tVal2.tv_nsec );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getAsString (t, stringBuffer, 50) );
    EXPECT_STREQ (expectedDate, stringBuffer);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetAsStringLargeExtendedValues)
{
    /* This test represents the time - "3001-01-01 00:00:00" */
    char                    stringBuffer[50];
    mamaDateTime            t            = NULL;
    int64_t                 inSecs      = 32535216000;
    long                    inNSecs     = 0;
    int64_t                 outSecs     = 0;
    long                    outNSecs    = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    if (sizeof(time_t) < sizeof(int64_t))
    {
        // This string cannot be parsed correctly on 32 bit - expect error
        EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getAsString (t, stringBuffer, 50) );
    }
    else
    {
        const char* expectedDate = "3001-01-01 00:00:00";
        EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getAsString (t, stringBuffer, 50) );
        EXPECT_STREQ (expectedDate, stringBuffer);
    }

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetTimeAsStringLargeExtendedNegativeValues)
{
    struct timespec         tVal1, tVal2;
    char                    stringBuffer[50];
    mamaDateTime            t            = NULL;
    const char*             expectedTime = "00:00:00";

    /* The following timeval represents the time - "1969-12-02 00:00:00" */
    tVal1.tv_sec = -2592000;
    tVal1.tv_nsec = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStructTimeSpec(t, &tVal1) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTimeSpec(t, &tVal2) );
    EXPECT_EQ ( tVal1.tv_sec, tVal2.tv_sec );
    EXPECT_EQ ( tVal1.tv_nsec, tVal2.tv_nsec );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getTimeAsString(t, stringBuffer, 50) );
    EXPECT_STREQ (expectedTime, stringBuffer);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetTimeAsStringLargeExtendedValues)
{
    /* This test represents the time - "3001-01-01 00:00:00" */
    const char*             expectedTime = "00:00:00";
    char                    stringBuffer[50];
    mamaDateTime            t           = NULL;
    int64_t                 inSecs      = 32535216000;
    long                    inNSecs     = 0;
    int64_t                 outSecs     = 0;
    long                    outNSecs    = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getTimeAsString(t, stringBuffer, 50) );
    EXPECT_STREQ (expectedTime, stringBuffer);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetDateAsStringLargeExtendedNegativeValues)
{
    struct timespec         tVal1, tVal2;
    char                    stringBuffer[50];
    mamaDateTime            t            = NULL;
    const char*             expectedDate = "1969-12-02";

    /* The following timeval represents the time - "1969-12-02 00:00:00" */
    tVal1.tv_sec = -2592000;
    tVal1.tv_nsec = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStructTimeSpec(t, &tVal1) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTimeSpec(t, &tVal2) );
    EXPECT_EQ ( tVal1.tv_sec, tVal2.tv_sec );
    EXPECT_EQ ( tVal1.tv_nsec, tVal2.tv_nsec );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getDateAsString(t, stringBuffer, 50) );
    EXPECT_STREQ (expectedDate, stringBuffer);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}


TEST_F (MamaDateTimeTestC, TestGetDateAsStringLargeExtendedValues)
{
    /* The following test represents the time - "3001-01-01 00:00:00" */
    char                    stringBuffer[50];
    mamaDateTime            t            = NULL;
    const char*             expectedDate = "3001-01-01";
    int64_t                 inSecs      = 32535216000;
    long                    inNSecs     = 0;
    int64_t                 outSecs     = 0;
    long                    outNSecs    = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    if (sizeof(time_t) < sizeof(int64_t))
    {
        EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getDateAsString(t, stringBuffer, 50) );
    }
    else
    {
        EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getDateAsString(t, stringBuffer, 50) );
        EXPECT_STREQ (expectedDate, stringBuffer);
    }

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetAsFormattedStringWithTzLargeExtendedNegativeValues)
{
    struct timespec  tVal1, tVal2;
    mamaDateTime     t                     = NULL;
    char             stringDate[100]       = "";
    char             completeDateTime[100] = "";

    /* The following timeval represents the time - "1969-12-02 00:00:00" */
    tVal1.tv_sec = -2592000;
    tVal1.tv_nsec = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStructTimeSpec(t, &tVal1) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTimeSpec(t, &tVal2) );
    EXPECT_EQ ( tVal1.tv_sec, tVal2.tv_sec );
    EXPECT_EQ ( tVal1.tv_nsec, tVal2.tv_nsec );

    /* Get the string representation of the data */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_getAsFormattedString (t, stringDate, 100, "%Y-%m-%d %H:%M:%S"));

    /* Get the string representation of the data */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_getAsFormattedStringWithTz (t, completeDateTime, 100, "%Y-%m-%d %H:%M:%S", mamaTimeZone_utc()));

    EXPECT_STREQ ( stringDate, completeDateTime );

    /* Destroy the date */
    EXPECT_EQ (MAMA_STATUS_OK,
               mamaDateTime_destroy (t));
}

TEST_F (MamaDateTimeTestC, TestGetAsFormattedStringWithTzLargeExtendedValues)
{
    /* The following test represents the time - "3001-01-01 00:00:00" */
    mamaDateTime     t                     = NULL;
    char             stringDate[100]       = "";
    char             completeDateTime[100] = "";
    int64_t          inSecs      = 32535216000;
    long             inNSecs     = 0;
    int64_t          outSecs     = 0;
    long             outNSecs    = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    if (sizeof(time_t) < sizeof(int64_t))
    {
        EXPECT_EQ ( MAMA_STATUS_INVALID_ARG,
                    mamaDateTime_getAsFormattedString (t, stringDate, 100, "%Y-%m-%d %H:%M:%S") );
    }
    else
    {
        /* Get the string representation of the data */
        EXPECT_EQ ( MAMA_STATUS_OK,
                    mamaDateTime_getAsFormattedString (t, stringDate, 100, "%Y-%m-%d %H:%M:%S") );

        /* Get the string representation of the data */
        EXPECT_EQ ( MAMA_STATUS_OK,
                    mamaDateTime_getAsFormattedStringWithTz (t, completeDateTime, 100, "%Y-%m-%d %H:%M:%S", mamaTimeZone_utc()) );

        EXPECT_STREQ ( stringDate, completeDateTime );
    }
    /* Destroy the date */
    EXPECT_EQ ( MAMA_STATUS_OK,
                mamaDateTime_destroy (t) );
}

TEST_F (MamaDateTimeTestC, TestGetStructTmWithTzLargeExtendedNegativeValues)
{
    struct timespec  tVal1, tVal2;
    struct tm 	     tM;
    mamaDateTime     t 	= NULL;
    mamaTimeZone     tz = mamaTimeZone_utc();

    int sec = 21, min = 3, hour = 10, mday = 2, mon = 11, year = 69, wday = 2, yday = 335;

    /* The following timeval represents the time - "1969-12-02 10:03:21" */
    tVal1.tv_sec = -2555799;
    tVal1.tv_nsec = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setFromStructTimeSpec(t, &tVal1) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTimeSpec(t, &tVal2) );
    EXPECT_EQ ( tVal1.tv_sec, tVal2.tv_sec );
    EXPECT_EQ ( tVal1.tv_nsec, tVal2.tv_nsec );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTmWithTz(t, &tM, tz) );
    EXPECT_EQ ( sec, tM.tm_sec );
    EXPECT_EQ ( min, tM.tm_min );
    EXPECT_EQ ( hour, tM.tm_hour );
    EXPECT_EQ ( mday, tM.tm_mday );
    EXPECT_EQ ( mon, tM.tm_mon );
    EXPECT_EQ ( year, tM.tm_year );
    EXPECT_EQ ( wday, tM.tm_wday );
    EXPECT_EQ ( yday, tM.tm_yday );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}


TEST_F (MamaDateTimeTestC, TestGetStructTmWithTzLargeExtendedValues)
{
    /* The following test represents the time - "3001-01-01 10:03:21" */
    struct tm 	     tM;
    mamaDateTime     t 	         = NULL;
    mamaTimeZone     tz          = mamaTimeZone_utc();
    int64_t          inSecs      = 32535252201;
    long             inNSecs     = 0;
    int64_t          outSecs     = 0;
    long             outNSecs    = 0;

    int sec = 21, min = 3, hour = 10, mday = 1, mon = 0, year = 1101, wday = 4, yday = 0;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    setMamaDateTimeExtendedValue(t, inSecs, inNSecs);
    getMamaDateTimeExtendedValue(t, outSecs, outNSecs);

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );

    if (sizeof(time_t) < sizeof(int64_t))
    {
        EXPECT_EQ ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTmWithTz(t, &tM, tz) );
    }
    else
    {
        EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getStructTmWithTz(t, &tM, tz) );
        EXPECT_EQ ( sec, tM.tm_sec );
        EXPECT_EQ ( min, tM.tm_min );
        EXPECT_EQ ( hour, tM.tm_hour );
        EXPECT_EQ ( mday, tM.tm_mday );
        EXPECT_EQ ( mon, tM.tm_mon );
        EXPECT_EQ ( year, tM.tm_year );
        EXPECT_EQ ( wday, tM.tm_wday );
        EXPECT_EQ ( yday, tM.tm_yday );
    }

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}


TEST_F (MamaDateTimeTestC, TestGetEpochTimeExtGranularFunctions)
{
    /* The following test represents the time - "3001-01-01 10:03:21" */
    mamaDateTime          t            = NULL;
    int64_t               inSecs       = 32535252201;
    uint32_t              inNSecs      = 123456789;
    mamaDateTimeHints     inHints      = MAMA_DATE_TIME_HAS_DATE;
    mamaDateTimePrecision inPrecision  = MAMA_DATE_TIME_PREC_NANOSECONDS;
    int64_t               outSecs      = 0;
    uint32_t              outNSecs     = 0;
    mamaDateTimeHints     outHints     = 0;
    mamaDateTimePrecision outPrecision = MAMA_DATE_TIME_PREC_UNKNOWN;

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeExt(t, inSecs, inNSecs) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setHints(t, inHints) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setPrecision(t, inPrecision) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getEpochTimeExt(t, &outSecs, &outNSecs) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getHints(t, &outHints) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_getPrecision(t, &outPrecision) );

    EXPECT_EQ ( inSecs, outSecs );
    EXPECT_EQ ( inNSecs, outNSecs );
    EXPECT_EQ ( inHints, outHints );
    EXPECT_EQ ( inPrecision, outPrecision );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F (MamaDateTimeTestC, TestGetEpochTimePreWindowsEpoch)
{
    /* The following test represents the time - "April 15, 1017 2:42:09" */
    mamaDateTime          t            = NULL;
    int64_t               inSecs       = -30064771071LL;
    uint32_t              inNSecs      = 0;
    struct tm             resultTm;
    char                  buf[64];

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_create(&t) );

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setEpochTimeExt(t, inSecs, inNSecs) );
    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_setHints(t, MAMA_DATE_TIME_HAS_TIME) );

    ASSERT_EQ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getAsFormattedString(t, buf, sizeof(buf), "%Y") );
    ASSERT_EQ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getStructTm(t, &resultTm) );
    ASSERT_EQ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getAsString(t, buf, sizeof(buf)) );
    ASSERT_EQ( MAMA_STATUS_INVALID_ARG, mamaDateTime_getDateAsString(t, buf, sizeof(buf)) );

    // This method should work since it does arithmetic to extract time if necessary
    ASSERT_EQ( MAMA_STATUS_OK, mamaDateTime_getTimeAsString(t, buf, sizeof(buf)) );
    EXPECT_STREQ("02:42:09.000", buf);

    EXPECT_EQ ( MAMA_STATUS_OK, mamaDateTime_destroy(t) );
}

TEST_F(MamaDateTimeTestC, TestSetInvalidSubsecs)
{
    struct timeval          tVal;
    struct timespec         tVal1;
    mamaDateTime            t;

    tVal.tv_sec = 1484674307;
    tVal.tv_usec = 1000000;

    tVal1.tv_sec = 1484674307;
    tVal1.tv_nsec = 1000000000;


    int64_t               inSecs = 1484674307;
    uint32_t              inNSecs = 1000000000;

    mama_u32_t secs = 43200, micSecs = 1000000;
    mamaDateTimePrecision precision = MAMA_DATE_TIME_PREC_MICROSECONDS;
    mamaDateTimeHints hints = MAMA_DATE_TIME_HAS_TIME;
    mama_u32_t hour = 9, minute = 21, second = 12, microsecond = 1000000;
    mamaTimeZone tz = mamaTimeZone_utc();

    EXPECT_EQ(MAMA_STATUS_OK, mamaDateTime_create(&t));

    EXPECT_EQ(MAMA_STATUS_INVALID_ARG, mamaDateTime_setFromStructTimeVal(t, &tVal));
    EXPECT_EQ(MAMA_STATUS_INVALID_ARG, mamaDateTime_setEpochTimeExt(t, inSecs, inNSecs));
    EXPECT_EQ(MAMA_STATUS_INVALID_ARG, mamaDateTime_setEpochTime(t, secs, micSecs, precision));
    EXPECT_EQ(MAMA_STATUS_INVALID_ARG, mamaDateTime_setWithHints(t, secs, micSecs, precision, hints));
    EXPECT_EQ(MAMA_STATUS_INVALID_ARG, mamaDateTime_setTimeWithPrecisionAndTz(t, hour, minute, second, microsecond, precision, tz));
    EXPECT_EQ(MAMA_STATUS_INVALID_ARG, mamaDateTime_setFromStructTimeSpec(t, &tVal1));


    EXPECT_EQ(MAMA_STATUS_OK, mamaDateTime_destroy(t));
}
