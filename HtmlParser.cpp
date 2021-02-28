#include "HtmlParser.h"

HtmlParser::HtmlParser(QObject* parent)
	: QObject(parent), m_pNetworkAccessManager{ new QNetworkAccessManager(this) }, m_pNetworkReply{ nullptr }, m_oldUrl{  } {

	m_pNetworkAccessManager->setRedirectPolicy(QNetworkRequest::RedirectPolicy::ManualRedirectPolicy);


}

void HtmlParser::onFinished() {
	QVariant newUrlVar = m_pNetworkReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (newUrlVar.isValid()) {
		QUrl newUrl = newUrlVar.value<QUrl>();
		if (newUrl.host() == QString("")) newUrl = QUrl(QString("https://") + m_oldUrl.host() + newUrl.toString());
		m_pNetworkReply->deleteLater();
		m_pNetworkReply = nullptr;
		m_oldUrl = newUrl;
		parse(newUrl.toString());
	} else onParsed();
}

void HtmlParser::parse(const QString& url) {
	m_pNetworkReply = m_pNetworkAccessManager->get(QNetworkRequest(QUrl(url)));
	QObject::connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(onFinished()));
	m_oldUrl = url;
}

void HtmlParser::onParsed() {
	QByteArray htmlPageByte = m_pNetworkReply->readAll();
	QString htmlPage = QString(htmlPageByte);
	emit parsed(htmlPage);
	m_pNetworkReply->deleteLater();
	m_pNetworkReply = nullptr;
}
