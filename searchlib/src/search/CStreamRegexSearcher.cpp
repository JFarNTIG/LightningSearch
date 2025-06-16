// SPDX-License-Identifier: GPL-2.0
#include <search/CStreamRegexSearcher.hpp>

#include <regex>
#include <string>
#include <istream>
#include <iterator>

CStreamRegexSearcher::CStreamRegexSearcher(std::wstring const& pattern,
                                                  bool caseInsensitive,
                                                  bool wholeMatch)
    : m_pattern(pattern),
      m_isCaseInsensitive(caseInsensitive),
      m_isWholeMatch(wholeMatch)
{
    auto flags = std::regex_constants::ECMAScript;

    if(caseInsensitive) {
        flags |= std::regex_constants::icase;
    }

    m_regex = std::wregex(pattern, flags);
}

bool CStreamRegexSearcher::searchText(std::wistream &in) const
{
    if(!in.good()) {
        return false;
    }

    // Read the entire stream into a buffer
    // and apply the regex search to the entire buffer.
    std::wstring contents{ std::istreambuf_iterator<wchar_t>(in),
                           std::istreambuf_iterator<wchar_t>() };

    // Either match or search depending on the provided option
    if(m_isWholeMatch) {
        return std::regex_match(contents, m_regex);
    } else {
        return std::regex_search(contents, m_regex);
    }
}
