cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/wbmdo/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")


set(OPENSSL_ROOT_DIR "C:/Users/wbmdo/vcpkg/installed/x64-mingw-dynamic")
set(OPENSSL_USE_STATIC_LIBS FALSE)
set(SQLITE3_ROOT_DIR "C:/Users/wbmdo/vcpkg/installed/x64-mingw-dynamic")


project(Password_proj VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)

set(PROJECT_SOURCES
        main.cpp
        src/vault.cpp
        src/vault.h
        src/hash.cpp
        src/hash.h
        src/encdec.cpp
        src/encdec.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Password_proj
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        # newmaster.h newmaster.cpp newmaster.ui
        # login.h login.cpp login.ui
        # mainwindow.h mainwindow.cpp mainwindow.ui
        # addpassword.h addpassword.cpp addpassword.ui
        resources.qrc
        # confirmdelete.h confirmdelete.cpp confirmdelete.ui

        fonts/IBMPlexMono-SemiBold.ttf
        widgets/addpassword.cpp widgets/addpassword.h widgets/addpassword.ui widgets/confirmdelete.cpp widgets/confirmdelete.h widgets/confirmdelete.ui widgets/createmaster.cpp widgets/createmaster.h widgets/denypass.cpp widgets/denypass.h widgets/denypass.ui widgets/login.cpp widgets/login.h widgets/login.ui widgets/mainwindow.cpp widgets/mainwindow.h widgets/mainwindow.ui widgets/newmaster.cpp widgets/newmaster.h widgets/newmaster.ui widgets/nopass.cpp widgets/nopass.h widgets/nopass.ui
        #denypass.h denypass.cpp denypass.ui

    )
#fonts
target_sources(Password_proj PRIVATE

)
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Password_proj APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Password_proj SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Password_proj
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(Password_proj PRIVATE
    "${SQLITE3_ROOT_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(Password_proj PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    "C:/Users/wbmdo/vcpkg/installed/x64-mingw-dynamic/lib/libsqlite3.dll.a"
)
target_link_libraries(Password_proj PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Password_proj PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(Password_proj PRIVATE Qt6::Sql)



# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Password_proj)
endif()
set_target_properties(Password_proj PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Password_proj
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Password_proj)
endif()
