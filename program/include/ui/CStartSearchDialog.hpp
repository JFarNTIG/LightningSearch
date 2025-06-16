// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <ui/CFilterListWidget.hpp>
#include <ui/CFolderListWidget.hpp>

#include <QDialog>

class CStartSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CStartSearchDialog(QWidget *parent = nullptr);
    virtual ~CStartSearchDialog();

    std::vector<std::filesystem::path> getDirectories();
    std::vector<IFilter *> getFilters();

private:
    CFolderListWidget *m_folderListWidget;
    CFilterListWidget *m_filterListWidget;
};
