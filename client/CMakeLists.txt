cmake_minimum_required(VERSION 3.16)

project(WebstockClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets NetWork)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        capitalWindow/loginWindow.h
        capitalWindow/loginWindow.cpp
        capitalWindow/loginWindow.cpp
        capitalWindow/signupWindow.cpp
        capitalWindow/signupWindow.h
        dataManager.cpp
        dataManager.h
        dataManager.h
        userManager.cpp
        userManager.h
        capitalWindow/userWindow.cpp
        capitalWindow/userWindow.h
        capitalWindow/userWindow.h
        userUI/userInfo.cpp
        userUI/userInfo.h
        userUI/userInfo.h
        userUI/subUserInfoUI/infoUI.cpp
        userUI/subUserInfoUI/infoUI.h
        userUI/subUserInfoUI/infoUI.h
        userUI/subUserInfoUI/changeInfoUI.cpp
        userUI/subUserInfoUI/changeInfoUI.h
        userUI/subUserInfoUI/changeInfoUI.h
        userUI/subUserInfoUI/changePasswordUI.cpp
        userUI/subUserInfoUI/changePasswordUI.h
        userUI/subUserInfoUI/changePasswordUI.h
        userUI/userMassage.cpp
        userUI/userMassage.h
        userUI/userMassage.h
        myServer.cpp
        myServer.h
        myServer.h
        userUI/subUserMassageUI/friendUI.cpp
        userUI/subUserMassageUI/friendUI.h
        userUI/subUserMassageUI/friendUI.h
        userData/Friend.cpp
        userData/Friend.h
        userData/Friend.h
        userData/Email.cpp
        userData/Email.h
        userData/Email.h
        userData/Email.h
        userUI/UserEmail.cpp
        userUI/UserEmail.h
        userUI/subUserEmailUI/EmailUI.cpp
        userUI/subUserEmailUI/EmailUI.h
        userUI/subUserEmailUI/EmailUI.h
        userData/Futures.cpp
        userData/Futures.h
        userData/Futures.h
        userUI/subUserInfoUI/friendAddUI.cpp
        userUI/subUserInfoUI/friendAddUI.h
        userUI/subUserInfoUI/friendAddUI.h
        userUI/userFutures.cpp
        userUI/userFutures.h
        userUI/userFutures.h
        userUI/subUserFuturesUI/FuturesUI.cpp
        userUI/subUserFuturesUI/FuturesUI.h
        userUI/subUserFuturesUI/FuturesUI.h
        userUI/subUserFuturesUI/FuturesUI.h
        userUI/subUserFuturesUI/configFuturesUI.cpp
        userUI/subUserFuturesUI/configFuturesUI.h
        userUI/subUserFuturesUI/configFuturesUI.h
        userUI/subUserInfoUI/addFuturesUI.cpp
        userUI/subUserInfoUI/addFuturesUI.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WebstockClient
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET WebstockClient APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(WebstockClient SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(WebstockClient
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(WebstockClient PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.WebstockClient)
endif()
set_target_properties(WebstockClient PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS WebstockClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WebstockClient)
endif()
