#ifndef MAINWIINDOW_HPP
#define MAINWIINDOW_HPP

#include <QWidget>

class MainWiindow : public QWidget
{
    Q_OBJECT
private:
    typedef QWidget P;
public:
    MainWiindow(QWidget *parent = 0);
    ~MainWiindow();
};

#endif // MAINWIINDOW_HPP
