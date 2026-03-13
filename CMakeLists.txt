cmake_minimum_required(VERSION 3.16)

project(CafeReservation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
    main.cpp

    database/DatabaseManager.h
    database/DatabaseManager.cpp

    models/User.h
    models/Table.h
    models/Reservation.h

    repositories/IUserRepository.h
    repositories/ITableRepository.h
    repositories/IReservationRepository.h
    repositories/UserRepository.h
    repositories/UserRepository.cpp
    repositories/TableRepository.h
    repositories/TableRepository.cpp
    repositories/ReservationRepository.h
    repositories/ReservationRepository.cpp

    services/AuthService.h
    services/AuthService.cpp
    services/ReservationService.h
    services/ReservationService.cpp
    services/TableService.h
    services/TableService.cpp

    views/LoginView.h
    views/LoginView.cpp
    views/LoginView.ui
    views/MainView.h
    views/MainView.cpp
    views/MainView.ui
    views/ReservationDialog.h
    views/ReservationDialog.cpp
    views/ReservationDialog.ui
    views/AdminView.h
    views/AdminView.cpp
    views/AdminView.ui
    views/TableManagementView.h
    views/TableManagementView.cpp
    views/TableManagementView.ui

    presenters/LoginPresenter.h
    presenters/LoginPresenter.cpp
    presenters/ReservationPresenter.h
    presenters/ReservationPresenter.cpp
    presenters/AdminPresenter.h
    presenters/AdminPresenter.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CafeReservation
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(CafeReservation SHARED ${PROJECT_SOURCES})
    else()
        add_executable(CafeReservation ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(CafeReservation PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

target_include_directories(CafeReservation PRIVATE ${CMAKE_SOURCE_DIR})

set_target_properties(CafeReservation PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CafeReservation
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CafeReservation)
endif()

# Тести
add_subdirectory(tests)
