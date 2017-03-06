#include "fb2textpaginator.h"
#include <QFont>
#include <QFile>
#include <QDebug>
#include <QUrl>
#include <QString>
#include <QFontMetrics>


QStringList FB2TextPaginator::splitTextToWords(QString temp)
{
    QStringList tempList;
    tempList.append(temp);

    for (int i = 0; i < tempList.size(); i++)
    {
        int pos = tempList[i].indexOf("<");
        if (pos > 0)
        {
            tempList.append(tempList[i].right(tempList[i].size() - pos));
            tempList[i] = tempList[i].left(pos);
            i++;
        }

        if (tempList[i][0] == '<')
        {
            pos = tempList[i].indexOf(">");
            if (pos == -1)
            {
                do
                {
                    doc->operator>>(temp);
                    tempList[i].append(" " + temp);
                }
                while (temp.indexOf(">") == -1);
            }

            if (pos != tempList[i].size() - 1)
            {
                tempList.append(tempList[i].right(tempList[i].size() - pos - 1));
                tempList[i] = tempList[i].left(pos + 1);
            }
        }
    }

    for (int i = 0; i < tempList.size(); i++)
    {
        if (tempList[i][0] != '<')
        {
            int pos = tempList[i].indexOf("-");
            if (pos > 0 && pos != tempList[i].size() - 1)
            {
                QStringList newTemp = tempList[i].split("-");
                if (newTemp.size() > 1)
                {
                    for (int j = 0; j < newTemp.size() - 1; j++)
                        newTemp[j] += "-";

                    tempList.removeAt(i);

                    for (int j = 0; j < newTemp.size(); j++)
                        tempList.insert(i++, newTemp[j]);
                }
            }
        }
        else
        {
            if (tempList[i] == "<title>")
                tempList[i] = "<TitleText>";
            if (tempList[i] == "</title>")
                tempList[i] = "</TitleText>";
            if (tempList[i] == "<empty-line/>")
                tempList[i] = "<br/>";
            if (tempList[i] == "<subtitle>")
            {
                tempList[i] = "<p>";
                tempList.insert(++i, "<epigraph>");
            }
            if (tempList[i] == "</subtitle>")
            {
                tempList[i] = "</epigraph>";
                tempList.insert(++i, "</p>");
            }
        }
    }
    return tempList;
}


void FB2TextPaginator::parseBookText()
{
    QFile bookFile(book->File);
    if (bookFile.open(QIODevice::ReadOnly))
    {
        doc = new QTextStream(&bookFile);
        doc->setCodec(book->getBookCodec().toStdString().c_str());

        QString temp;

        do
            doc->operator >>(temp);
        while(temp.indexOf("<body>") == -1);
        temp = temp.remove(0, temp.indexOf("<body>") + 6);
        bookText.append(splitTextToWords(temp));

        bool flag = true;
        while(!doc->atEnd())
        {
            doc->operator >>(temp);
            if (temp.indexOf("<binary") != -1)
                flag = false;

            if (flag)
            {
                bookText.append(splitTextToWords(temp));
                strCount = bookText.size();
            }
            else
                bookText.append(temp);
        }

        bookText.back() = bookText.back().remove(bookText.back().indexOf("</FictionBook>"), bookText.back().size());

        bookFile.close();
    }
    else
    {
        qDebug()<<"невозможно открыть файл книги";
    }

    delete doc;
}


FB2TextPaginator::FB2TextPaginator()
{
    columnWidth = columnHeight = 0;

    currentBStrNum = currentTextPos = currentEStrNum = 0;
    strCount = 0;

    currentColumn = 0;
    currentWordPos = 0;

    wordWidth = wordHeight = 0;

    currentWidth = currentHeight = 0;

    stringStep = stringHeight = 0;

    tagType = false;
    parseDirection = false;

    beginParagrafTail = ParagrafTail = false;
}



QString FB2TextPaginator::startParser(Book *OpeningBook, const int &Pwidth, const int &Pheight)
{
    book = OpeningBook;
    ProgramSettings = Settings::getSettings();
    currentEStrNum = currentBStrNum = book->getBookProgress();
    currentTextPos = 0;


    tagStack.fromList(book->getBookProgressTagStack());
    if (tagStack.size() == 0)
        tagStack.push("Text");

    parseBookText();
    createTableOfContents();
    setHTMLinf();
    setFontMap();
    setLinespaceMap();
    setPageGeometry(Pwidth, Pheight);

    return getPageForward();
}

FB2TextPaginator::~FB2TextPaginator()
{
    foreach (auto FontMetricsPointer, fontMap) {
        delete FontMetricsPointer;
    }
}

