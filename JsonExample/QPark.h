#ifndef QPARK_H
#define QPARK_H

#include <QObject>
#include <QString>


class QPark : public QObject
{
    Q_OBJECT
        Q_PROPERTY(QString nameA READ getNameA WRITE setNameA)
        Q_PROPERTY(bool m_bool MEMBER m_bool)
        Q_PROPERTY(float m_float MEMBER m_float)
        Q_PROPERTY(QString m_string MEMBER m_string)
        Q_PROPERTY(int m_age MEMBER m_age)
        Q_PROPERTY(double m_money MEMBER m_money)
        Q_PROPERTY(QString m_address MEMBER m_address)
        Q_PROPERTY(QList<QSharedPointer<QPark>> m_childList MEMBER m_childList)

public:
    Q_INVOKABLE QPark() 
    {
        m_nameA = "QPark";
    };
    ~QPark() {};

    QString getNameA()
	{
		return m_nameA;
	}
	void setNameA(QString value)
	{
		m_nameA = value;
	}

public:

	bool m_bool{ false };
	float m_float{ 11.11f };
    QString m_string{ "string" };

	int m_age{ 0 };
	double m_money{ 22.22 };
    QString m_address;
    QList<QSharedPointer<QPark>> m_childList;
private:
    QString m_nameA;

};
Q_DECLARE_METATYPE(QPark*)
Q_DECLARE_METATYPE(QList<QSharedPointer<QPark>>)
#endif // QPARK_H
