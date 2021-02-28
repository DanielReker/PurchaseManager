#include "ProductType.h"

ProductType::ProductType(QString productID, int row)
	: QObject(), m_productID{ productID }, m_row{ row }, m_productName{ tr("unknownProduct") }, m_pItem{ new QTableWidgetItem(m_productName) } {
}

QTableWidgetItem* ProductType::getItem() {
	return m_pItem;
}

int ProductType::getRow() const {
	return m_row;
}

QString ProductType::getID() const {
	return m_productID;
}

void ProductType::setProductType(const QString& productName) {
	m_productName = productName;
	m_pItem->setText(productName);
}
