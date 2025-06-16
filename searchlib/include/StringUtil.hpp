// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <algorithm>
#include <string>
#include <cctype>
#include <cwctype>
#include <locale>

/**
 * @brief Transform a string to lowercase.
 */
inline std::string lower(std::string const &szText) {
    std::string szResult(szText);
    std::transform(szResult.begin(), szResult.end(), szResult.begin(),
                   [](char c) { return std::tolower(c); });
    return szResult;
}

/**
 * @brief Transform a string to uppercase.
 */
inline std::string upper(std::string const &szText) {
    std::string szResult(szText);
    std::transform(szResult.begin(), szResult.end(), szResult.begin(),
                   [](char c) { return std::toupper(c); });
    return szResult;
}

/**
 * @brief Transform a wstring to lowercase.
 */
inline std::wstring wlower(std::wstring const &wText) {
    std::wstring wResult(wText);
    std::transform(wResult.begin(), wResult.end(), wResult.begin(),
                   [](wchar_t c) { return std::towlower(c); });
    return wResult;
}
 
/**
 * @brief Transform a wstring to uppercase.
 */
inline std::wstring wupper(std::wstring const &wText) {
    std::wstring wResult(wText);
    std::transform(wResult.begin(), wResult.end(), wResult.begin(),
                   [](wchar_t c) { return std::towupper(c); });
    return wResult;
}