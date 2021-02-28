#pragma once

#include "Shop.h"

class MVideo : public Shop {
private:


public:
	MVideo();

public slots:
	virtual void requestProductInformation(int localityID, int productID) override;
	virtual void applyProductInformation(QString htmlPage) override;

signals:

};

