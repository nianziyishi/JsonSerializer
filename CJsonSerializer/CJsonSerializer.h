#ifndef CJSONHELPER_H
#define CJSONHELPER_H


#include "CISerializer.h"
#include "CIDeserializer.h"

namespace CJsonHelper
{
    class CJsonSerializer
    {
    public:
        /// @brief 序列化对象为json字符串
        /// @tparam TSource 对象类型
        /// @param source 对象指针引用
        /// @return json字符串
        template<class TSource>
        const std::string serialize(const TSource& source) const
        {
            CISerializer<TSource> subject;
            return subject.serialize(source);
        }

        /// @brief 反序列化json字符串为数据对象
        /// @tparam TReturn TSource 对象类型
        /// @param data json字符串
        /// @param isPrintError 是否打印错误信息
        /// @return 对象指针
        template<class TReturn>
        TReturn deserialize(const std::string& data, bool isPrintError = false) const
        {
            CIDeserializer<TReturn> subject;
            return subject.deserialize(data, isPrintError);
        }
    };
}

#endif
