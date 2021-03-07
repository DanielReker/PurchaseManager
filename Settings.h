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
	

};

