#include "HtmlParser.h"

#include "Settings.h"


HtmlParser::HtmlParser(QObject* parent)
	: QObject(parent), m_pNetworkAccessManager{ new QNetworkAccessManager(this) }, m_pNetworkReply{ nullptr },
	m_oldUrl{  }, m_timeout{ new QTimer(this) }, m_networkRequest{  } {

	m_pNetworkAccessManager->setRedirectPolicy(QNetworkRequest::RedirectPolicy::ManualRedirectPolicy);
	m_networkRequest.setRawHeader(QByteArray("user-agent"), QByteArray("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.190 Safari/537.36"));

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
	m_networkRequest.setUrl(QUrl(url));
	m_pNetworkReply = m_pNetworkAccessManager->get(m_networkRequest);
	QObject::connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(onFinished()));
	m_oldUrl = url;

	m_timeout.start(1000 * Settings::getValue("internetRequestTimeoutSec", Settings::s_defaultInternetRequestTimeoutSec).toInt());
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
