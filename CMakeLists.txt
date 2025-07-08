# CMake 最低版本要求
cmake_minimum_required(VERSION 3.16)

# 项目名称和C++标准
project(MyTextEditor VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 寻找Qt6 ---
# 寻找构建所需的Qt模块，我们从核心的 Widgets 开始
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- Qt特性集成 ---
# 启用这些特性可以让CMake自动处理Qt的特殊文件
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# --- 定义源文件和头文件 ---
# 将所有源文件分组，便于管理
set(CORE_SOURCES
    src/core/Document.cpp
    # src/core/FileManager.cpp
    # src/core/AppSettings.cpp
)

set(UI_SOURCES
    src/ui/MainWindow.cpp
    # src/ui/EditorWidget.cpp
    # src/ui/widgets/LineNumberArea.cpp
    # src/ui/dialogs/FindDialog.cpp
    # src/ui/dialogs/AboutDialog.cpp
    # src/ui/dialogs/SettingsDialog.cpp
)

set(SYNTAX_SOURCES
    # src/syntax/Highlighter.cpp
)

set(UI_HEADERS
    src/ui/MainWindow.h
    # src/ui/EditorWidget.h
    # src/ui/widgets/LineNumberArea.h
    # src/ui/dialogs/FindDialog.h
    # src/ui/dialogs/AboutDialog.h
    # src/ui/dialogs/SettingsDialog.h
)

set(CORE_HEADERS
    src/core/Document.h
    # src/core/FileManager.h
    # src/core/AppSettings.h
)


# Qt 资源文件 (.qrc)
set(RESOURCE_FILES
    # resources/resources.qrc
)

# --- 创建可执行文件 ---
# add_executable命令会创建一个名为 MyTextEditor 的可执行文件
add_executable(MyTextEditor
    src/main.cpp
    
    ${CORE_SOURCES}
    ${UI_SOURCES}
    ${SYNTAX_SOURCES}
    
    # 尽管头文件通常不需要在这里列出，但对于IDE的集成，
    # 将它们包含在源代码列表中是有益的。
    # 另一种做法是使用 target_sources()
    ${CORE_HEADERS}
    ${UI_HEADERS}
    # src/syntax/Highlighter.h
    # src/utils/Singleton.h

    ${UI_FILES}
    ${RESOURCE_FILES}
)

# --- 链接库 ---
# 将我们的可执行文件与Qt的Widgets模块链接
target_link_libraries(MyTextEditor PRIVATE Qt6::Widgets)

# 添加 include 目录，解决头文件查找问题
target_include_directories(MyTextEditor PRIVATE ${CMAKE_SOURCE_DIR}/src)

# --- (可选) 添加测试子目录 ---
# 如果你想构建测试，可以取消下面的注释
# enable_testing()
# add_subdirectory(tests)

# --- 安装规则 (可选) ---
# 如果需要创建安装包，可以在此定义安装规则
# install(TARGETS MyTextEditor DESTINATION bin)