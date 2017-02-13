greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT       += core gui
QT       += xml

TARGET = Reader

TEMPLATE = app

TRANSLATIONS = Languages/en.ts\
               Languages/ru.ts

RC_FILE = app.rc

SOURCES += main.cpp\
    answer_dialog.cpp \
    styles.cpp \
    settings_layout.cpp \
    synchronization_layout.cpp \
    books.cpp \
    settings.cpp \
    book_or_folder.cpp \
    settings_programlayout.cpp \
    settings_profilelayout.cpp \
    settings_readerlayout.cpp \
    book_page.cpp \
    synchronization.cpp \
    genresmap.cpp \
    booktableofcontents.cpp \
    fb2textpaginator.cpp \
    library.cpp \
    library_layout.cpp \
    library_handler.cpp \
    QTabSwitcher.cpp \
    searchwindow.cpp \
    main_window.cpp \
    login_window.cpp \
    reading_window.cpp \
    window_manager.cpp

HEADERS  += \
    answer_dialog.h \
    styles.h \
    settings_layout.h \
    synchronization_layout.h \
    books.h \
    settings.h \
    book_or_folder.h \
    settings_programlayout.h \
    settings_profilelayout.h \
    settings_readerlayout.h \
    book_page.h \
    synchronization.h \
    search_window.h \
    genresmap.h \
    booktableofcontents.h \
    fb2textpaginator.h \
    library.h \
    library_layout.h \
    library_handler.h \
    QTabSwitcher.h \
    reading_window_копия.h \
    main_window.h \
    login_window.h \
    reading_window.h \
    window_manager.h

FORMS    += \
    settings_programlayout.ui \
    settings_profilelayout.ui \
    settings_readerlayout.ui \
    book_page.ui \
    booktableofcontents.ui \
    library_layout.ui \
    main_window.ui \
    login_window.ui \
    reading_window.ui

RESOURCES += \
    images.qrc
