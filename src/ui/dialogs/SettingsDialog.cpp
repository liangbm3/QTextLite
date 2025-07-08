#include "ui/dialogs/SettingsDialog.h"
#include "core/AppSettings.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFontComboBox>
#include <QPushButton>
#include <QDialogButtonBox>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Settings"));//设置标题

    m_fontComboBox = new QFontComboBox(this);//新建字体选择框

    QFormLayout *formLayout = new QFormLayout;//使用表单布局来组织控件
    formLayout->addRow(tr("Editor Font:"), m_fontComboBox);//将标签和字体选择框添加到布局中

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);//三个按钮：确定、取消、应用
    m_okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    m_cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
    m_applyButton = buttonBox->addButton(QDialogButtonBox::Apply);

    //分别连接到对应的槽函数
    connect(m_okButton, &QPushButton::clicked, this, [this]()
            {
        applyChanges();
        accept();
    });
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_applyButton, &QPushButton::clicked, this, &SettingsDialog::applyChanges);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
}

//重载，当调用loadCurrentSettings时会触发，会将当前设置的字体加载到界面控件中
void SettingsDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    loadCurrentSettings();
}

//从单例中获取当前编辑器字体
void SettingsDialog::loadCurrentSettings()
{
    m_fontComboBox->setCurrentFont(AppSettings::instance().editorFont());
}

//将用户在字体选择框中选择的字体应用到设置中
void SettingsDialog::applyChanges()
{
    AppSettings::instance().setEditorFont(m_fontComboBox->currentFont());
}