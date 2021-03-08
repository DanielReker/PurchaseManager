#pragma once

#include <QDialog>

#include "ui_SettingsDialog.h"

class SettingsDialog : public QDialog {
	Q_OBJECT

private:
	Ui::SettingsDialog m_ui;

public:
	SettingsDialog(QWidget* pParent = nullptr);

public slots:
	void saveSettings();


signals:

};

