cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# KDE Application Version, managed by release script
set(KDE_APPLICATIONS_VERSION_MAJOR "19")
set(KDE_APPLICATIONS_VERSION_MINOR "12")
set(KDE_APPLICATIONS_VERSION_MICRO "1")
set(KDE_APPLICATIONS_VERSION "${KDE_APPLICATIONS_VERSION_MAJOR}.${KDE_APPLICATIONS_VERSION_MINOR}.${KDE_APPLICATIONS_VERSION_MICRO}")

project(blinken VERSION ${KDE_APPLICATIONS_VERSION})

# minimal Qt requirement
set(QT_MIN_VERSION "5.9.0")
set(KF5_MIN_VERSION "5.46.0")

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
if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
   add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
   add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x060000)
endif()

add_subdirectory( doc )
add_subdirectory( src ) 
add_subdirectory( images ) 
add_subdirectory( icons ) 
add_subdirectory( sounds ) 
add_subdirectory( fonts ) 

########### install files ###############

install( FILES README.packagers  DESTINATION  ${KDE_INSTALL_DATADIR}/blinken/ )
install( FILES org.kde.blinken.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

