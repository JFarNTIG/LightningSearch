// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>

#include <filesystem>
#include <vector>

/**
 * @brief A widget that shows a list of search folder paths.
 *
 * Provides two buttons:
 *  - "Add Search Folder": lets user pick a folder via QFileDialog, then adds its path to the list.
 *  - "Remove": removes the currently selected folder from the list.
 *
 * Use allSearchFolders() to retrieve the current list of search folders.
 */
class CFolderListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CFolderListWidget(QWidget *parent = nullptr);
    virtual ~CFolderListWidget() = default;

    /**
     * @brief Returns all folder paths as a vector of std::filesystem::path.
     */
    std::vector<std::filesystem::path> allSearchFolders() const;

private slots:
    /**
     * @brief Called when the "Add Search Folder" button is pressed.
     *        Opens a directory dialog and, if a folder is chosen, adds it to the list.
     */
    void onAddFolderClicked();

    /**
     * @brief Called when the "Remove" button is pressed.
     *        Removes the currently selected folder from the list.
     */
    void onRemoveFolderClicked();

private:
    QListView *m_listView;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;

    QStringListModel *m_model;
};
