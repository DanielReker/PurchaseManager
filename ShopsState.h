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

	static QStringList getItemsList(const QString& shopName, const QString& tagName, const QString& itemTagName);
	static void removeItem(const QString& shopName, const QString& tagName, const QString& itemText);

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

	static QStringList getAddedLocalities(const QString& shopName);
	static QStringList getAddedProducts(const QString& shopName);
	static QStringList getAllLocalities(const QString& shopName);

public slots:


signals:


};

