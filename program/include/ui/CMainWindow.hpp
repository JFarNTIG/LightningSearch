// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <search/CSearchEngine.hpp>
#include <ui/CSearchResultModel.hpp>
#include <ui/CFilterListWidget.hpp>
#include <ui/CFolderListWidget.hpp>

#include <QAction>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTableView>
#include <QFileDialog>
#include <QTimer>
#include <QStatusBar>

#include <memory>

class CMainWindow : public QMainWindow {
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = nullptr);
    virtual ~CMainWindow();

private slots:
    void onSearchClicked();
    void updateTick();

private:
    void setupUI();
    void setupConnections();
    void createActions();
    void createMenus();

    CSearchEngine *m_searchEngine;

    QTimer *m_updateTimer;
    QPushButton *m_searchBtn;
    QTableView *m_tableView;

    CSearchResultModel *m_resultModel; // Owned by the Qt parent system

    QAction *m_newAct;
    QAction *m_exitAct;

    QMenu *m_fileMenu;
};
