#include "ParsedInformation.h"

ParsedInformation::ParsedInformation(const QString& localityName, int localityID, const QString& productName, int productID, bool productAvailable)
	: m_localityName{ localityName }, m_localityID{ localityID }, m_productName{ productName }, m_productID{ productID }, m_productAvailable{ productAvailable } {

}
