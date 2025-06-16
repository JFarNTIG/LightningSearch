// SPDX-License-Identifier: GPL-2.0
#include <ui/CMainWindow.hpp>

#include <ui/CFilterListWidget.hpp>
#include <ui/CStartSearchDialog.hpp>

#include <QCoreApplication>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>

// Qt MOC source file
#include "ui/moc_CMainWindow.cpp"

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupConnections();
    
    // Create the results model
    m_resultModel = new CSearchResultModel(this);

    // Set model to the QTableView
    m_tableView->setModel(m_resultModel);

    m_searchEngine = nullptr;

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateTick()));
    m_updateTimer->start(500);
}

CMainWindow::~CMainWindow() {
    if(m_searchEngine) {
        delete m_searchEngine;
    }
}

void CMainWindow::onSearchClicked() {
    CStartSearchDialog dialog(this);
    
    if(dialog.exec() == QDialog::Rejected) {
        return;
    }

    // Clear previous results
    m_resultModel->clear();

    if(m_searchEngine != nullptr) {
        delete m_searchEngine;
    }

    CSearchQuery *searchQuery = new CSearchQuery;
    searchQuery->setDirectories(dialog.getDirectories());
    searchQuery->setFilters(dialog.getFilters());
    searchQuery->addResultObserver(m_resultModel);
    m_searchEngine = new CSearchEngine(searchQuery);

    // start the search
    m_searchEngine->performSearch();
}

void CMainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    m_searchBtn = new QPushButton("Search", centralWidget);
    m_tableView = new QTableView(centralWidget);

    QHBoxLayout *searchSettingsLayout = new QHBoxLayout;

    mainLayout->addLayout(searchSettingsLayout);

    mainLayout->addWidget(m_searchBtn);
    mainLayout->addWidget(m_tableView);

    createActions();
    createMenus();

    setMinimumSize(640, 480);
    resize(720, 500);
}

void CMainWindow::setupConnections() {
    connect(m_searchBtn, &QPushButton::clicked, this, &CMainWindow::onSearchClicked);
}

void CMainWindow::updateTick() {
    if(m_searchEngine == nullptr) {
        statusBar()->showMessage("Ready");
    } else {
        int pendingOperations = m_searchEngine->getPendingOperations();
        int totalFilesToSearch = m_searchEngine->getTotalFilesToSearch();
        int totalFilesSearched = m_searchEngine->getTotalFilesSearched();
        int totalMatches = m_searchEngine->getTotalMatches();

        QString pendingOperationsText = QString("Pending operations: ") + QString::number(pendingOperations);
        QString filesToSearchText = QString("Total files: ") + QString::number(totalFilesToSearch);
        QString filesSearchedText = QString("Searched: ") + QString::number(totalFilesSearched);
        QString matchesText = QString("Matches: ") + QString::number(totalMatches);

        if(pendingOperations == 0) {
            QString progressText = filesSearchedText + QString(" | ") +
                                    matchesText;
            statusBar()->showMessage(QString("Search completed! ") + progressText);
        } else {
            QString progressText = filesToSearchText + QString(" | ") +
                                    filesSearchedText + QString(" | ") +
                                    matchesText;
            statusBar()->showMessage(QString("Searching... ") + progressText);
        }
    }
}

void CMainWindow::createActions() {
    m_newAct = new QAction(tr("&New"), this);

    m_newAct->setShortcuts(QKeySequence::New);
    m_newAct->setStatusTip(tr("Start a new search"));

    connect(m_newAct, &QAction::triggered, this, &CMainWindow::onSearchClicked);

    m_exitAct = new QAction(tr("E&xit"), this);

    m_exitAct->setShortcuts(QKeySequence::Quit);
    m_exitAct->setStatusTip(tr("Exit the program"));

    connect(m_exitAct, &QAction::triggered, qApp, &QCoreApplication::quit, Qt::QueuedConnection);
}

void CMainWindow::createMenus() {
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAct);
}