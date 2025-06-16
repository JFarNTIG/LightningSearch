// SPDX-License-Identifier: GPL-2.0
#include <ui/CStartSearchDialog.hpp>

#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

// Qt MOC source file
#include "ui/moc_CStartSearchDialog.cpp"

CStartSearchDialog::CStartSearchDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Start Search"));

    // Create the QTabWidget
    QTabWidget *tabWidget = new QTabWidget(this);

    m_folderListWidget = new CFolderListWidget(this);
    m_filterListWidget = new CFilterListWidget(this);

    // Directories
    QWidget *directoriesTab = new QWidget(this);
    QVBoxLayout *directoriesLayout = new QVBoxLayout(directoriesTab);
    directoriesLayout->addWidget(m_folderListWidget);
    directoriesTab->setLayout(directoriesLayout);

    // Filters
    QWidget *filtersTab = new QWidget(this);
    QVBoxLayout *filtersLayout = new QVBoxLayout(filtersTab);
    filtersLayout->addWidget(m_filterListWidget);
    filtersTab->setLayout(filtersLayout);

    // Settings
    QWidget *settingsTab = new QWidget(this);
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsTab);
    settingsLayout->addWidget(new QLabel(tr("Settings tab content goes here."), this));
    settingsTab->setLayout(settingsLayout);

    // Add the tabs to the QTabWidget
    tabWidget->addTab(directoriesTab, tr("Directories"));
    tabWidget->addTab(filtersTab, tr("Filters"));
    tabWidget->addTab(settingsTab, tr("Settings"));

    // Create the buttons
    QPushButton *searchButton = new QPushButton(tr("Search"), this);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);

    // Connect the buttons to standard dialog accept/reject
    connect(searchButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    // Layout for the buttons at the bottom
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(cancelButton);

    // Main layout for the dialog
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

CStartSearchDialog::~CStartSearchDialog()
{
    // nothing to do
}

std::vector<std::filesystem::path> CStartSearchDialog::getDirectories() {
    return m_folderListWidget->allSearchFolders();
}

std::vector<IFilter *> CStartSearchDialog::getFilters() {
    return m_filterListWidget->extractFilters();
}