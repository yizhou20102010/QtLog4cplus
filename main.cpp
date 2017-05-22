#include <QCoreApplication>
#undef UNICODE
#include<log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include<log4cplus/consoleappender.h>
#include <log4cplus/qt5debugappender.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iostream>
using namespace log4cplus;
using namespace log4cplus::helpers;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///程序配置格式
    // 定义一个控制台的Appender
    SharedAppenderPtr pConsoleAppender(new ConsoleAppender());

    //定义一个简单的layout，并绑定到appender
    std::auto_ptr<Layout> pSimpleLayout(new SimpleLayout());
    pConsoleAppender->setLayout(pSimpleLayout);
    // 定义Logger,并设置优先级
    Logger pTestLogger = Logger::getInstance("LoggerName");
    pTestLogger.setLogLevel(WARN_LOG_LEVEL);

    // 将需要关联Logger的Appender添加到Logger上
    pTestLogger.addAppender(pConsoleAppender);

    // 输出日志信息
    LOG4CPLUS_WARN(pTestLogger, "This is a <Warn> log message...");

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 读取配置文件，输出log
    ///
//    PropertyConfigurator::doConfigure("F:\\Test\\QtLog4cplusTest\\log4cplus.cfg");

//    Logger testlog = Logger::getRoot();

//    for(int i=0;i<10000;i++)
//    {
//        LOG4CPLUS_DEBUG(testlog, "This is a DEBUG message");
//        LOG4CPLUS_INFO(testlog, "This is a INFO message");
//        LOG4CPLUS_WARN(testlog, "This is a WARN message");
//        LOG4CPLUS_ERROR(testlog, "This is a ERROR message");
//        LOG4CPLUS_FATAL(testlog, "This is a FATAL message");
//        std::cout<<i<<std::endl;
//    }

//    std::cout<<"Over"<<std::endl;

    return a.exec();
}
