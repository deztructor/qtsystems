/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Copyright (C) 2014 Jolla Ltd.
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSystems module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
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

#ifndef QBATTERYINFO_STATEFS_P_H
#define QBATTERYINFO_STATEFS_P_H

#include <qbatteryinfo.h>

class ContextProperty;

QT_BEGIN_NAMESPACE

class PropertyInfo;
class QBatteryInfoPrivate;
std::vector<PropertyInfo*> createInfo(QBatteryInfoPrivate *);

class QBatteryInfoPrivate : public QObject
{
    Q_OBJECT

public:
    QBatteryInfoPrivate(QBatteryInfo *);
    QBatteryInfoPrivate(int, QBatteryInfo *);
    ~QBatteryInfoPrivate() {}

    bool isValid() const;
    int level() const;
    int cycleCount() const;
    float temperature() const;
    int batteryCount() const;
    int batteryIndex() const;
    int currentFlow() const;
    int maximumCapacity() const;
    int remainingCapacity() const;
    int remainingChargingTime() const;
    int voltage() const;
    QBatteryInfo::ChargerType chargerType() const;
    QBatteryInfo::ChargingState chargingState() const;
    QBatteryInfo::LevelStatus levelStatus() const;
    QBatteryInfo::Health health() const;

    void setBatteryIndex(int);

Q_SIGNALS:
    // void batteryIndexChanged(int batteryIndex);
    void validChanged(bool isValid);
    //void batteryCountChanged(int count);
    void chargerTypeChanged(QBatteryInfo::ChargerType type);
    void chargingStateChanged(QBatteryInfo::ChargingState state);
    void levelChanged(int level);
    void currentFlowChanged(int flow);
    void cycleCountChanged(int cycleCount);
    void remainingCapacityChanged(int capacity);
    void remainingChargingTimeChanged(int seconds);
    void voltageChanged(int voltage);
    void levelStatusChanged(QBatteryInfo::LevelStatus levelStatus);
    void healthChanged(QBatteryInfo::Health health);
    void temperatureChanged(float temperature);

private:
    void init();

    friend class PropertyInfo;
    friend std::vector<PropertyInfo*> createInfo(QBatteryInfoPrivate *);
    std::vector<PropertyInfo*> properties_;

    void onPercentage(ContextProperty*);
    void onBatteryLevel(ContextProperty*);
    void onChargingState(ContextProperty*);
    void onTimeUntilFull(ContextProperty*);
    void onCurrent(ContextProperty*);
    void onVoltage(ContextProperty*);
    void onTemperature(ContextProperty*);
    void onCharger(ContextProperty*);
};

typedef void (QBatteryInfoPrivate::*PropertyFn)(ContextProperty *);
class PropertyInfo : public QObject
{
    Q_OBJECT
public:
    PropertyInfo(QString const &, QBatteryInfoPrivate *, PropertyFn);
    ContextProperty const *source() const { return source_; }

private slots:
    void onValueChanged();

private:
    ContextProperty *source_;
    PropertyFn fn_;
};

QT_END_NAMESPACE

#endif // QBATTERYINFO_STATEFS_P_H
