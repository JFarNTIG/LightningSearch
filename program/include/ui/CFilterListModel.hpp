// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <search/IFilter.hpp>

#include <QAbstractListModel>

#include <vector>

/**
 * @brief This class stores a list of filters. Each row corresponds to one filter.
 * Implements QAbstractListModel so that the filter list can be
 * shown in the UI.
 */
class CFilterListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CFilterListModel(QObject *parent = nullptr);
    virtual ~CFilterListModel();

    Q_INVOKABLE virtual int rowCount(QModelIndex const &parent = QModelIndex()) const;
    Q_INVOKABLE virtual QVariant data(QModelIndex const &index, int role = Qt::DisplayRole) const;

    /**
     * @brief Add a filter to the filter list.
     * The filter list model assumes ownership of the filter object.
     */
    void addFilter(IFilter *filter);

    /**
     * @brief Remove the filter at the specified index.
     */
    void removeFilterAt(int row);

    /**
     * @brief Get the filter at the specified index.
     */
    IFilter *getFilterAt(int row) const;

    /**
     * @brief Get the list of filters as a vector.
     * 
     * NOTE: This will clear the internal list of filters.
     * Ownership of the filter objects is transferred to the caller
     * of this function.
     */
    std::vector<IFilter*> extractFilters();

private:
    std::vector<IFilter*> m_filters;
};
