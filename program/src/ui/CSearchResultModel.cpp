// SPDX-License-Identifier: GPL-2.0
#include <ui/CSearchResultModel.hpp>

// Qt MOC source file
#include "ui/moc_CSearchResultModel.cpp"

CSearchResultModel::CSearchResultModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // nothing to do
}

CSearchResultModel::~CSearchResultModel() {
    // nothing to do
}

int CSearchResultModel::rowCount(QModelIndex const &parent) const {
    Q_UNUSED(parent);
    return static_cast<int>(m_results.size());
}

int CSearchResultModel::columnCount(QModelIndex const &parent) const {
    Q_UNUSED(parent);
    return 2;
}

QVariant CSearchResultModel::headerData(int section,
                                Qt::Orientation orientation,
                                int role) const
{
    // We only want to customize the horizontal headers (the column headers)
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Filename");
        case 1:
            return QString("Path");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}


QVariant CSearchResultModel::data(QModelIndex const &index, int role) const {
    if(!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    int row = index.row();
    int col = index.column();
    
    if(row < 0 || row >= static_cast<int>(m_results.size())) {
        return QVariant();
    }

    auto const &path = m_results[row];
    switch(col) {
    case 0:
        return QString::fromStdWString(path.filename().wstring());
    case 1:
        return QString::fromStdWString(path.wstring());
    default:
        return QVariant();
    }
}

void CSearchResultModel::onFileMatched(std::filesystem::path const &matchedFile) {
    // Important: This function will potentially be called from
    // other threads (the search worker threads).
    // 
    // Since the file path was passed as an lvalue reference,
    // it would be unsafe to capture it in the lambda, because the
    // file path may go out of scope before the lambda is invoked.
    // Hence, we create a copy of the file path and capture the
    // copy in the lambda.
    std::filesystem::path filePathCopy(matchedFile);

    // Use QMetaObject to invoke fileResultAvailable callback.
    // This ensures that fileResultAvailable is called from the
    // main thread.
    QMetaObject::invokeMethod(this, [this, filePathCopy]() {
        fileResultAvailable(filePathCopy);
    }, Qt::QueuedConnection);
}

void CSearchResultModel::clear() {
    beginResetModel();
    m_results.clear();
    endResetModel();
}

void CSearchResultModel::fileResultAvailable(std::filesystem::path const matchedFile) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_results.push_back(matchedFile);
    endInsertRows();
}