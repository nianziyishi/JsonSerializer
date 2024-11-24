#ifndef CIDESERIALIZER_H
#define CIDESERIALIZER_H

#include "CDeserializerBase.h"

namespace CJsonHelper
{
    template<class TReturn>
    class CIDeserializer : public CDeserializerBase
    {
    };

    template<class TReturn>
    class CIDeserializer<TReturn*> : public CDeserializerBase
    {
    public:

        TReturn* deserialize(const std::string& jsonStr, bool isPrintError) const
        {
            TReturn* _object{};
            deserialize(_object, jsonStr, isPrintError);
            return _object;
        }

        void deserialize(TReturn*& object, const std::string& jsonStr, bool isPrintError) const
        {
            if (jsonStr.empty()) return;

            setIsPrintError(isPrintError);
            auto _jsonDoc = isValidJson(jsonStr);
            if (_jsonDoc)
            {
                if (!object)
                {
                    object = new TReturn();
                }
                deserializeObject((ObjectBase*&)object);
            }

        }
    };
}

#endif // CIDESERIALIZER_H
