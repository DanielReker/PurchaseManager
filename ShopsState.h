#pragma once

#include <QObject>
#include <QDomDocument>

// Singleton
class ShopsState : public QObject {
private:
	QDomDocument m_instance;

	static QDomDocument* s_instance;

	static QString s_currentShopName;

	static void save();
	static bool validate();

	static QDomElement getShop(const QString& shopName);
	static QDomElement getCurrentShop();

public:
	ShopsState(const QStringList& shopsNames);

	static void setCurrentShopName(const QString& name);

	static void addLocality(const QString& localityID, const QString& shopName);
	static void addLocality(const QString& localityID);

	static void addProduct(const QString& productID, const QString& shopName);
	static void addProduct(const QString& productID);

	static void removeLocality(const QString& localityID, const QString& shopName);
	static void removeLocality(const QString& localityID);

	static void removeProduct(const QString& productID, const QString& shopName);
	static void removeProduct(const QString& productID);

	static QStringList getAddedLocalities();
	static QStringList getAddedProducts();
	static QStringList getAllLocalities();

public slots:


signals:


};

