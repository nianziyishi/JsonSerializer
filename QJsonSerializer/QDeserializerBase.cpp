#include "QDeserializerBase.h"
#include <QMetaProperty>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "CustomType.h"

namespace QJsonHelper
{
    void QDeserializerBase::deserialize(const QByteArray& data, void* instance, const QMetaObject* metaObject)
    {
        QJsonParseError errorinfo;
        const QJsonDocument& document = QJsonDocument::fromJson(data, &errorinfo);
        if (errorinfo.error == QJsonParseError::NoError)
        {
            if (document.isObject())
            {
                const QJsonObject& root = document.object();
                deserializeObject(root, instance, metaObject);
            }
            else
            {
                qDebug() << QStringLiteral("Json解析出错：Json数据应该为对象");
                qDebug() << "Json data:" << QString(data);
            }
        }
        else
        {
            qDebug() << "Not is Json format data, error info :" << errorinfo.errorString();
            qDebug() << "Json data:" << QString(data);
        }
    }


    QVariant QDeserializerBase::deserializeQList(const QJsonArray& array, QString typeName, int userType)
    {
        if (typeName == "bool")
        {
            QList<bool> _listValue;
            foreach(auto element, array) _listValue.append(element.toBool());
            QVariant data(userType, &_listValue);
            return data;
        }
        else if (typeName == "int32_t" || typeName == "int")
        {
            QList<int32_t> _listValue;
            foreach(auto element, array) _listValue.append(element.toInt());
            QVariant data(userType, &_listValue);
            return data;
        }
        else if (typeName == "int64_t")
        {
            QList<int64_t> _listValue;
            foreach(auto element, array) _listValue.append(element.toString().toLongLong());
            QVariant data(userType, &_listValue);
            return data;
        }
        else if (typeName == "long")
        {
            QList<long> _listValue;
            foreach(auto element, array) _listValue.append(element.toString().toLong());
            QVariant data(userType, &_listValue);
            return data;
        }
        else if (typeName == "float")
        {
            QList<float> _listValue;
            foreach(auto element, array) _listValue.append(element.toDouble());
            QVariant data(userType, &_listValue);
            return data;
        }
        else if (typeName == "double")
        {
            QList<double> _listValue;
            foreach(auto element, array) _listValue.append(element.toDouble());
            QVariant data(userType, &_listValue);
            return data;
        }
        else if (typeName == "QString")
        {
            QStringList _listValue;
            foreach(auto element, array) _listValue.append(element.toString());
            QVariant data(userType, &_listValue);
            return data;
        }
        else if (typeName == "QPair<int,int>*")
        {
            QList<QPair<int, int>> _listValue;
            for (int i = 0; i < array.size(); i += 2)
                _listValue.append({ array[i].toInt(), array[i + 1].toInt() });
            QVariant data(userType, &_listValue);
            return data;
        }
        else if (typeName == "QPair<double,double>*")
        {
            QList<QPair<double, double>> _listValue;
            for (int i = 0; i < array.size(); i += 2)
                _listValue.append({ array[i].toDouble(),array[i + 1].toDouble() });
            QVariant data(userType, &_listValue);
            return data;
        }
        else if (typeName == "QList<int>*")
        {
            QList<QList<int>> _listValue;
            for (int i = 0; i < array.size(); ++i)
            {
                QList<int> _listValueChild;
                const QJsonArray& _jsonArray = array[i].toArray();
                for (int j = 0; j < _jsonArray.size(); ++j)
                {
                    _listValueChild.append(_jsonArray[j].toInt());
                }
                _listValue.append(_listValueChild);
            }
            QVariant data(userType, &_listValue);
            return data;
        }
        else
        {
            qDebug() << QString("unknow type : QList<%1>!").arg(typeName);
        }

        return {};
    }

    template<class T>
    void QDeserializerBase::deserializeSharedPtrQList(const QJsonArray& array, QList<QSharedPointer<T>>& target, QString metaType)
    {
        foreach(auto _element, array)
        {
            const QJsonObject& _elementObject = _element.toObject();
            if (_elementObject.isEmpty())
            {
                target.append(Q_NULLPTR);// 添加空对象
            }
            else
            {
                auto _type_id = QMetaType::type(metaType.toStdString().data());
                if (_type_id != QMetaType::UnknownType)
                {
                    auto _metaObjectPtr = QMetaType::metaObjectForType(_type_id);
                    auto _object = QSharedPointer<T>(_metaObjectPtr->newInstance());
                    target.append(_object);
                    deserializeObject(_elementObject, _object.get(), _object->metaObject());
                }
                else
                {
                    qDebug() << QString("Please register custom type! For example: qRegisterMetaType<%1>(\"%1\")").arg(metaType);
                }
            }
        }
    }

