#include <styles.h>

////////////////////////общее///////////////////////////

void setWindowTopButtonsStyle (QString windowButtonsStyle [], const QString style)
{
    if (style == "red")
    {
        windowButtonsStyle[0] = "#min_button{"
                                "border-image: url(:/window_buttons/min.png);"
                                "border: none;"
                                "background:none;}"
                                "#min_button:hover{"
                                "background-color:rgb(150, 0, 65);}";

        windowButtonsStyle[1] = "#full_size_button{"
                                "border-image: url(:/window_buttons/full_size.png);"
                                "border: none;"
                                "background:none;}"
                                "#full_size_button:hover{"
                                "background-color:rgb(150, 0, 65);}";

        windowButtonsStyle[2] = "#exit_button{"
                                "border-image: url(:/window_buttons/exit.png);"
                                "border: none;"
                                "background:none;}"
                                "#exit_button:hover {"
                                "background-color:rgb(203, 0, 88);}";
    }
}

void setBackgroundWindowColor(QString windowButtonsStyle [], const QString style)
{
    if (style == "red")
        windowButtonsStyle[0] = "#MainWidget {background-color:rgb(150, 0, 60);}";
}


////////////////////////логин///////////////////////////////
void setLoginWindowStyle (QString windowButtonsStyle [], const QString style)
{
    if (style == "red")
    {
        windowButtonsStyle[0] = "#login{"
                                "background-color:rgb(151, 151, 227);"
                                "border: none;}"
                                "#login:hover{"
                                "background-color:rgb(130, 130, 195);}";

        windowButtonsStyle[1] = "QLineEdit{"
                                "background-color:rgb(130, 0, 55);"
                                "color: white;}";

        windowButtonsStyle[2] = "#Recovery{"
                                "color: white;"
                                "margin: 0;"
                                "text-align:right;"
                                "border:none}"
                                "#Recovery:hover{"
                                "color: rgb(151, 151, 227);}";

        windowButtonsStyle[3] = "#Registration{"
                                "text-align:left;"
                                "border:none;"
                                "margin:0;"
                                "color: white;}"
                                "#Registration:hover{"
                                "color: rgb(151, 151, 227);}";

        windowButtonsStyle[4] = "#Remember{color: white;}"
                                "#Remember:hover{color: rgb(151, 151, 227);}"
                                "#Remember::indicator:off{	image: url(:check_no.png);}"
                                "#Remember::indicator:on{	image: url(:check_ok.png);}";

    }
}


///////////////////////////библиотека////////////////////////////////
void setMenusButtonsStyle (QString menuButtonsStyle [], const QString style)
{
    if (style == "red")
    {
        menuButtonsStyle[0] =  " #Library "
                               " {background-color: rgb(162, 0, 70); "
                               " background-image: url(:/menu_buttons/Library_img.png);"
                               " border:none;} "
                               " #Library:hover "
                               " { background-color:rgb(147, 0, 60); } ";

        menuButtonsStyle[1] =  " #Settings "
                               " {background-color: rgb(162, 0, 70); "
                               " background-image: url(:/menu_buttons/Settings_img.png);"
                               " border:none;} "
                               " #Settings:hover "
                               " { background-color:rgb(147, 0, 60); } ";

        menuButtonsStyle[2] =  " #Synchronization "
                               " {background-color: rgb(162, 0, 70); "
                               " background-image: url(:/menu_buttons/Synchronization_img.png);"
                               " border:none;} "
                               " #Synchronization:hover "
                               " { background-color:rgb(147, 0, 60); } ";

        menuButtonsStyle[3] = " #Logout "
                              " {background-color: rgb(162, 0, 70); "
                              " background-image: url(:/menu_buttons/Logout_img.png);"
                              " border:none;} "
                              " #Logout:hover "
                              " { background-color:rgb(147, 0, 60); } ";


        menuButtonsStyle[4] = " #Library "
                              " {background-color: rgb(150, 0, 60); "
                              " background-image: url(:/menu_buttons/Library_img.png);"
                              " border:none;} "
                              " #Library:hover "
                              " { background-color:rgb(147, 0, 60); } ";

        menuButtonsStyle[5] = " #Settings "
                              " {background-color: rgb(150, 0, 60); "
                              " background-image: url(:/menu_buttons/Settings_img.png);"
                              " border:none;} "
                              " #Settings:hover "
                              " { background-color:rgb(147, 0, 60); } ";

        menuButtonsStyle[6] = " #Synchronization "
                              " {background-color: rgb(150, 0, 60); "
                              " background-image: url(:/menu_buttons/Synchronization_img.png);"
                              " border:none;} "
                              " #Synchronization:hover "
                              " { background-color:rgb(147, 0, 60); } ";

        menuButtonsStyle[7] = " #Logout "
                              " {background-color: rgb(150, 0, 60); "
                              " background-image: url(:/menu_buttons/Logout_img.png);"
                              " border:none;} "
                              " #Logout:hover "
                              " { background-color:rgb(147, 0, 60); } ";


        menuButtonsStyle[8] = "QWidget{background-color: rgb(162, 0, 70); border:none; }";
    }
}

