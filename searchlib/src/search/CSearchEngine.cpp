// SPDX-License-Identifier: GPL-2.0
#include <search/CSearchEngine.hpp>

#include <iostream>

#define BATCH_SIZE 64

CSearchEngine::CSearchEngine(CSearchQuery *searchQuery)
    : m_searchQuery(searchQuery),
      m_pendingOperations(0),
      m_totalFilesToSearch(0),
      m_totalFilesSearched(0),
      m_totalMatches(0)
{
    // Since the effectiveness of threads is limited by the number of cores
    // the machine has, we want to set number of threads in the thread pool
    // based on hardware concurrency.
    // For example:
    //  2 cores => 2 threads
    //  4 cores => 4 threads
    // etc.
    auto const numWorkerThreads = std::thread::hardware_concurrency();

    m_threadPool = new CThreadPool(numWorkerThreads);
}

CSearchEngine::~CSearchEngine()
{
    delete m_searchQuery;

    // Important to note that this delete is potentially blocking
    // because CThreadPool's destructor sends the terminate signal to
    // all worker threads and then joins on them.
    delete m_threadPool;
}

void CSearchEngine::performSearch() {
    std::vector<std::filesystem::path> searchPaths = m_searchQuery->getDirectories();

    for(std::filesystem::path const &path : searchPaths) {
        spawnEnumerateWorker(path);
    }
}

void CSearchEngine::spawnEnumerateWorker(std::filesystem::path const enumPath) {
    auto enumerateWorkerFunc = [this](std::filesystem::path const enumPath) {
        std::vector<std::filesystem::path> paths;
        std::error_code ec;

        // If we used a range-based for loop here, the default behavior of the
        // recursive directory iterator would be to throw an exception when
        // a filesystem error occurs. These are very annoying to deal with,
        // and if uncaught will cause the current task to fail "silently"
        // because we are running in an std::packaged_task.
        // Instead of using a range-based for loop, we construct a
        // recursive_directory_iterator using its non-throwing constructor
        // and provide an std::error_code.
        auto dirIterator = std::filesystem::recursive_directory_iterator(
            enumPath,
            std::filesystem::directory_options::skip_permission_denied, 
            ec);

        auto dirEntryEnd = std::filesystem::end(dirIterator);

        for(auto dirEntry = std::filesystem::begin(dirIterator); dirEntry != dirEntryEnd; dirEntry.increment(ec)) {
            if(ec) {
                std::cout << "warning: error code " << ec.value() << " while enumerating directory\n";
                continue;
            }

            std::filesystem::path filePath = dirEntry->path();

            if(!std::filesystem::is_regular_file(filePath)) {
                continue;
            }

            paths.push_back(filePath);

            // m_searchedFilesMutex.lock();
            // m_searchedFilesSet.insert(p.path().wstring());
            // m_searchedFilesMutex.unlock();

            m_totalFilesToSearch++;

            if(paths.size() > BATCH_SIZE) {
                spawnSearchWorker(std::move(paths));

                // After std::move, the paths vector is now in an unspecified
                // (but valid) state. We call clear() to ensure the vector
                // is empty before continuing the loop.
                paths.clear();
            }
        }

        if(paths.size() > 0) {
            spawnSearchWorker(std::move(paths));
        }

        m_pendingOperations--;
    };

    // Before enqueuing the worker thread, add one
    // to the pending operations count.
    m_pendingOperations++;

    // Enqueue the worker thread
    m_threadPool->enqueue(enumerateWorkerFunc, enumPath);
}

void CSearchEngine::spawnSearchWorker(std::vector<std::filesystem::path> fileList) {
    auto searchWorkerFunc = [this](std::vector<std::filesystem::path> const fileList) {
        size_t numMatches = 0;

        for(auto &filePath : fileList) {
            m_totalFilesSearched++;

            // m_searchedFilesMutex.lock();
            // m_searchedFilesSet.erase(filePath.wstring());
            // m_searchedFilesMutex.unlock();

            bool isMatch = matchesAllFilters(filePath);

            if(isMatch) {
                m_totalMatches++;

                notifyAllObservers(filePath);
            }
        }

        m_pendingOperations--;
    };

    // Before enqueuing the worker thread, add one
    // to the pending operations count.
    m_pendingOperations++;

    // Enqueue the worker thread
    m_threadPool->enqueue(searchWorkerFunc, std::move(fileList));
}

bool CSearchEngine::matchesAllFilters(std::filesystem::path const &filePath) {
    std::vector<IFilter *> filters = m_searchQuery->getFilters();

    for(auto &f : filters) {
        bool isMatch = false;

        isMatch = f->filterFile(filePath);

        if(!isMatch) {
            return false;
        }
    }

    return true;
}

void CSearchEngine::notifyAllObservers(std::filesystem::path const &matchedFile) {
    std::vector<ISearchObserver *> resultObservers = m_searchQuery->getResultObservers();

    for(auto &observer : resultObservers) {
        observer->onFileMatched(matchedFile);
    }
}

int CSearchEngine::getPendingOperations() {
    return m_pendingOperations.load();
}

int CSearchEngine::getTotalFilesToSearch() {
    return m_totalFilesToSearch.load();
}

int CSearchEngine::getTotalFilesSearched() {
    return m_totalFilesSearched.load();
}

int CSearchEngine::getTotalMatches() {
    return m_totalMatches.load();
}