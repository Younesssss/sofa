/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2019 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program. If not, see <http://www.gnu.org/licenses/>.              *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_GUI_QT_QMOUSEWHEELADJUSTEMENTGUARD_H
#define SOFA_GUI_QT_QMOUSEWHEELADJUSTEMENTGUARD_H

#include <QObject>

namespace sofa
{

namespace gui
{

namespace qt
{

/// @brief Filter qt events to allows wheel event to only be accepted when the widget has focus.
///
/// To use it you need to do:
///    myWidget->setFocusPolicy(Qt::StrongFocus);
///    myWidget->installEventFilter(new MouseWheelWidgetAdjustmentGuard(ui.comboBox));
///
/// This code is grabbed from:
/// https://stackoverflow.com/questions/5821802/qspinbox-inside-a-qscrollarea-how-to-prevent-spin-box-from-stealing-focus-when
class QMouseWheelAdjustmentGuard : public QObject
{
public:
    explicit QMouseWheelAdjustmentGuard(QObject *parent);

protected:
    bool eventFilter(QObject* o, QEvent* e) override;
};

} /// namespace qt
} /// namespace gui
} /// namespace sofa

#endif /// SOFA_GUI_QT_QMOUSEWHEELADJUSTEMENTGUARD_H

