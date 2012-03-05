/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtQml module of the Qt Toolkit.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QJSENGINE_H
#define QJSENGINE_H

#include <QtCore/qmetatype.h>

#include <QtCore/qvariant.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qobject.h>
#include <QtQml/qjsvalue.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QV8Engine;

template <typename T>
inline T qjsvalue_cast(const QJSValue &);

class QJSEnginePrivate;
class Q_QML_EXPORT QJSEngine
    : public QObject
{
    Q_OBJECT
public:
#ifdef QT_DEPRECATED
    enum ContextOwnership {
        AdoptCurrentContext,
        CreateNewContext
    };
    QT_DEPRECATED explicit QJSEngine(ContextOwnership ownership);
#endif

    QJSEngine();
    explicit QJSEngine(QObject *parent);
    virtual ~QJSEngine();

    QJSValue globalObject() const;

    QJSValue evaluate(const QString &program, const QString &fileName = QString(), int lineNumber = 1);

    QJSValue newObject();
    QJSValue newArray(uint length = 0);

    QJSValue newQObject(QObject *object);

    template <typename T>
    inline QJSValue toScriptValue(const T &value)
    {
        return create(qMetaTypeId<T>(), &value);
    }
    template <typename T>
    inline T fromScriptValue(const QJSValue &value)
    {
        return qjsvalue_cast<T>(value);
    }

    void collectGarbage();

    QV8Engine *handle() const { return d; }

#ifdef QT_DEPRECATED
    QT_DEPRECATED bool hasUncaughtException() const;
    QT_DEPRECATED QJSValue uncaughtException() const;
    QT_DEPRECATED void clearExceptions();
#endif

Q_SIGNALS:
    void signalHandlerException(const QJSValue &exception);

private:
    QJSValue create(int type, const void *ptr);

    static bool convertV2(const QJSValue &value, int type, void *ptr);

    friend inline bool qjsvalue_cast_helper(const QJSValue &, int, void *);

protected:
    QJSEngine(QJSEnginePrivate &dd, QObject *parent = 0);

private:
    QV8Engine *d;
    Q_DISABLE_COPY(QJSEngine)
    Q_DECLARE_PRIVATE(QJSEngine)
    friend class QV8Engine;
};

inline bool qjsvalue_cast_helper(const QJSValue &value, int type, void *ptr)
{
    return QJSEngine::convertV2(value, type, ptr);
}

template<typename T>
T qjsvalue_cast(const QJSValue &value)
{
    T t;
    const int id = qMetaTypeId<T>();

    if (qjsvalue_cast_helper(value, id, &t))
        return t;
    else if (value.isVariant())
        return qvariant_cast<T>(value.toVariant());

    return T();
}

template <>
inline QVariant qjsvalue_cast<QVariant>(const QJSValue &value)
{
    return value.toVariant();
}

QT_END_NAMESPACE

QT_END_HEADER

#endif // QJSENGINE_H
