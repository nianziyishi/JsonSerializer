#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <string>
#include <map>
#include <iostream>

namespace CJsonHelper
{
    typedef void* (*pCreateObject)(void);

    template <typename T>
    T* createObj(void)
    {
        auto _object = new T;
        return _object;
    }

    class ObjectFactory
    {

    public:
        void* CreateObjectByName(std::string className)
        {
            auto _iter = m_classMap.find(className);
            if (_iter != m_classMap.end())
            {
                return _iter->second(); //函数指针的调用
            }

            printf("The \"%s\" is not registered, please register first!\n", className.data());
            return nullptr;
        }

        void registClass(std::string className, pCreateObject method)
        {
            auto _iter = m_classMap.find(className);
            if (_iter == m_classMap.end())
            {
                m_classMap.insert(std::pair<std::string, pCreateObject>(className, method));
                return;
            }

            //printf("The \"%s\" has been registered, please check!\n", className.data());
        }

        static ObjectFactory& getInstance()
        {
            static ObjectFactory _objectFactory;
            return _objectFactory;
        }

    private:
        ObjectFactory() {};

    private:
        std::map<std::string, pCreateObject> m_classMap;
    };


    class RegisterObject
    {
    public:
        RegisterObject(std::string className, pCreateObject ptrCreateFn)
        {
            ObjectFactory::getInstance().registClass(className, ptrCreateFn);
        }
    };

}

#define REGISTER_CLASS(className)										\
    static CJsonHelper::RegisterObject g_creatorRegister##className(    \
    #className, reinterpret_cast<CJsonHelper::pCreateObject>(CJsonHelper::createObj<className>));

#endif // OBJECTFACTORY_H
