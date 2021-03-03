#include "HtmlParser.h"



HtmlParser::HtmlParser(QObject* parent)
	: QObject(parent), m_pNetworkAccessManager{ new QNetworkAccessManager(this) }, m_pNetworkReply{ nullptr }, m_oldUrl{  }, m_timeout{ new QTimer(this) } {

	m_pNetworkAccessManager->setRedirectPolicy(QNetworkRequest::RedirectPolicy::ManualRedirectPolicy);

	m_timeout.setSingleShot(true);
	QObject::connect(&m_timeout, SIGNAL(timeout()), this, SLOT(onParsed()));
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

	//TODO: Make HTML parsing timeout configurable
	m_timeout.start(10000);
}

void HtmlParser::onParsed() {
	if (m_timeout.isActive()) {
		m_timeout.stop();
		if (m_pNetworkReply->error() > 0) emit parsed("", true);
		else { // Success, no errors
			QByteArray htmlPageByte = m_pNetworkReply->readAll();
			QString htmlPage = QString(htmlPageByte);
			emit parsed(htmlPage);
		}
	} else {
		m_pNetworkReply->abort();
		emit parsed("", true);
	}

	m_pNetworkReply->deleteLater();
	m_pNetworkReply = nullptr;
}
