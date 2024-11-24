#ifndef PARK_H
#define PARK_H

#include "CJsonSerializer/ObjectBase.h"


class Park : CJsonHelper::ObjectBase
{
public:
    Park()
	{
		this->setProperty();
	}

	void setProperty() override
	{

		addPropertyB(m_nameA)
		addPropertyB(m_age)
		addPropertyB(m_money)
		addPropertyB(m_address)

		addPropertyB(m_null)
		addPropertyB(m_bool)
		addPropertyB(m_int)
		addPropertyB(m_uint)
		addPropertyB(m_int64)
		addPropertyB(m_uint64)
		addPropertyB(m_float)
		addPropertyB(m_double)
		addPropertyB(m_string)

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

	bool m_null{ false };
	bool m_bool{ false };
	int32_t m_int{ 123 };
	uint32_t m_uint{ 456 };
	int64_t m_int64{ 789 };
	uint64_t m_uint64{ 10 };
	float m_float{ 11.11f };
	double m_double{ 22.22 };
	std::string m_string{ "string" };

	int m_age{ 0 };
	double m_money{ 0 };
	std::string m_address;

private:
	std::string m_nameA;

};

REGISTER_CLASS(Park)
#endif // PARK_H
