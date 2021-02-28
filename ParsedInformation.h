#pragma once

#include <QObject>
#include <QString>

class ParsedInformation : public QObject {
	Q_OBJECT

private:
	QString m_localityName;
	QString m_productName;

	int m_localityID;
	int m_productID;

	bool m_productAvailable;

public:
	ParsedInformation(const QString& localityName, int localityID, const QString& productName, int productID, bool productAvailable);

};

