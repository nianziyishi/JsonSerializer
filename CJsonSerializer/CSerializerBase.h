#ifndef CSERIALIZERBASE_H
#define CSERIALIZERBASE_H

#include <string>
#include <memory>
#include "ObjectBase.h"

namespace CJsonHelper
{
    class SerializerBasePrivate;

    class CSerializerBase
    {
    protected:

        //  序列化对象
        const std::string serializeObject(ObjectBase* const source) const;

        //  序列化子对象
        void serializeChildObject(ObjectBase* const source, void* jsonValue, void* allocator) const;

        //  序列化基础属性         //BaseType
        void serializeBaseProperty(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const;

        //  序列化共享指针         //std::shared_ptr<BaseType>
        void serializeSharedPtr(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const;

        //  序列化基础属性list     //std::list<BaseType>
        void serializeBaseTypeList(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const;

        //  序列化共享指针list     //std::list<std::shared_ptr<BaseType>>
        void serializeSharedPtrList(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const;

        //  序列化基础属性vector     //std::vector<BaseType>
        void serializeBaseTypeVector(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const;

        //  序列化共享指针vector     //std::vector<std::shared_ptr<BaseType>>
        void serializeSharedPtrVector(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const;

        //  序列化共享指针list<pair>     //std::list<std::pair<BaseType, BaseType>>
        void serializePairList(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const;

        //  序列化共享指针vector<pair>     //std::vector<std::pair<BaseType, BaseType>>
        void serializePairVector(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const;


    };
}
#endif // SERIALIZERBASE_H