void FB2TextPaginator::setFontMap()
{

    QFont textFont(CurProfile.RegularStyle.Family, CurProfile.RegularStyle.Size, 49*(CurProfile.RegularStyle.Style%2) + 50, CurProfile.RegularStyle.Style/10);
    fontMap.insert("Text", new QFontMetrics(textFont));

    textFont.setWeight(75);
    fontMap.insert("strong", new QFontMetrics(textFont));

    QFont titleFont(CurProfile.TitleStyle.Family, CurProfile.TitleStyle.Size, 49*(CurProfile.TitleStyle.Style%2) + 50, CurProfile.TitleStyle.Style/10);
    fontMap.insert("TitleText", new QFontMetrics(titleFont));

    QFont subtitleFont(CurProfile.SubtitleStyle.Family, CurProfile.SubtitleStyle.Size, 49*(CurProfile.SubtitleStyle.Style%2) + 50, CurProfile.SubtitleStyle.Style/10);
    fontMap.insert("epigraph", new QFontMetrics(subtitleFont));

    QFont empFont(CurProfile.EmphasizedStyle.Family, CurProfile.EmphasizedStyle.Size, 49*(CurProfile.EmphasizedStyle.Style%2) + 50, CurProfile.EmphasizedStyle.Style/10);
    fontMap.insert("emphasis", new QFontMetrics(empFont));

    QFont noteFont(CurProfile.NoteStyle.Family, CurProfile.NoteStyle.Size, 49*(CurProfile.NoteStyle.Style%2) + 50, CurProfile.NoteStyle.Style/10);
    fontMap.insert("Note", new QFontMetrics(noteFont));
}

void FB2TextPaginator::setLinespaceMap()
{
    linespaceMap.insert("Text", CurProfile.RegularStyle.LineSpacing * fontMap["Text"]->lineSpacing());
    linespaceMap.insert("strong", CurProfile.RegularStyle.LineSpacing * fontMap["strong"]->lineSpacing());
    linespaceMap.insert("TitleText", CurProfile.TitleStyle.LineSpacing * fontMap["TitleText"]->lineSpacing());
    linespaceMap.insert("epigraph", CurProfile.SubtitleStyle.LineSpacing * fontMap["epigraph"]->lineSpacing());
    linespaceMap.insert("emphasis", CurProfile.EmphasizedStyle.LineSpacing * fontMap["emphasis"]->lineSpacing());
    linespaceMap.insert("Note", CurProfile.NoteStyle.LineSpacing * fontMap["Note"]->lineSpacing());
}

void FB2TextPaginator::setPageGeometry(const int &width, const int &height)
{
    columnWidth = (width - 10 - 30*(CurProfile.ColumnCount-1) - CurProfile.TextLeftRightIdent/100 - CurProfile.TextLeftRightIdent%100)/CurProfile.ColumnCount;
    columnHeight = height - 20 - CurProfile.TextTopBottomIdent/100 - CurProfile.TextTopBottomIdent%100;
}

void FB2TextPaginator::setHTMLinf()
{
    CurProfile = ProgramSettings->getCurrentReadProfileElem();


    PageHTMLHeader = "<style type='text/css'>"
                     "p{"
                          "margin-top:" + QString::number(CurProfile.ParLeftTopIdent%100) + "px;"
                          "margin-bottom:0px;"
                          "text-indent:" + QString::number(CurProfile.ParLeftTopIdent/100) + "px;"
                          "text-align-last: justify;}"

                     "p.begin{text-indent: 0px;}"

                     "TitleText{"
                           + CurProfile.TitleStyle.getHTMLStyle() + ";}"

                     "epigraph{"
                           + CurProfile.SubtitleStyle.getHTMLStyle() + ";}"

                     "Text{"
                           + CurProfile.RegularStyle.getHTMLStyle() + ";}"

                     "emphasis{"
                           + CurProfile.EmphasizedStyle.getHTMLStyle() + ";}"

                     "Note{"
                           + CurProfile.NoteStyle.getHTMLStyle() + ";}"

                     "strong{"
                          "font-weight:" + QString::number(99) + ";}"

                      + ((CurProfile.BackgroundType == true) ? ("body{background-color:" + CurProfile.BackgroundImage + ";}") : ("")) +
                  "</style>"
                  "<body>"
                  "<table border='0' style='"
                    "table-layout: fixed;"
                    "empty-cells: show;"
                    "margin-top: " + QString::number(CurProfile.TextTopBottomIdent/100) + "px;"
                    "margin-bottom: " + QString::number(CurProfile.TextTopBottomIdent%100) + "px;"
                    "margin-left:" + QString::number(CurProfile.TextLeftRightIdent/100) + "px;"
                    "margin-right:" + QString::number(CurProfile.TextLeftRightIdent%100) + "px;' "
                    "width='100%'"
                    " cellspacing='-30' cellpadding='30'>"
                  "<tr>"
                  "<td align = 'justify' width = '" + QString::number(100/CurProfile.ColumnCount) + "%'>"
                  "<Text>";

    PageHTMLSep = "</Text></td><td align = 'justify' width = '" + QString::number(100/CurProfile.ColumnCount) + "%'><Text>";
    PageHTMLBottom = "</Text></td></tr></table></body>";
}


