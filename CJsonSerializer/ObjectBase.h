#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include <cstring>
#include <algorithm>
#include <list>
#include <vector>
#include <memory>
#include "ObjectFactory.h"

namespace CJsonHelper
{
    // 添加基础类型
#define addPropertyB(value) addPropertyT(nullptr, #value, &value);
// 添加自定义类类型
#define addPropertyC(type, value) addPropertyT(#type, #value, &value);


    enum class BaseType
    {
        Null = 0,

        Bool,
        Int32,
        UInt32,
        Int64,
        UInt64,
        Float,
        Double,
        String,
        Object,

        Undefined = 0x80
    };


    enum class UserType
    {
        Null = 0,

        BaseType,			//BaseType
        SharedPtr,			//std::shared_ptr<BaseType>
        BaseTypeList,		//std::list<BaseType>
        SharedPtrList,		//std::list<std::shared_ptr<BaseType>>
        BaseTypeVector,		//std::vector<BaseType>
        SharedPtrVector,	//std::vector<std::shared_ptr<BaseType>>
        PairList,			//std::list<std::pair<BaseType,BaseType>>
        PairVector,			//std::vector<std::pair<BaseType,BaseType>>

        Undefined = 0x80
    };


    struct UserData
    {
        bool		m_isNull{ true };				//是否为空
        UserType	m_userType{ UserType::Null };	//用户类型
        BaseType	m_baseType{ BaseType::Null };	//基础类型
        const char* m_typeName{};					//类型名称
        const char* m_key{};						//属性名称
        void* m_value{};						//属性值指针
    };

    class ObjectBase
    {
    public:
        ObjectBase()
        {
        };

        virtual void setProperty() {};


        const char* getClassName()
        {
            return typeid(*this).name();
        }

        static ObjectBase* createObject(const char* className)
        {
            if (!std::strlen(className)) return nullptr;
            auto _ptrObject = (ObjectBase*)ObjectFactory::getInstance().CreateObjectByName(className);
            return _ptrObject;
        }

        size_t propertyCount()
        {
            return m_property.size();
        }

        std::shared_ptr<UserData> propertyType(size_t index)
        {
            size_t _count = 0;
            for (auto _iter = m_property.begin(); _iter != m_property.end(); ++_iter)
            {
                if (_count == index)
                {
                    return *_iter;
                }
                ++_count;
            }
            return {};
        }

        std::shared_ptr<UserData> propertyType(const char* key)
        {
            for (auto _iter = m_property.begin(); _iter != m_property.end(); ++_iter)
            {
                if (!std::strncmp((*_iter)->m_key, key, std::strlen(key)))
                {
                    return *_iter;
                }
            }
            return {};
        }

        std::vector<std::shared_ptr<UserData>>& getPropertyList()
        {
            return m_property;
        }


    protected:

        template<typename TSource>
        void addPropertyT(const char* type, const char* key, const TSource* value)
        {
            BaseType _baseType = getPropertyType(value);
            if (_baseType == BaseType::Undefined)
            {
                printf("Unsupported type! key name = %s\n", key);
                return;
            }

            bool _isNull = value ? false : true;
            addPropertyData(type, key, (void*)value, _baseType, UserType::BaseType, _isNull);

        }

        template<typename TSource>
        void addPropertyT(const char* type, const char* key, const std::shared_ptr<TSource>* value)
        {
            TSource _testType;
            BaseType _baseType = getPropertyType(&_testType);
            if (_baseType == BaseType::Undefined)
            {
                printf("Unsupported type! key name = %s\n", key);
                return;
            }

            bool _isNull = *value ? false : true;
            addPropertyData(type, key, (void*)value, _baseType, UserType::SharedPtr, _isNull);

        }

        template<typename TSource>
        void addPropertyT(const char* type, const char* key, const std::list<TSource>* value)
        {
            TSource _testType;
            BaseType _baseType = getPropertyType(&_testType);
            if (_baseType == BaseType::Undefined)
            {
                printf("Unsupported type! key name = %s\n", key);
                return;
            }

            bool _isNull = value->size() ? false : true;
            addPropertyData(type, key, (void*)value, _baseType, UserType::BaseTypeList, _isNull);

        }

        template<typename TSource>
        void addPropertyT(const char* type, const char* key, const std::list<std::shared_ptr<TSource>>* value)
        {
            TSource _testType;
            BaseType _baseType = getPropertyType(&_testType);
            if (_baseType == BaseType::Undefined)
            {
                printf("Unsupported type! key name = %s\n", key);
                return;
            }

            bool _isNull = value->size() ? false : true;
            addPropertyData(type, key, (void*)value, _baseType, UserType::SharedPtrList, _isNull);

        }


