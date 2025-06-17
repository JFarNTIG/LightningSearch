// SPDX-License-Identifier: GPL-2.0
#include <ui/CFilterBuildWidget.hpp>
#include <ui/CFilterContentsWidget.hpp>
#include <ui/CFilterNameWidget.hpp>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

// Qt MOC source file
#include "ui/moc_CFilterBuildWidget.cpp"

CFilterBuildWidget::CFilterBuildWidget(QWidget *parent)
    : QWidget(parent)
{
    // Create combo box
    m_filterTypeCombo = new QComboBox(this);

    // Create the stacked widget
    m_stackedWidget = new QStackedWidget(this);

    // Add combo box options
    m_filterTypeCombo->addItem(tr("File Name"));
    m_filterTypeCombo->addItem(tr("File Contents"));

    // Create filter widgets
    CFilterNameWidget *m_nameWidget = new CFilterNameWidget(this);
    CFilterContentsWidget *m_contentsWidget = new CFilterContentsWidget(this);

    // Add them to the stacked widget
    m_stackedWidget->addWidget(m_nameWidget);   // index 0
    m_stackedWidget->addWidget(m_contentsWidget);

    // Create the "Add" button
    m_addButton = new QPushButton(tr("Add"), this);

    // Connect signals and slots
    connect(m_filterTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CFilterBuildWidget::onFilterTypeChanged);

    // When "Add" is clicked, we create the filter and emit filterCreated()
    connect(m_addButton, &QPushButton::clicked,
            this, &CFilterBuildWidget::onAddFilterClicked);

    // Lay out widgets
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *comboLayout = new QHBoxLayout();
    comboLayout->addWidget(new QLabel(tr("Filter by:"), this));
    comboLayout->addWidget(m_filterTypeCombo);

    mainLayout->addLayout(comboLayout);
    mainLayout->addWidget(m_stackedWidget);
    mainLayout->addWidget(m_addButton);

    setLayout(mainLayout);
}

void CFilterBuildWidget::onFilterTypeChanged(int index)
{
    // Switch the stacked widget to the appropriate index
    m_stackedWidget->setCurrentIndex(index);
}

void CFilterBuildWidget::onAddFilterClicked()
{
    // Determine which widget is currently visible in the stack.
    // We receive the widget pointer from Qt as a QWidget *.
    QWidget *current = m_stackedWidget->currentWidget();

    // The filter widget is also an instance of IFilterWidget, which
    // is what we want to send to whoever connected to the signal.
    IFilterWidget *filterWidget = dynamic_cast<IFilterWidget*>(current);

    // If the widget is not actually an instance of IFilterWidget,
    // then dynamic_cast will return nullptr. (this should never happen)
    // However, we only want to create the filter and emit the signal if the cast
    // actually succeeded.
    if(filterWidget) {
        // Create the filter.
        IFilter *filter = filterWidget->createFilter();

        // Emit the signal. Whoever is connected receives the new filter.
        emit filterCreated(filter);
    }
}
