#ifndef FB2PARSER_H
#define FB2PARSER_H

#include <books.h>
#include <book_image_table.h>

#include <QTreeWidgetItem>

class FB2Parser
{
public:
    FB2Parser(Book *book, QStringList &text, BookImageTable *&ImageTable,
              QTreeWidgetItem *&tableOfContents, QHash <QString, QStringList> &notesTable);
    ~FB2Parser();

private:
    void parseText(QStringList &bookText);
    void createTableOfContents(const QStringList &bookText, QTreeWidgetItem *&tableOfContents) const;
    void createNotesTable(QStringList &bookText, QHash <QString, QStringList> &notesTable) const;

    Book *Obook;

    QTextStream* doc;
};

#endif // FB2PARSER_H
