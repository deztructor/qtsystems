/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtSystems module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#ifndef QJSONDBWRAPPER_P_H
#define QJSONDBWRAPPER_P_H

#include <QObject>
#include <QMap>

#include <jsondb-global.h>

Q_ADDON_JSONDB_BEGIN_NAMESPACE
class JsonDbClient;
Q_ADDON_JSONDB_END_NAMESPACE
Q_USE_JSONDB_NAMESPACE

QT_BEGIN_NAMESPACE

class QJsonDbWrapper : public QObject
{
    Q_OBJECT
public:
    QJsonDbWrapper(QObject *parent = 0);
    virtual ~QJsonDbWrapper();

    // DeviceInfo Interface
    QString getUniqueDeviceID();

Q_SIGNALS:
    void responseAvailable();

private Q_SLOTS:
    void onError(int, int, QString);
    void onResponse(int reqId, const QVariant& response);
    void onTimerExpired();

private:
    QVariant getProperty(const QString objectType, const QString property);

    JsonDbClient *jsonclient;
    QMap<int,QVariant> responses;
};

QT_END_NAMESPACE

#endif // QJSONDBWRAPPER_P_H