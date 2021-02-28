#pragma once

#include "Shop.h"

class Eldorado : public Shop {
private:


public:
	Eldorado();

public slots:
	virtual void requestProductInformation(QString localityID, QString productID) override;
	virtual void applyProductInformation(QString htmlPage) override;

signals:

};

