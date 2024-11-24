#include <iostream>
#include "City.h"
#include "Park.h"
#include "QPark.h"
#include "CJsonSerializer/CJsonSerializer.h"
#include "QJsonSerializer/QJsonSerializer.h"


void testCJson();
void testQJson();

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";
    
    testCJson();
    testQJson();

    return 0;

}


void testCJson()
{

    auto _city = new City();
    _city->setNameA("C++语言自由城");
    _city->m_age = 18;
    _city->m_money = 120000;
    _city->m_address = "自由城程序路径！";

    _city->m_intPtr = std::make_shared<int>(100);

    _city->m_listAge.push_back(200);
    _city->m_listAge.push_back(300);
    _city->m_listAge.push_back(500);
    _city->m_listMoney.push_back(0.4567);
    _city->m_listMoney.push_back(100.3245);
    _city->m_listMoney.push_back(2300.7896);

    _city->m_listAddress.push_back("2300");
    _city->m_listAddress.push_back("6500");
    _city->m_listAddress.push_back("中文asd00.54");

    _city->m_vectorAddress.push_back("2300vector");
    _city->m_vectorAddress.push_back("6500vector");
    _city->m_vectorAddress.push_back("中文asd00.54vector");

    _city->m_listIntPtr.push_back(std::shared_ptr<int>());
    _city->m_listIntPtr.push_back(std::make_shared<int>(10));
    _city->m_listIntPtr.push_back(std::make_shared<int>(20));

    _city->m_vectorIntPtr.push_back(std::shared_ptr<int>());
    _city->m_vectorIntPtr.push_back(std::make_shared<int>(30));
    _city->m_vectorIntPtr.push_back(std::make_shared<int>(40));

    _city->m_objectAPtrPark = std::make_shared<Park>();
    _city->m_objectAPtrPark->setNameA("_zoo->m_objectAPtrPark");
    _city->m_objectAPtrPark->m_address = "_zoo->m_objectAPtrPark->m_address";

    _city->m_listObjectAPtrPark.push_back(std::shared_ptr<Park>());
    _city->m_listObjectAPtrPark.push_back(std::make_shared<Park>());
    _city->m_listObjectAPtrPark.push_back(std::make_shared<Park>());

    _city->m_vectorObjectAPtrPark.push_back(std::shared_ptr<Park>());
    _city->m_vectorObjectAPtrPark.push_back(std::make_shared<Park>());
    _city->m_vectorObjectAPtrPark.push_back(std::make_shared<Park>());

    _city->m_listPairInt.push_back(std::make_pair(3, 300));
    _city->m_listPairInt.push_back(std::make_pair(3, 600));
    _city->m_listPairInt.push_back(std::make_pair(4, 400));

    _city->m_vectorPairDouble.push_back(std::make_pair(3.33, 300.33));
    _city->m_vectorPairDouble.push_back(std::make_pair(3.66, 600.66));
    _city->m_vectorPairDouble.push_back(std::make_pair(4.44, 400.44));

    _city->m_listPairString.push_back(std::make_pair("firstKey1", "firstValue1"));
    _city->m_listPairString.push_back(std::make_pair("firstKey2", "firstValue2"));
    _city->m_listPairString.push_back(std::make_pair("firstKey3", "firstValue3"));

    _city->m_vectorPairString.push_back(std::make_pair("firstKey1", "firstValue1"));
    _city->m_vectorPairString.push_back(std::make_pair("firstKey2", "firstValue2"));
    _city->m_vectorPairString.push_back(std::make_pair("firstKey3", "firstValue3"));

    CJsonHelper::CJsonSerializer _serializer;

    std::cout << "\n=================C版 自定义类转Json字符串\n";

    // 将自定义类对象 转换为 json字符串
    std::string _jsonText;
    //for (int _index = 0; _index < 10000; ++_index)
    {
        _jsonText = _serializer.serialize<City*>(_city);
    }
    printf("_json Data is : \n%s\n", _jsonText.data());

    std::cout << "\n=================C版 Json字符串转自定义类\n";

    // 将json字符串 转换为 自定义类对象
    City* _jsonObject{};
    //for (int _index = 0; _index < 10000; ++_index)
    {
        _jsonObject = _serializer.deserialize<City*>(_jsonText);//_strJson
    }

    _jsonText = _serializer.serialize<City*>(_jsonObject);
    printf("_json Data is : \n%s\n", _jsonText.data());


}

void testQJson()
{
    qRegisterMetaType<QPark*>();
    qRegisterMetaType<QList<QSharedPointer<QPark>>>();

    QPark* _park = new QPark();
    _park->m_childList.append(QSharedPointer<QPark>(new QPark()));
    _park->m_childList.append(QSharedPointer<QPark>(new QPark()));
    _park->m_childList.append(QSharedPointer<QPark>(new QPark()));

    QJsonHelper::QJsonSerializer _serializer;


    std::cout << "\n=================Q版 自定义类转Json字符串\n";

    // 将自定义类对象 转换为 json字符串
    QByteArray _jsonText;
    //for (int _index = 0; _index < 10000; ++_index)
    {
        _jsonText = _serializer.serialize<QPark*>(_park);
    }
    printf("_json Data is : \n%s\n", _jsonText.toStdString().data());

    std::cout << "\n=================Q版 Json字符串转自定义类\n";

    // 将json字符串 转换为 自定义类对象
    QPark* _jsonObject{};
    //for (int _index = 0; _index < 10000; ++_index)
    {
        _jsonObject = _serializer.deserialize<QPark*>(_jsonText);//_strJson
    }

    _jsonText = _serializer.serialize<QPark*>(_jsonObject);
    printf("_json Data is : \n%s\n", _jsonText.toStdString().data());


}