void setTabButtonsStyle (QString tabButtonsStyle [], const QString style)
{
    if (style == "red")
    {
        tabButtonsStyle[0] = "QPushButton "
                             "{background-color: rgb(162, 0, 70);"
                             "border:none;"
                             "color:white;}"
                             "QPushButton:hover"
                             "{background-color:rgb(147, 0, 60);}";

        tabButtonsStyle[1] = "QPushButton "
                             "{background-color: rgb(150, 0, 60);"
                             "border:none;"
                             "color:white;}"
                             "QPushButton:hover"
                             "{background-color:rgb(147, 0, 60);}";

        tabButtonsStyle[2] = "QPushButton "
                             "{background-color: rgb(162, 0, 70);"
                             "border:none;"
                             "color:white;"
                             "text-align:right;}"
                             "QPushButton:hover"
                             "{background-color:rgb(147, 0, 60);}";

        tabButtonsStyle[3] = "QComboBox:editable, QComboBox:!editable, QComboBox::drop-down:editable{"
                             "border:none;"
                             "background-color: rgb(162, 0, 70);"
                             "color:white;}"
                             "QComboBox::drop-down{"
                             "max-width:0px;"
                             "border:none;}"
                             "QComboBox:editable:hover, QComboBox:!editable:hover, QComboBox QAbstractItemView::item:selected{"
                             "background-color: rgb(150, 0, 60);}"
                             "QComboBox QAbstractItemView, QComboBox QAbstractItemView::item{"
                             "background-color: rgb(162, 0, 70);"
                             "outline: 0px;"
                             "border:none;"
                             "color:white;"
                             "min-height: 35px;}";
    }
}

void setAnswerDialogStyle (QString windowStyle [], const QString style)
{
    if (style == "red")
    {
        windowStyle[0] = "AnswerDialog{background-color:rgb(130, 0, 55);}";

        windowStyle[1] = "QPushButton"
                         "{background-color:rgb(25, 156, 255);"
                         "border: none;}"
                         "QPushButton:hover"
                         "{background-color:rgb(25, 156, 255);"
                         "border: none;}";

        windowStyle[2] = "QPushButton"
                         "{background-color: rgb(162, 0, 70);"
                         "border: none;}"
                         "QPushButton:hover"
                         "{background-color:rgb(210, 68, 68);"
                         "border: none;}";

        windowStyle[3] = "color: white";
    }
}

void setBookOrFolderStyle (QString buttonStyle [], const QString style)
{
    if (style == "red")
    {
        buttonStyle[0] = " QPushButton "
                         " {background-color: rgb(162, 0, 70); "
                         " border:none;"
                         " color: white;}"
                         " QPushButton:hover "
                         " { background-color:rgb(147, 0, 60); } ";
    }
}

void setLibraryStyle (QString ListViewStyle[], const QString style)
{
    if (style == "red")
    {
        ListViewStyle[0] = " QListView"
                           " {background-color:rgb(150, 0, 60);"
                           " border:none;"
                           " color: white;}"
                           " QListView::item"
                           " {outline: 0px;"
                           " border: none;}"
                           " QListView::item:hover, QListView::item:selected,"
                           " QListView:selected"
                           " {color:white;"
                           " outline: 0px;"
                           " border: none;"
                           " background-color:rgb(100, 0, 30);}";
    }
}

////////////////////////////////////////////////////////////////////
void setBookPageStyle(QString BookPageStyle[], const QString style)
{
    if (style == "red")
    {
        BookPageStyle[0] =  "#BookPage"
                            "{background-color:rgb(162, 0, 70);"
                            "border: 1px solid;"
                            "border-color:black;}";

        BookPageStyle[1] =  "#startReading"
                            "{background-color:rgb(151, 151, 227);"
                            "border: none;"
                            "color:white;}"
                            "#startReading:hover"
                            "{background-color:rgb(130, 130, 195);}";

        BookPageStyle[2] =  "QLabel"
                            "{color:white;}";

        BookPageStyle[3] =  "#exit_button{"
                            "border-image: url(:/window_buttons/exit.png);"
                            "border: none;"
                            "background:none;}"
                            "#exit_button:hover {"
                            "background-color:rgb(203, 0, 88);}";

        BookPageStyle[4] = "QTextEdit"
                           "{background-color:rgb(162, 0, 70);"
                           "border:none;"
                           "color:white;}";

        BookPageStyle[5] = "#deleteBook"
                           "{background-color: rgb(162, 0, 70);"
                           "border: none;}"
                           "#deleteBook:hover"
                           "{background-color:rgb(210, 68, 68);}";

        BookPageStyle[6] = "#leftLine, #leftLine_2"
                           "{background-color: rgb(157, 0, 60);"
                           "color: rgb(157, 0, 60);}";

    }
}
