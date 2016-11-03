#include "answer_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <styles.h>


AnswerDialog::AnswerDialog(int pos_x, int pos_y, QString text)
{
    AnswerDialog::resize(300,90);
    AnswerDialog::move(pos_x, pos_y);

    AnswerDialog::setWindowFlags(Qt::FramelessWindowHint);
    AnswerDialog::setWindowFlags(Qt::Popup);

    AnswerDialog::yes = new QPushButton("&Yes");
    AnswerDialog::yes->setFlat(true);
    AnswerDialog::yes->setMinimumHeight(25);

    AnswerDialog::no = new QPushButton("No");
    AnswerDialog::no->setFlat(true);
    AnswerDialog::no->setMinimumHeight(25);    

    AnswerDialog::message= new QLabel(text);
    AnswerDialog::message->setFont(QFont("MS Shell Dlg 2", 12, QFont::Normal));

    //////////////////////////////////////////////////////////////
    QString styles[8];
    setAnswerDialogStyle (styles, "red");
    AnswerDialog::setStyleSheet(styles[0]);
    AnswerDialog::yes->setStyleSheet(styles[1]);
    AnswerDialog::no->setStyleSheet(styles[2]);
    AnswerDialog::message->setStyleSheet(styles[3]);
    //////////////////////////////////////////////////////////////

    connect(yes, SIGNAL(clicked()), SLOT(accept()));
    connect(no, SIGNAL(clicked()), SLOT(reject()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(yes);
    buttonLayout->addWidget(no);

    QVBoxLayout *windowLayout = new QVBoxLayout;
    windowLayout->addWidget(AnswerDialog::message,0,Qt::AlignCenter);
    windowLayout->addLayout(buttonLayout);


    setLayout(windowLayout);

}