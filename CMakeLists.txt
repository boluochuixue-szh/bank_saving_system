cmake_minimum_required(VERSION 3.19)
project(BankManagement LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(BankManagement
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    customer.h customer.cpp
    transaction.h transaction.cpp
    banksystem.h banksystem.cpp
    userwindow.h userwindow.cpp userwindow.ui
    adminwindow.h adminwindow.cpp adminwindow.ui
    customertablemodel.h customertablemodel.cpp
)

target_link_libraries(BankManagement
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS BankManagement
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET BankManagement
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
