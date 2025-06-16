// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <ui/IFilterWidget.hpp>

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QStackedWidget>

class CFilterBuildWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CFilterBuildWidget(QWidget *parent = nullptr);
    virtual ~CFilterBuildWidget() = default;

signals:
    /**
     * @brief Emitted when the user clicks "Add" and we create a new IFilter object.
     *        The receiver must take ownership and eventually delete this pointer.
     */
    void filterCreated(IFilter *filter);

private slots:
    /**
     * @brief Switches the visible widget in the QStackedWidget.
     */
    void onFilterTypeChanged(int index);

    /**
     * @brief Called when the user clicks the Add button.
     * Creates the filter and emits filterCreated().
     */
    void onAddFilterClicked();

private:
    QComboBox *m_filterTypeCombo;
    QStackedWidget *m_stackedWidget;
    QPushButton *m_addButton;
};
