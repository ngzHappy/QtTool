#include "TextEdit.hpp"
#include "TextLayout.hpp"
#include <QtGui/qtextdocument.h>
#include <QtGui/qtextcursor.h>
#include <QtGui/qtextformat.h>
#include <QtCore/qdebug.h>



TextEdit::TextEdit(QWidget * p)
    :P(p)
{

    QTextDocument * doc = new QTextDocument(this);
    TextLayout * layout_ = new TextLayout(doc);
    doc->setDocumentLayout(layout_);

    QTextFrameFormat format_=doc->rootFrame()->frameFormat();
    format_.setPosition(QTextFrameFormat::FloatLeft);

    doc->rootFrame()->setFrameFormat(format_);
    format_.setBackground(QColor(122,122,122));
    format_.setBorderStyle(QTextFrameFormat::BorderStyle_DotDash);
    format_.setBorder(1);
    format_.setMargin(1);
    format_.setPadding(0);


    //{
    //    QTextCursor tc(doc);
    //    tc.movePosition( QTextCursor::Start );
    //    tc.insertBlock();
    //}

    this->setDocument(doc);

    {
        QTextCursor tc = this->textCursor();
        tc.movePosition( QTextCursor::MoveOperation::Start );
        tc.insertText("abcd:");
        this->setTextCursor(tc);
    }

    {
        QTextCursor tc = this->textCursor();
        tc.movePosition( QTextCursor::MoveOperation::End );
        tc.insertFrame(format_);
        this->setTextCursor(tc);
    }

    {
        QTextCursor tc = this->textCursor();
        tc.movePosition( QTextCursor::MoveOperation::End );
        tc.insertFrame(format_);
        this->setTextCursor(tc);
    }



   /* qDebug()<< "frame" <<doc->rootFrame()->firstPosition();
    QTextCursor tc( doc->rootFrame() );
    tc.movePosition(QTextCursor::MoveOperation::Start);
    tc.insertText(QString::fromUtf8(u8R"(通话记录:)"));
    this->setTextCursor(tc);
*/
    this->setReadOnly(true);
}
