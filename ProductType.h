#pragma once

#include <QObject>
#include <QString>
#include <QTableWidget>

class Shop;

class ProductType : public QObject {
	Q_OBJECT

private:
	int m_productID;
	int m_row;
	QString m_productName;

	QTableWidgetItem* m_pItem;

public:
	ProductType(int productID, int row);

	QTableWidgetItem* getItem();

	int getRow() const;

	int getID() const;

public slots:
	void setProductType(const QString& productName);

signals:

};

