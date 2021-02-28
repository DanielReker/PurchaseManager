#include "Product.h"

Product::Product(Locality* pLocality, ProductType* pProductType)
	: QObject(), m_pItem{ new QTableWidgetItem("Unknown") }, m_pProductType{ pProductType }, m_pLocality{ pLocality }, m_status{ Status::UNKNOWN } {

}

QTableWidgetItem* Product::getItem() const {
	return m_pItem;
}

int Product::getColumn() const {
	return m_pLocality->getColumn();
}

int Product::getRow() const {
	return m_pProductType->getRow();
}

int Product::getLocalityID() const {
	return m_pLocality->getID();
}

int Product::getProductID() const {
	return m_pProductType->getID();
}

void Product::setLocalityName(const QString& localityName) {
	m_pLocality->setLocalityName(localityName);
}

void Product::setStatus(Status status) {
	m_status = status;
	
	//TODO: Remake with icons
	switch (status) {
	case Product::Status::UNKNOWN:
	{
		m_pItem->setText("Unknown");
		break;
	}

	case Product::Status::AVAILABLE:
	{
		m_pItem->setText("Available");
		break;
	}

	case Product::Status::UNAVAILABLE:
	{
		m_pItem->setText("Unavailable");
		break;
	}

	default:
		break;
	}

}

void Product::setProductType(const QString& productType) {
	m_pProductType->setProductType(productType);
}

bool operator==(const Product& p1, const Product& p2) {
	return p1.getColumn() == p2.getColumn() && p1.getRow() == p2.getRow();
}

bool operator!=(const Product& p1, const Product& p2) {
	return !(p1 == p2);
}