void FB2TextPaginator::createTableOfContents()
{
    for (long long i = 0; i < strCount; i++)
        if (bookText[i] == "<section>")
        {
            if (bookText[++i] == "<TitleText>")
            {
                long long pos;
                QString text;
                for (pos = i + 1; pos < strCount && bookText[pos][0] == '<'; pos++);
                for (; pos < strCount && bookText[pos] != "</TitleText>"; pos++)
                    if (bookText[pos][0] != '<')
                        text += bookText[pos] + " ";
                TableOfContentsText.append(text);
                TableOfContentsIndexes.append(i);
                i = pos;
            }
        }
}


QStringList FB2TextPaginator::getBookContentTable()
{
    return TableOfContentsText;
}


long long FB2TextPaginator::getCurrentSectionIndex()
{
    int pos;
    for (pos = 1; pos < TableOfContentsIndexes.size() && currentBStrNum > TableOfContentsIndexes[pos]; ++pos);
    if (currentBStrNum > TableOfContentsIndexes.back())
        return TableOfContentsIndexes.size() - 1;
    return pos;
}


QString FB2TextPaginator::goToSection(const int &sectionIndex)
{
    currentTextPos = currentEStrNum = TableOfContentsIndexes[sectionIndex];
    tagStack.clear();
    tagStack.append("Text");
    parseDirection = false;
    return getPageForward();
}


int FB2TextPaginator::getWordHeight()
{
    for (int i = tagStack.size() - 1; i > 0; i--)
        if (linespaceMap.contains(tagStack[i]))
            return linespaceMap[tagStack[i]];
    return linespaceMap[tagStack[0]];
}


int FB2TextPaginator::getWordWidth(const QString &word)
{
    for (int i = tagStack.size() - 1; i > 0; i--)
        if (fontMap.contains(tagStack[i]))
            return fontMap[tagStack[i]]->width(word);
    return fontMap[tagStack[0]]->width(word);
}


int FB2TextPaginator::getSpaceWidth()
{
    for (int i = tagStack.size() - 1; i > 0; i--)
        if (fontMap.contains(tagStack[i]))
            return fontMap[tagStack[i]]->charWidth(" ", 0);
    return fontMap[tagStack[0]]->charWidth(" ", 0);
}


void FB2TextPaginator::findTagsTail()
{
    bool realParseDirection = parseDirection;
    int parseResalt = 1;
    parseDirection = true;

    do
    {
        if (parseResalt == 1)
            applyTag();

        if (realParseDirection)
            Columns[currentColumn].prepend(bookText[currentTextPos]);
        else
            Columns[currentColumn].remove(Columns[currentColumn].lastIndexOf(bookText[currentTextPos]), bookText[currentTextPos].size());

        if (bookText[currentTextPos-1][0] == '<')
        {
            tag = bookText[--currentTextPos];
            parseResalt = parseTag();
            if (tagType == true && parseResalt != 2)
                break;
        }
        else
            break;
    }
    while(currentTextPos > 0);
    currentTextPos++;
    parseDirection = realParseDirection;
}


