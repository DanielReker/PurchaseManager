#include "ProductsUpdater.h"

#include "Settings.h"

#include <algorithm>

void ProductsUpdater::startTimer() {
	m_pNextUpdateTimer->start(Settings::getValue("afterUpdateDelayMsec", Settings::s_defaultAfterUpdateDelayMsec).toInt());
}

ProductsUpdater::ProductsUpdater(QObject* parent) : m_updateQueue{  }, m_pNextUpdateTimer{ new QTimer(this) } {
	QObject::connect(m_pNextUpdateTimer, SIGNAL(timeout()), this, SLOT(beginUpdate()));
	m_pNextUpdateTimer->setSingleShot(true);
	startTimer();
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
	for (size_t i = 0; i < m_updateQueue.size(); i++) {
		if (m_updateQueue[i] != nullptr && m_updateQueue[i]->getRow() == row) {
			m_updateQueue[i] = nullptr;
		}
	}
}

void ProductsUpdater::beginUpdate() {
	if (m_updateQueue.size() == 0) startTimer();
	else if(m_updateQueue.front() != nullptr) emit requestParsingInformation(m_updateQueue.front()->getLocalityID(), m_updateQueue.front()->getProductID());
	else {
		m_updateQueue.pop_front();
		startTimer();
	}
}

void ProductsUpdater::onInformationReceived(const QString& productName, const QString& localityName, Product::Status status) {
	if (m_updateQueue.front() != nullptr) {
		m_updateQueue.front()->setLocalityName(localityName);
		m_updateQueue.front()->setProductType(productName);
		m_updateQueue.front()->setStatus(status);
	}
	m_updateQueue.pop_front();
	startTimer();
}
