#ifndef QIDESERIALIZER_H
#define QIDESERIALIZER_H

#include <QByteArray>
#include <QDebug>
#include "QDeserializerBase.h"

namespace QJsonHelper
{
    template<class TReturn>
    class QIDeserializer
    {
    };

    template<class TReturn>
    class QIDeserializer<TReturn*>
    {
    public:

        TReturn* deserialize(const QByteArray& data) const
        {
            const QMetaObject& metaObject = TReturn::staticMetaObject;
            TReturn* instance = (TReturn*)metaObject.newInstance();
            if (instance)
            {
                QDeserializerBase::deserialize(data, instance, &TReturn::staticMetaObject);
            }
            else
            {
                qCritical() << "Failed to create an instance of type" << metaObject.className();
                qCritical() << "Did you forget to declare a public parameterless Q_INVOKABLE constructor?";
            }
            return instance;
        }

    };

}

#endif // QIDESERIALIZER_H
