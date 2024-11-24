#ifndef QSERIALIZERBASE_H
#define QSERIALIZERBASE_H

#include <QObject>

namespace QJsonHelper
{
    class QSerializerBase
    {
    public:
        static QByteArray serialize(const void* source, const QMetaObject& metaObject, bool isCompact);

    private:

        //  QList<BaseType>
        static QJsonArray serializeQList(const void* source, QString typeName);

        //  QSharedPointer<QObject>
        template<class T>
        static QJsonObject serializeSharedPtr(QSharedPointer<T>& source);

        //  QList<QSharedPointer<QObject>>
        template<class T>
        static QJsonArray serializeSharedPtrQList(QList<QSharedPointer<T>>& source);

        static QJsonObject serializeObject(const void* source, const QMetaObject* metaObject);

    };
}

#endif // QSERIALIZERBASE_H
