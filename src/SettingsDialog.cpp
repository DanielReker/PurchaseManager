#include "SettingsDialog.h"

#include "Settings.h"

#include <QDebug>

SettingsDialog::SettingsDialog(QWidget* pParent) : QDialog(pParent) {
	m_ui.setupUi(this);
	setWindowIcon(QIcon(":/menu/settings"));

	m_ui.statusBarDelaySec->setValue(Settings::getValue("statusBarDelaySec", Settings::s_defaultStatusBarDelaySec).toInt());
	m_ui.autoUpdateIntervalMinutes->setValue(Settings::getValue("autoUpdateIntervalMinutes", Settings::s_defaultAutoUpdateIntervalMinutes).toInt());
	m_ui.afterUpdateDelayMsec->setValue(Settings::getValue("afterUpdateDelayMsec", Settings::s_defaultAfterUpdateDelayMsec).toInt());
	m_ui.internetRequestTimeoutSec->setValue(Settings::getValue("internetRequestTimeoutSec", Settings::s_defaultInternetRequestTimeoutSec).toInt());

	QObject::connect(this, SIGNAL(accepted()), this, SLOT(saveSettings()));
	QObject::connect(this, SIGNAL(rejected()), this, SLOT(saveSettings()));
}

void SettingsDialog::retranslateUI() {
	m_ui.retranslateUi(this);
}

void SettingsDialog::saveSettings() {
	Settings::setValue("statusBarDelaySec", m_ui.statusBarDelaySec->value());
	Settings::setValue("autoUpdateIntervalMinutes", m_ui.autoUpdateIntervalMinutes->value());
	Settings::setValue("afterUpdateDelayMsec", m_ui.afterUpdateDelayMsec->value());
	Settings::setValue("internetRequestTimeoutSec", m_ui.internetRequestTimeoutSec->value());
}