    void QDeserializerBase::deserializeObject(const QJsonObject& object, void* instance, const QMetaObject* metaObject)
    {
        if (!instance) return;
        if (object.isEmpty()) instance = Q_NULLPTR;

        for (int _index = 1; _index < metaObject->propertyCount(); ++_index)
        {
            const QMetaProperty& _property = metaObject->property(_index);
            const QByteArray& _name = _property.name();
            const QJsonValue& _value = object.value(_name);
            const QJsonValue::Type _type = _value.type();
            switch (_type)
            {
            case QJsonValue::Object:
            {
                const int _userTypeID = _property.userType();

                if (_userTypeID != QMetaType::UnknownType)
                {
                    const QMetaType _userType(_userTypeID);
                    const QMetaObject* _childMetaObject = _userType.metaObject();

                    //对象指针 QObject*
                    if (_childMetaObject)
                    {
                        const QJsonObject& _jsonObject = _value.toObject();
                        QObject* _object = Q_NULLPTR;
                        if (!_jsonObject.isEmpty())
                        {
                            _object = _childMetaObject->newInstance();
                            if (!_object)
                            {
                                qCritical() << "Failed to create an instance of type" << metaObject->className();
                                qCritical() << "Did you forget to declare a public parameterless Q_INVOKABLE constructor?";
                            }
                        }
                        const QVariant _data(_userTypeID, &_object);
                        deserializeObject(_jsonObject, _object, _childMetaObject);
                        _property.write((QObject*)instance, _data);
                    }
                    //对象智能指针 QSharedPointer<Object>
                    else
                    {
                        QString _typeName = QString(_property.typeName());
                        QString _className;
                        auto _type = CustomType::GetQListType(_typeName, _className);
                        if (_type == EnumCustomType::SharedPtr)
                        {
                            _className.append('*');
                            auto _typeID = QMetaType::type(_className.toStdString().data());
                            if (_typeID != QMetaType::UnknownType)
                            {
                                const QJsonObject& _jsonObject = _value.toObject();
                                if (!_jsonObject.isEmpty())
                                {
                                    auto _metaObjectPtr = QMetaType::metaObjectForType(_typeID);
                                    auto _objectPtr = QSharedPointer<QObject>(_metaObjectPtr->newInstance());
                                    deserializeObject(_jsonObject, _objectPtr.get(), _objectPtr->metaObject());
                                    const QVariant _data(_userTypeID, &_objectPtr);
                                    _property.write((QObject*)instance, _data);
                                }
                            }
                            else
                            {
                                qDebug() << QString("Please register custom type! For example: qRegisterMetaType<%1>(\"%1\")").arg(_className);
                            }
                        }
                    }

                }
                else
                {
                    qCritical() << "Unknown user type for property" << _name;
                }

                break;
            }
            case QJsonValue::Array:
            {
                const QJsonArray& _jsonArray = _value.toArray();
                QString _typeName = QString(_property.typeName());
                QString _className;
                auto _type = CustomType::GetQListType(_typeName, _className);
                if (_type == EnumCustomType::QList)
                {
                    const int _userTypeID = _property.userType();
                    if (_userTypeID != QMetaType::UnknownType)
                    {
                        auto _data = deserializeQList(_jsonArray, _className, _userTypeID);
                        _property.write((QObject*)instance, _data);
                    }
                }
                else if (_type == EnumCustomType::SharedPtrQList)
                {
                    const int _userTypeID = _property.userType();
                    if (_userTypeID != QMetaType::UnknownType)
                    {
                        QList<QSharedPointer<QObject>> _list_object;
                        deserializeSharedPtrQList(_jsonArray, _list_object, _className);
                        const QVariant _data(_userTypeID, &_list_object);
                        _property.write((QObject*)instance, _data);
                    }
                }
                else
                {
                    qDebug() << QString("unknow type:%1!").arg(_typeName);
                }
                break;
            }
            case QJsonValue::Undefined:
            {
                qDebug() << QString("QJsonValue Type Undefined:%1, Class Name:%2, Property Name:%3")
                    .arg(_property.typeName()).arg(metaObject->className()).arg(_property.name());
                break;
            }
            case QJsonValue::Null:
            {
                break;
            }
            default:
            {
                const QVariant& _variant = _value.toVariant();
                _property.write((QObject*)instance, _variant);
                break;
            }
            }
        }
    }
}
