// SPDX-License-Identifier: GPL-2.0
#include <search/CStreamSearcher.hpp>

#include <StringUtil.hpp>

#include <vector>

CStreamSearcher::CStreamSearcher(std::wstring const &matchText,
                bool const caseInsensitive,
                bool const wholeMatch,
                size_t const maxBufferSize)
    : m_isCaseInsensitive(caseInsensitive),
    m_isWholeMatch(wholeMatch),
    m_maxBufferSize(maxBufferSize)
{
    if(caseInsensitive) {
        // If the match is case-insensitive, initialize the
        // match text with the lowercase version.
        // This way, avoid calling lower() every call to searchText
        // and get better performance.

        m_matchText = wlower(matchText);
    } else {
        // Otherwise, keep the name the same.

        m_matchText = matchText;
    }
}

bool CStreamSearcher::searchText(std::wistream &in) const {
    if(!in.good()) {
        return false;
    }
    
    size_t const bufferLen = m_maxBufferSize;
    std::vector<wchar_t> buffer(bufferLen);

    // If this is a whole match, move the buffer window in
    // even intervals.
    // Otherwise, the buffer window will be offset
    // by the size of the match text. This is needed so that
    // we don't miss matches that overlap a multiple of the
    // buffer size.
    size_t const overlapChars = m_isWholeMatch ? 0 : m_matchText.size();
    size_t bufferPos = 0;

    do {
        in.read(buffer.data(), bufferLen);
        size_t const charsRead = in.gcount();

        if(charsRead == 0) {
            break;
        }

        bool bufferResult;

        if(m_isWholeMatch) {
            bufferResult = bufferedMatch(buffer.data(), charsRead, bufferPos);

            // for a full match:
            // every chunk must match.
            // if any chunk does not match,
            // then fail
            if(!bufferResult) {
                return false;
            }
        } else {
            bufferResult = bufferedSearch(buffer.data(), charsRead);

            // for a search:
            // one chunk must contain the substring
            // if any chunk contains the substring,
            // then succeed
            if(bufferResult) {
                return true;
            }
        }

        bufferPos += bufferLen;

        // At this point, if the buffer window should be offset,
        // then we should rewind
        if(!in.eof() && overlapChars > 0) {
            if(in.seekg(-overlapChars, std::ios::cur))
            {
                // Ready for next chunk (stream remains good)
                bufferPos -= overlapChars;
            } else {
                // stream refused seek - reset error so the loop
                // ends cleanly
                in.clear();
            }
        }
    } while(in.good());

    // if it was a whole match and we got this far,
    // then the match was successful (no chunk had a mismatch)
    // and we should return TRUE
    // Otherwise, if it was a search, then the
    // search was unsuccessful (no chunk had the substring)
    // and we should return FALSE
    return m_isWholeMatch;
}

bool CStreamSearcher::bufferedSearch(wchar_t const *text, size_t const size) const {
    // We will do this differently depending on if the match
    // should be case-insensitive or not.
    std::wstring lowerText;

    if(m_isCaseInsensitive) {
        // If the match is case-insensitive, convert the string
        // to lowercase.
        lowerText = wlower(std::wstring(text, size));
    } else {
        // Otherwise, search the raw filename.
        lowerText = std::wstring(text, size);
    }

    // Find the match string in the file name.
    return lowerText.find(m_matchText) != std::wstring::npos;
}

bool CStreamSearcher::bufferedMatch(wchar_t const *text, size_t const size, size_t const bufferPos) const {
    // Example:
    // * if match string is "ABCDEFGHIJKL"
    // * buffer pos is 4
    // * size is 2, then this method should check:
    // "ABCDEFGHIJKL"
    //      ^^
    // and compare against the text buffer,
    // which is "EF". So in this example,
    // the function should return true.

    // impossible for this to be a match if
    // the buffer window runs outside the
    // match text
    if(bufferPos + size > m_matchText.size()) {
        return false;
    }
    
    // We will do this differently depending on if the match
    // should be case-insensitive or not.
    std::wstring lowerText;

    if(m_isCaseInsensitive) {
        // If the match is case-insensitive, convert the string
        // to lowercase.
        lowerText = wlower(std::wstring(text, size));
    } else {
        // Otherwise, match the raw text.
        lowerText = std::wstring(text, size);
    }

    std::wstring matchChunk = m_matchText.substr(bufferPos, size);

    return lowerText == matchChunk;
}