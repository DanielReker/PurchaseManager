#pragma once

#include <QObject>
#include <QString>
#include <QTableWidget>

class Shop;

class Locality : public QObject {
	Q_OBJECT

private:
	QString m_localityID;
	int m_column;
	QString m_localityName;

	QTableWidgetItem* m_pItem;

public:
	Locality(QString localityID, int column);

	QTableWidgetItem* getItem();

	int getColumn() const;
	void setColumn(int column);

	QString getID() const;

public slots:
	void setLocalityName(const QString& localityName);

signals:

};

