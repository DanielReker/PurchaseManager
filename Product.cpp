#include "Product.h"

Product::Product(Locality* pLocality, ProductType* pProductType)
	: QObject(), m_pItem{ new QTableWidgetItem() }, m_pProductType{ pProductType }, m_pLocality{ pLocality }, m_status{ Status::UNKNOWN } {

	setStatus(Product::Status::UNKNOWN);

	// Making item's text not editable
	m_pItem->setFlags(m_pItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
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

QString Product::getLocalityID() const {
	return m_pLocality->getID();
}

QString Product::getProductID() const {
	return m_pProductType->getID();
}

void Product::setLocalityName(const QString& localityName) {
	m_pLocality->setLocalityName(localityName);
}

void Product::setStatus(Status status) {
	m_status = status;
	
	switch (status) {
	case Product::Status::UNKNOWN:
	{
		m_pItem->setText(tr("unknown"));
		m_pItem->setIcon(QIcon(":/status/unknown"));
		break;
	}

	case Product::Status::AVAILABLE:
	{
		m_pItem->setText(tr("available"));
		m_pItem->setIcon(QIcon(":/status/available"));
		break;
	}

	case Product::Status::UNAVAILABLE:
	{
		m_pItem->setText(tr("unavailable"));
		m_pItem->setIcon(QIcon(":/status/unavailable"));
		break;
	}
	case Product::Status::ERROR:
	{
		m_pItem->setText(tr("error"));
		m_pItem->setIcon(QIcon(":/status/error"));
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
