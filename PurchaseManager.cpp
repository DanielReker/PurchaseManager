#include "PurchaseManager.h"

#include <QDir>
#include <QIcon>


PurchaseManager::PurchaseManager(QWidget* parent)
    : QMainWindow(parent), m_selectedShopIndex{ -1 }, m_pTranslator{ nullptr }, m_pLangGroup{ nullptr } {


    m_ui.setupUi(this);
    setupShops();
    setupButtons();
    selectShop(0);
    setupLanguageMenu();

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

void PurchaseManager::setupLanguageMenu() {
    m_pLangGroup = new QActionGroup(m_ui.menuLanguage);
    m_pLangGroup->setExclusive(false);
    QObject::connect(m_pLangGroup, SIGNAL(triggered(QAction*)), this, SLOT(onLanguageChanged(QAction*)));

    QStringList fileNames = QDir(":/translations").entryList(QStringList("purchasemanager_*.qm"));
    for (size_t i = 0; i < fileNames.size(); i++) {
        QString locale = fileNames[i];
        locale.truncate(locale.lastIndexOf('.'));
        locale.remove(0, locale.lastIndexOf('_') + 1); // i.e. "en", "ru", etc

        QString lang = QLocale::languageToString(QLocale(locale).language());
        QIcon icon(QString(":/flags/%1").arg(locale));
        QAction* pAction = new QAction(icon, lang, this);
        pAction->setData(locale);
        m_ui.menuLanguage->addAction(pAction);
        m_pLangGroup->addAction(pAction);

        //TODO: Make startup language configurable
        if (locale == QString("en")) pAction->trigger();
    }

}

void PurchaseManager::onMessage(const QString& message, int timeout) {
    m_ui.statusBar->showMessage(message, timeout);
}

void PurchaseManager::onLanguageChanged(QAction* pAction) {
    // In PurchaseManager's ctor m_pTranslator is initialized by nullptr so we have to allocate it with new; otherwise old translator should be removed
    if (m_pTranslator == nullptr) m_pTranslator = new QTranslator(this);
    else QApplication::removeTranslator(m_pTranslator);

    //TODO: Handling situation if loading file is not found
    m_pTranslator->load(QString(":/translations/purchasemanager_%1.qm").arg(pAction->data().toString()));
    QApplication::installTranslator(m_pTranslator);
    m_ui.retranslateUi(this);
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