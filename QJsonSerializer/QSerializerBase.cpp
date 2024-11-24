#include "QSerializerBase.h"
#include <QMetaProperty>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "CustomType.h"

namespace QJsonHelper
{
    QByteArray QSerializerBase::serialize(const void* source, const QMetaObject& metaObject, bool isCompact)
    {
        const QJsonObject& object = serializeObject(source, &metaObject);

        QJsonDocument document;
        document.setObject(object);

        return document.toJson(isCompact ? QJsonDocument::Compact : QJsonDocument::Indented);
    }

    QJsonArray QSerializerBase::serializeQList(const void* source, QString typeName)
    {
        QJsonArray _array;

        if (typeName == "bool")
        {
            auto _listValue = (QList<bool>*)source;
            for (const auto& _element : *_listValue) _array.append(_element);
        }
        else if (typeName == "int32_t" || typeName == "int")
        {
            auto _listValue = (QList<int32_t>*)source;
            for (const auto& _element : *_listValue) _array.append(_element);
        }
        else if (typeName == "int64_t")
        {
            auto _listValue = (QList<qint64>*)source;
            for (const auto& _element : *_listValue) _array.append(_element);
        }
        else if (typeName == "long")
        {
            auto _listValue = (QList<qint64>*)source;
            for (const auto& _element : *_listValue) _array.append(_element);
        }
        else if (typeName == "float")
        {
            auto _listValue = (QList<float>*)source;
            for (const auto& _element : *_listValue) _array.append(_element);
        }
        else if (typeName == "double")
        {
            auto _listValue = (QList<double>*)source;
            for (const auto& _element : *_listValue) _array.append(_element);
        }
        else if (typeName == "QString")
        {
            auto _listValue = (QList<QString>*)source;
            for (const auto& _element : *_listValue) _array.append(_element);
        }
        else if (typeName == "QPair<int,int>*")
        {
            auto _listValue = (QList<QPair<int, int>>*)source;
            for (const auto& _element : *_listValue)
            {
                _array.append(_element.first);
                _array.append(_element.second);
            }
        }
        else if (typeName == "QPair<double,double>*")
        {
            auto _listValue = (QList<QPair<double, double>>*)source;
            for (const auto& _element : *_listValue)
            {
                _array.append(_element.first);
                _array.append(_element.second);
            }
        }
        else if (typeName == "QList<int>*")
        {
            auto _listValue = (QList<QList<int>>*)source;
            for (const auto& childList : *_listValue)
            {
                QJsonArray _childArray;
                for (const auto& i : childList)
                {
                    _childArray.append(i);
                }
                _array.append(_childArray);
            }
        }
        else
        {
            qDebug() << QString("unknow type : QList<%1>!").arg(typeName);
        }

        return _array;
    }

    template<class T>
    QJsonObject QSerializerBase::serializeSharedPtr(QSharedPointer<T>& source)
    {
        QJsonObject _target;
        if (!source.isNull())
        {
            _target = serializeObject(source.get(), source->metaObject());
        }
        return _target;
    }

    template<class T>
    QJsonArray QSerializerBase::serializeSharedPtrQList(QList<QSharedPointer<T>>& source)
    {
        QJsonArray _array;
        foreach(const auto & _element, source)
        {
            if (_element)
            {
                _array.append(serializeObject(_element.get(), _element->metaObject()));
            }
        }
        return _array;
    }

    QJsonObject QSerializerBase::serializeObject(const void* source, const QMetaObject* metaObject)
    {
        QJsonObject _target;

        for (int _index = 1; _index < metaObject->propertyCount(); ++_index)
        {
            const QMetaProperty& _property = metaObject->property(_index);
            const QByteArray& _name = _property.name();

            const QVariant& _value = _property.read((QObject*)source);
            const QVariant::Type _type = _value.type();

            if (_type == QVariant::UserType)
            {
                const int _userTypeID = _property.userType();
                const int _flags = QMetaType::typeFlags(_userTypeID);

                if (_flags & QMetaType::PointerToQObject)
                {
                    auto _object = _value.constData();
                    if (_object)
                    {
                        auto _childMetaObject = QMetaType::metaObjectForType(_userTypeID);
                        _target[_name] = serializeObject(_object, _childMetaObject);
                    }
                }
                else
                {
                    QString _typeName = QString(_property.typeName());
                    QString _className;
                    auto _type = CustomType::GetQListType(_typeName, _className);
                    if (_type == EnumCustomType::QList)
                    {
                        _target[_name] = serializeQList(_value.data(), _className);
                    }
                    else if (_type == EnumCustomType::SharedPtr)
                    {
                        const auto& _factory = (QSharedPointer<QObject>*)_value.data();
                        _target[_name] = serializeSharedPtr(*_factory);
                    }
                    else if (_type == EnumCustomType::SharedPtrQList)
                    {
                        const auto& _factory = (QList<QSharedPointer<QObject>>*)_value.data();
                        _target[_name] = serializeSharedPtrQList(*_factory);
                    }
                    else
                    {
                        qDebug() << QString("unknow type:%1, flags=%2!").arg(_typeName).arg(_flags);
                    }
                }
            }
            else
            {
                if (_property.type() == QVariant::DateTime && _value.toDateTime().isNull())
                {
                    _target[_name] = QJsonValue();
                }
                else
                {
                    _target[_name] = QJsonValue::fromVariant(_value);
                }
            }
        }

        return _target;
    }

}
