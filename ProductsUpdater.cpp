#include "ProductsUpdater.h"

#include <algorithm>

ProductsUpdater::ProductsUpdater(QObject* parent) : m_updateQueue{  }, m_pNextUpdateTimer{ new QTimer(this) } {
	//TODO: Make update timer delay configurable
	m_pNextUpdateTimer->setInterval(1000);

	QObject::connect(m_pNextUpdateTimer, SIGNAL(timeout()), this, SLOT(beginUpdate()));
	m_pNextUpdateTimer->setSingleShot(true);
	m_pNextUpdateTimer->start();
}

void ProductsUpdater::addProduct(Product* pProduct) {
	
	// Checking if product is already in update queue
	if (pProduct != nullptr && std::find(m_updateQueue.begin(), m_updateQueue.end(), pProduct) == m_updateQueue.end()) {
		m_updateQueue.push_back(pProduct);
	}
}

void ProductsUpdater::localityRemoved(int column) {
	for (size_t i = 0; i < m_updateQueue.size(); i++) {
		if (m_updateQueue[i] != nullptr && m_updateQueue[i]->getColumn() == column) {
			m_updateQueue[i] = nullptr;
		}
	}
}

void ProductsUpdater::productTypeRemoved(int row) {
}

void ProductsUpdater::beginUpdate() {
	if (m_updateQueue.size() == 0) m_pNextUpdateTimer->start();
	else if(m_updateQueue.front() != nullptr) emit requestParsingInformation(m_updateQueue.front()->getLocalityID(), m_updateQueue.front()->getProductID());
	else {
		m_updateQueue.pop_front();
		m_pNextUpdateTimer->start();
	}
}

void ProductsUpdater::onInformationReceived(const QString& productName, const QString& localityName, Product::Status status) {
	if (m_updateQueue.front() != nullptr) {
		m_updateQueue.front()->setLocalityName(localityName);
		m_updateQueue.front()->setProductType(productName);
		m_updateQueue.front()->setStatus(status);
	}
	m_updateQueue.pop_front();
	m_pNextUpdateTimer->start();
}
