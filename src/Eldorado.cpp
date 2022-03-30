#include "Eldorado.h"

//TODO: Remake methods from MVideo's to Eldorado's own, MVideo methods were temprorarily put here just in debug purposes

Eldorado::Eldorado() : Shop("Eldorado") {

}

void Eldorado::applyProductInformation(QString htmlPage, bool errorOccured) {

	emit informationReceived("New eldorado product name", "New eldorado locality name", Product::Status::AVAILABLE);
}

void Eldorado::requestProductInformation(QString localityID, QString productID) {
	m_pHtmlParser->parse("https://www.mvideo.ru/products/" + productID + "?cityId=CityCZ_" + localityID);
}
