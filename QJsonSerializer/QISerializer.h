#ifndef QISERIALIZER_H
#define QISERIALIZER_H

#include <QByteArray>
#include "QSerializerBase.h"

namespace QJsonHelper
{
    template<class TSource>
    class QISerializer
    {
    };

    template<class TSource>
    class QISerializer<TSource*>
    {
    public:
        QByteArray serialize(TSource* const& source) const
        {
            return QSerializerBase::serialize(source, TSource::staticMetaObject, m_isCompact);
        }

        void setCompact(bool isCompact)
        {
            m_isCompact = isCompact;
        }

    private:
        bool m_isCompact{ true };
    };

}

#endif // QISERIALIZER_H
