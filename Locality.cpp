#include "Locality.h"

Locality::Locality(QString localityID, int column)
	: QObject(), m_localityID{ localityID }, m_column{ column }, m_localityName{ tr("unknownLocality") }, m_pItem{ new QTableWidgetItem(m_localityName) } {
}

QTableWidgetItem* Locality::getItem() {
	return m_pItem;
}

int Locality::getColumn() const {
	return m_column;
}

void Locality::setColumn(int column) {
	m_column = column;
}

QString Locality::getID() const {
	return m_localityID;
}

void Locality::setLocalityName(const QString& localityName) {
	m_localityName = localityName;
	m_pItem->setText(localityName);
}
