// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <search/IStreamSearcher.hpp>

/**
 * @brief Class representing a search through some form of stream.
 * Supports options for case sensitivity and whole or partial matches.
 * Optionally, regex mode may be specified, in which case the match string is
 * treated as a regex expression.
 */
class CStreamSearcher : public IStreamSearcher {
public:
    /**
     * @brief Create a stream searcher.
     * 
     * @param matchText the match string
     * @param caseInsensitive if true, the match is case insensitive, otherwise case sensitive. Ignored for regex searches.
     * @param wholeMatch if true, whole match, otherwise partial match
     */
    CStreamSearcher(std::wstring const &matchText,
                    bool const caseInsensitive = false,
                    bool const wholeMatch = false,
                    size_t const maxBufferSize = 1000000);

    std::wstring getMatchText() const { return m_matchText; }
    bool isCaseInsensitive() const { return m_isCaseInsensitive; }
    bool isWholeMatch() const { return m_isWholeMatch; }

    /**
     * @brief Perform search on an input stream.
     * 
     * @return true if full or partial match according to options; otherwise false
     */
    virtual bool searchText(std::wistream &in) const;

private:
    /**
     * @brief Private implementation method. Performs buffered non-regex search.
     */
    bool bufferedSearch(wchar_t const *text, size_t const size) const;

    /**
     * @brief Compare a chunk from an input stream against a chunk
     * in the match string.
     * @param bufferPos index of the start of the chunk within the stream
     * @param text pointer to start of the chunk from the stream
     * @param size size of the chunk
     * @return true if the chunk matches, false otherwise
     */
    bool bufferedMatch(wchar_t const *text, size_t const size, size_t const streamPos) const;

private:
    std::wstring m_matchText;
    bool m_isCaseInsensitive;
    bool m_isWholeMatch;
    size_t m_maxBufferSize;
};