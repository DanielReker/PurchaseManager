#include "Shop.h"

#include <QApplication>

Shop::Shop(const QString& name)
	: QObject(), m_pHtmlParser{ new HtmlParser() }, m_name{ name }, m_pProductsTable{ new QTableWidget() },
	m_pLocalitiesList{ new QListWidget() }, m_pProductUpdater(new ProductsUpdater()) {

	QObject::connect(this, SIGNAL(informationReceived(QString, QString, Product::Status)), m_pProductUpdater, SLOT(onInformationReceived(QString, QString, Product::Status)));
	QObject::connect(m_pHtmlParser, SIGNAL(parsed(QString, bool)), this, SLOT(applyProductInformation(QString, bool)));
	QObject::connect(m_pProductUpdater, SIGNAL(requestParsingInformation(QString, QString)), this, SLOT(requestProductInformation(QString, QString)));

	m_pProductsTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
	m_pProductsTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
}

QString Shop::getName() const {
	return m_name;
}

QTableWidget* Shop::getProductsTable() const {
	return m_pProductsTable;
}


void Shop::addLocality(QString localityID) {
	bool alreadyInList = false;
	for (size_t i = 0; i < m_localities.size(); i++) {
		if (m_localities[i]->getID() == localityID) {
			alreadyInList = true;
			break;
		}
	}
	if (!alreadyInList) {
		size_t column = m_pProductsTable->columnCount();
		m_localities.push_back(new Locality(localityID, column));
		m_pProductsTable->insertColumn(column);
		m_pProductsTable->setHorizontalHeaderItem(column, m_localities[column]->getItem());

		for (size_t row = 0; row < m_productTypes.size(); row++) {
			Product* pProduct = new Product(m_localities[column], m_productTypes[row]);
			m_pProductUpdater->addProduct(pProduct);
			m_products[row].push_back(pProduct);
			m_pProductsTable->setItem(row, column, m_products[row][column]->getItem());
		}
	} else emit message(tr("localityWithTheSameIDisAlreadyInList"), 10000);
}

void Shop::addProductType(QString productID) {
	bool alreadyInList = false;
	for (size_t i = 0; i < m_productTypes.size(); i++) {
		if (m_productTypes[i]->getID() == productID) {
			alreadyInList = true;
			break;
		}
	}
	if (!alreadyInList) {
		size_t row = m_pProductsTable->rowCount();
		m_productTypes.push_back(new ProductType(productID, row));
		m_pProductsTable->insertRow(row);
		m_pProductsTable->setVerticalHeaderItem(row, m_productTypes[row]->getItem());

		m_products.push_back(std::vector< Product* >{  });

		for (size_t column = 0; column < m_localities.size(); column++) {
			Product* pProduct = new Product(m_localities[column], m_productTypes[row]);
			m_pProductUpdater->addProduct(pProduct);
			m_products[row].push_back(pProduct);
			m_pProductsTable->setItem(row, column, m_products[row][column]->getItem());
		}
	} else emit message(tr("productWithTheSameIDisAlreadyInList"), 10000);
}