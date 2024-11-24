#ifndef CDESERIALIZERBASE_H
#define CDESERIALIZERBASE_H

#include <string>
#include "ObjectBase.h"

namespace CJsonHelper
{
    class DeserializerPrivate;

    class CDeserializerBase
    {
    public:
        CDeserializerBase();
        ~CDeserializerBase();
        void setIsPrintError(bool isPrintError) const
        {
            auto _ptr = const_cast<CDeserializerBase*>(this);
            _ptr->m_isPrintError = isPrintError;
        }

    protected:

        bool isValidJson(const std::string& jsonStr) const;


        //  反序列化对象
        void deserializeObject(ObjectBase*& object) const;

        //  反序列化子对象
        void deserializeChildObject(ObjectBase*& object, void* jsonValue) const;

        //  反序列化对象处理
        void deserializeObjectHandle(ObjectBase*& object, void* jsonObject) const;

        //  反序列化基础属性         //BaseType
        void deserializeBaseProperty(std::shared_ptr<UserData>& userData, void* jsonValue) const;

        //  反序列化共享指针         //std::shared_ptr<BaseType>
        void deserializeSharedPtr(std::shared_ptr<UserData> userData, void* jsonValue) const;

        //  反序列化基础属性list     //std::list<BaseType>
        void deserializeBaseTypeList(std::shared_ptr<UserData> userData, void* jsonValue) const;

        //  反序列化共享指针list     //std::list<std::shared_ptr<BaseType>>
        void deserializeSharedPtrList(std::shared_ptr<UserData> userData, void* jsonValue) const;

        //  反序列化基础属性vector     //std::vector<BaseType>
        void deserializeBaseTypeVector(std::shared_ptr<UserData> userData, void* jsonValue) const;

        //  反序列化共享指针vector     //std::vector<std::shared_ptr<BaseType>>
        void deserializeSharedPtrVector(std::shared_ptr<UserData> userData, void* jsonValue) const;

        //  反序列化基础属性list<pair>     //std::list<std::pair<BaseType, BaseType>>
        void deserializePairList(std::shared_ptr<UserData> userData, void* jsonValue) const;

        //  反序列化共享指针vector<pair>     //std::vector<std::pair<BaseType, BaseType>>
        void deserializePairVector(std::shared_ptr<UserData> userData, void* jsonValue) const;


        bool m_isPrintError{ false };   //是否打印错误信息
    private:
        DeserializerPrivate* m_data{};
    };
}
#endif // CDESERIALIZERBASE_H
