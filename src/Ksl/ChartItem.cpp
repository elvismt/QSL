/*
 * Copyright (C) 2016  Elvis Teixeira
 *
 * This source code is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <Ksl/ChartItem_p.h>
#include <Ksl/ChartScale.h>
#include <Ksl/ChartEngine.h>

KSL_BEGIN_NAMESPACE

ChartEngine* ChartItem::chartEngine() const {
    KSL_PUBLIC(const ChartItem);
    return m->chartEngine;
}

ChartScale* ChartItem::scale() const {
    KSL_PUBLIC(const ChartItem);
    return m->scale;
}

QString ChartItem::name() const {
    KSL_PUBLIC(const ChartItem);
    return m->name;
}

bool ChartItem::visible() const {
    KSL_PUBLIC(const ChartItem);
    return m->visible;
}

bool ChartItem::rescalable() const {
    KSL_PUBLIC(const ChartItem);
    return m->rescalable;
}

void ChartItem::setScale(ChartScale *scale) {
    KSL_PUBLIC(ChartItem);
    ChartEngine *newChart = scale ? scale->chartEngine() : nullptr;
    ChartEngine *oldChart = m->chartEngine;

    m->chartEngine = newChart;
    m->scale = scale;

    if (oldChart) {
        disconnect(this, &ChartItem::appearenceChanged,
                   oldChart, &ChartEngine::onAppearenceChange);
        disconnect(this, &ChartItem::dataChanged,
                   oldChart, &ChartEngine::onDataChange);
    }
    if (newChart) {
        connect(this, &ChartItem::appearenceChanged,
                newChart, &ChartEngine::onAppearenceChange);
        connect(this, &ChartItem::dataChanged,
                newChart, &ChartEngine::onDataChange);
    }
}

void ChartItem::setName(const QString &name) {
    KSL_PUBLIC(ChartItem);
    if (m->name != name) {
        m->name = name;
        emit appearenceChanged(this);
    }
}

void ChartItem::setVisible(bool visible) {
    KSL_PUBLIC(ChartItem);
    if (m->visible != visible) {
        m->visible = visible;
        emit appearenceChanged(this);
    }
}

KSL_END_NAMESPACE
