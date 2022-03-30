cmake_minimum_required (VERSION 3.19)

project ("PurchaseManager")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH ${QT_PATH})
find_package(Qt5 COMPONENTS Core Gui Network Widgets Xml LinguistTools REQUIRED)

file(GLOB_RECURSE SOURCES
	"src/*.cpp"
	"src/*.h"
	"src/*.qrc"
	"src/*.ui"
)

if(WIN32)
	add_executable (PurchaseManager WIN32 ${SOURCES})
elseif(UNIX)
	add_executable (PurchaseManager ${SOURCES})
endif()

target_link_libraries(PurchaseManager Qt5::Core)
target_link_libraries(PurchaseManager Qt5::Gui)
target_link_libraries(PurchaseManager Qt5::Network)
target_link_libraries(PurchaseManager Qt5::Widgets)
target_link_libraries(PurchaseManager Qt5::Xml)

#file(GLOB_RECURSE TRANSLATIONS "translations/*.ts")
#qt5_add_translation(QM_FILES purchasemanager_en.ts)

message("qmFiles: " ${QM_FILES})

set(TS_FILES purchasemanager_en.ts purchasemanager_ru.ts)
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "l10n")
qt5_add_translation(QM_FILES ${TS_FILES})