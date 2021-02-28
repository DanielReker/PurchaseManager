#pragma once

#include "HtmlParser.h"
#include "Product.h"

#include <QObject>
#include <QTimer>

#include <deque>

class ProductsUpdater : public QObject {
	Q_OBJECT

private:
	QTimer* m_pNextUpdateTimer;

	// Deque is used instead of queue because queue unlike deque has not iterators to find products, but it should be used as default queue
	std::deque< Product* > m_updateQueue;

public:
	ProductsUpdater();

	void addProduct(Product* pProduct);

public slots:
	void onInformationReceived(const QString& productName, const QString& localityName, Product::Status status);

	void beginUpdate();

signals:
	void requestParsingInformation(int localityID, int productID);

};

