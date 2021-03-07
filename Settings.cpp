#include "Settings.h"

#include <QDir>
#include <QDebug>

Settings* Settings::s_instance = nullptr;

Settings::Settings() : m_settings{ new QSettings(QDir::currentPath() + "/config.ini", QSettings::Format::IniFormat) } {
	if (s_instance == nullptr) s_instance = this;
}

Settings::~Settings() {
	
}

bool Settings::contains(const QString& key) {
	return s_instance->m_settings->contains(key);
}

QVariant Settings::getValue(const QString& key, const QVariant& defaultValue) {
	return s_instance->m_settings->value(key, defaultValue);
}

void Settings::setValue(const QString& key, const QVariant& value) {
	s_instance->m_settings->setValue(key, value);
}
