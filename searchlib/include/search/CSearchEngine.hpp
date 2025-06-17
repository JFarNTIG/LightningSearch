// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <CThreadPool.hpp>
#include <search/CSearchQuery.hpp>

#include <atomic>
#include <filesystem>
#include <vector>
#include <memory>
#include <mutex>
#include <set>

class CSearchEngine {
public:
    explicit CSearchEngine(CSearchQuery *searchQuery);
    virtual ~CSearchEngine();

    /**
     * @brief Initiate a search according to the specifications
     * of the given search query.
     */
    virtual void performSearch();

    /**
     * @brief Get the number of pending operations for the
     * current search query.
     */
    virtual int getPendingOperations();

    /**
     * @brief Get the number of files that have been enumerated
     * in the search directories so far.
     */
    virtual int getTotalFilesToSearch();

     /**
     * @brief Get the number of files that have been compared
     * against the search criteria so far.
     */
    virtual int getTotalFilesSearched();

    /**
     * @brief Get the number of files that have matched all the
     * search criteria so far.
     */
    virtual int getTotalMatches();

private:
    void spawnEnumerateWorker(std::filesystem::path const enumPath);
    void spawnSearchWorker(std::vector<std::filesystem::path> searchPath);

    bool matchesAllFilters(std::filesystem::path const &filePath);
    void notifyAllObservers(std::filesystem::path const &matchedFile);

    CSearchQuery *m_searchQuery;
    CThreadPool *m_threadPool;
    std::atomic_int m_pendingOperations;
    std::atomic_int m_totalFilesToSearch;
    std::atomic_int m_totalFilesSearched;
    std::atomic_int m_totalMatches;
};
