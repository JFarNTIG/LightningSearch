// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <ui/IFilterWidget.hpp>

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>

class CFilterNameWidget : public IFilterWidget
{
    Q_OBJECT
public:
    explicit CFilterNameWidget(QWidget *parent = nullptr);

    /**
     * @brief Create a new filter object based on the widget's input fields.
     *
     * NOTE: The caller takes ownership of the returned filter object and is responsible for deleting it.
     */
    virtual IFilter *createFilter();

private:
    QLineEdit *m_lineEdit;
    QCheckBox *m_caseSensitiveCheck;
    QCheckBox *m_wholeMatchCheck;
    QCheckBox *m_regexModeCheck;
};
