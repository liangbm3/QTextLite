#include "ui/dialogs/FindDialog.h"

#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

FindDialog::FindDialog(QWidget *parent) : QDialog(parent)
{
    // 创建所有的UI控件
    m_findLineEdit = new QLineEdit(this);                                // 查找输入
    m_findPreviousButton = new QPushButton(tr("Find &Previous"), this); // 查找上一个按钮
    m_replaceLineEdit = new QLineEdit(this);                             // 替换输入
    m_caseSensitiveCheckBox = new QCheckBox(tr("Case Sensitive"), this); // 是否区分大小写
    m_findButton = new QPushButton(tr("&Find Next"), this);              // 查找按钮
    m_replaceButton = new QPushButton(tr("&Replace"), this);             // 替换按钮
    m_replaceAllButton = new QPushButton(tr("Replace &All"), this);      // 替换所有按钮
    m_closeButton = new QPushButton(tr("Close"), this);                  // 关闭按钮

    // 按钮属性
    m_findButton->setDefault(true);        // 设置查找按钮为默认按钮，这样回车时自动触发
    m_findButton->setEnabled(false);       // 初始状态不可用
    m_findPreviousButton->setEnabled(false); // 初始状态不可用
    m_replaceButton->setEnabled(false);    // 初始状态不可用
    m_replaceAllButton->setEnabled(false); // 初始状态不可用

    // 连接信号和槽
    connect(m_findLineEdit, &QLineEdit::textChanged, this, &FindDialog::onTextChanged);
    connect(m_findButton, &QPushButton::clicked, this, &FindDialog::onFindClicked);
    connect(m_findPreviousButton, &QPushButton::clicked, this, &FindDialog::onFindPreviousClicked);
    connect(m_replaceButton, &QPushButton::clicked, this, &FindDialog::onReplaceClicked);
    connect(m_replaceAllButton, &QPushButton::clicked, this, &FindDialog::onReplaceAllClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &FindDialog::close);

    // 设置布局
    QGridLayout *leftLayout = new QGridLayout;                    // 左侧网格布局
    leftLayout->addWidget(new QLabel(tr("Find what:")), 0, 0);    // 添加查找标签
    leftLayout->addWidget(m_findLineEdit, 0, 1);                  // 添加查找输入框
    leftLayout->addWidget(new QLabel(tr("Replace with:")), 1, 0); // 添加替换标签
    leftLayout->addWidget(m_replaceLineEdit, 1, 1);               // 添加替换输入框
    leftLayout->addWidget(m_caseSensitiveCheckBox, 2, 0, 1, 2);   // 添加区分大小写复选框

    QVBoxLayout *rightLayout = new QVBoxLayout; // 右侧垂直布局
    rightLayout->addWidget(m_findButton);       // 添加查找按钮
    rightLayout->addWidget(m_findPreviousButton); // 添加查找上一个按钮
    rightLayout->addWidget(m_replaceButton);    // 添加替换按钮
    rightLayout->addWidget(m_replaceAllButton); // 添加替换所有按钮
    rightLayout->addWidget(m_closeButton);      // 添加关闭按钮
    rightLayout->addStretch();                  // 弹性空间，按钮向上排列

    QHBoxLayout *mainLayout = new QHBoxLayout(this); // 主布局，水平布局
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    setWindowTitle(tr("Find and Replace")); // 添加标题
    // 设置为非模态对话框的关键是 setWindowModality
    setWindowModality(Qt::NonModal);
}

FindDialog::~FindDialog()
{
    // 析构函数，Qt会自动清理子控件
}

void FindDialog::onFindClicked()
{
    //判断大小写复选框是否被选中，并将选项传递给信号发射出去
    Qt::CaseSensitivity cs = m_caseSensitiveCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    // 发射 findNext 信号，将任务交给主窗口
    emit findNext(m_findLineEdit->text(), cs);
}

void FindDialog::onFindPreviousClicked()
{
    Qt::CaseSensitivity cs = m_caseSensitiveCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    // 发射 findPrevious 信号，将任务交给主窗口
    emit findPrevious(m_findLineEdit->text(), cs);
}

void FindDialog::onReplaceClicked()
{
    // 只需发射替换字符串，因为替换操作总是作用于当前选区
    emit replace(m_replaceLineEdit->text());
}

void FindDialog::onReplaceAllClicked()
{
    //判断是否区分大小写，将查找文本、替换文本和大小写选项发射出去
    Qt::CaseSensitivity cs = m_caseSensitiveCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    emit replaceAll(m_findLineEdit->text(), m_replaceLineEdit->text(), cs);
}

void FindDialog::onTextChanged()//当输入框内容有变化时
{
    //如果有内容，则启用查找、替换、替换所有按钮，否则禁用
    bool hasText = !m_findLineEdit->text().isEmpty();
    m_findButton->setEnabled(hasText);
    m_findPreviousButton->setEnabled(hasText);
    m_replaceButton->setEnabled(hasText);
    m_replaceAllButton->setEnabled(hasText);
}

QString FindDialog::findText() const {
    return m_findLineEdit->text();
}

bool FindDialog::isCaseSensitive() const {
    return m_caseSensitiveCheckBox->isChecked();
}

void FindDialog::setFindText(const QString &text) {
    m_findLineEdit->setText(text);
}

void FindDialog::focusOnFindLineEdit() {
    m_findLineEdit->setFocus();
    m_findLineEdit->selectAll();
}