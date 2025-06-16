// SPDX-License-Identifier: GPL-2.0
#include <ui/CFolderListWidget.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

// Qt MOC source file
#include "ui/moc_CFolderListWidget.cpp"

CFolderListWidget::CFolderListWidget(QWidget *parent)
    : QWidget(parent)
{
    // Create child widgets
    m_listView   = new QListView(this);
    m_addButton  = new QPushButton(tr("Add Search Folder"), this);
    m_removeButton = new QPushButton(tr("Remove"), this);

    // Create and set the model
    m_model = new QStringListModel(this);
    m_listView->setModel(m_model);

    // Connect signals/slots
    connect(m_addButton, &QPushButton::clicked,
            this, &CFolderListWidget::onAddFolderClicked);

    connect(m_removeButton, &QPushButton::clicked,
            this, &CFolderListWidget::onRemoveFolderClicked);

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_removeButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_listView);

    setLayout(mainLayout);
}

std::vector<std::filesystem::path> CFolderListWidget::allSearchFolders() const
{
    // Convert each QString in the model to a std::filesystem::path
    std::vector<std::filesystem::path> results;
    QStringList folderList = m_model->stringList();
    results.reserve(folderList.size());

    for (QString const &folder : folderList) {
        // Convert each folder to a wstring.
        results.emplace_back(folder.toStdWString());
    }

    return results;
}

void CFolderListWidget::onAddFolderClicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(
        this,
        tr("Select Search Folder"),
        QString()
    );

    if (!dirPath.isEmpty()) {
        // Append to the model's list
        QStringList currentList = m_model->stringList();
        currentList.append(dirPath);
        m_model->setStringList(currentList);
    }
}

void CFolderListWidget::onRemoveFolderClicked()
{
    QModelIndex currentIndex = m_listView->currentIndex();
    
    if (!currentIndex.isValid()) {
        QMessageBox::information(
            this,
            tr("No Selection"),
            tr("Please select a folder to remove.")
        );
        return;
    }

    // Remove the selected row from the model
    m_model->removeRow(currentIndex.row());
}
