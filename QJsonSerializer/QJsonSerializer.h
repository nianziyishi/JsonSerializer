#ifndef QJSONSERIALIZER_H
#define QJSONSERIALIZER_H

#include "QISerializer.h"
#include "QIDeserializer.h"

namespace QJsonHelper
{
    class QJsonSerializer
    {
    public:

        template<class TSource>
        QByteArray serialize(const TSource& source, bool isCompact = true) const
        {
            QISerializer<TSource> subject;
            subject.setCompact(isCompact);
            return subject.serialize(source);
        }

        template<class TReturn>
        TReturn deserialize(const QByteArray& data) const
        {
            QIDeserializer<TReturn> subject;
            return subject.deserialize(data);
        }

        template<class TReturn>
        void deserialize(const QByteArray& data, TReturn& target) const
        {
            QJsonHelper::QDeserializer<TReturn> subject;
            subject.deserialize(data, target);
        }

    };

}

#endif // QJSONSERIALIZER_H
