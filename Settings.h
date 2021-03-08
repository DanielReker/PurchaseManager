#pragma once

#include <QSettings>

class Settings{
private:
	QSettings* m_settings;
	static Settings* s_instance;

public:
	Settings();
	~Settings();

	static bool contains(const QString& key);
	static QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant());
	static void setValue(const QString& key, const QVariant& value);

	//Default settings, used to handle situation when configuration file doesnt exist or was removed
	static const int s_defaultStatusBarDelaySec = 5;
	static const int s_defaultAutoUpdateIntervalMinutes = 5;
	static const int s_defaultAfterUpdateDelayMsec = 1000;
	static const int s_defaultInternetRequestTimeoutSec = 10;

};

