#include "core/AppSettings.h"
#include <QSettings>
#include <QFontDatabase>

AppSettings::AppSettings(QObject *parent) : QObject(parent)
{
    // QSettings 会根据我们在 main.cpp 中设置的组织和应用名自动找到存储位置
    m_settings = new QSettings(this);
    load();
}

void AppSettings::load()
{
    // 只加载字体设置
    // value() 的第二个参数是默认值，如果配置不存在则使用它
    // 我们选择一个通用的等宽字体作为默认值
    m_editorFont = m_settings->value("editor/font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>();
}

QFont AppSettings::editorFont() const
{
    return m_editorFont;
}

void AppSettings::setEditorFont(const QFont &font)
{
    if (m_editorFont != font)
    {
        m_editorFont = font;
        m_settings->setValue("editor/font", m_editorFont);
        emit settingsChanged();
    }
}