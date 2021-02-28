#include "MVideo.h"

MVideo::MVideo() : Shop("MVideo") {

}

void MVideo::applyProductInformation(QString htmlPage) {
	QString productName;
	QString localityName;
	Product::Status status = Product::Status::UNKNOWN;

	//TODO: Handling sutiation when product ID is invalid (doesn't exist in shop)
	int localityBegin = htmlPage.indexOf("<span class=\"header-top-line__link-text\">") + QString("<span class=\"header-top-line__link-text\">").length();
	int localityLength = htmlPage.mid(localityBegin).indexOf("</span>");
	localityName = (localityBegin != -1 && localityLength != -1) ? htmlPage.mid(localityBegin, localityLength) : "Unknown locality";

	int productBegin = htmlPage.indexOf("\'productName\'") + QString("\'productName\': \'").length();
	int productLength = htmlPage.mid(productBegin).indexOf("'");
	productName = (productBegin != -1 && productLength != -1) ? htmlPage.mid(productBegin, productLength) : "Unknown product";

	int statusBegin = htmlPage.indexOf("\'productAvailability\'") + QString("\'productAvailability\': \'").length();
	int statusLength = htmlPage.mid(statusBegin).indexOf("'");
	if (statusBegin == -1 || statusLength == -1) status = Product::Status::UNKNOWN;
	else if (htmlPage.mid(statusBegin, statusLength) == QString("unavailable")) status = Product::Status::UNAVAILABLE;
	else if (htmlPage.mid(statusBegin, statusLength) == QString("available")) status = Product::Status::AVAILABLE;

	emit informationReceived(productName, localityName, status);
}

void MVideo::requestProductInformation(int localityID, int productID) {
	m_pHtmlParser->parse("https://www.mvideo.ru/products/" + QString::number(productID) + "?cityId=CityCZ_" + QString::number(localityID));
}
