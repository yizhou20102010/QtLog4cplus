# log4cplus在Qt5配置过程及出现问题的解决方法

## 配置环境

Log4cplus1.2.1+vs2015+Qt5.7.0

## 配置过程

1. vs2015中打开log4cplus-1.2.1-rc1\msvc10文件夹下log4cplus.sln，移除不必要的项目，如卸载test和Qt4DebugAppender；

2. 在Qt5DebugAppender项目属性的包括目录和库文件目录中添加Qt5的include文件夹（如：%Qt_Dir%\5.7\msvc2015\include）及lib库文件目录（如%Qt_Dir%\5.7\msvc2015\lib）

3. 将解决方案中所有项目属性中"c/c++" -> "语言" 中的"将WChar_t视为内置类型"设置为“否(/Zc:wchar_t-_)；

4. 编译成Debug版本和Release版本；

5. 复制log4cplus-1.2.1-rc1下include文件夹和debug版本、release版本中的lib和dll到指定文件夹，如log4cplus；

6. 在Qt项目的.pro文件中添加include文件夹路径以及库文件

   ```c++
   //include path
   INCLUDEPATH+=$$quote(***/log4cplus/include)
   //libs
   win32:CONFIG(release, debug|release): LIBS += -Lquote(***/log4cplus/libs/) -llog4cplus
   else:win32:CONFIG(debug, debug|release): LIBS += -Lquote(***/log4cplus/libs/) -llog4cplusD
   ```

## 应用简单例子

```c++
#include <QCoreApplication>
#undef UNICODE
#include<log4cplus/logger.h>
#include<log4cplus/consoleappender.h>
#include <log4cplus/qt5debugappender.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 定义一个控制台的Appender
    SharedAppenderPtr pConsoleAppender(new ConsoleAppender());
    
    // 定义Logger,并设置优先级
    Logger pTestLogger = Logger::getInstance("LoggerName");
    pTestLogger.setLogLevel(WARN_LOG_LEVEL);
    
    // 将需要关联Logger的Appender添加到Logger上
    pTestLogger.addAppender(pConsoleAppender);
    
    // 输出日志信息
    LOG4CPLUS_WARN(pTestLogger, "This is a <Warn> log message...");

    return a.exec();
}
```



## 出现问题

1. vs2015编译时报警：无法打开源文件 "QtGlobal"

   解决办法：打开文件“qt5debugappender.cxx”将其中的#include <QtGlobal>替换为#include <QtCore/QtGlobal>。

2. qt编译以上程序时，出现： error: C2665: “log4cplus::Logger::getInstance”: 2 个重载中没有一个可以转换所有参数类型；

   解决办法：在包含log4cplus相关头文件之前添加#undef UNICODE;

3. qt编译时出现：error: C3861: “LOG4CPLUS_WARN”: 找不到标识符

4. 解决办法：添加头文件#include <log4cplus/loggingmacros.h>