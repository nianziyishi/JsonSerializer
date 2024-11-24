#include "CDeserializerBase.h"
#include "document.h"
#include "writer.h"

namespace CJsonHelper
{
    std::string getObjectClassName(std::string typeName)
    {
        auto _indexStart = typeName.rfind('<');
        auto _indexEnd = typeName.find('>');
        std::string _className{};
        if (_indexStart != -1 && _indexEnd != -1)
        {
            _className = typeName.substr(_indexStart + 1, _indexEnd - _indexStart - 1);
        }

        return _className;
    }

    class DeserializerPrivate
    {
    public:
        DeserializerPrivate() {}
        rapidjson::Document m_jsonDoc;
    };

    CDeserializerBase::CDeserializerBase()
    {
        m_data = new DeserializerPrivate;
    }
    CDeserializerBase::~CDeserializerBase()
    {
        if (m_data)
        {
            delete m_data;
            m_data = nullptr;
        }
    }

    bool CDeserializerBase::isValidJson(const std::string& jsonStr) const
    {
        m_data->m_jsonDoc.Parse<0>(jsonStr.data());

        if (!m_data->m_jsonDoc.HasParseError())
        {
            return true;
        }

        return false;
    }

    void CDeserializerBase::deserializeObject(ObjectBase*& object) const
    {
        auto _jsonObject = m_data->m_jsonDoc.GetObject();

        deserializeObjectHandle(object, &_jsonObject);

    }


    //  反序列化子对象

