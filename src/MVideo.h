#pragma once

#include "Shop.h"

#include <QString>

class MVideo : public Shop {
private:


public:
	MVideo();

public slots:
	virtual void requestProductInformation(QString localityID, QString productID) override;
	virtual void applyProductInformation(QString htmlPage, bool errorOccured) override;

signals:

};

