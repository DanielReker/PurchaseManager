#pragma once

#include "Locality.h"
#include "ProductType.h"
#include "Product.h"

#include "HtmlParser.h"
#include "ProductsUpdater.h"

#include <QString>
#include <QListWidget>
#include <QTableWidget>
#include <QTimer>

#include <vector>


class Shop : public QObject {
	Q_OBJECT

private:
	ProductsUpdater* m_pProductUpdater;
	QTimer m_updateTimer;

	QString m_name;

	QTableWidget* m_pProductsTable;
	QListWidget* m_pLocalitiesList;

	std::vector<Locality*> m_localities;
	std::vector<ProductType*> m_productTypes;

	std::vector< std::vector<Product*> > m_products; // Contains: [ ProductType ] [ Locality ]   or   [ Row ] [ Column ]


protected:
	Shop(const QString& name);


	HtmlParser* m_pHtmlParser;

public:
	QString getName() const;
	QTableWidget* getProductsTable() const;

public slots:
	void addLocality(QString localityID);
	void addProductType(QString productID);

	virtual void requestProductInformation(QString localityID, QString productID) = 0;
	virtual void applyProductInformation(QString htmlPage, bool errorOccured) = 0;

	void updateAll();

signals:
	void informationReceived(const QString& productName, const QString& localityName, Product::Status status);

	void message(const QString& text, int timeout);
};

