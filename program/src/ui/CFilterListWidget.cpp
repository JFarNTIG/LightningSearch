// SPDX-License-Identifier: GPL-2.0
#include <ui/CFilterListWidget.hpp>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QModelIndex>

// Qt MOC source file
#include "ui/moc_CFilterListWidget.cpp"

CFilterListWidget::CFilterListWidget(QWidget *parent)
    : QWidget(parent)
{
    // Create child widgets
    m_filterBuildWidget = new CFilterBuildWidget(this);
    m_filterListView = new QListView(this);
    m_removeButton = new QPushButton(tr("Remove"), this);

    // Create our custom model
    m_filterModel = new CFilterListModel(this);

    // Assign the model to the list view
    m_filterListView->setModel(m_filterModel);

    // Connect signals
    connect(m_filterBuildWidget, &CFilterBuildWidget::filterCreated,
            this, &CFilterListWidget::onFilterCreated);
    connect(m_removeButton, &QPushButton::clicked,
            this, &CFilterListWidget::onRemoveFilterClicked);

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_filterBuildWidget);
    mainLayout->addWidget(m_filterListView);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_removeButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

std::vector<IFilter*> CFilterListWidget::extractFilters()
{
    return m_filterModel->extractFilters();
}

void CFilterListWidget::onFilterCreated(IFilter *filter)
{
    // The model will now take care of storing and eventually deleting it
    m_filterModel->addFilter(filter);
}

void CFilterListWidget::onRemoveFilterClicked()
{
    QModelIndex currentIndex = m_filterListView->currentIndex();
    
    if (!currentIndex.isValid()) {
        QMessageBox::information(this, tr("No Selection"),
                                 tr("Please select a filter to remove."));
        return;
    }

    m_filterModel->removeFilterAt(currentIndex.row());
}
