#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QString>
#include <QStringList>

namespace QJsonHelper
{
    enum class EnumCustomType
    {
        None = 0,               //  unknow
        SharedPtr = 1,          //  QSharedPointer<QObject>
        QList = 2,              //  QList<BaseType>
        SharedPtrQList = 3,     //  QList<QSharedPointer<QObject>>
    };

    class CustomType
    {
    public:

        static EnumCustomType GetQListType(QString qlistTypeName, QString& className)
        {
            if (qlistTypeName == "QStringList")
            {
                className = "QString";
                return EnumCustomType::QList;
            }

            auto _listFlag = qlistTypeName.split('<');
            if (_listFlag.size() == 2)
            {
                auto _listType = _listFlag.at(1).split('>');
                if (_listFlag.at(0) == "QSharedPointer")
                {
                    className = _listType.at(0);
                    return EnumCustomType::SharedPtr;
                }
                else if (_listType.size() == 2 && _listType.at(1).isEmpty())
                {
                    if (_listFlag.at(0) == QString("QList"))
                    {
                        className = QString(_listFlag.at(1)).remove('>');
                        return EnumCustomType::QList;
                    }
                }
            }
            else if (_listFlag.size() == 3)
            {
                if (_listFlag.at(0) == QString("QList") && _listFlag.at(1) == QString("QSharedPointer"))
                {
                    auto _listType = _listFlag.at(2).split('>');
                    className = _listType.at(0) + '*';
                    return EnumCustomType::SharedPtrQList;
                }
                else if (_listFlag.at(0) == QString("QList") && _listFlag.at(1) == QString("QPair") && _listFlag.at(2) == QString("int,int> >"))
                {
                    className = "QPair<int,int>*";
                    return EnumCustomType::QList;
                }
                else if (_listFlag.at(0) == QString("QList") && _listFlag.at(1) == QString("QPair") && _listFlag.at(2) == QString("double,double> >"))
                {
                    className = "QPair<double,double>*";
                    return EnumCustomType::QList;
                }
                else if (_listFlag.at(0) == QString("QList") && _listFlag.at(1) == QString("QList") && _listFlag.at(2) == QString("int> >"))
                {
                    className = "QList<int>*";
                    return EnumCustomType::QList;
                }
            }

            return EnumCustomType::None;
        }

    };

}

#endif // JSONHELPER_H
