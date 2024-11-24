#ifndef QDESERIALIZERBASE_H
#define QDESERIALIZERBASE_H

#include <QObject>

namespace QJsonHelper
{
    class QDeserializerBase
    {
    public:
        static void deserialize(const QByteArray& data, void* instance, const QMetaObject* metaObject);

    private:

        //  QList<BaseType>
        static QVariant deserializeQList(const QJsonArray& array, QString typeName, int userType);

        //  QList<QSharedPointer<QObject>>
        template<class T>
        static void deserializeSharedPtrQList(const QJsonArray& array, QList<QSharedPointer<T>>& target, QString metaType);

        static void deserializeObject(const QJsonObject& object, void* instance, const QMetaObject* metaObject);

    };

}

#endif // QDESERIALIZERBASE_H
