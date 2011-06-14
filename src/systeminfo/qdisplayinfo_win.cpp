/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtSystemKit module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdisplayinfo_win_p.h"

#include <QtGui/qapplication.h>
#include <QtGui/qdesktopwidget.h>
#include <QtGui/qpixmap.h>

QT_BEGIN_NAMESPACE

QDisplayInfoPrivate::QDisplayInfoPrivate(QDisplayInfo *parent)
    : q_ptr(parent)
    , hDC(NULL)
{
}

QDisplayInfoPrivate::~QDisplayInfoPrivate()
{
    ReleaseDC(NULL, hDC);
}

int QDisplayInfoPrivate::colorDepth(int screen)
{
    if (hDC == NULL)
        hDC = GetDC(qApp->desktop()->screen(screen)->winId());
    return GetDeviceCaps(hDC, LOGPIXELSX);
}

int QDisplayInfoPrivate::contrast(int screen)
{
    Q_UNUSED(screen)
    return -1;
}

int QDisplayInfoPrivate::displayBrightness(int screen)
{
    Q_UNUSED(screen)
    return -1;
}

int QDisplayInfoPrivate::dpiX(int screen)
{
    if (hDC == NULL)
        hDC = GetDC(qApp->desktop()->screen(screen)->winId());
    return GetDeviceCaps(hDC, LOGPIXELSX);
}

int QDisplayInfoPrivate::dpiY(int screen)
{
    if (hDC == NULL)
        hDC = GetDC(qApp->desktop()->screen(screen)->winId());
    return GetDeviceCaps(hDC, LOGPIXELSY);
}

int QDisplayInfoPrivate::physicalHeight(int screen)
{
    if (hDC == NULL)
        hDC = GetDC(qApp->desktop()->screen(screen)->winId());
    return GetDeviceCaps(hDC, VERTSIZE);
}

int QDisplayInfoPrivate::physicalWidth(int screen)
{
    if (hDC == NULL)
        hDC = GetDC(qApp->desktop()->screen(screen)->winId());
    return GetDeviceCaps(hDC, HORZSIZE);
}

QDisplayInfo::BacklightState QDisplayInfoPrivate::backlightState(int screen)
{
    Q_UNUSED(screen)
    return QDisplayInfo::BacklightUnknown;
}

QT_END_NAMESPACE