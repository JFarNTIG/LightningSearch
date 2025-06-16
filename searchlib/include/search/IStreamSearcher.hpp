// SPDX-License-Identifier: GPL-2.0
#pragma once

#include <istream>

class IStreamSearcher {
public:
    virtual bool searchText(std::wistream &in) const = 0;
};