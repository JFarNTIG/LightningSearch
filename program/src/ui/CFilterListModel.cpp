// SPDX-License-Identifier: GPL-2.0
#include <ui/CFilterListModel.hpp>
#include <QString>

// Qt MOC source file
#include "ui/moc_CFilterListModel.cpp"

CFilterListModel::CFilterListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

CFilterListModel::~CFilterListModel()
{
    // Delete all stored filter pointers
    for(IFilter *filter : m_filters) {
        delete filter;
    }

    m_filters.clear();
}

int CFilterListModel::rowCount(QModelIndex const &parent) const
{
    if(parent.isValid()) {
        // For a simple list model, we ignore parent
        return 0;
    }
    return static_cast<int>(m_filters.size());
}

QVariant CFilterListModel::data(QModelIndex const &index, int role) const
{
    if(!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    int row = index.row();
    if(row < 0 || row >= static_cast<int>(m_filters.size())) {
        return QVariant();
    }

    IFilter *filter = m_filters[row];
    if(!filter) {
        return QVariant();
    }

    std::wstring text = filter->getText();
    return QString::fromStdWString(text);
}

void CFilterListModel::addFilter(IFilter *filter)
{
    if(!filter) {
        return;
    }

    int newRow = static_cast<int>(m_filters.size());

    // We must call beginInsertRows / endInsertRows to notify views
    // that the data is changing.
    beginInsertRows(QModelIndex(), newRow, newRow);
    m_filters.push_back(filter);
    endInsertRows();
}

void CFilterListModel::removeFilterAt(int row)
{
    if(row < 0 || row >= static_cast<int>(m_filters.size())) {
        return;
    }

    // We must call beginRemoveRows / endRemoveRows to notify views
    // that the data is changing.
    beginRemoveRows(QModelIndex(), row, row);

    // Destroy the filter object.
    IFilter *filter = m_filters[row];
    delete filter;

    // Erase the filter object from the vector.
    m_filters.erase(m_filters.begin() + row);

    endRemoveRows();
}

IFilter *CFilterListModel::getFilterAt(int row) const
{
    // Make sure that the specified row is actually a valid 
    // index in the vector.

    if(row < 0 || row >= static_cast<int>(m_filters.size())) {

        // In this case, we return nullptr to prevent an array out of bounds
        // crash.

        return nullptr;
    }

    return m_filters[row];
}

std::vector<IFilter*> CFilterListModel::extractFilters()
{
    // Return a copy of the vector
    std::vector<IFilter*> filterList = m_filters;
    m_filters.clear();
    return filterList;
}
