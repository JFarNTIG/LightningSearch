// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <filesystem>
#include <string>

class IFilter {
public:
    virtual ~IFilter() = default;

    /**
     * @brief Filter function that determines if a file is included in the search or not.
     */
    virtual bool filterFile(std::filesystem::path const &filePath) const = 0;

    /**
     * @brief Represent the filter and its options as a text string.
     */
    virtual std::wstring getText() const = 0;
};