cmake_minimum_required (VERSION 3.8)

project ("PurchaseManager")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH ${QT_PATH})
find_package(Qt5 COMPONENTS Core Gui Network Widgets Xml REQUIRED)

add_executable (PurchaseManager WIN32
	main.cpp

	PurchaseManager.qrc

	PurchaseManager.cpp
	PurchaseManager.h

	Eldorado.cpp
	Eldorado.h

	HtmlParser.cpp
	HtmlParser.h

	Locality.cpp
	Locality.h

	MVideo.cpp
	MVideo.h

	Product.cpp
	Product.h

	ProductsUpdater.cpp
	ProductsUpdater.h

	ProductType.cpp
	ProductType.h

	Settings.cpp
	Settings.h

	SettingsDialog.cpp
	SettingsDialog.h

	Shop.cpp
	Shop.h

	ShopsState.cpp
	ShopsState.h
)

target_link_libraries(PurchaseManager Qt5::Core)
target_link_libraries(PurchaseManager Qt5::Gui)
target_link_libraries(PurchaseManager Qt5::Network)
target_link_libraries(PurchaseManager Qt5::Widgets)
target_link_libraries(PurchaseManager Qt5::Xml)