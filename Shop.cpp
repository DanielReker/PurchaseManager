#include "Shop.h"

#include "Settings.h"
#include "ShopsState.h"

#include <QApplication>


// Returns selected item or nullptr if no items selected
Product* Shop::getSelectedProduct() {
	Product* result = nullptr;
	QList< QTableWidgetItem* > selectedItems = m_pProductsTable->selectedItems();
	// Using 0th item only because table cells were made single-selectable so there cannot be 1 or more items
	if (selectedItems.size() > 0) result = m_products[selectedItems[0]->row()][selectedItems[0]->column()];
	return result;
}

Shop::Shop(const QString& name)
	: QObject(), m_pHtmlParser{ new HtmlParser(this) }, m_name{ name }, m_pProductsTable{ new QTableWidget() },
	m_pLocalitiesList{ new QListWidget() }, m_pProductUpdater(new ProductsUpdater()), m_updateTimer{ this } {

	QObject::connect(this, SIGNAL(informationReceived(QString, QString, Product::Status)), m_pProductUpdater, SLOT(onInformationReceived(QString, QString, Product::Status)));
	QObject::connect(m_pHtmlParser, SIGNAL(parsed(QString, bool)), this, SLOT(applyProductInformation(QString, bool)));
	QObject::connect(m_pProductUpdater, SIGNAL(requestParsingInformation(QString, QString)), this, SLOT(requestProductInformation(QString, QString)));

	m_pProductsTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
	m_pProductsTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

	QObject::connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(updateAll()));
	m_updateTimer.start(60 * 1000 * Settings::getValue("autoUpdateIntervalMinutes", Settings::s_defaultAutoUpdateIntervalMinutes).toInt());
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
		ShopsState::addLocality(localityID);
		
	} else emit message(tr("localityWithTheSameIDisAlreadyInList"), 1000 * Settings::getValue("statusBarDelaySec", Settings::s_defaultStatusBarDelaySec).toInt());
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

		ShopsState::addProduct(productID);
	} else emit message(tr("productWithTheSameIDisAlreadyInList"), 1000 * Settings::getValue("statusBarDelaySec", Settings::s_defaultStatusBarDelaySec).toInt());
}

void Shop::updateAll() {
	for (size_t i = 0; i < m_products.size(); i++) {
		for (size_t j = 0; j < m_products[i].size(); j++) {
			m_pProductUpdater->addProduct(m_products[i][j]);
		}
	}
}

void Shop::updateSelected() {
	Product* selectedProduct = getSelectedProduct();
	if (selectedProduct != nullptr) m_pProductUpdater->addProduct(selectedProduct);
	else message(tr("itemIsNotSelected"), 1000 * Settings::getValue("statusBarDelaySec", Settings::s_defaultStatusBarDelaySec).toInt());
}

void Shop::removeSelectedLocality() {
	Product* selectedProduct = getSelectedProduct();
	if (selectedProduct != nullptr) {
		int column = selectedProduct->getColumn();
		m_pProductUpdater->localityRemoved(column);
		m_pProductsTable->removeColumn(column);
		for (size_t i = 0; i < m_products.size(); i++) {
			delete m_products[i][column];
			m_products[i].erase(m_products[i].begin() + column);
			
		}
		delete m_localities[column];
		m_localities.erase(m_localities.begin() + column);
		for (size_t i = 0; i < m_localities.size(); i++) m_localities[i]->setColumn(i);
	} else message(tr("itemIsNotSelected"), 1000 * Settings::getValue("statusBarDelaySec", Settings::s_defaultStatusBarDelaySec).toInt());
}

void Shop::removeSelectedProduct() {
	Product* selectedProduct = getSelectedProduct();
	if (selectedProduct != nullptr) {
		int row = selectedProduct->getRow();
		m_pProductUpdater->productTypeRemoved(row);
		m_pProductsTable->removeRow(row);
		for (size_t i = 0; i < m_products[row].size(); i++) {
			delete m_products[row][i];
		}
		m_products.erase(m_products.begin() + row);
		delete m_productTypes[row];
		m_productTypes.erase(m_productTypes.begin() + row);
		for (size_t i = 0; i < m_productTypes.size(); i++) m_productTypes[i]->setRow(i);
	} else message(tr("itemIsNotSelected"), 1000 * Settings::getValue("statusBarDelaySec", Settings::s_defaultStatusBarDelaySec).toInt());
}
