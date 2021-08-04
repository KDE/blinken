cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "21")
set(RELEASE_SERVICE_VERSION_MINOR "08")
set(RELEASE_SERVICE_VERSION_MICRO "0")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(blinken VERSION ${RELEASE_SERVICE_VERSION})

# minimal Qt requirement
set(QT_MIN_VERSION "5.10.0")
set(KF5_MIN_VERSION "5.55.0")

# ECM
find_package (ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)

# where to look first for cmake modules
set (CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package (Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core Widgets Svg)

find_package (KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    I18n XmlGui GuiAddons DocTools DBusAddons Crash
)

find_package (Phonon4Qt5 REQUIRED)

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(ECMSetupVersion)

# global include directories
include_directories (${CMAKE_CURRENT_BINARY_DIR})


add_subdirectory( doc )
add_subdirectory( src ) 
add_subdirectory( images ) 
add_subdirectory( icons ) 
add_subdirectory( sounds ) 
add_subdirectory( fonts ) 

########### install files ###############

ki18n_install(po)
if (KF5DocTools_FOUND)
    kdoctools_install(po)
endif()
install( FILES README.packagers  DESTINATION  ${KDE_INSTALL_DATADIR}/blinken/ )
install( FILES org.kde.blinken.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

