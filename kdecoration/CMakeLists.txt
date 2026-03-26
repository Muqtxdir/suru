add_definitions(-DTRANSLATION_DOMAIN="suru_kwin_deco")

set(CMAKE_AUTOMOC ON)

set(surudecoration_SRCS
    surudecoration.cpp
    shadowprovider.cpp
    buttonbase.cpp
    buttonfactory.cpp
    closebutton.cpp
    maximizebutton.cpp
    minimizebutton.cpp
    menubutton.cpp
    onalldesktopsbutton.cpp
    keepabovebutton.cpp
    keepbelowbutton.cpp
    shadebutton.cpp
    applicationmenubutton.cpp
    contexthelpbutton.cpp
)

add_library(surudecoration MODULE ${surudecoration_SRCS})

set_target_properties(surudecoration PROPERTIES
    OUTPUT_NAME org.ubuntu.suru
)

target_link_libraries(surudecoration PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::DBus
    KF6::CoreAddons
    KDecoration3::KDecoration
)

install(
    TARGETS surudecoration
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/org.kde.kdecoration3
)
