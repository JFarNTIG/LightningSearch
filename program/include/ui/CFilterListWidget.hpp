// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <ui/CFilterListModel.hpp>
#include <ui/CFilterBuildWidget.hpp>

#include <QWidget>
#include <QListView>
#include <QPushButton>

/**
 * @brief This widget contains a filter build widget (CFilterBuildWidget)
 * and couples it with a list of active filters. Widgets created in the filter build
 * widget are added to the list, and there are controls for removing and
 * managing filters in the list.
 * 
 * We use a custom model (CFilterListModel) which displays
 * a textual representation for each filter.
 */
class CFilterListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CFilterListWidget(QWidget *parent = nullptr);
    virtual ~CFilterListWidget() = default;

    /**
     * @brief Returns a copy of all current filters in the model.
     */
    std::vector<IFilter*> extractFilters();

private slots:
    /**
     * @brief Called when CFilterBuildWidget emits filterCreated().
     * Adds a new filter to the model.
     */
    void onFilterCreated(IFilter *filter);

    /**
     * @brief Called when the user clicks on the Remove button.
     * Removes the currently selected filter from the model.
     */
    void onRemoveFilterClicked();

private:
    CFilterBuildWidget *m_filterBuildWidget;
    QListView *m_filterListView;
    QPushButton *m_removeButton;

    CFilterListModel *m_filterModel;
};
