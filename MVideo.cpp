#include "MVideo.h"

#include <QRegularExpression>

MVideo::MVideo() : Shop("MVideo") {

}

void MVideo::applyProductInformation(QString htmlPage, bool errorOccured) {
	QString productName = "";
	QString localityName = "";
	Product::Status status = Product::Status::ERROR;

	if (!errorOccured) {
		QRegularExpressionMatch productNameMatch = QRegularExpression(R"('productName': '(.+?)')").match(htmlPage);
		if (productNameMatch.hasMatch()) productName = productNameMatch.captured(1);

		QRegularExpressionMatch localityNameMatch = QRegularExpression(R"('cityName':[^\w\s]'(.+?)')").match(htmlPage);
		if (localityNameMatch.hasMatch()) localityName = localityNameMatch.captured(1);

		QRegularExpressionMatch productAvailabilityMatch = QRegularExpression(R"('productAvailability': '(.+?)')").match(htmlPage);
		if (productAvailabilityMatch.captured(1) == "available") status = Product::Status::AVAILABLE;
		else if (productAvailabilityMatch.captured(1) == "unavailable") status = Product::Status::UNAVAILABLE;
	}
	emit informationReceived(productName, localityName, status);
}

void MVideo::requestProductInformation(QString localityID, QString productID) {
	m_pHtmlParser->parse("https://www.mvideo.ru/products/" + productID + "?cityId=" + localityID);
}
