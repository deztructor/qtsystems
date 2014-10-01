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

#include "qbatteryinfo_statefs_p.h"
#include <contextproperty.h>
#include <QDebug>

QT_BEGIN_NAMESPACE


#define QS_(str) QStringLiteral(str)

PropertyInfo::PropertyInfo(QString const &name
                           , QBatteryInfoPrivate *parent
                           , PropertyFn fn)
    : QObject(parent)
    , source_(new ContextProperty(name, this))
    , fn_(fn)
{
    connect(source_, &ContextProperty::valueChanged
            , this, &PropertyInfo::onValueChanged);
}

void PropertyInfo::onValueChanged()
{
    auto obj = static_cast<QBatteryInfoPrivate*>(parent());
    (obj->*fn_)(source_);
}

enum class Prop {
    Percentage
        , BatteryLevel
        , ChargingState
        , TimeUntilFull
        , Current
        , Voltage
        , Temperature
        , Charger
        , Last_ = Charger
};

static ContextProperty const * get(std::vector<PropertyInfo*> const &v, Prop p)
{
    return v[static_cast<size_t>(p)]->source();
}

static QVariant value(std::vector<PropertyInfo*> const &v, Prop p)
{
    return get(v, p)->value();
}

static QBatteryInfo::ChargerType chargerTypeFromName(QString const &name)
{
    static const QMap<QString, QBatteryInfo::ChargerType> types = {{
            {"unknown", QBatteryInfo::UnknownCharger}
            , {"", QBatteryInfo::UnknownCharger}
            , {"dcp", QBatteryInfo::WallCharger}
            , {"usb", QBatteryInfo::USBCharger}
            , {"cdp", QBatteryInfo::WallCharger}
        }};
    return types.value(name, QBatteryInfo::UnknownCharger);
}

static QBatteryInfo::ChargingState chargingStateFromName(QString const &name)
{
    static const QMap<QString, QBatteryInfo::ChargingState> states = {
        {"unknown", QBatteryInfo::UnknownChargingState }
        , {"charging", QBatteryInfo::Charging}
        , {"discharging", QBatteryInfo::Discharging}
        , {"idle", QBatteryInfo::IdleChargingState}
    };
    return states.value(name, QBatteryInfo::UnknownChargingState);
}

std::vector<PropertyInfo*> createInfo(QBatteryInfoPrivate *parent)
{
    return std::vector<PropertyInfo*> {{
            new PropertyInfo(QS_("Battery.ChargePercentage"), parent
                             , &QBatteryInfoPrivate::onPercentage),
                new PropertyInfo(QS_("Battery.Level"), parent
                                 , &QBatteryInfoPrivate::onBatteryLevel),
                new PropertyInfo(QS_("Battery.ChargingState"), parent
                                 , &QBatteryInfoPrivate::onChargingState),
                new PropertyInfo(QS_("Battery.TimeUntilFull"), parent
                                 , &QBatteryInfoPrivate::onTimeUntilFull),
                new PropertyInfo(QS_("Battery.Current"), parent
                                 , &QBatteryInfoPrivate::onCurrent),
                new PropertyInfo(QS_("Battery.Voltage"), parent
                                 , &QBatteryInfoPrivate::onVoltage),
                new PropertyInfo(QS_("Battery.Temperature"), parent
                                 , &QBatteryInfoPrivate::onTemperature),
                new PropertyInfo(QS_("Battery.ChargerType"), parent
                                 , &QBatteryInfoPrivate::onCharger)
                }};
}

QBatteryInfoPrivate::QBatteryInfoPrivate(QBatteryInfo *parent)
    : QObject(parent)
    , properties_(std::move(createInfo(this)))
{
    init();
}

QBatteryInfoPrivate::QBatteryInfoPrivate(int, QBatteryInfo *parent)
    : QObject(parent)
    , properties_(std::move(createInfo(this)))
{
    init();
}

void QBatteryInfoPrivate::init()
{
}

void QBatteryInfoPrivate::onPercentage(ContextProperty*)
{
    emit remainingCapacityChanged(remainingCapacity());
}

void QBatteryInfoPrivate::onBatteryLevel(ContextProperty *)
{
    emit levelStatusChanged(levelStatus());
    emit validChanged(isValid());
}

void QBatteryInfoPrivate::onChargingState(ContextProperty *)
{
    emit chargingStateChanged(chargingState());
    emit validChanged(isValid());
}

void QBatteryInfoPrivate::onTimeUntilFull(ContextProperty*)
{
    emit remainingChargingTimeChanged(remainingChargingTime());
}

void QBatteryInfoPrivate::onCurrent(ContextProperty*)
{
    emit currentFlowChanged(currentFlow());
}

void QBatteryInfoPrivate::onVoltage(ContextProperty*)
{
    emit voltageChanged(voltage());
}

void QBatteryInfoPrivate::onTemperature(ContextProperty*)
{
    emit temperatureChanged(temperature());
}

void QBatteryInfoPrivate::onCharger(ContextProperty*)
{
    emit chargerTypeChanged(chargerType());
}

int QBatteryInfoPrivate::batteryCount() const
{
    // only one battery is supported now
    return 1;
}

int QBatteryInfoPrivate::batteryIndex() const
{
    // only one battery is supported now
    return 0;
}

void QBatteryInfoPrivate::setBatteryIndex(int)
{
    // only one battery is supported now
}

int QBatteryInfoPrivate::currentFlow() const
{
    return value(properties_, Prop::Current).toInt(); // TODO units
}

int QBatteryInfoPrivate::maximumCapacity() const
{
    return 100;
}

int QBatteryInfoPrivate::remainingCapacity() const
{
    return value(properties_, Prop::Percentage).toInt();
}

int QBatteryInfoPrivate::remainingChargingTime() const
{
    return value(properties_, Prop::TimeUntilFull).toInt();
}

int QBatteryInfoPrivate::voltage() const
{
    return value(properties_, Prop::Voltage).toInt();
}

QBatteryInfo::ChargerType QBatteryInfoPrivate::chargerType() const
{
    auto name = value(properties_, Prop::Charger).toString();
    return chargerTypeFromName(name);
}

QBatteryInfo::ChargingState QBatteryInfoPrivate::chargingState() const
{
    return chargingStateFromName(value(properties_, Prop::ChargingState).toString());
}

QBatteryInfo::LevelStatus QBatteryInfoPrivate::levelStatus() const
{
    auto level = value(properties_, Prop::BatteryLevel).toString();
    auto state = value(properties_, Prop::ChargingState).toString();
    return (level == QS_("empty")
            ? QBatteryInfo::LevelEmpty
            : (level == QS_("low")
               ? QBatteryInfo::LevelLow
               : (state == QS_("idle")
                  ? QBatteryInfo::LevelFull
                  : QBatteryInfo::LevelOk)));
}

QBatteryInfo::Health QBatteryInfoPrivate::health() const
{
    return QBatteryInfo::HealthUnknown;
}

bool QBatteryInfoPrivate::isValid() const
{
    return chargingState() != QBatteryInfo::UnknownChargingState;
}

float QBatteryInfoPrivate::temperature() const
{
    // oC
    return value(properties_, Prop::Temperature).toDouble() / 10;
}

int QBatteryInfoPrivate::level() const
{
    return remainingCapacity(); // TODO
}

int QBatteryInfoPrivate::cycleCount() const
{
    return 1; // TODO
}

QT_END_NAMESPACE
