// SPDX-License-Identifier: GPL-2.0
#include <search/CSearchQuery.hpp>

CSearchQuery::CSearchQuery() {
    // nothing to do
}

CSearchQuery::~CSearchQuery() {
    for(IFilter *filter : m_filters) {
        delete filter;
    }

    m_filters.clear();
}

void CSearchQuery::setDirectories(std::vector<std::filesystem::path> const &searchPaths) {
    m_searchPaths = searchPaths;
}

std::vector<std::filesystem::path> CSearchQuery::getDirectories() const {
    return m_searchPaths;
}

void CSearchQuery::setFilters(std::vector<IFilter *> const &filters) {
    m_filters = filters;
}

std::vector<IFilter *> CSearchQuery::getFilters() const {
    return m_filters;
}

void CSearchQuery::addResultObserver(ISearchObserver *observer) {
    m_observers.push_back(observer);
}

std::vector<ISearchObserver *> CSearchQuery::getResultObservers() const {
    return m_observers;
}