#pragma once

#include "MVideo.h"
#include "Eldorado.h"

#include "HtmlParser.h"

#include <QtWidgets/QMainWindow>
#include <QTableWidget>
#include <QTranslator>

#include <vector>

#include "ui_PurchaseManager.h"

class PurchaseManager : public QMainWindow {
    Q_OBJECT

public:
    enum class Shops {
        MVIDEO,
        ELDORADO,
        MAX_SHOPS
    };

private:
    Ui::PurchaseManagerClass m_ui;
    QTranslator* m_pTranslator;

    std::vector<Shop*> m_shops;

    int m_selectedShopIndex;


    void setupShops();
    void setupButtons();

    void changeProductsTable(int index);

public:
    PurchaseManager(QWidget *parent = Q_NULLPTR);

    

public slots:
    void selectShop(int index);

    void addProductType();
    void addLocality();

    void onMessage(const QString& message, int timeout);

signals:
    void productAdded(int productID);
    void localityAdded(int localityID);
};
