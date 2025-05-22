#include <logger.h>

Logger& Logger::getInstance() {
    static Logger logger;
    return logger;
}

Logger::Logger() {
    logFile_.open("data/log.txt", std::ios::app);  // 以追加模式打开文件
    // 判断文件是否成功打开
    if (!logFile_.is_open()) {
        // 抛出异常
        throw std::runtime_error("Filed to open log file");
    }
}

Logger::~Logger() {
    // 关闭文件
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::log(const std::string& message) {
    // 为线程安全自动加锁
    std::lock_guard<std::mutex> lock(mutex_);

    if (logFile_.is_open()) {
        // 获取当前时间
        auto now = std::chrono::system_clock::now();
        // 将now转换为time_t类型的时间戳
        auto now_time = std::chrono::system_clock::to_time_t(now);
        char buffer[100];
        // strftime是C语言中的标准时间格式化函数
        // localtime用于把
        // time_t类型的时间戳，转换成本地时间（人类能看懂的结构体）。
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S",
                      std::localtime(&now_time));
        // 将char类型的buffer转换为std::string类型
        logFile_ << std::string(buffer) << " : " << message << std::endl;
    }
}