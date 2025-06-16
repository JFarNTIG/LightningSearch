// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <search/IFilter.hpp>

#include <filesystem>
#include <vector>
#include <memory>
#include <sstream>

class CFilterCombine : public IFilter {
public:
    enum class Mode { AND, OR };

    CFilterCombine(Mode mode)
        : m_mode(mode)
    {}

    virtual ~CFilterCombine() {
        for(auto &filter : m_filters) {
            delete filter;
        }

        m_filters.clear();
    }

    void addFilter(IFilter *filter) {
        m_filters.push_back(filter);
    }

    virtual bool filterFile(std::filesystem::path const &filePath) {
        if(m_filters.empty()) {
            return true;
        }

        switch(m_mode) {
            case Mode::AND:
                for(auto &f : m_filters) {
                    if(!f->filterFile(filePath)) {
                        return false;
                    }
                }
                return true;

            case Mode::OR:
                for(auto &f : m_filters) {
                    if(f->filterFile(filePath)) {
                        return true;
                    }
                }
                return false;
        }
        return false;
    }

    /**
     * @brief Represent the filter and its options as a
     * text string.
     */
    virtual std::wstring getText() {
        std::wstringstream wss;
        wss << L"Multiple filters";
        return wss.str();
    }

private:
    std::vector<IFilter *> m_filters;
    Mode m_mode;
};
