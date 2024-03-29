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

	// Deque is used instead of queue because queue unlike deque have not iterators to find products, but it should be used as default queue
	std::deque< Product* > m_updateQueue;

	void startTimer();

public:
	ProductsUpdater(QObject* parent = nullptr);

	void addProduct(Product* pProduct);

	void localityRemoved(int column);
	void productTypeRemoved(int row);

public slots:
	void onInformationReceived(const QString& productName, const QString& localityName, Product::Status status);

	void beginUpdate();

signals:
	void requestParsingInformation(QString localityID, QString productID);

};

