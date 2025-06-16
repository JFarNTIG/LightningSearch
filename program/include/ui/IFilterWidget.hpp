// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <search/IFilter.hpp>

#include <QWidget>

/**
 * @brief An interface representing a widget that produces filter objects.
 */
class IFilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IFilterWidget(QWidget *parent = nullptr);
    virtual ~IFilterWidget();

    /**
     * @brief Create a new filter object based on the widget's input fields.
     *
     * NOTE: The caller takes ownership of the returned filter object and is responsible for deleting it.
     */
    virtual IFilter* createFilter() = 0;
};
