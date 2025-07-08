#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>

class EditorWidget; // 前向声明 EditorWidget 类

class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    // 构造函数，接受一个父控件 editorWidget 指针
    explicit LineNumberArea(EditorWidget* editor);

    // 重写 sizeHint 函数来告诉布局系统这个控件希望的尺寸
    QSize sizeHint() const override;
protected:
    // 重写 paintEvent 函数来绘制行号
    void paintEvent(QPaintEvent *event) override;
private:
    EditorWidget *m_editor; // 指向编辑器控件的指针
};

#endif // LINENUMBERAREA_H