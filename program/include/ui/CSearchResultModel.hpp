// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <search/ISearchObserver.hpp>

#include <QAbstractTableModel>
#include <QString>

#include <string>
#include <vector>
#include <filesystem>

class CSearchResultModel : public QAbstractTableModel, public ISearchObserver {
    Q_OBJECT
    
public:
    CSearchResultModel(QObject *parent = nullptr);
    virtual ~CSearchResultModel();

    // Implementation of QAbstractTableModel::rowCount
    Q_INVOKABLE virtual int rowCount(QModelIndex const &parent = QModelIndex()) const;

    // Implementation of QAbstractTableModel::columnCount
    Q_INVOKABLE virtual int columnCount(QModelIndex const &parent = QModelIndex()) const;

    // Implementation of QAbstractTableModel::headerData
    Q_INVOKABLE virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
                                
    // Implementation of QAbstractTableModel::data
    Q_INVOKABLE virtual QVariant data(QModelIndex const &index, int role) const;

    // Implementation of ISearchObserver::on_file_matched
    virtual void onFileMatched(std::filesystem::path const &matchedFile);

    void clear();

private:
    void fileResultAvailable(std::filesystem::path const matchedFile);

    std::vector<std::filesystem::path> m_results;
};
