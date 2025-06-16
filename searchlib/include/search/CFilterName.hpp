// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <search/IFilter.hpp>
#include <search/IStreamSearcher.hpp>

#include <filesystem>
#include <sstream>
#include <string>
#include <regex>

/**
 * @brief Class which filters files by filename.
 */
class CFilterName : public IFilter {
public:
    /**
     * @brief Create the CFilterName instance.
     * 
     * @param matchText the match string
     * @param caseInsensitive if true, the match is case insensitive, otherwise case sensitive
     * @param wholeMatch if true, whole match, otherwise partial match
     * @param isRegex if true, matchText is treated as a regex expression, and the caseInsensitive option is ignored
     */
    CFilterName(std::wstring const &matchText,
                bool const caseInsensitive = false,
                bool const wholeMatch = false,
                bool const isRegex = false);

    virtual ~CFilterName();

    virtual bool filterFile(std::filesystem::path const &filePath) const;

    /**
     * @brief Represent the filter and its options as a
     * text string.
     */
    virtual std::wstring getText() const;

private:
    IStreamSearcher *m_streamSearcher;
    bool m_isCaseInsensitive;
    bool m_isWholeMatch;
    bool m_isRegex;
};