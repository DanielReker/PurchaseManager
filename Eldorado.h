#pragma once

#include "Shop.h"

class Eldorado : public Shop {
private:


public:
	Eldorado();

public slots:
	virtual void requestProductInformation(int localityID, int productID) override;
	virtual void applyProductInformation(QString htmlPage) override;

signals:

};

