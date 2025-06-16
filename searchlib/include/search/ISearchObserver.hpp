// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <filesystem>

class ISearchObserver {
public:
    virtual ~ISearchObserver() = default;

    /**
     * @brief Observer function called when a file matches the search
     * criteria.
     */
    virtual void onFileMatched(std::filesystem::path const &matchedFile) = 0;
};