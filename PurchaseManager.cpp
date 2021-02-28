#include "PurchaseManager.h"


PurchaseManager::PurchaseManager(QWidget* parent) : QMainWindow(parent), m_selectedShopIndex{ -1 }, m_pTranslator{ new QTranslator() } {
    //TODO: Make translations
    m_pTranslator->load(":/Translations/purchasemanager_ru.qm");
    QApplication::installTranslator(m_pTranslator);


    m_ui.setupUi(this);
    setupShops();
    setupButtons();
    selectShop(0);


}

void PurchaseManager::addProductType() {
    bool parsedSuccessfully;
    int productID = m_ui.productIDLine->text().toInt(&parsedSuccessfully);
    if (parsedSuccessfully) emit productAdded(productID);
    //TODO: Make status bar delay configurable
    else onMessage("Invalid product identifier", 10000);
    m_ui.productIDLine->clear();
}

void PurchaseManager::addLocality() {
    bool parsedSuccessfully;
    int localityID = m_ui.localityIDLine->text().toInt(&parsedSuccessfully);
    if (parsedSuccessfully) emit localityAdded(localityID);
    //TODO: Make status bar delay configurable
    else onMessage("Invalid locality identifier", 10000);
    m_ui.localityIDLine->clear();
}

void PurchaseManager::changeProductsTable(int index) {
    m_ui.productsTables->setCurrentIndex(index);
}

void PurchaseManager::setupShops() {
    m_shops.resize(static_cast<size_t>(Shops::MAX_SHOPS));

    m_shops[static_cast<size_t>(Shops::MVIDEO)] = new MVideo();
    m_shops[static_cast<size_t>(Shops::ELDORADO)] = new Eldorado();

    for (size_t i = 0; i < m_shops.size(); i++) {
        QObject::connect(m_shops[i], SIGNAL(message(QString, int)), this, SLOT(onMessage(QString, int)));
        m_ui.shopSelectionComboBox->addItem(m_shops[i]->getName());
        m_ui.productsTables->addWidget(m_shops[i]->getProductsTable());
    }

    QObject::connect(m_ui.shopSelectionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectShop(int)));
}

void PurchaseManager::setupButtons() {
    QObject::connect(m_ui.actionQuit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

    QObject::connect(m_ui.addProductButton, SIGNAL(clicked()), this, SLOT(addProductType()));
    QObject::connect(m_ui.addLocalityButton, SIGNAL(clicked()), this, SLOT(addLocality()));
}

void PurchaseManager::onMessage(const QString& message, int timeout) {
    m_ui.statusBar->showMessage(message, timeout);
}

void PurchaseManager::selectShop(int index) {

    //First selection (on initializing), when m_selectedShopIndex is initialized by -1
    if (m_selectedShopIndex >= 0) {
        QObject::disconnect(this, SIGNAL(productAdded(int)), m_shops[m_selectedShopIndex], SLOT(addProductType(int)));
        QObject::disconnect(this, SIGNAL(localityAdded(int)), m_shops[m_selectedShopIndex], SLOT(addLocality(int)));
    }

    changeProductsTable(index);
    m_selectedShopIndex = index;

    QObject::connect(this, SIGNAL(productAdded(int)), m_shops[m_selectedShopIndex], SLOT(addProductType(int)));
    QObject::connect(this, SIGNAL(localityAdded(int)), m_shops[m_selectedShopIndex], SLOT(addLocality(int)));
}