#pragma once

#include "MVideo.h"
#include "Eldorado.h"

#include "HtmlParser.h"

#include <QtWidgets/QMainWindow>
#include <QTableWidget>
#include <QTranslator>
#include <QActionGroup>

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

    std::vector<Shop*> m_shops;

    int m_selectedShopIndex;

    QActionGroup* m_pLangGroup;
    QTranslator* m_pTranslator;

    void setupShops();
    void setupButtons();
    void setupLanguageMenu();

    void changeProductsTable(int index);

public:
    PurchaseManager(QWidget *parent = Q_NULLPTR);

    

public slots:
    void selectShop(int index);

    void addProductType();
    void addLocality();

    void onMessage(const QString& message, int timeout);

    void onLanguageChanged(QAction* pAction);

signals:
    void productAdded(QString productID);
    void localityAdded(QString localityID);
};
