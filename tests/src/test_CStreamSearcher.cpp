// SPDX-License-Identifier: GPL-2.0
#include <search/CStreamSearcher.hpp>

#include <gtest/gtest.h>

#include <sstream>
#include <locale>
#include <codecvt>

/**
 * @brief Helper function that searches in a string
 * using CStreamSearcher. The input string is converted
 * first into a wstringstream.
 */
static bool runSearch(std::wstring const &haystack,
                      std::wstring const &needle,
                      bool caseInsensitive,
                      bool wholeMatch,
                      size_t const maxBufferSize = 1000000)
{
    std::wstringstream ss(haystack);
    CStreamSearcher searcher(needle, caseInsensitive, wholeMatch, maxBufferSize);
    return searcher.searchText(ss);
}

/* --------------------------------------------------------------------------
 *                     Whole-match, case-sensitive tests
 * --------------------------------------------------------------------------*/
TEST(StreamSearcher, WholeMatchCaseSensitive_Positive)
{
    EXPECT_TRUE(runSearch(L"alpha beta gamma", L"alpha beta gamma",
                          /*caseInsensitive=*/false,
                          /*wholeMatch=*/true));
}

TEST(StreamSearcher, WholeMatchCaseSensitive_Negative_WrongCase)
{
    EXPECT_FALSE(runSearch(L"alpha Beta gamma", L"alpha beta gamma",
                           /*caseInsensitive=*/false,
                           /*wholeMatch=*/true));
}

TEST(StreamSearcher, WholeMatchCaseSensitive_Negative_PartialFound)
{
    // "bet" is only a substring of "beta" – whole match should fail.
    EXPECT_FALSE(runSearch(L"alpha beta gamma", L"bet",
                           /*caseInsensitive=*/false,
                           /*wholeMatch=*/true));
}

/* --------------------------------------------------------------------------
 *                     Whole-match, case-insensitive tests
 * --------------------------------------------------------------------------*/
TEST(StreamSearcher, WholeMatchCaseInsensitive_Positive_MixedCase)
{
    EXPECT_TRUE(runSearch(L"alpha Beta gamma", L"alpha beta gamma",
                          /*caseInsensitive=*/true,
                          /*wholeMatch=*/true));
}

/* --------------------------------------------------------------------------
 *                     Partial-match tests
 * --------------------------------------------------------------------------*/
TEST(StreamSearcher, PartialMatchCaseSensitive_Positive)
{
    EXPECT_TRUE(runSearch(L"concatenate", L"cat",
                          /*caseInsensitive=*/false,
                          /*wholeMatch=*/false));
}

TEST(StreamSearcher, PartialMatchCaseInsensitive_Positive_MixedCase)
{
    EXPECT_TRUE(runSearch(L"CONCATENATE", L"cat",
                          /*caseInsensitive=*/true,
                          /*wholeMatch=*/false));
}

TEST(StreamSearcher, PartialMatch_Negative)
{
    EXPECT_FALSE(runSearch(L"running swiftly", L"slow",
                           /*caseInsensitive=*/false,
                           /*wholeMatch=*/false));
}

/* --------------------------------------------------------------------------
 *                     Buffer-boundary test
 * --------------------------------------------------------------------------*/

/**
 * This stress-test feeds a long string to ensure the searcher copes with
 * internal buffering.
 * The substring does not cross over a chunk boundary.
 */
TEST(StreamSearcher, HandlesLargeStream_Search_NonFirstChunk)
{
    std::wstring longText(50'000, L'a');   // "aaaa..."
    longText.replace(30'000, 4, L"beta");  // embed "beta" in the middle

    EXPECT_TRUE(runSearch(longText, L"beta",
                          /*caseInsensitive=*/false,
                          /*wholeMatch=*/false,
                          /*maxBufferSize=*/25'000));
}

/**
 * This stress-test feeds a long string to ensure the searcher copes with
 * internal buffering.
 * The substring crosses a chunk boundary.
 */
TEST(StreamSearcher, HandlesLargeStream_Search_AcrossChunkBoundary)
{
    std::wstring longText(50'000, L'a');   // "aaaa..."
    longText.replace(24'998, 4, L"beta");  // embed "beta" in the middle

    EXPECT_TRUE(runSearch(longText, L"beta",
                          /*caseInsensitive=*/false,
                          /*wholeMatch=*/false,
                          /*maxBufferSize=*/25'000));
}

/**
 * This stress-test feeds a long string to ensure the searcher copes with
 * internal buffering.
 * Match very long strings where a substring crosses a chunk boundary.
 */
TEST(StreamSearcher, HandlesLargeStream_Match_AcrossChunkBoundary)
{
    std::wstring longText(50'000, L'a');   // "aaaa..."
    longText.replace(24'998, 4, L"beta");  // embed "beta" in the middle

    std::wstring longText2(50'000, L'A');   // "AAAA..."
    longText2.replace(24'998, 4, L"BEta");  // embed "BEta" in the middle

    EXPECT_TRUE(runSearch(longText, longText2,
                          /*caseInsensitive=*/true,
                          /*wholeMatch=*/true,
                          /*maxBufferSize=*/25'000));
}