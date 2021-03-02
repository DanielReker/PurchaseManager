#include "MVideo.h"

MVideo::MVideo() : Shop("MVideo") {

}

void MVideo::applyProductInformation(QString htmlPage, bool errorOccured) {
	//TODO: Rewrite parsing product information better

	QString productName = "";
	QString localityName = "";
	Product::Status status = Product::Status::UNKNOWN;
	
	if (errorOccured) status = Product::Status::ERROR;
	else {
		int localityBegin = htmlPage.indexOf("<span class=\"header-top-line__link-text\">");
		int productBegin = htmlPage.indexOf("\'productName\'");
		int statusBegin = htmlPage.indexOf("\'productAvailability\'");
		if (localityBegin == -1 || productBegin == -1 || statusBegin == -1) status = Product::Status::ERROR;
		else {
			localityBegin += QString("<span class=\"header-top-line__link-text\">").length();
			productBegin += QString("\'productName\': \'").length();
			statusBegin += QString("\'productAvailability\': \'").length();

			int localityLength = htmlPage.mid(localityBegin).indexOf("</span>");
			localityName = htmlPage.mid(localityBegin, localityLength);

			int productLength = htmlPage.mid(productBegin).indexOf("'");
			productName = htmlPage.mid(productBegin, productLength);

			int statusLength = htmlPage.mid(statusBegin).indexOf("'");
			if (htmlPage.mid(statusBegin, statusLength) == QString("unavailable")) status = Product::Status::UNAVAILABLE;
			else if (htmlPage.mid(statusBegin, statusLength) == QString("available")) status = Product::Status::AVAILABLE;
			else status = Product::Status::ERROR;
		}
	}

	emit informationReceived(productName, localityName, status);
}

void MVideo::requestProductInformation(QString localityID, QString productID) {
	m_pHtmlParser->parse("https://www.mvideo.ru/products/" + productID + "?cityId=" + localityID);
}
