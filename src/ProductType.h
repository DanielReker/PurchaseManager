#pragma once

#include <QObject>
#include <QString>
#include <QTableWidget>

class Shop;

class ProductType : public QObject {
	Q_OBJECT

private:
	QString m_productID;
	int m_row;
	QString m_productName;

	QTableWidgetItem* m_pItem;

public:
	ProductType(QString productID, int row);

	QTableWidgetItem* getItem();

	int getRow() const;
	void setRow(int row);

	QString getID() const;

public slots:
	void setProductType(const QString& productName);

signals:

};

