// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <search/IFilter.hpp>
#include <search/ISearchObserver.hpp>

#include <filesystem>
#include <vector>

class CSearchQuery {
public:
    explicit CSearchQuery();
    virtual ~CSearchQuery();

    /**
     * @brief Set the list of directories to search.
     */
    virtual void setDirectories(std::vector<std::filesystem::path> const &paths);

    /**
     * @brief Get the list of directories to search.
     */
    virtual std::vector<std::filesystem::path> getDirectories() const;

    /**
     * @brief Set the list of filters to be used when searching.
     */
    virtual void setFilters(std::vector<IFilter *> const &filters);

    /**
     * @brief Get the list of filters to be used when searching.
     */
    virtual std::vector<IFilter *> getFilters() const;

    /**
     * @brief Adds an observer to the result observer list.
     * The search query does NOT take ownership of
     * the observer.
     * 
     * IMPORTANT: Calls to the observer function may happen from
     * other threads!
     */
    virtual void addResultObserver(ISearchObserver *observer);

    /**
     * @brief Get the list of result observers.
     */
    virtual std::vector<ISearchObserver *> getResultObservers() const;

private:
    std::vector<std::filesystem::path> m_searchPaths;
    std::vector<IFilter *> m_filters;
    std::vector<ISearchObserver *> m_observers;
};