#include <gtest/gtest.h>
#include "TimeParser.h"

// Test suite: TimeParserTest

TEST(TimeParserTest, TestCaseCorrectTime) {

    // Note that this test fails on purpose!!

    // Test with correct time string
    char time_test[] = "000205";
    EXPECT_EQ(time_parse(time_test),125);     //tämä vertaa funktion palautusarvoa ja oletusarvoa

    char time_test2[] = "000105";
    EXPECT_EQ(time_parse(time_test2),65);

    char time_test3[] = "110105";
    EXPECT_EQ(time_parse(time_test3),39665);

}

TEST(TimeParserTest, TestCaseInCorrectTime) {

    // Note that this test fails on purpose!!

    // Test with correct time string
    char time_test[] = "000077";        //yli 60 sek
    EXPECT_EQ(time_parse(time_test),TIME_VALUE_ERROR);

    char time_test2[] = "00008805";     //liian pitkä merkkijono
    EXPECT_EQ(time_parse(time_test2),TIME_LEN_ERROR);

    char time_test3[] = "00a015";       //sisältää muutakin kuin numeroita
    EXPECT_EQ(time_parse(time_test3),TIME_VALUE_TYPE_ERROR);

    char time_test4[] = "1000-1";        //sisältää muutakin kuin numeroita
    EXPECT_EQ(time_parse(time_test4),TIME_VALUE_TYPE_ERROR);

    char time_test5[] = "240000";        //liikaa tunteja
    EXPECT_EQ(time_parse(time_test5),TIME_VALUE_ERROR);

    char time_test6[] = "000000";        //0 ei saa toteuttaa ajastinkeskeytystä turhaa
    EXPECT_EQ(time_parse(time_test6),TIME_VALUE_ERROR);

}

// https://google.github.io/googletest/reference/testing.html
// https://google.github.io/googletest/reference/assertions.html
 
