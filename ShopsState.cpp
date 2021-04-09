#include "ShopsState.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

QDomDocument* ShopsState::s_instance = nullptr;

QString ShopsState::s_currentShopName = "";


ShopsState::ShopsState(const QStringList& shopsNames) : m_instance{ "PurchaseManager" } {
	if (s_instance == nullptr) { // Loading and validating ShopsState.xml file
		QFile shopsState("ShopsState.xml");
		if (shopsState.open(QIODevice::ReadOnly)) m_instance.setContent(&shopsState);

		QDomElement root = m_instance.documentElement();
		if (root.isNull()) {
			root = m_instance.createElement("Shops");
			m_instance.appendChild(root);
		}

		QDomNodeList shopsNodes = root.childNodes();

		for (size_t i = 0; i < shopsNames.size(); i++) {
			QDomElement shop;
			QDomNode shopNode;
			for (size_t j = 0; j < shopsNodes.size(); j++) {
				shopNode = shopsNodes.at(j);
				if (!(shopNode.isElement() && shopNode.toElement().tagName() == "Shop" && shopNode.attributes().contains("name")
					&& shopNode.attributes().namedItem("name").toAttr().value() == shopsNames[i])) {

					shop = QDomElement(); // Setting to null

				} else {
					shop = shopNode.toElement();
					break;
				}

			}
			if (shop.isNull()) {
				shop = m_instance.createElement("Shop");
				root.appendChild(shop);
			}
			if (!shop.hasAttribute("name")) shop.setAttribute("name", shopsNames[i]);
			QDomNodeList shopChildNodes = shop.childNodes();
			bool addedProducts = false, addedLocalities = false, allLocalities = false;
			for (size_t i = 0; i < shopChildNodes.size(); i++) {
				QDomNode shopChildNode = shopChildNodes.at(i);
				if (!shopChildNode.isElement()) continue;
				if (shopChildNode.toElement().tagName() == "AddedProducts") addedProducts = true;
				if (shopChildNode.toElement().tagName() == "AddedLocalities") addedLocalities = true;
				if (shopChildNode.toElement().tagName() == "AllLocalities") allLocalities = true;
			}
			if (!addedProducts) {
				QDomElement addedProducts = m_instance.createElement("AddedProducts");
				shop.appendChild(addedProducts);
			}
			if (!addedLocalities) {
				QDomElement addedLocalities = m_instance.createElement("AddedLocalities");
				shop.appendChild(addedLocalities);
			}
			if (!allLocalities) {
				QDomElement allLocalities = m_instance.createElement("AllLocalities");
				shop.appendChild(allLocalities);
			}
		}
		s_instance = &m_instance;

		//TODO: Remove debug
		setCurrentShopName("0");
		save();
	}
}

void ShopsState::setCurrentShopName(const QString& name) {
	if (s_instance != nullptr) s_currentShopName = name;
}

QDomElement ShopsState::getShop(const QString& shopName) {
	QDomNodeList shops = s_instance->elementsByTagName("Shop");
	QDomElement shop;
	for (size_t i = 0; i < shops.size(); i++) {
		if (shops.at(i).isElement() && shops.at(i).toElement().attribute("name") == shopName) {
			shop = shops.at(i).toElement();
			break;
		}
	}
	return shop;
}

QDomElement ShopsState::getCurrentShop() {
	return getShop(s_currentShopName);
}

QStringList ShopsState::getItemsList(const QString& shopName, const QString& tagName, const QString& itemTagName) {
	QDomElement shop = getShop(shopName);
	QStringList list;
	QDomNodeList elementNodes = shop.elementsByTagName(tagName);
	for (size_t i = 0; i < elementNodes.count(); i++) {
		QDomElement element = elementNodes.at(i).toElement();
		if (!element.isNull()) {
			QDomNodeList itemsList = element.elementsByTagName(itemTagName);
			for (size_t j = 0; j < itemsList.count(); j++) {
				if (itemsList.at(j).isElement()) list.append(itemsList.at(j).toElement().text());
			}
			break;
		}
	}
	return list;
}

void ShopsState::save() {
	if (validate()) {
		QFile file("ShopsState.xml");
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream stream(&file);
		stream << s_instance->toString();
	}
}

bool ShopsState::validate() {
	return s_instance != nullptr && s_currentShopName != "";
}

void ShopsState::addLocality(const QString& localityID, const QString& shopName) {
	QDomElement shop = getShop(shopName);
	if (!shop.isNull()) {
		QDomNodeList addedLocalitiesList = shop.elementsByTagName("AddedLocalities");
		QDomElement addedLocalities = addedLocalitiesList.count() > 0 ? addedLocalitiesList.at(0).toElement() : QDomElement();
		if (!addedLocalities.isNull()) {
			QDomElement locality = s_instance->createElement("Locality");
			addedLocalities.appendChild(locality);
			QDomText text = s_instance->createTextNode(localityID);
			locality.appendChild(text);
		}
	}
	save();
}

void ShopsState::addLocality(const QString& localityID) {
	addLocality(localityID, s_currentShopName);
}

void ShopsState::addProduct(const QString& productID, const QString& shopName) {
	QDomElement shop = getShop(shopName);
	if (!shop.isNull()) {
		QDomNodeList addedProductsList = shop.elementsByTagName("AddedProducts");
		QDomElement addedProducts = addedProductsList.count() > 0 ? addedProductsList.at(0).toElement() : QDomElement();
		if (!addedProducts.isNull()) {
			QDomElement product = s_instance->createElement("Product");
			addedProducts.appendChild(product);
			QDomText text = s_instance->createTextNode(productID);
			product.appendChild(text);
		}
	}
	save();
}

void ShopsState::addProduct(const QString& productID) {
	addProduct(productID, s_currentShopName);
}

void ShopsState::removeLocality(const QString& localityID, const QString& shopName) {

}

void ShopsState::removeLocality(const QString& localityID) {
	removeLocality(localityID, s_currentShopName);
}

void ShopsState::removeProduct(const QString& productID, const QString& shopName) {

}

void ShopsState::removeProduct(const QString& productID) {
	removeProduct(productID, s_currentShopName);
}

QStringList ShopsState::getAddedLocalities(const QString& shopName) {
	return getItemsList(shopName, "AddedLocalities", "Locality");
}

QStringList ShopsState::getAddedProducts(const QString& shopName) {
	return getItemsList(shopName, "AddedProducts", "Product");
}

QStringList ShopsState::getAllLocalities(const QString& shopName) {
	return getItemsList(shopName, "AllLocalities", "Locality");
}




