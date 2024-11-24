#include <vector>
#include "CSerializerBase.h"
#include "document.h"
#include "writer.h"

namespace CJsonHelper
{
    const std::string CSerializerBase::serializeObject(ObjectBase* const source) const
    {
        rapidjson::Document _doc;
        _doc.SetObject();
        auto& _allocator = _doc.GetAllocator();

        serializeChildObject(source, &_doc, &_allocator);

        rapidjson::StringBuffer _buffer;
        rapidjson::Writer<rapidjson::StringBuffer> _writer1(_buffer);
        _doc.Accept(_writer1);

        return { _buffer.GetString(), _buffer.GetSize() };
    }

    //  序列化子对象
    void CSerializerBase::serializeChildObject(ObjectBase* const source, void* jsonValue, void* allocator) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;
        auto& _allocator = *(rapidjson::Document::AllocatorType*)allocator;

        source->setProperty();
        size_t _count = source->propertyCount();
        for (size_t index = 0; index < _count; ++index)
        {
            auto _userData = source->propertyType(index);
            if (_userData->m_userType == UserType::Null)
            {
                rapidjson::Value nullObj(rapidjson::kNullType);
                _jsonValue->AddMember(rapidjson::StringRef(_userData->m_key), nullObj, _allocator);
            }
            else if (_userData->m_userType == UserType::BaseType)
            {
                serializeBaseProperty(_userData, jsonValue, allocator);
            }
            else if (_userData->m_userType == UserType::SharedPtr)
            {
                serializeSharedPtr(_userData, jsonValue, allocator);
            }
            else if (_userData->m_userType == UserType::BaseTypeList)
            {
                serializeBaseTypeList(_userData, jsonValue, allocator);
            }
            else if (_userData->m_userType == UserType::SharedPtrList)
            {
                serializeSharedPtrList(_userData, jsonValue, allocator);
            }
            else if (_userData->m_userType == UserType::BaseTypeVector)
            {
                serializeBaseTypeVector(_userData, jsonValue, allocator);
            }
            else if (_userData->m_userType == UserType::SharedPtrVector)
            {
                serializeSharedPtrVector(_userData, jsonValue, allocator);
            }
            else if (_userData->m_userType == UserType::PairList)
            {
                serializePairList(_userData, jsonValue, allocator);
            }
            else if (_userData->m_userType == UserType::PairVector)
            {
                serializePairVector(_userData, jsonValue, allocator);
            }
            //else if (_userData->m_userType == UserType::Undefined)
            //{
            //    //printf("Undefined type : %d\n", _type.first);
            //}
            else
            {
                printf("Serialize error: value is unknow type! key name = %s\n", _userData->m_key);
            }
        }
    }


    //  序列化基础属性         //BaseType

    void CSerializerBase::serializeBaseProperty(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;
        auto& _allocator = *(rapidjson::Document::AllocatorType*)allocator;

        if (userData->m_isNull)
        {
            rapidjson::Value nullObj(rapidjson::kNullType);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), nullObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Bool)
        {
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(bool*)userData->m_value, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int32)
        {
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(int32_t*)userData->m_value, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt32)
        {
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(uint32_t*)userData->m_value, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int64)
        {
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(int64_t*)userData->m_value, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt64)
        {
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(uint64_t*)userData->m_value, _allocator);
        }
        else if (userData->m_baseType == BaseType::Float)
        {
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(float*)userData->m_value, _allocator);
        }
        else if (userData->m_baseType == BaseType::Double)
        {
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(double*)userData->m_value, _allocator);
        }
        else if (userData->m_baseType == BaseType::String)
        {
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), rapidjson::StringRef((*(std::string*)userData->m_value).data()), _allocator);
        }
        else if (userData->m_baseType == BaseType::Object)
        {
            printf("key : %s; BaseType not support Object type : %d\n", userData->m_key, userData->m_baseType);
        }
        else if (userData->m_baseType == BaseType::Undefined)
        {
            printf("key : %s; Undefined type : %d\n", userData->m_key, userData->m_baseType);
        }
        else
        {
            printf("key : %s; Unknow type : %d\n", userData->m_key, userData->m_baseType);
        }

    }


    //  序列化共享指针         //std::shared_ptr<BaseType>

    void CSerializerBase::serializeSharedPtr(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;
        auto& _allocator = *(rapidjson::Document::AllocatorType*)allocator;

        if (userData->m_isNull)
        {
            rapidjson::Value nullObj(rapidjson::kNullType);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), nullObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Bool)
        {
            auto& _dataValue = *(std::shared_ptr<bool>*)userData->m_value;
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(_dataValue.get()), _allocator);
        }
        else if (userData->m_baseType == BaseType::Int32)
        {
            auto& _dataValue = *(std::shared_ptr<int32_t>*)userData->m_value;
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(_dataValue.get()), _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt32)
        {
            auto& _dataValue = *(std::shared_ptr<uint32_t>*)userData->m_value;
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(_dataValue.get()), _allocator);
        }
        else if (userData->m_baseType == BaseType::Int64)
        {
            auto& _dataValue = *(std::shared_ptr<int64_t>*)userData->m_value;
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(_dataValue.get()), _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt64)
        {
            auto& _dataValue = *(std::shared_ptr<uint64_t>*)userData->m_value;
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(_dataValue.get()), _allocator);
        }
        else if (userData->m_baseType == BaseType::Float)
        {
            auto& _dataValue = *(std::shared_ptr<float>*)userData->m_value;
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(_dataValue.get()), _allocator);
        }
        else if (userData->m_baseType == BaseType::Double)
        {
            auto& _dataValue = *(std::shared_ptr<double>*)userData->m_value;
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), *(_dataValue.get()), _allocator);
        }
        else if (userData->m_baseType == BaseType::String)
        {
            auto& _dataValue = *(std::shared_ptr<std::string>*)userData->m_value;
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), rapidjson::StringRef(_dataValue.get()->data()), _allocator);
        }
        else if (userData->m_baseType == BaseType::Object)
        {
            rapidjson::Value _childObj(rapidjson::kObjectType);
            auto& _objectBase = *(std::shared_ptr<ObjectBase>*)userData->m_value;
            _objectBase->setProperty();
            size_t _count = _objectBase->propertyCount();
            for (size_t index = 0; index < _count; ++index)
            {
                serializeBaseProperty(_objectBase->propertyType(index), &_childObj, allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Undefined)
        {
            printf("key : %s; Undefined type : %d\n", userData->m_key, userData->m_baseType);
        }
        else
        {
            printf("key : %s; Unknow type : %d\n", userData->m_key, userData->m_baseType);
        }

    }

    //  序列化基础属性list     //std::list<BaseType>

    void CSerializerBase::serializeBaseTypeList(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;
        auto& _allocator = *(rapidjson::Document::AllocatorType*)allocator;

        if (userData->m_isNull)
        {
            rapidjson::Value nullObj(rapidjson::kNullType);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), nullObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Bool)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<bool>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<int32_t>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<uint32_t>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<int64_t>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<uint64_t>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Float)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<float>*)userData->m_value;
            for (auto& _iter : _dataValue) _jsonValue->PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Double)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<double>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::String)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::string>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(rapidjson::StringRef(_iter.data()), _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Object)
        {
            printf("key : %s; std::list<BaseType> not support Object type : %d\n", userData->m_key, userData->m_baseType);
        }
        else if (userData->m_baseType == BaseType::Undefined)
        {
            printf("key : %s; Undefined type : %d\n", userData->m_key, userData->m_baseType);
        }
        else
        {
            printf("key : %s; Unknow type : %d\n", userData->m_key, userData->m_baseType);
        }

    }

    //  序列化共享指针list     //std::list<std::shared_ptr<BaseType>>

    void CSerializerBase::serializeSharedPtrList(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;
        auto& _allocator = *(rapidjson::Document::AllocatorType*)allocator;

        if (userData->m_isNull)
        {
            rapidjson::Value nullObj(rapidjson::kNullType);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), nullObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Bool)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::shared_ptr<bool>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::shared_ptr<int32_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::shared_ptr<uint32_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::shared_ptr<int64_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::shared_ptr<uint64_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Float)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::shared_ptr<float>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Double)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::shared_ptr<double>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::String)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::shared_ptr<std::string>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(rapidjson::StringRef(_iter.get()->data()), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Object)
        {
            rapidjson::Value _childObjArray(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::shared_ptr<ObjectBase>>*)userData->m_value;
            for (auto& itemSource : _dataValue)
            {
                if (itemSource)
                {
                    rapidjson::Value _childObj(rapidjson::kObjectType);
                    serializeChildObject(itemSource.get(), &_childObj, allocator);
                    _childObjArray.PushBack(_childObj, _allocator);
                }
                else
                {
                    rapidjson::Value _nullObj(rapidjson::kNullType);
                    _childObjArray.PushBack(_nullObj, _allocator);
                }
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObjArray, _allocator);
        }
        else if (userData->m_baseType == BaseType::Undefined)
        {
            printf("key : %s; Undefined type : %d\n", userData->m_key, userData->m_baseType);
        }
        else
        {
            printf("key : %s; Unknow type : %d\n", userData->m_key, userData->m_baseType);
        }

    }



    //  序列化基础属性vector     //std::vector<BaseType>

    void CSerializerBase::serializeBaseTypeVector(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;
        auto& _allocator = *(rapidjson::Document::AllocatorType*)allocator;

        if (userData->m_isNull)
        {
            rapidjson::Value nullObj(rapidjson::kNullType);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), nullObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Bool)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<bool>*)userData->m_value;
            for (auto _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<int32_t>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<uint32_t>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<int64_t>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<uint64_t>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Float)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<float>*)userData->m_value;
            for (auto& _iter : _dataValue) _jsonValue->PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Double)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<double>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(_iter, _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::String)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::string>*)userData->m_value;
            for (auto& _iter : _dataValue) _childObj.PushBack(rapidjson::StringRef(_iter.data()), _allocator);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Object)
        {
            printf("key : %s; std::vector<BaseType> not support Object type : %d\n", userData->m_key, userData->m_baseType);
        }
        else if (userData->m_baseType == BaseType::Undefined)
        {
            printf("key : %s; Undefined type : %d\n", userData->m_key, userData->m_baseType);
        }
        else
        {
            printf("key : %s; Unknow type : %d\n", userData->m_key, userData->m_baseType);
        }

    }

    //  序列化共享指针vector     //std::vector<std::shared_ptr<BaseType>>

    void CSerializerBase::serializeSharedPtrVector(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;
        auto& _allocator = *(rapidjson::Document::AllocatorType*)allocator;

        if (userData->m_isNull)
        {
            rapidjson::Value nullObj(rapidjson::kNullType);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), nullObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Bool)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::shared_ptr<bool>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::shared_ptr<int32_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::shared_ptr<uint32_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::shared_ptr<int64_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::shared_ptr<uint64_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Float)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::shared_ptr<float>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Double)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::shared_ptr<double>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(*_iter.get(), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::String)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::shared_ptr<std::string>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                if (_iter)
                    _childObj.PushBack(rapidjson::StringRef(_iter.get()->data()), _allocator);
                else
                    _childObj.PushBack(_nullObj, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Object)
        {
            rapidjson::Value _childObjArray(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::shared_ptr<ObjectBase>>*)userData->m_value;
            for (auto& itemSource : _dataValue)
            {
                if (itemSource)
                {
                    rapidjson::Value _childObj(rapidjson::kObjectType);
                    serializeChildObject(itemSource.get(), &_childObj, allocator);
                    _childObjArray.PushBack(_childObj, _allocator);
                }
                else
                {
                    rapidjson::Value _nullObj(rapidjson::kNullType);
                    _childObjArray.PushBack(_nullObj, _allocator);
                }
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObjArray, _allocator);
        }
        else if (userData->m_baseType == BaseType::Undefined)
        {
            printf("key : %s; Undefined type : %d\n", userData->m_key, userData->m_baseType);
        }
        else
        {
            printf("key : %s; Unknow type : %d\n", userData->m_key, userData->m_baseType);
        }

    }



    //  序列化共享指针list<pair>     //std::list<std::pair<BaseType, BaseType>>

    void CSerializerBase::serializePairList(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;
        auto& _allocator = *(rapidjson::Document::AllocatorType*)allocator;

        if (userData->m_isNull)
        {
            rapidjson::Value nullObj(rapidjson::kNullType);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), nullObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Bool)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::pair<bool, bool>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::pair<int32_t, int32_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::pair<uint32_t, uint32_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::pair<int64_t, int64_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::pair<uint64_t, uint64_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Float)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::pair<float, float>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Double)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::pair<double, double>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::String)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::list<std::pair<std::string, std::string>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(rapidjson::StringRef(_iter.first.data()), _allocator);
                _childObj.PushBack(rapidjson::StringRef(_iter.second.data()), _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Object)
        {
            printf("key : %s; Unsupported type : %d\n", userData->m_key, userData->m_baseType);
        }
        else if (userData->m_baseType == BaseType::Undefined)
        {
            printf("key : %s; Undefined type : %d\n", userData->m_key, userData->m_baseType);
        }
        else
        {
            printf("key : %s; Unknow type : %d\n", userData->m_key, userData->m_baseType);
        }

    }


    //  序列化共享指针vector<pair>     //std::vector<std::pair<BaseType, BaseType>>

    void CSerializerBase::serializePairVector(std::shared_ptr<UserData> userData, void* jsonValue, void* allocator) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;
        auto& _allocator = *(rapidjson::Document::AllocatorType*)allocator;

        if (userData->m_isNull)
        {
            rapidjson::Value nullObj(rapidjson::kNullType);
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), nullObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Bool)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::pair<bool, bool>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::pair<int32_t, int32_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt32)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::pair<uint32_t, uint32_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Int64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::pair<int64_t, int64_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::UInt64)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::pair<uint64_t, uint64_t>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Float)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::pair<float, float>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Double)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::pair<double, double>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(_iter.first, _allocator);
                _childObj.PushBack(_iter.second, _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::String)
        {
            rapidjson::Value _childObj(rapidjson::kArrayType);
            auto& _dataValue = *(std::vector<std::pair<std::string, std::string>>*)userData->m_value;
            rapidjson::Value _nullObj(rapidjson::kNullType);
            for (auto& _iter : _dataValue)
            {
                _childObj.PushBack(rapidjson::StringRef(_iter.first.data()), _allocator);
                _childObj.PushBack(rapidjson::StringRef(_iter.second.data()), _allocator);
            }
            _jsonValue->AddMember(rapidjson::StringRef(userData->m_key), _childObj, _allocator);
        }
        else if (userData->m_baseType == BaseType::Object)
        {
            printf("key : %s; Unsupported type : %d\n", userData->m_key, userData->m_baseType);
        }
        else if (userData->m_baseType == BaseType::Undefined)
        {
            printf("key : %s; Undefined type : %d\n", userData->m_key, userData->m_baseType);
        }
        else
        {
            printf("key : %s; Unknow type : %d\n", userData->m_key, userData->m_baseType);
        }

    }
}