// 1 - нужный тег, 0 - бесполезный тег, 2 - лишний тег, 3 - тег перевода колонки
int FB2TextPaginator::parseTag()
{
    tag = tag.mid(1, tag.size()-2);
    if (tag[0] == '/')
    {
        tag  = tag.remove(0, 1);
        tagType = true;
    }
    else
        tagType = false;
    if (tag == "p /")
        return 0;
    if (tag == "p")
    {
        if (tagType == parseDirection)
        {
            if (currentHeight + stringHeight + (CurProfile.ParLeftTopIdent%100)*((int)!parseDirection) > columnHeight)
            {
                findTagsTail();
                return 3;
            }
        }
        else
        {
            if (!parseDirection)
                currentWidth = CurProfile.ParLeftTopIdent/100;
            else
                currentWidth = 0;

            currentHeight += stringHeight + CurProfile.ParLeftTopIdent%100;
            stringHeight = 0;

            if (ParagrafTail && parseDirection)
            {
                Columns[currentColumn].prepend("<p class='end'>");
                ParagrafTail = false;
                return 2;
            }
        }
        return 1;
    }

    if (tag == "section" && tagType == parseDirection)
    {
        currentTextPos += (parseDirection)?-1:1;
        return 3;
    }

    if (tag.mid(0,2) == "a " || (tag == "a" && tagType == parseDirection))
    {
        return 2;
    }

    if (tag.mid(0,5) == "image")
    {
        return 2;
    }

    if (tag == "br/")
    {
        tagType = true;
        if (currentHeight == 0)
            return 2;

        if (currentHeight + getWordHeight() + CurProfile.ParLeftTopIdent%100 > columnHeight)
            return 2;

        currentHeight += getWordHeight() + CurProfile.ParLeftTopIdent%100;

        if (parseDirection)
            Columns[currentColumn].prepend("<p>&nbsp;</p>");
        else
            Columns[currentColumn] += "<p>&nbsp;</p>";

        stringHeight = 0;
        return 2;
    }

    if (tag == "epigraph" || tag == "TitleText" || tag == "subtitle" || tag == "emphasis" || tag == "Note" || tag == "strong")
        return 1;

    return 2;
}


void FB2TextPaginator::applyTag()
{
    if (tag != "")
    {
        if (tagStack.back() == tag)
        {
            if (tagType == !parseDirection)
            {
                word = "";
                tagStack.pop();
            }
        }
        else if (tagType == parseDirection)
        {
            tagStack.push(tag);
        }
    }
}


bool FB2TextPaginator::applyWord()
{
    if (word != "")
    {
        wordHeight = getWordHeight();
        if (wordHeight > stringHeight)
        {
            stringHeight = wordHeight;
            if (currentHeight + stringHeight > columnHeight)
            {
                //перенос на сл колонку
                //отрезать stringWordCount слов от колонки
                if (parseDirection)
                    currentTextPos++;
                else
                    currentTextPos--;
                return false;
            }
        }

        wordWidth = getWordWidth(word);

        if (currentWidth + wordWidth + getSpaceWidth() > columnWidth)
        {
            if (currentWidth + wordWidth > columnWidth)
            {
                if (currentHeight + stringHeight + wordHeight  > columnHeight)
                {
                    //переносим колонку
                    return false;
                }
                else
                {
                    //переносим строку
                    currentHeight += stringHeight;
                    currentWidth = wordWidth;

                    if (word.right(1) != "-")
                        currentWidth += getSpaceWidth();
                }
            }
            else
                currentWidth += wordWidth;
        }
        else
            currentWidth += wordWidth + getSpaceWidth();
    }
    return true;
}


QString FB2TextPaginator::getPageForward()
{
    if (currentTextPos < strCount)
    {
        if (!parseDirection)
        {
            beginTagStack = tagStack;
            beginParagrafTail = ParagrafTail;
        }
        else
        {
            tagStack = beginTagStack;
            ParagrafTail = beginParagrafTail;
        }

        parseDirection = false;

        Columns.clear();
        currentTextPos = currentBStrNum = currentEStrNum;
        book->setBookProgress(currentBStrNum, getProgress(), tagStack.toList());
        wordWidth = wordHeight = 0;
        tagType = 0;

        for (currentColumn = 0; currentColumn < CurProfile.ColumnCount; currentColumn++)
        {
            columnTail = "";

            for (int p = 1; p < tagStack.size(); p++)
                columnTail += "<" + tagStack[p] + ">";

            if (ParagrafTail)
            {
                //учитываем разделение параграфа
                int pos = columnTail.lastIndexOf("<p>");
                if (pos != -1)
                    columnTail.insert(pos + 2, " class = 'begin'");

                ParagrafTail = false;
            }

            Columns.append(columnTail);

            currentHeight = currentWidth = 0;
            stringHeight = 0;
            for (; currentTextPos < strCount && currentTextPos < bookText.size(); currentTextPos++)
            {
                if (bookText[currentTextPos].size())
                {
                    if (bookText[currentTextPos][0] == '<')
                    {
                        tag = bookText[currentTextPos];
                        int parseResalt = parseTag();
                        if (parseResalt == 1)
                            applyTag();
                        else
                            if (parseResalt == 2)
                                continue;
                        else
                            if (parseResalt == 3)
                                break;
                    }
                    else
                    {
                        word = bookText[currentTextPos];
                        if (!applyWord())
                        {
                            // переносим колонку с разделением параграфа
                            int pos = Columns[currentColumn].lastIndexOf("<p>");
                            Columns[currentColumn].insert(pos+2, " class = 'end'");
                            ParagrafTail = true;

                            break;
                        }
                    }
                }
                Columns[currentColumn] += bookText[currentTextPos];
                if (bookText[currentTextPos].right(1) != "-" || bookText[currentTextPos].size() == 1)
                    Columns[currentColumn] += " ";
            }

            for (int p = tagStack.size() - 1; p > 0; p--)
                Columns[currentColumn] += "</" + tagStack[p] + ">";

            currentEStrNum = currentTextPos;
        }

        HTMLPage = Columns[0];

        for (int i = 1; i < CurProfile.ColumnCount; i++)
            HTMLPage += PageHTMLSep + Columns[i];

        debugSave(PageHTMLHeader + HTMLPage + PageHTMLBottom);
    }
    return PageHTMLHeader + HTMLPage + PageHTMLBottom;
}


