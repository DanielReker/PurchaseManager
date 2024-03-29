#include "PurchaseManager.h"

#include "ShopsState.h"
#include "Settings.h"

#include <QDir>
#include <QIcon>


PurchaseManager::PurchaseManager(QWidget* parent)
    : QMainWindow(parent), m_selectedShopIndex{ -1 }, m_pTranslator{ nullptr }, m_pLangGroup{ nullptr }, m_pSettingsDialog{ new SettingsDialog() } {

    m_ui.setupUi(this);
    

    setupButtons();
    setupLanguageMenu();
    setupSettingsDialog();


    setupShops();
    selectShop(0);
}

void PurchaseManager::addProductType() {
    emit productAdded(m_ui.productIDLine->text());
    m_ui.productIDLine->clear();
}

void PurchaseManager::addLocality() {
    emit localityAdded(m_ui.localityIDLine->text());
    m_ui.localityIDLine->clear();
}

void PurchaseManager::updateAll() {
    m_shops[m_selectedShopIndex]->updateAll();
}

void PurchaseManager::updateSelected() {
    m_shops[m_selectedShopIndex]->updateSelected();
}

void PurchaseManager::removeSelectedLocality() {
    m_shops[m_selectedShopIndex]->removeSelectedLocality();
}

void PurchaseManager::removeSelectedProduct() {
    m_shops[m_selectedShopIndex]->removeSelectedProduct();
}

void PurchaseManager::changeProductsTable(int index) {
    m_ui.productsTables->setCurrentIndex(index);
}

void PurchaseManager::setupShops() {
    m_shops.resize(static_cast<size_t>(Shops::MAX_SHOPS));

    m_shops[static_cast<size_t>(Shops::MVIDEO)] = new MVideo();
    m_shops[static_cast<size_t>(Shops::ELDORADO)] = new Eldorado();

    QStringList shopNames;

    for (size_t i = 0; i < m_shops.size(); i++) {
        QObject::connect(m_shops[i], SIGNAL(message(QString, int)), this, SLOT(onMessage(QString, int)));
        m_ui.shopSelectionComboBox->addItem(m_shops[i]->getName());
        m_ui.productsTables->addWidget(m_shops[i]->getProductsTable());
        shopNames.append(m_shops[i]->getName());
    }

    new ShopsState(shopNames); // Not assigned to any pointer because it's singleton and will be assigned to static pointer in constructor

    // Loading added shops and products from ShopsState.xml file
    for (size_t i = 0; i < m_shops.size(); i++) {
        QStringList addedLocalities = ShopsState::getAddedLocalities(m_shops.at(i)->getName());
        for (size_t j = 0; j < addedLocalities.count(); j++) {
            m_shops.at(i)->addLocality(addedLocalities.at(j));
        }

        QStringList addedProducts = ShopsState::getAddedProducts(m_shops.at(i)->getName());
        for (size_t j = 0; j < addedProducts.count(); j++) {
            m_shops.at(i)->addProductType(addedProducts.at(j));
        }
    }

    QObject::connect(m_ui.shopSelectionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectShop(int)));
}

void PurchaseManager::setupButtons() {
    QObject::connect(m_ui.actionQuit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

    QObject::connect(m_ui.addProductButton, SIGNAL(clicked()), this, SLOT(addProductType()));
    QObject::connect(m_ui.addLocalityButton, SIGNAL(clicked()), this, SLOT(addLocality()));

    QObject::connect(m_ui.updateAllButton, SIGNAL(clicked()), this, SLOT(updateAll()));
    QObject::connect(m_ui.updateSelectedButton, SIGNAL(clicked()), this, SLOT(updateSelected()));

    QObject::connect(m_ui.removeSelectedLocalityButton, SIGNAL(clicked()), this, SLOT(removeSelectedLocality()));
    QObject::connect(m_ui.removeSelectedProductButton, SIGNAL(clicked()), this, SLOT(removeSelectedProduct()));
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

        if (locale == Settings::getValue("language", Settings::s_defaultLanguage)) pAction->trigger();
    }

}

void PurchaseManager::setupSettingsDialog() {
    QObject::connect(m_ui.actionSettings, SIGNAL(triggered()), this, SLOT(openSettingsDialog()));
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
    m_pSettingsDialog->retranslateUI();
    Settings::setValue("language", pAction->data());
}

void PurchaseManager::openSettingsDialog() {
    m_pSettingsDialog->exec();
}

void PurchaseManager::selectShop(int index) {

    //First selection (on initializing), when m_selectedShopIndex is initialized by -1
    if (m_selectedShopIndex >= 0) {
        QObject::disconnect(this, SIGNAL(productAdded(QString)), m_shops[m_selectedShopIndex], SLOT(addProductType(QString)));
        QObject::disconnect(this, SIGNAL(localityAdded(QString)), m_shops[m_selectedShopIndex], SLOT(addLocality(QString)));
    }

    changeProductsTable(index);
    m_selectedShopIndex = index;

    QObject::connect(this, SIGNAL(productAdded(QString)), m_shops[m_selectedShopIndex], SLOT(addProductType(QString)));
    QObject::connect(this, SIGNAL(localityAdded(QString)), m_shops[m_selectedShopIndex], SLOT(addLocality(QString)));

    ShopsState::setCurrentShopName(m_shops[index]->getName());
}