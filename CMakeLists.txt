cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "25")
set(RELEASE_SERVICE_VERSION_MINOR "04")
set(RELEASE_SERVICE_VERSION_MICRO "1")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(blinken VERSION ${RELEASE_SERVICE_VERSION})

option(QML_VERSION_FOR_DESKTOP "enable QML version for desktop" OFF)

if(ANDROID OR QML_VERSION_FOR_DESKTOP)
    set(QML_VERSION ON)
else()
    set(QML_VERSION OFF)
endif()

# minimal Qt requirement
set(QT_MIN_VERSION "6.5.0")
set(KF_MIN_VERSION "6.0.0")

# ECM
find_package (ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

# where to look first for cmake modules
set (CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMAddAppIcon)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(FeatureSummary)
include(ECMDeprecationSettings)

if(ANDROID)
    include(ECMAddAndroidApk)
endif()

if(QML_VERSION)
    add_compile_definitions(QML_VERSION)
    include(ECMQmlModule)

    find_package (Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS 
        Core Quick Gui Qml Multimedia
    )
    find_package (KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
        CoreAddons I18n GuiAddons Kirigami 
    )
    find_package(KF6Config ${KF_MIN_VERSION} REQUIRED)
    find_package(KF6ConfigWidgets ${KF_MIN_VERSION} REQUIRED)
    find_package(KF6KirigamiAddons 1.0 REQUIRED)

else()

    find_package (Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED 
        Core Widgets Svg Multimedia Qml
    )
    find_package (KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
        CoreAddons I18n XmlGui GuiAddons DocTools DBusAddons Crash
    )
    add_subdirectory( doc )
endif()

# global include directories
include_directories (${CMAKE_CURRENT_BINARY_DIR})
ecm_set_disabled_deprecation_versions(QT 6.8.0  KF 6.5.0)


add_subdirectory( src )
add_subdirectory( images )
add_subdirectory( sounds )
add_subdirectory( icons )
add_subdirectory( fonts )

########### install files ###############

ki18n_install(po)

if (KF6DocTools_FOUND)
    kdoctools_install(po)
endif()

install( FILES README.packagers  DESTINATION  ${KDE_INSTALL_DATADIR}/blinken/ )
install( FILES org.kde.blinken.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