QString FB2TextPaginator::getPageBackward()
{
    if (currentBStrNum > 0)
    {
        if (parseDirection)
        {
            beginTagStack = tagStack;
            beginParagrafTail = ParagrafTail;
        }
        else
        {
            tagStack = beginTagStack;
            ParagrafTail = beginParagrafTail;
        }

        parseDirection = true;
        Columns.clear();

        currentEStrNum = currentBStrNum;
        currentTextPos = currentBStrNum - 1;

        wordWidth = wordHeight = 0;
        tagType = 0;

        for (currentColumn = 0; currentColumn < CurProfile.ColumnCount; currentColumn++)
        {
            ///дописать разрыв параграфа

            columnTail = "";
            for (int p = tagStack.size() - 1; p > 0 ; p--)
                columnTail += "</" + tagStack[p] + ">";
            Columns.append(columnTail);

            currentHeight = 0;
            currentWidth = 0;
            stringHeight = 0;

            for (; currentTextPos >= 0; currentTextPos--)
            {
                if (bookText[currentTextPos][0] == '<')
                {
                    tag = bookText[currentTextPos];
                    int parseResalt = parseTag();
                    if (parseResalt == 1)
                        applyTag();
                    else
                        if (parseResalt == 2)
                            continue;
                    else
                        if (parseResalt == 3)
                            break;
                }
                else
                {
                    word = bookText[currentTextPos];
                    if (!applyWord())
                    {
                        ParagrafTail = true;
                        break;
                    }
                }

                if (bookText[currentTextPos].right(1) != "-")
                    Columns[currentColumn].prepend(" ");

                Columns[currentColumn].prepend(bookText[currentTextPos]);
            }

            columnTail = "";
            for (int p = tagStack.size() - 1; p > 0; p--)
                columnTail.prepend( "<" + tagStack[p] + ">");

            if (ParagrafTail)
            {
                int pos = columnTail.indexOf("<p>");
                if (pos != -1)
                    columnTail.insert(pos+2, " class = 'begin'");
            }

            Columns[currentColumn].prepend(columnTail);
        }

        currentBStrNum = currentTextPos + 1;

        book->setBookProgress(currentBStrNum,getProgress(), tagStack.toList());

        HTMLPage = Columns[CurProfile.ColumnCount - 1];
        for (int i = CurProfile.ColumnCount - 2; i >= 0; i--)
            HTMLPage += PageHTMLSep + Columns[i];

    }

    return PageHTMLHeader + HTMLPage + PageHTMLBottom;
}


QString FB2TextPaginator::updatePage(const int &width, const int &height)
{
    setPageGeometry(width, height);
    currentEStrNum = currentBStrNum;
    tagStack = beginTagStack;
    return getPageForward();
}


QString FB2TextPaginator::updateSettings(const int &width, const int &height)
{
    setPageGeometry(width, height);
    setHTMLinf();
    setFontMap();
    setLinespaceMap();
    currentEStrNum = currentBStrNum;
    tagStack = beginTagStack;
    return getPageForward();
}


float FB2TextPaginator::getProgress()
{
    if (currentBStrNum > 0 && strCount)
        return (((float)currentEStrNum/(float)strCount) * 100);
    else
        return 0;
}


void FB2TextPaginator::debugSave(const QString &HTMLPage)
{
    QFile asd("F:/asd.html");
    asd.open(QIODevice::WriteOnly);
    QTextStream out(&asd);
    out<<PageHTMLHeader + HTMLPage + PageHTMLBottom;
    asd.close();
}
