#ifndef TEXTEDIT_HPP
#define TEXTEDIT_HPP

#include <QtUtility.hpp>
#include <QtWidgets/qtextedit.h>


class TextEdit : public QTextEdit
{
    Q_OBJECT
private:
    typedef QTextEdit P;
public:
    TextEdit(QWidget * /**/=nullptr);
};

#endif // TEXTEDIT_HPP
