#include "fb2_parser.h"
#include "xml_text_parser.h"

#include <QFile>
#include <QStringList>

#include <QDebug>


FB2Parser::FB2Parser(Book *book, QStringList &bookText, BookImageTable*& ImageTable, QTreeWidgetItem*& tableOfContents, QHash <QString, QStringList> &notesTable)
{
    Obook = book;    

    bool result;
    QByteArray byteArr = Obook->getFB2BookByteArray(result);

    if (!result)
        return;

    doc = new QTextStream(&byteArr);
    doc->setCodec(Obook->getCodec().toStdString().c_str());


    parseText(bookText);

    delete doc;

    createTableOfContents(bookText, tableOfContents);
    createNotesTable(bookText, notesTable);

    ImageTable = new BookImageTable(Obook);
}


FB2Parser::~FB2Parser()
{

}


void FB2Parser::parseText(QStringList &bookText)
{
    QString coverName;

    QString temp;

    do
    {
        doc->operator >> (temp);

        int coverPos = temp.indexOf("<coverpage>");
        if (coverPos != -1)
        {
            if (temp.indexOf("</coverpage>", coverPos+12) == -1)
            {
                do
                {
                    coverName += temp;
                    doc->operator >> (temp);
                }
                while ((temp.indexOf("</coverpage>") == -1 && temp.indexOf("<body>") == -1) && !doc->atEnd());
                coverName += temp;
            }

            temp = coverName.right(coverName.size() - coverName.indexOf("</coverpage>") - 12);
            coverName = parseTagAttribute(coverName, "href");
            coverName.remove(0,1);
        }
    }
    while(temp.indexOf("<body") == -1 && !doc->atEnd());

    int binEndPos = -1;
    if (temp.indexOf(">", temp.indexOf("<body")) == -1)
    {
        while (!doc->atEnd())
        {
            doc->operator >>(temp);
            binEndPos = temp.indexOf(">") + 1;
            if (binEndPos != 0)
                break;
        }
    }
    else
        binEndPos = temp.indexOf("<body");

    temp = temp.remove(0, binEndPos);
    bookText.append(splitTextToWords(temp));

    QString tagTail;
    QStringList list;
    bool flag = true;

    while(!doc->atEnd() && flag)
    {
        doc->operator >>(temp);

        if (!tagTail.isEmpty())
        {
            temp.prepend(tagTail + " ");
            tagTail.clear();
        }

        int binPos = temp.indexOf("<binary");
        if (binPos != -1)
        {
            list = splitTextToWords(temp.left(binPos));
            flag = false;
        }
        else
            list = splitTextToWords(temp);


        if (QString(list.last()).at(0) == '<' && QString(list.last()).indexOf(">") == -1)
        {
            tagTail = QString(list.last());
            list.removeAt(list.size() -1);
        }

        bookText.append(list);
    }


    if (Obook->haveCoverImage())
        bookText.prepend("<image l:href=\"#" + coverName + "\"/>");
}


void FB2Parser::createTableOfContents(const QStringList &bookText, QTreeWidgetItem*& tableOfContents) const
{
    tableOfContents = new QTreeWidgetItem(0);
    tableOfContents->setWhatsThis(0, QString::number(0));
    QTreeWidgetItem* curItem = tableOfContents;

    for (long long i = 0; i < bookText.size(); i++)
    {
        if (bookText[i] == "<section>")
        {
            QString text;
            long long pos = i;

            for (; i < bookText.size(); i++)
                if (bookText[i] == "<title>" || bookText[i] == "</section>")
                    break;

            if (bookText[i] == "<title>")
            {
                i++;

                for (i = i+1; i < bookText.size(); i++)
                {
                    if (bookText[i][0] != '<')
                        text += bookText[i] + " ";
                    else
                    {
                        if (bookText[i] == "</p>")
                            text += '\n';
                        else if (bookText[i] == "</title>" || bookText[i] == "</section>")
                            break;
                    }
                }


                if (text[text.size() - 1] == '\n')
                    text.remove(text.size() - 1, 1);

                if (!text.isEmpty())
                {
                    QTreeWidgetItem* item = new QTreeWidgetItem(curItem);
                    item->setWhatsThis(0,QString::number(pos));

                    curItem->addChild(item);
                    curItem = item;

                    curItem->setText(0, text);
                }
            }
        }

        if (bookText[i] == "</section>")
        {
            if (curItem->parent() != 0)
                curItem = curItem->parent();
        }

        if (bookText[i] == "</body>")
            break;
    }
}


void FB2Parser::createNotesTable(QStringList &bookText, QHash <QString, QStringList> &notesTable) const
{
    notesTable.clear();

    int notesPos;

    for (notesPos = 0; notesPos < bookText.size(); notesPos++)
    {
        if (bookText[notesPos][0] == "<")
            if (bookText[notesPos].indexOf("<body") != -1)
            {
                if (parseTagAttribute(bookText[notesPos], "name") == "notes")
                    break;
            }
    }

    if (notesPos != 0 && notesPos != bookText.size())
    {
        for (int i = notesPos; i < bookText.size(); i++)
        {
            if (bookText[i] == "</body>")
                break;

            if (bookText[notesPos][0] == "<")
                if (bookText[i].indexOf("<section") != -1)
                {
                    QString noteID = parseTagAttribute(bookText[i], "id");
                    if (!noteID.isEmpty())
                    {
                        QStringList noteText;
                        for (i = i + 1; i < bookText.size(); i++)
                        {
                            if (bookText[i].indexOf("</section") == -1)
                                noteText.append(bookText[i]);
                            else
                                break;
                        }

                        notesTable.insert(noteID, noteText);
                    }
                }
        }
    }
}
