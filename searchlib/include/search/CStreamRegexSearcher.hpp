// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <search/IStreamSearcher.hpp>

#include <regex>

class CStreamRegexSearcher : public IStreamSearcher {
public:
    /**
     * @brief Create a stream searcher which searches by regular expression.
     * @param pattern Regular expression in ECMAScript syntax.
     * @param caseInsensitive Compile the regex with std::regex_constants::icase.
     * @param wholeMatch Whether to match the entire input stream, or search.
     */
    CStreamRegexSearcher(std::wstring const &pattern,
                         bool caseInsensitive = false,
                         bool wholeMatch = false);

    std::wstring getPattern() const { return m_pattern; }
    bool isCaseInsensitive() const { return m_isCaseInsensitive; }
    bool isWholeMatch() const { return m_isWholeMatch; }

    /**
     * @brief Search an entire wide-character stream. The function loads the whole
     * stream into memory in one go (suitable for log files, config files, etc.).
     */
    virtual bool searchText(std::wistream &in) const;

private:
    std::wregex m_regex;
    std::wstring m_pattern;
    bool m_isCaseInsensitive;
    bool m_isWholeMatch;
};