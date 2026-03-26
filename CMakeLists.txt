cmake_minimum_required(VERSION 3.16)
project(suru)
set(PROJECT_VERSION "0.1.0")

set(KF6_MIN_VERSION "6.22.0")
set(QT_MIN_VERSION "6.10.0")

include(FeatureSummary)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_NO_CREATE_VERSIONLESS_TARGETS ON)
set(QT_NO_CREATE_VERSIONLESS_FUNCTIONS ON)

include(KDEInstallDirs6)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS Gui DBus Widgets)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    Config
    I18n
    KCMUtils
)

find_package(KDecoration3 REQUIRED)

add_subdirectory(kdecoration)
add_subdirectory(colors)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