        template<typename TSource>
        void addPropertyT(const char* type, const char* key, const std::vector<TSource>* value)
        {
            TSource _testType;
            BaseType _baseType = getPropertyType(&_testType);
            if (_baseType == BaseType::Undefined)
            {
                printf("Unsupported type! key name = %s\n", key);
                return;
            }

            bool _isNull = value->size() ? false : true;
            addPropertyData(type, key, (void*)value, _baseType, UserType::BaseTypeVector, _isNull);

        }

        template<typename TSource>
        void addPropertyT(const char* type, const char* key, const std::vector<std::shared_ptr<TSource>>* value)
        {
            TSource _testType;
            BaseType _baseType = getPropertyType(&_testType);
            if (_baseType == BaseType::Undefined)
            {
                printf("Unsupported type! key name = %s\n", key);
                return;
            }

            bool _isNull = value->size() ? false : true;
            addPropertyData(type, key, (void*)value, _baseType, UserType::SharedPtrVector, _isNull);

        }


        template<typename TSource>
        void addPropertyT(const char* type, const char* key, const std::list<std::pair<TSource, TSource>>* value)
        {
            TSource _testType;
            BaseType _baseType = getPropertyType(&_testType);
            if (_baseType == BaseType::Undefined || _baseType == BaseType::Object)
            {
                printf("Unsupported type! key name = %s\n", key);
                return;
            }

            bool _isNull = value->size() ? false : true;
            addPropertyData(type, key, (void*)value, _baseType, UserType::PairList, _isNull);

        }

        template<typename TSource>
        void addPropertyT(const char* type, const char* key, const std::vector<std::pair<TSource, TSource>>* value)
        {
            TSource _testType;
            BaseType _baseType = getPropertyType(&_testType);
            if (_baseType == BaseType::Undefined || _baseType == BaseType::Object)
            {
                printf("Unsupported type! key name = %s\n", key);
                return;
            }

            bool _isNull = value->size() ? false : true;
            addPropertyData(type, key, (void*)value, _baseType, UserType::PairVector, _isNull);

        }


    private:

        void addPropertyData(const char* type, const char* key, void* value, BaseType baseType, UserType userType, bool isNull)
        {

            auto _userData = std::make_shared<UserData>();
            _userData->m_baseType = baseType;
            _userData->m_userType = userType;
            _userData->m_typeName = type;
            _userData->m_key = key;
            _userData->m_value = value;
            _userData->m_isNull = isNull;

            addProperty(_userData);

        }

        void addProperty(std::shared_ptr<UserData>& dataPtr)
        {
            auto iterFind = std::find_if(m_property.begin(), m_property.end(), [&](std::shared_ptr<UserData>& iter)
                {
                    return iter->m_key == dataPtr->m_key;
                });

            if (iterFind != m_property.end())
            {
                if (iterFind->get()->m_value != dataPtr->m_value)
                    iterFind->get()->m_value = dataPtr->m_value;
                if (iterFind->get()->m_baseType != dataPtr->m_baseType)
                    iterFind->get()->m_baseType = dataPtr->m_baseType;
                if (iterFind->get()->m_isNull != dataPtr->m_isNull)
                    iterFind->get()->m_isNull = dataPtr->m_isNull;
            }
            else
            {
                m_property.push_back(dataPtr);
            }

        }

        template<typename TSource>
        BaseType getPropertyType(const TSource* valuePtr)
        {
            BaseType _type = BaseType::Undefined;

            if (std::is_same<typename std::decay<TSource>::type, bool>::value)
            {
                _type = BaseType::Bool;
            }
            else if (std::is_same<typename std::decay<TSource>::type, int32_t>::value)
            {
                _type = BaseType::Int32;
            }
            else if (std::is_same<typename std::decay<TSource>::type, uint32_t>::value)
            {
                _type = BaseType::UInt32;
            }
            else if (std::is_same<typename std::decay<TSource>::type, int64_t>::value)
            {
                _type = BaseType::Int64;
            }
            else if (std::is_same<typename std::decay<TSource>::type, uint64_t>::value)
            {
                _type = BaseType::UInt64;
            }
            else if (std::is_same<typename std::decay<TSource>::type, float>::value)
            {
                _type = BaseType::Float;
            }
            else if (std::is_same<typename std::decay<TSource>::type, double>::value)
            {
                _type = BaseType::Double;
            }
            else if (std::is_same<typename std::decay<TSource>::type, std::string>::value)
            {
                _type = BaseType::String;
            }
            else if (std::is_base_of<ObjectBase, TSource>::value)
            {
                _type = BaseType::Object;
            }

            return _type;
        }


    private:
        std::vector<std::shared_ptr<UserData>> m_property;

    };
}
#endif // OBJECTBASE_H
