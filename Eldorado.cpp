#include "Eldorado.h"

//TODO: Remake methods from MVideo's to Eldorado's own, MVideo methods temprorarily put here just in debug purposes

Eldorado::Eldorado() : Shop("Eldorado") {

}

void Eldorado::applyProductInformation(QString htmlPage) {

	emit informationReceived("New eldorado product name", "New eldorado locality name", Product::Status::AVAILABLE);
}

void Eldorado::requestProductInformation(int localityID, int productID) {
	m_pHtmlParser->parse("https://www.mvideo.ru/products/" + QString::number(productID) + "?cityId=CityCZ_" + QString::number(localityID));
}