    void CDeserializerBase::deserializeChildObject(ObjectBase*& object, void* jsonValue) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;
        auto _jsonObject = _jsonValue->GetObject();
        deserializeObjectHandle(object, &_jsonObject);

    }

    //  反序列化对象处理

    void CDeserializerBase::deserializeObjectHandle(ObjectBase*& object, void* jsonObject) const
    {
        auto _jsonObject = (rapidjson::GenericObject<false, rapidjson::Value>*) jsonObject;

        rapidjson::Value _key;
        rapidjson::Value _value;
        rapidjson::Document::AllocatorType _allocator;
        for (auto _iter = _jsonObject->MemberBegin(); _iter != _jsonObject->MemberEnd(); ++_iter)
        {
            _key.CopyFrom(_iter->name, _allocator);
            if (_key.IsString())
            {
                auto _keyName = _key.GetString();
                //printf("\r\nname: %s\r\n", _keyName);

                _value.CopyFrom(_iter->value, _allocator);

                auto _userData = object->propertyType(_keyName);
                if (!_userData)
                {
                    if (m_isPrintError) printf("not exist key name : %s", _keyName);
                    continue;
                }

                if (_value.IsNull())
                {
                    auto _oValue = nullptr;
                }
                else if (_userData->m_userType == UserType::BaseType)
                {
                    deserializeBaseProperty(_userData, &_value);
                }
                else if (_userData->m_userType == UserType::SharedPtr)
                {
                    deserializeSharedPtr(_userData, &_value);
                }
                else if (_userData->m_userType == UserType::BaseTypeList)
                {
                    deserializeBaseTypeList(_userData, &_value);
                }
                else if (_userData->m_userType == UserType::SharedPtrList)
                {
                    deserializeSharedPtrList(_userData, &_value);
                }
                else if (_userData->m_userType == UserType::BaseTypeVector)
                {
                    deserializeBaseTypeVector(_userData, &_value);
                }
                else if (_userData->m_userType == UserType::SharedPtrVector)
                {
                    deserializeSharedPtrVector(_userData, &_value);
                }
                else if (_userData->m_userType == UserType::PairList)
                {
                    deserializePairList(_userData, &_value);
                }
                else if (_userData->m_userType == UserType::PairVector)
                {
                    deserializePairVector(_userData, &_value);
                }
                else
                {
                    if (m_isPrintError) printf("not exist m_userType : %d", (int)_userData->m_userType);
                }

            }
            else
            {
                if (m_isPrintError) printf("Deserialize: key not is string!\n");
            }
        }
    }


    //  反序列化基础属性         //BaseType

    void CDeserializerBase::deserializeBaseProperty(std::shared_ptr<UserData>& userData, void* jsonValue) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;

        if (_jsonValue->IsNull())
        {
            //printf("std::shared_ptr<BaseType> is null.\n");
        }
        else if (userData->m_baseType == BaseType::Bool && _jsonValue->IsBool())
        {
            auto _value = _jsonValue->GetBool();
            *(bool*)userData->m_value = _value;
        }
        else if (userData->m_baseType == BaseType::Int32 && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetInt();
            *(int32_t*)userData->m_value = _value;
        }
        else if (userData->m_baseType == BaseType::UInt32 && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetUint();
            *(uint32_t*)userData->m_value = _value;
        }
        else if (userData->m_baseType == BaseType::Int64 && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetInt64();
            *(int64_t*)userData->m_value = _value;
        }
        else if (userData->m_baseType == BaseType::UInt64 && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetUint64();
            *(uint64_t*)userData->m_value = _value;
        }
        else if (userData->m_baseType == BaseType::Float && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetFloat();
            *(float*)userData->m_value = _value;
        }
        else if (userData->m_baseType == BaseType::Double && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetDouble();
            *(double*)userData->m_value = _value;
        }
        else if (userData->m_baseType == BaseType::String && _jsonValue->IsString())
        {
            auto _value = _jsonValue->GetString();
            *(std::string*)userData->m_value = _value;
        }
        else if (userData->m_baseType == BaseType::Object && _jsonValue->IsObject())
        {
            if (m_isPrintError) printf("Unsupported format Object! key name = %s\n", userData->m_key);
        }
        else
        {
            if (m_isPrintError) printf("Deserialize BaseType error: value is unknow type! key name = %s\n", userData->m_key);
        }
    }


    //  反序列化共享指针         //std::shared_ptr<BaseType>

    void CDeserializerBase::deserializeSharedPtr(std::shared_ptr<UserData> userData, void* jsonValue) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;

        if (_jsonValue->IsNull())
        {
            //printf("std::shared_ptr<BaseType> is null.\n");
        }
        else if (userData->m_baseType == BaseType::Bool && _jsonValue->IsBool())
        {
            auto _value = _jsonValue->GetBool();
            *(std::shared_ptr<bool>*)userData->m_value = std::make_shared<bool>(_value);
        }
        else if (userData->m_baseType == BaseType::Int32 && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetInt();
            *(std::shared_ptr<int32_t>*)userData->m_value = std::make_shared<int32_t>(_value);
        }
        else if (userData->m_baseType == BaseType::UInt32 && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetUint();
            *(std::shared_ptr<uint32_t>*)userData->m_value = std::make_shared<uint32_t>(_value);
        }
        else if (userData->m_baseType == BaseType::Int64 && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetInt64();
            *(std::shared_ptr<int64_t>*)userData->m_value = std::make_shared<int64_t>(_value);
        }
        else if (userData->m_baseType == BaseType::UInt64 && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetUint64();
            *(std::shared_ptr<uint64_t>*)userData->m_value = std::make_shared<uint64_t>(_value);
        }
        else if (userData->m_baseType == BaseType::Float && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetFloat();
            *(std::shared_ptr<float>*)userData->m_value = std::make_shared<float>(_value);
        }
        else if (userData->m_baseType == BaseType::Double && _jsonValue->IsNumber())
        {
            auto _value = _jsonValue->GetDouble();
            *(std::shared_ptr<double>*)userData->m_value = std::make_shared<double>(_value);
        }
        else if (userData->m_baseType == BaseType::String && _jsonValue->IsString())
        {
            auto _value = _jsonValue->GetString();
            *(std::shared_ptr<std::string>*)userData->m_value = std::make_shared<std::string>(_value);
        }
        else if (userData->m_baseType == BaseType::Object && _jsonValue->IsObject())
        {
            if (!userData->m_typeName)
            {
                if (m_isPrintError) printf("Deserialize std::shared_ptr<BaseType> error: type name is null, please check! key name = %s\n", userData->m_key);
                return;
            }

            std::string _className = getObjectClassName(userData->m_typeName);
            auto _objectPtr = ObjectBase::createObject(_className.data());
            *(std::shared_ptr<ObjectBase>*)userData->m_value = std::shared_ptr<ObjectBase>(_objectPtr);
            deserializeChildObject(_objectPtr, jsonValue);
        }
        else
        {
            if (m_isPrintError) printf("Deserialize std::shared_ptr<BaseType> error: value is unknow type! key name = %s\n", userData->m_key);
        }
    }

    //  反序列化基础属性list     //std::list<BaseType>

    void CDeserializerBase::deserializeBaseTypeList(std::shared_ptr<UserData> userData, void* jsonValue) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;

        if (_jsonValue->IsArray())
        {
            auto _oValue = _jsonValue->GetArray();
            for (auto& _item : _oValue)
            {
                if (_item.IsNull())
                {
                    //printf("std::list<BaseType> is null.\n");
                }
                else if (userData->m_baseType == BaseType::Bool && _item.IsBool())
                {
                    auto _value = _item.GetBool();
                    ((std::list<bool>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::Int32 && _item.IsNumber())
                {
                    auto _value = _item.GetInt();
                    ((std::list<int32_t>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::UInt32 && _item.IsNumber())
                {
                    auto _value = _item.GetUint();
                    ((std::list<uint32_t>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::Int64 && _item.IsNumber())
                {
                    auto _value = _item.GetInt64();
                    ((std::list<int64_t>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::UInt64 && _item.IsNumber())
                {
                    auto _value = _item.GetUint64();
                    ((std::list<uint64_t>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::Float && _item.IsNumber())
                {
                    auto _value = _item.GetFloat();
                    ((std::list<float>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::Double && _item.IsNumber())
                {
                    auto _value = _item.GetDouble();
                    ((std::list<double>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::String && _item.IsString())
                {
                    auto _value = _item.GetString();
                    ((std::list<std::string>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::Object && _item.IsObject())
                {
                    if (m_isPrintError) printf("Unsupported format std::list<Object>! key name = %s\n", userData->m_key);
                }
                else
                {
                    if (m_isPrintError) printf("Deserialize std::list<BaseType> error: value is unknow type! key name = %s\n", userData->m_key);
                }
            }

        }
    }

    //  反序列化共享指针list     //std::list<std::shared_ptr<BaseType>>

    void CDeserializerBase::deserializeSharedPtrList(std::shared_ptr<UserData> userData, void* jsonValue) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;

        if (_jsonValue->IsArray())
        {
            auto _oValue = _jsonValue->GetArray();
            for (auto& _item : _oValue)
            {
                if (_item.IsNull())
                {
                    //printf("std::list<std::shared_ptr<BaseType>> is null.\n");
                }
                else if (userData->m_baseType == BaseType::Bool && _item.IsBool())
                {
                    auto _value = _item.GetBool();
                    auto _valuePtr = std::make_shared<bool>(_value);
                    ((std::list<std::shared_ptr<bool>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::Int32 && _item.IsNumber())
                {
                    auto _value = _item.GetInt();
                    auto _valuePtr = std::make_shared<int32_t>(_value);
                    ((std::list<std::shared_ptr<int32_t>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::UInt32 && _item.IsNumber())
                {
                    auto _value = _item.GetUint();
                    auto _valuePtr = std::make_shared<uint32_t>(_value);
                    ((std::list<std::shared_ptr<uint32_t>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::Int64 && _item.IsNumber())
                {
                    auto _value = _item.GetInt64();
                    auto _valuePtr = std::make_shared<int64_t>(_value);
                    ((std::list<std::shared_ptr<int64_t>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::UInt64 && _item.IsNumber())
                {
                    auto _value = _item.GetUint64();
                    auto _valuePtr = std::make_shared<uint64_t>(_value);
                    ((std::list<std::shared_ptr<uint64_t>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::Float && _item.IsNumber())
                {
                    auto _value = _item.GetFloat();
                    auto _valuePtr = std::make_shared<float>(_value);
                    ((std::list<std::shared_ptr<float>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::Double && _item.IsNumber())
                {
                    auto _value = _item.GetDouble();
                    auto _valuePtr = std::make_shared<double>(_value);
                    ((std::list<std::shared_ptr<double>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::String && _item.IsString())
                {
                    auto _value = _item.GetString();
                    auto _valuePtr = std::make_shared<std::string>(_value);
                    ((std::list<std::shared_ptr<std::string>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::Object && _item.IsObject())
                {
                    if (!userData->m_typeName)
                    {
                        if (m_isPrintError) printf("Deserialize std::list<std::shared_ptr<BaseType>> error: type name is null, please check! key name = %s\n", userData->m_key);
                        return;
                    }

                    std::string _className = getObjectClassName(userData->m_typeName);
                    auto _objectPtr = ObjectBase::createObject(_className.data());
                    ((std::list<std::shared_ptr<ObjectBase>>*)userData->m_value)->push_back(std::shared_ptr<ObjectBase>(_objectPtr));
                    deserializeChildObject(_objectPtr, &_item);
                }
                else
                {
                    if (m_isPrintError) printf("Deserialize std::list<std::shared_ptr<BaseType>> error: value is unknow type! key name = %s\n", userData->m_key);
                }
            }

        }
    }


    //  反序列化基础属性vector     //std::vector<BaseType>

    void CDeserializerBase::deserializeBaseTypeVector(std::shared_ptr<UserData> userData, void* jsonValue) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;

        if (_jsonValue->IsArray())
        {
            auto _oValue = _jsonValue->GetArray();
            for (auto& _item : _oValue)
            {
                if (_item.IsNull())
                {
                    //printf("std::vector<BaseType> is null.\n");
                }
                else if (userData->m_baseType == BaseType::Bool && _item.IsBool())
                {
                    auto _value = _item.GetBool();
                    ((std::vector<bool>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::Int32 && _item.IsNumber())
                {
                    auto _value = _item.GetInt();
                    ((std::vector<int32_t>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::UInt32 && _item.IsNumber())
                {
                    auto _value = _item.GetUint();
                    ((std::vector<uint32_t>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::Int64 && _item.IsNumber())
                {
                    auto _value = _item.GetInt64();
                    ((std::vector<int64_t>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::UInt64 && _item.IsNumber())
                {
                    auto _value = _item.GetUint64();
                    ((std::vector<uint64_t>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::Float && _item.IsNumber())
                {
                    auto _value = _item.GetFloat();
                    ((std::vector<float>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::Double && _item.IsNumber())
                {
                    auto _value = _item.GetDouble();
                    ((std::vector<double>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::String && _item.IsString())
                {
                    auto _value = _item.GetString();
                    ((std::vector<std::string>*)userData->m_value)->push_back(_value);
                }
                else if (userData->m_baseType == BaseType::Object && _item.IsObject())
                {
                    //auto _value = _item.GetObject();
                    //std::string _className = getObjectClassName(userData->m_typeName);
                    //auto _objectPtr = ObjectBase::createObject(_className.data());
                    //((std::vector<std::shared_ptr<ObjectBase>>*)userData->m_value)->push_back(std::shared_ptr<ObjectBase>(_objectPtr));
                    //deserializeChildObject(_objectPtr, &_item);
                    if (m_isPrintError) printf("Unsupported format std::vector<Object>! key name = %s\n", userData->m_key);
                }
                else
                {
                    if (m_isPrintError) printf("Deserialize std::vector<BaseType> error: value is unknow type! key name = %s\n", userData->m_key);
                }
            }

        }
    }

    //  反序列化共享指针vector     //std::vector<std::shared_ptr<BaseType>>

    void CDeserializerBase::deserializeSharedPtrVector(std::shared_ptr<UserData> userData, void* jsonValue) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;

        if (_jsonValue->IsArray())
        {
            auto _oValue = _jsonValue->GetArray();
            for (auto& _item : _oValue)
            {
                if (_item.IsNull())
                {
                    //printf("std::vector<std::shared_ptr<BaseType>> is null.\n");
                }
                else if (userData->m_baseType == BaseType::Bool && _item.IsBool())
                {
                    auto _value = _item.GetBool();
                    auto _valuePtr = std::make_shared<bool>(_value);
                    ((std::vector<std::shared_ptr<bool>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::Int32 && _item.IsNumber())
                {
                    auto _value = _item.GetInt();
                    auto _valuePtr = std::make_shared<int32_t>(_value);
                    ((std::vector<std::shared_ptr<int32_t>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::UInt32 && _item.IsNumber())
                {
                    auto _value = _item.GetUint();
                    auto _valuePtr = std::make_shared<uint32_t>(_value);
                    ((std::vector<std::shared_ptr<uint32_t>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::Int64 && _item.IsNumber())
                {
                    auto _value = _item.GetInt64();
                    auto _valuePtr = std::make_shared<int64_t>(_value);
                    ((std::vector<std::shared_ptr<int64_t>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::UInt64 && _item.IsNumber())
                {
                    auto _value = _item.GetUint64();
                    auto _valuePtr = std::make_shared<uint64_t>(_value);
                    ((std::vector<std::shared_ptr<uint64_t>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::Float && _item.IsNumber())
                {
                    auto _value = _item.GetFloat();
                    auto _valuePtr = std::make_shared<float>(_value);
                    ((std::vector<std::shared_ptr<float>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::Double && _item.IsNumber())
                {
                    auto _value = _item.GetDouble();
                    auto _valuePtr = std::make_shared<double>(_value);
                    ((std::vector<std::shared_ptr<double>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::String && _item.IsString())
                {
                    auto _value = _item.GetString();
                    auto _valuePtr = std::make_shared<std::string>(_value);
                    ((std::vector<std::shared_ptr<std::string>>*)userData->m_value)->push_back(_valuePtr);
                }
                else if (userData->m_baseType == BaseType::Object && _item.IsObject())
                {
                    if (!userData->m_typeName)
                    {
                        if (m_isPrintError) printf("Deserialize std::vector<std::shared_ptr<BaseType>> error: type name is null, please check! key name = %s\n", userData->m_key);
                        return;
                    }

                    std::string _className = getObjectClassName(userData->m_typeName);
                    auto _objectPtr = ObjectBase::createObject(_className.data());
                    ((std::vector<std::shared_ptr<ObjectBase>>*)userData->m_value)->push_back(std::shared_ptr<ObjectBase>(_objectPtr));
                    deserializeChildObject(_objectPtr, &_item);
                }
                else
                {
                    if (m_isPrintError) printf("Deserialize std::vector<std::shared_ptr<BaseType>> error: value is unknow type! key name = %s\n", userData->m_key);
                }
            }

        }
    }


    //  反序列化基础属性list<pair>     //std::list<std::pair<BaseType, BaseType>>

    void CDeserializerBase::deserializePairList(std::shared_ptr<UserData> userData, void* jsonValue) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;

        if (_jsonValue->IsArray())
        {
            size_t _count = 0;
            auto _oValue = _jsonValue->GetArray();
            for (auto& _item : _oValue)
            {
                _count++;
                if (_count == 2) _count = 0;

                if (_item.IsNull())
                {
                    //printf("std::list<std::pair<BaseType, BaseType>> is null.\n");
                }
                else if (userData->m_baseType == BaseType::Bool && _item.IsBool())
                {
                    auto _value = _item.GetBool();
                    auto _data = (std::list<std::pair<bool, bool>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, false));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::Int32 && _item.IsNumber())
                {
                    auto _value = _item.GetInt();
                    auto _data = (std::list<std::pair<int32_t, int32_t>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::UInt32 && _item.IsNumber())
                {
                    auto _value = _item.GetUint();
                    auto _data = (std::list<std::pair<uint32_t, uint32_t>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::Int64 && _item.IsNumber())
                {
                    auto _value = _item.GetInt64();
                    auto _data = (std::list<std::pair<int64_t, int64_t>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::UInt64 && _item.IsNumber())
                {
                    auto _value = _item.GetUint64();
                    auto _data = (std::list<std::pair<uint64_t, uint64_t>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::Float && _item.IsNumber())
                {
                    auto _value = _item.GetFloat();
                    auto _data = (std::list<std::pair<float, float>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0.0f));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::Double && _item.IsNumber())
                {
                    auto _value = _item.GetDouble();
                    auto _data = (std::list<std::pair<double, double>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0.0));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::String && _item.IsString())
                {
                    auto _value = _item.GetString();
                    auto _data = (std::list<std::pair<std::string, std::string>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, ""));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::Object && _item.IsObject())
                {
                    if (m_isPrintError) printf("Unsupported format std::list<std::pair<BaseType, BaseType>>! key name = %s\n", userData->m_key);
                }
                else
                {
                    if (m_isPrintError) printf("Deserialize std::list<std::pair<BaseType, BaseType>> error: value is unknow type! key name = %s\n", userData->m_key);
                }

            }

        }
    }


    //  反序列化共享指针vector<pair>     //std::vector<std::pair<BaseType, BaseType>>

    void CDeserializerBase::deserializePairVector(std::shared_ptr<UserData> userData, void* jsonValue) const
    {
        auto _jsonValue = (rapidjson::Value*)jsonValue;

        if (_jsonValue->IsArray())
        {
            size_t _count = 0;
            auto _oValue = _jsonValue->GetArray();
            for (auto& _item : _oValue)
            {
                _count++;
                if (_count == 2) _count = 0;

                if (_item.IsNull())
                {
                    //printf("std::list<std::pair<BaseType, BaseType>> is null.\n");
                }
                else if (userData->m_baseType == BaseType::Bool && _item.IsBool())
                {
                    auto _value = _item.GetBool();
                    auto _data = (std::vector<std::pair<bool, bool>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, false));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::Int32 && _item.IsNumber())
                {
                    auto _value = _item.GetInt();
                    auto _data = (std::vector<std::pair<int32_t, int32_t>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::UInt32 && _item.IsNumber())
                {
                    auto _value = _item.GetUint();
                    auto _data = (std::vector<std::pair<uint32_t, uint32_t>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::Int64 && _item.IsNumber())
                {
                    auto _value = _item.GetInt64();
                    auto _data = (std::vector<std::pair<int64_t, int64_t>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::UInt64 && _item.IsNumber())
                {
                    auto _value = _item.GetUint64();
                    auto _data = (std::vector<std::pair<uint64_t, uint64_t>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::Float && _item.IsNumber())
                {
                    auto _value = _item.GetFloat();
                    auto _data = (std::vector<std::pair<float, float>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0.0f));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::Double && _item.IsNumber())
                {
                    auto _value = _item.GetDouble();
                    auto _data = (std::vector<std::pair<double, double>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, 0.0));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::String && _item.IsString())
                {
                    auto _value = _item.GetString();
                    auto _data = (std::vector<std::pair<std::string, std::string>>*)userData->m_value;
                    if (_count == 1)
                        _data->push_back(std::make_pair(_value, ""));
                    else
                        _data->back().second = _value;
                }
                else if (userData->m_baseType == BaseType::Object && _item.IsObject())
                {
                    if (m_isPrintError) printf("Unsupported format std::vector<std::pair<BaseType, BaseType>>! key name = %s\n", userData->m_key);
                }
                else
                {
                    if (m_isPrintError) printf("Deserialize std::vector<std::pair<BaseType, BaseType>> error: value is unknow type! key name = %s\n", userData->m_key);
                }

            }

        }
    }
}
