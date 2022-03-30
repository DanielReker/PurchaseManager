#pragma once

#include "ProductType.h"
#include "Locality.h"

#include <QObject>
#include <QNetworkReply>
#include <QTableWidget>

class Product : public QObject {
	Q_OBJECT

public:
	enum class Status {
		UNKNOWN,
		AVAILABLE,
		UNAVAILABLE,
		ERROR,
		MAX_STATUSES
	};

private:
	QTableWidgetItem* m_pItem;

	ProductType* m_pProductType;
	Locality* m_pLocality;

	Status m_status;

public:
	Product(Locality* pLocality, ProductType* pProductType);

	virtual ~Product();

	QTableWidgetItem* getItem() const;

	int getColumn() const;
	int getRow() const;

	QString getLocalityID() const;
	QString getProductID() const;

	friend bool operator== (const Product& p1, const Product& p2);
	friend bool operator!= (const Product& p1, const Product& p2);

public slots:
	void setProductType(const QString& productType);
	void setLocalityName(const QString& localityName);
	void setStatus(Status status);

signals:

};

