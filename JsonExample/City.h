#ifndef CITY_H
#define CITY_H

#include "CJsonSerializer/ObjectBase.h"

class Park;

class City : public CJsonHelper::ObjectBase
{

public:
    City()
	{
		this->setProperty();
	}
	~City() {}

	void setProperty() override
	{
		addPropertyB(m_nameA)
		addPropertyB(m_age)
		addPropertyB(m_money)
		addPropertyB(m_address)

		addPropertyB(m_intPtr)
		addPropertyB(m_intPtrEmpty)		//空指针

		addPropertyB(m_listAge)
		addPropertyB(m_listMoney)
		addPropertyB(m_listAddress)
		addPropertyB(m_listAddressEmpty)

		addPropertyB(m_vectorAddress)
		addPropertyB(m_vectorAddressEmpty)

		addPropertyB(m_listIntPtr)
		addPropertyB(m_listIntPtrEmpty)	//空指针

		addPropertyB(m_vectorIntPtr)
		addPropertyB(m_vectorIntPtrEmpty)	//空指针

		addPropertyC(std::shared_ptr<Park>, m_objectAPtrPark)		//对象指针
		addPropertyC(std::shared_ptr<Park>, m_objectAPtrParkEmpty)		//空指针

		addPropertyC(std::list<std::shared_ptr<Park>>, m_listObjectAPtrPark)	//对象指针数组
		addPropertyC(std::list<std::shared_ptr<Park>>, m_listObjectAPtrParkEmpty)	//空指针

		addPropertyC(std::vector<std::shared_ptr<Park>>, m_vectorObjectAPtrPark)	//对象指针数组
		addPropertyC(std::vector<std::shared_ptr<Park>>, m_vectorObjectAPtrParkEmpty)	//空指针

		addPropertyB(m_listPairInt)	//pair数组
		addPropertyB(m_listPairIntEmpty)	//空指针

		addPropertyB(m_vectorPairDouble)	//pair数组
		addPropertyB(m_vectorPairDoubleEmpty)	//空指针

		addPropertyB(m_listPairString)	//pair数组
		addPropertyB(m_listPairStringEmpty)	//空指针

		addPropertyB(m_vectorPairString)	//pair数组
		addPropertyB(m_vectorPairStringEmpty)	//空指针

	}

	std::string nameA()
	{
		return m_nameA;
	}
	void setNameA(std::string value)
	{
		m_nameA = value;
	}


public:

	int m_age{ 0 };
	double m_money{ 0 };
	std::string m_address;

	std::shared_ptr<int> m_intPtr;
	std::shared_ptr<int> m_intPtrEmpty;		//空指针

	std::list<int> m_listAge;
	std::list<double> m_listMoney;
	std::list<std::string> m_listAddress;
	std::list<std::string> m_listAddressEmpty;

	std::vector<std::string> m_vectorAddress;
	std::vector<std::string> m_vectorAddressEmpty;

	std::list<std::shared_ptr<int>> m_listIntPtr;
	std::list<std::shared_ptr<int>> m_listIntPtrEmpty;	//空指针

	std::vector<std::shared_ptr<int>> m_vectorIntPtr;
	std::vector<std::shared_ptr<int>> m_vectorIntPtrEmpty;	//空指针

	std::shared_ptr<Park> m_objectAPtrPark;		//对象指针
	std::shared_ptr<Park> m_objectAPtrParkEmpty;		//空指针

	std::list<std::shared_ptr<Park>> m_listObjectAPtrPark;	//对象指针数组
	std::list<std::shared_ptr<Park>> m_listObjectAPtrParkEmpty;	//空指针

	std::vector<std::shared_ptr<Park>> m_vectorObjectAPtrPark;	//对象指针数组
	std::vector<std::shared_ptr<Park>> m_vectorObjectAPtrParkEmpty;	//空指针

	std::list<std::pair<int, int>> m_listPairInt;	//pair数组
	std::list<std::pair<int, int>> m_listPairIntEmpty;	//空指针

	std::vector<std::pair<double, double>> m_vectorPairDouble;	//pair数组
	std::vector<std::pair<double, double>> m_vectorPairDoubleEmpty;	//空指针

	std::list<std::pair<std::string, std::string>> m_listPairString;	//pair数组
	std::list<std::pair<std::string, std::string>> m_listPairStringEmpty;	//空指针

	std::vector<std::pair<std::string, std::string>> m_vectorPairString;	//pair数组
	std::vector<std::pair<std::string, std::string>> m_vectorPairStringEmpty;	//空指针


private:
	std::string m_nameA;

};

REGISTER_CLASS(City)
#endif // CITY_H
