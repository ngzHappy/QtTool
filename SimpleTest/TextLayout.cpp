#include "TextLayout.hpp"
#include "QtGui/qpainter.h"

TextLayout::TextLayout(QTextDocument * p)
    :P(p)
{

}


void TextLayout::drawBorder(
        QPainter *painter,
        const QRectF &rect,
        qreal topMargin,
        qreal bottomMargin,
        qreal border,
        const QBrush &brush,
        QTextFrameFormat::BorderStyle style) const{
    P::drawBorder(painter,rect,topMargin,bottomMargin,border,brush,style);
}

QRectF TextLayout::doLayout(int from,int oldLength,int length) {
    return P::doLayout(from,oldLength,length);
}
