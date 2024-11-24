#ifndef CISERIALIZER_H
#define CISERIALIZER_H

#include "CSerializerBase.h"

namespace CJsonHelper
{
    template<class TSource>
    class CISerializer : public CSerializerBase
    {
    };

    template<class TSource>
    class CISerializer<TSource*> : public CSerializerBase
    {
    public:
        const std::string serialize(TSource* const& source) const
        {
            if (!source) return {};

            auto _jsonStr = serializeObject((ObjectBase*)source);

            return _jsonStr;
        }

    };
}
#endif // CISERIALIZER_H
