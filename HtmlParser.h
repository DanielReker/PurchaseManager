#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

// Manual redirect is implemented because default QT's redirection system is not working properly in some cases

class HtmlParser : public QObject {
	Q_OBJECT

private:
	QNetworkAccessManager* m_pNetworkAccessManager;
	QNetworkReply* m_pNetworkReply;

	QUrl m_oldUrl;

public:
	HtmlParser(QObject* parent = Q_NULLPTR);
	void onParsed();

public slots:
	void onFinished();
	void parse(const QString& url);

signals:
	void parsed(QString htmlPage);

};

