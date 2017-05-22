# Log4cplus使用教程

[TOC]

## 一、 简介

log4cplus是C++编写的开源的日志系统. 具有线程安全、灵活、以及多粒度控制的特点，通过将信息划分优先级使其可以面向程序调试、运行、测试、和维护等全生命周期； 你可以选择将信息输出到屏幕、文件、NT event log、甚至是远程服务器；通过指定策略对日志进行定期备份等等。

## 二、 log4cplus框架

### 1. 基本元素

- **Layouts** ：布局器，控制输出消息的格式；
- **Appenders** ：挂接器，根据布局器设置格式，将消息输出到指定位置，如屏幕、文件等；
- **Logger** ：记录器，保存并跟踪对象日志信息变更的尸体日志对象；
- **Priorities** ：优先权，包括TRACE, DEBUG, INFO, WARNING, ERROR, FATAL；

### 2. 基本结构

![](http://img1.51cto.com/attachment/200903/200903301238384663546.jpg)

### 3. 基本步骤

1. 实例化一个appender对象： new ConsoleAppender()；
2. 实例化一个layout对象：new SimpleLayout()；
3. 将layout对象绑定(attach)到appender对象：appender->setLayout(layout)；
4. 实例化一个logger对象,调用静态函数：log4cplus ::Logger ::getInstance("logger_name")；  
5. 将appender对象绑定(attach)到logger对象:logger.addAppender(appender)，如省略此步骤，标准输出（屏幕）appender对象会绑定到logger；
6. 设置logger的优先级: logger.setLogLevel(ALL_LOG_LEVEL)，如省略此步骤，各种有限级的消息都将被记录。

### 4. 简单例子程序

```c++
#include<log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include<log4cplus/consoleappender.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

int main(int argc, char *argv[])
{
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
  
  return 0;
}

```

运行上面的例子，可以看到屏幕输出

```c++
WARN - This is a <Warn> log message...
```

## 三、 log4cplus详细内容介绍

### 1. Appender输出位置

(1) 控制台输出类（如上例中，将信息输出到屏幕）

```c++
ConsoleAppender
```

(2) 文件输出类(将信息按指定方式输出到指定文件)

```c++
FileAppender,	RollingFileAppender,	DailyRollingFileAppender 
```

**a. 类FileAppender**实现了基本的文件操作功能，构造函数如下

```c++
FileAppender(const log4cplus::tstring& filename ,

            LOG4CPLUS_OPEN_MODE_TYPE mode = LOG4CPLUS_FSTREAM_NAMESPACE::ios::trunc,

            bool immediateFlush = true);
```

filename：保存的文件名；

mode：文件类型， 可选择的文件类型包括app、ate、binary、in、out、trunc，缺省是trunc，表示将先前文件删除；

immediateFlush：缓冲刷新标志，如果为true表示每向文件写一条记录就刷新一次缓存，否则直到FileAppender被关闭或文件缓存已满才更新文件，默认true。

**b. 类RollingFileAppender**可以根据预先设定的大小来决定是否转储，当超过该大小，后续log信息会另存到新文件中，除了定义每个记录文件的大小之外，还要确定在RollingFileAppender类对象构造时最多需要多少个这样的记录文件(maxBackupIndex+1)，当存储的文件数目超过maxBackupIndex+1时，会删除最早生成的文件，保证整个文件数目等于maxBackupIndex+1，然后继续记录。构造函数如下：

```c++
RollingFileAppender(const log4cplus::tstring& filename, //保存的文件名
                    
                    long maxFileSize,                   //文件最大的尺寸，默认为10Mb
                    
                    int maxBackupIndex,				   //最大文件记录个数，默认为1
                    
                    bool immediateFlush);			   //缓冲刷新标志
```

例如

```c++
SharedAppenderPtr _append(new RollingFileAppender("Test.log", size * n, size));
```



**c. 类DailyRollingFileAppender**可以根据你预先设定的频度来决定是否转储，当超过该频度，后续log信息会另存到新文件中，这里的频度包括：MONTHLY （每月）、WEEKLY（每周）、DAILY（每日）、TWICE_DAILY（每两天）、HOURLY（每时）、MINUTELY（每分）。构造函数如下：

```c++
DailyRollingFileAppender(const log4cplus::tstring& filename,  //保存文件名
                         
                         DailyRollingFileSchedule schedule,   //存储频度
                         
                         bool immediateFlush,                 //缓冲刷新标志
                         
                         int maxBackupIndex)；                //最大文件记录数
```

例如：

```c++
SharedAppenderPtr _append(new DailyRollingFileAppender("Test.log", MINUTELY, true, 5));
```

### 2. Layout输出格式类

(1) **SimpleLayout类**是一种简单格式的布局器，在输出的原始信息之前加上LogLevel和一个"-"。

(2) **TTCCLayout类**：其格式由时间，线程ID，Logger和NDC组成；

(3) **PatternLayout类**是一种有词法分析功能的模式布局器,类似正则表达式。以“%”作为开头的特殊预定义标识符，将产生特殊的格式信息。

​	a. "%%", 转义为%；

​	b. "%c"，输出logger名称，如test.subtest 。也可以控制logger名称的显示层次，比如"%c{1}"时输出"test"，		其中数字表示层次。

​	c. "%D"，显示本地时间，比如："2004-10-16 18:55:45"，%d显示标准时间。     可以通过%d{...}定义更详细的显示格式，比如%d{%H:%M:%s}表示要显示小时:分钟：秒。大括号中可显示的预定义格式符如下：

​	           %a -- 表示礼拜几，英文缩写形式，比如"Fri"
​		   %A -- 表示礼拜几，比如"Friday"
​		   %b -- 表示几月份，英文缩写形式，比如"Oct"
​		   %B -- 表示几月份，"October"
​		   %c -- 标准的日期＋时间格式，如 "Sat Oct 16 18:56:19 2004"
​		   %d -- 表示今天是这个月的几号(1-31)"16"
​		   %H -- 表示当前时刻是几时(0-23)，如 "18"
​		   %I -- 表示当前时刻是几时(1-12)，如 "6"
​		   %j -- 表示今天是哪一天(1-366)，如 "290"
​		   %m -- 表示本月是哪一月(1-12)，如 "10"
​		   %M -- 表示当前时刻是哪一分钟(0-59)，如 "59"
​		   %p -- 表示现在是上午还是下午， AM or PM
​		   %q -- 表示当前时刻中毫秒部分(0-999)，如 "237"
​		   %Q -- 表示当前时刻中带小数的毫秒部分(0-999.999)，如 "430.732"
​		   %S -- 表示当前时刻的多少秒(0-59)，如 "32"
​		   %U -- 表示本周是今年的第几个礼拜，以周日为第一天开始计算(0-53)，如 "41"
​		   %w -- 表示礼拜几，(0-6, 礼拜天为0)，如 "6"	
​		   %W -- 表示本周是今年的第几个礼拜，以周一为第一天开始计算(0-53)，如 "41"
​		   %x -- 标准的日期格式，如 "10/16/04"
​		   %X -- 标准的时间格式，如 "19:02:34"
​		   %y -- 两位数的年份(0-99)，如 "04"
​		   %Y -- 四位数的年份，如 "2004"
​		   %Z -- 时区名，比如 "GMT"

​	d. "%F"，输出当前记录器所在的文件名称，比如"main.cpp"

​	e. "%L"，输出当前记录器所在的文件行号，比如"51"

​	f.  "%l"，输出当前记录器所在的文件名称和行号，比如"main.cpp:51"

​	g. "%m"，输出原始信息。

​	h. "%n"，换行符。

​	i. "%p"，输出LogLevel，比如"DEBUG"

​	j. "%t"，输出记录器所在的线程ID，比如 "1075298944"
​	k. "%x"，嵌套诊断上下文NDC (nested diagnostic context) 输出，从堆栈中弹出上下文信息，NDC可以用对不同源的log信息（同时地）交叉输出进行区分。
​	l. 格式对齐，比如"%-10m"时表示左对齐，宽度是10，当然其它的控制字符也可以相同的方式来使用，比如"%-12d"，"%-5p"等等。

### 3. Logger对象及优先级设置

 ####logger对象

log4cplus通过LogLevelManager、Logger、Filter三种方式，实现任意时刻输出的log信息。Log4cplus中logger的存储机制是通过一个层次化的机构来组织的。

- 获取Root级别的logger：`Logger::getRoot();`
- 自定义的logger：`Logger::getInstance("test");`
- 定义其子logger：`Logger::getInstance("test.subtest");`

#### Logger的优先级

logger优先级由低到高：

​    NOT_SET_LOG_LEVEL (-1) ：接受缺省的LogLevel，如果有父logger则继承它的LogLevel
​    ALL_LOG_LEVEL (0)：开放所有log信息输出
​    TRACE_LOG_LEVEL (0) ：开放trace信息输出(即ALL_LOG_LEVEL)
​    DEBUG_LOG_LEVEL (10000) ：开放debug信息输出
​    INFO_LOG_LEVEL (20000) ：开放info信息输出
​    WARN_LOG_LEVEL (30000) ：开放warning信息输出
​    ERROR_LOG_LEVEL (40000)：开放error信息输出
​    FATAL_LOG_LEVEL (50000)：开放fatal信息输出
​    OFF_LOG_LEVEL (60000)：关闭所有log信息输出

各个logger可以通过`setLogLevel`设置自己的优先级，当某个logger的LogLevel设置成NOT_SET_LOG_LEVEL时，该logger会继承父logger的优先级，另外，如果定义了重名的多个logger, 对其中任何一个的修改都会同时改变其它logger。

例1

```c++
SharedAppenderPtr _append(new ConsoleAppender());
_append->setName("test");
Logger root = Logger::getRoot();
root.addAppender(_append);
Logger test = Logger::getInstance("test");
Logger subTest = Logger::getInstance("test.subtest");
LogLevelManager& llm = getLogLevelManager();
test.setLogLevel(INFO_LOG_LEVEL);
LOG4CPLUS_FATAL(root,"root:"<<llm.toString(root.getChainedLogLevel()));
LOG4CPLUS_FATAL(root,"test:"<<llm.toString(test.getChainedLogLevel()));
LOG4CPLUS_FATAL(root,"test.subtest:"<<llm.toString(subTest.getChainedLogLevel()));
```

例2：通过设置LogLevel来控制用户的log信息输出，级别逐渐提高

```c++
LOG4CPLUS_TRACE(Logger::getRoot(),"info");
LOG4CPLUS_DEBUG(Logger::getRoot(),"info");
LOG4CPLUS_INFO(Logger::getRoot(),"info");
LOG4CPLUS_WARN(Logger::getRoot(),"info");
LOG4CPLUS_ERROR(Logger::getRoot(),"info");
LOG4CPLUS_FATAL(Logger::getRoot(),"info");
Logger root = Logger::getRoot();
root.setLogLevel(ALL_LOG_LEVEL);   //全部显示
root.setLogLevel(TRACE_LOG_LEVEL); //全部显示
root.setLogLevel(ERROR_LOG_LEVEL); //只显示ERROR、FATAL
root.setLogLevel(OFF_LOG_LEVEL);   //显示log disabled
```

#### 自定义LogLevel

```c++
//DEBUG_LOG_LEVEL  < NEW_LOG_LEVEL < INFO_LOG_LEVEL
const LogLevel NEW_LOG_LEVEL = 15000;
//定义宏
#define LOG4CPLUS_NEW(logger, logEvent) 

if(logger.isEnabledFor(NEW_LOG_LEVEL)) { 

  log4cplus::tostringstream _log4cplus_buf; 

  _log4cplus_buf << logEvent; 

  logger.forcedLog(NEW_LOG_LEVEL, _log4cplus_buf.str(),   

      FILE, LINE);}

```

在loglevel.cxx中加入``#define _HELLO_STRING LOG4CPLUS_TEXT("HELLO") ``然后修改``log4cplus::tstring defaultLogLevelToStringMethod(LogLevel ll)``函数，增加一个判断：``case HELLO_LOG_LEVEL:return _HELLO_STRING``;重新编译log4cplus源代码。

## 四、 log4cplus脚本配置

log4cplus通过``PropertyConfigurator``类实现基于脚本配置实现对logger、appender和layout的配置及log环境的配置（通过程序也可以）。从而过滤log信息及利用脚本配置来配合实现性能测试。

### 1. Appender配置语法

(1) 设置名称，格式如下

```c++
log4cplus.appender.appenderName=fully.qualified.name.of.appender.class
```

例如：

```c++
log4cplus.appender.append_1=log4cplus::ConsoleAppender
log4cplus.appender.append_2=log4cplus::FileAppender
log4cplus.appender.append_3=log4cplus::RollingFileAppender
log4cplus.appender.append_4=log4cplus::DailyRollingFileAppender
log4cplus.appender.append_4=log4cplus::SocketAppender
```

(2) 设置过滤器Filter

过滤器``LogLevelMatchFilter``、``LogLevelRangeFilter``、``StringMatchFilter``。

**LogLevelMatchFilter**：过滤条件包括LogLevelToMatch和AcceptOnMatch（true|false）， 只有当log信息的LogLevel值与LogLevelToMatch相同，且AcceptOnMatch为true时才会匹配。

**LogLevelRangeFilter**：过滤条件包括LogLevelMin、LogLevelMax和AcceptOnMatch，只有当log信息的LogLevel在LogLevelMin、LogLevelMax之间同时AcceptOnMatch为true时才会匹配。

**StringMatchFilter**：过滤条件包括StringToMatch和AcceptOnMatch，只有当log信息的LogLevel值与tringToMatch对应的LogLevel值相同，且AcceptOnMatch为true时会匹配。

过滤条件处理机制：先deny再allow，后写的条件会被先执行。比如：

```c++
log4cplus.appender.append_1.filters.1=log4cplus::spi::LogLevelMatchFilter
log4cplus.appender.append_1.filters.1.LogLevelToMatch=TRACE
log4cplus.appender.append_1.filters.1.AcceptOnMatch=true
log4cplus.appender.append_1.filters.2=log4cplus::spi::DenyAllFilter
```

首先执行filters.2的过滤条件，关闭所有过滤器，然后执行filters.1，仅匹配TRACE信息。

(3) 设置Layout：不设置（默认SimpleLayout）、TTCCLayout或PatternLayout

设置TTCCLayout

```c++
log4cplus.appender.ALL_MSGS.layout=log4cplus::TTCCLayout
```

设置PatternLayout

```c++
log4cplus.appender.append_1.layout=log4cplus::PatternLayout
log4cplus.appender.append_1.layout.ConversionPattern=%d{%m/%d/%y %H:%M:%S,%Q} [%t] %-5p - %m%n 
```

### 2. Logger的配置语法：rootLogger和non-root logger

**RootLogger**

```c++
log4cplus.rootLogger=[LogLevel], appenderName, appenderName, ...
```

**non-root logger**

```c++
log4cplus.logger.logger_name=[LogLevel|INHERITED], appenderName, appenderName, ...
```

脚本方式使用：加载urconfig.properties自定义的配置文件。

```c++
PropertyConfigurator::doConfigure("urconfig.properties");
```

### 3. 简单例子

配置文件定义

```c++
log4cplus.rootLogger=TRACE, ALL_MSGS, TRACE_MSGS, DEBUG_INFO_MSGS, FATAL_MSGS
log4cplus.appender.FATAL_MSGS=log4cplus::RollingFileAppender
log4cplus.appender.FATAL_MSGS.File=fatal_msgs.log
log4cplus.appender.FATAL_MSGS.layout=log4cplus::TTCCLayout
log4cplus.appender.FATAL_MSGS.filters.1=log4cplus::spi::StringMatchFilter
log4cplus.appender.FATAL_MSGS.filters.1.StringToMatch=FATAL
log4cplus.appender.FATAL_MSGS.filters.1.AcceptOnMatch=true
log4cplus.appender.FATAL_MSGS.filters.2=log4cplus::spi::DenyAllFilter
```

程序中引用加载

```c++
Logger root = Logger::getRoot();
PropertyConfigurator::doConfigure("urconfig.properties");
```





## 参考文献

[Log4cplus基础知识]: http://www.cnblogs.com/shenchao/archive/2013/08/12/3253606.html

[^log4cplus的使用]: http://blog.csdn.net/augusdi/article/details/8989494