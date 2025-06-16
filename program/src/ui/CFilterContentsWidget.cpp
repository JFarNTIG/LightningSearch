// SPDX-License-Identifier: GPL-2.0
#include <ui/CFilterContentsWidget.hpp>
#include <search/CFilterContents.hpp>

// Qt MOC source file
#include "ui/moc_CFilterContentsWidget.cpp"

CFilterContentsWidget::CFilterContentsWidget(QWidget* parent)
    : IFilterWidget(parent)
{
    // Create UI elements
    auto *layout = new QHBoxLayout(this);

    auto *label = new QLabel(tr("Search Text"), this);
    m_lineEdit = new QLineEdit(this);

    m_caseSensitiveCheck = new QCheckBox(tr("Case sensitive"), this);
    m_caseSensitiveCheck->setChecked(false);

    m_wholeMatchCheck = new QCheckBox(tr("Whole match"), this);
    m_wholeMatchCheck->setChecked(false);

    m_regexModeCheck = new QCheckBox(tr("Regex mode"), this);
    m_regexModeCheck->setChecked(false);

    // Add them to the layout
    layout->addWidget(label);
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_caseSensitiveCheck);
    layout->addWidget(m_wholeMatchCheck);
    layout->addWidget(m_regexModeCheck);

    setLayout(layout);
}

IFilter *CFilterContentsWidget::createFilter()
{
    std::wstring text = m_lineEdit->text().toStdWString();
    bool caseSensitive = m_caseSensitiveCheck->isChecked();
    bool wholeMatch = m_wholeMatchCheck->isChecked();
    bool regexMode = m_regexModeCheck->isChecked();

    return new CFilterContents(text, !caseSensitive, wholeMatch, regexMode);
}