#ifndef TEXTLAYOUT_HPP
#define TEXTLAYOUT_HPP

#include <QtUtility.hpp>

class TextLayout : public QtTextDocumentLayout {
    Q_OBJECT
private:
    typedef QtTextDocumentLayout P;
public:
    TextLayout(QTextDocument * /**/=nullptr);
protected:
    QRectF doLayout(int from, int oldLength, int length) override;
    void drawBorder(
            QPainter *painter,
            const QRectF &rect,
            qreal topMargin,
            qreal bottomMargin,
            qreal border,
            const QBrush &brush,
            QTextFrameFormat::BorderStyle style) const override;
};

#endif // TEXTLAYOUT_HPP
