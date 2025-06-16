// SPDX-License-Identifier: GPL-2.0
#include <search/CFilterContents.hpp>

#include <search/CStreamSearcher.hpp>
#include <search/CStreamRegexSearcher.hpp>

#include <codecvt>
#include <fstream>
#include <locale>
#include <stdexcept>

CFilterContents::CFilterContents(std::wstring const &matchText,
                bool const caseInsensitive,
                bool const wholeMatch,
                bool const isRegex)
    : m_isCaseInsensitive(caseInsensitive),
    m_isWholeMatch(wholeMatch),
    m_isRegex(isRegex)
{
    if(m_isRegex) {
        m_streamSearcher = new CStreamRegexSearcher(matchText, caseInsensitive, wholeMatch);
    } else {
        m_streamSearcher = new CStreamSearcher(matchText, caseInsensitive, wholeMatch);
    }
}

CFilterContents::~CFilterContents() {
    delete m_streamSearcher;
}

bool CFilterContents::filterFile(std::filesystem::path const &filePath) const {
    std::wifstream fileStream(filePath);

    return m_streamSearcher->searchText(fileStream);
}

std::wstring CFilterContents::getText() const {
    std::wstringstream wss;

    // Define all string constants used in building the string.
    // Since these are initialized once on program load, define them static.
    // TODO: Add localization support.
    static std::wstring const TXTCONST_NAME_MATCHES = L"File content matches";
    static std::wstring const TXTCONST_NAME_CONTAINS = L"File contains";
    static std::wstring const TXTCONST_CASE_SENSITIVE = L"case sensitive";
    static std::wstring const TXTCONST_CASE_INSENSITIVE = L"case insensitive";
    static std::wstring const TXTCONST_REGEX_MODE = L"use regex";
    static std::wstring const TXTCONST_CHARACTERS = L"characters";

    // Now we build a string using the text constants.
    // Example: L"Name matches <quoted match string> (regex mode)"
    //       or L"Name contains <quoted match string> (case insensitive)"
    wss << (m_isWholeMatch ? TXTCONST_NAME_MATCHES : TXTCONST_NAME_CONTAINS);
    wss << L" ";
    wss << L"(";
    if(m_isRegex) {
        wss << TXTCONST_REGEX_MODE;
    } else {
        wss << (m_isCaseInsensitive ? TXTCONST_CASE_INSENSITIVE : TXTCONST_CASE_SENSITIVE);
    }
    wss << L")";
    return wss.str();
}