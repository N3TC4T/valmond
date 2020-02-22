#pragma once

#include "glog/logging.h"
#include <string.h>

#define LOG_INFO LOG(INFO)
#define LOG_WARN LOG(WARNING)
#define LOG_ERROR LOG(ERROR)
#define LOG_FATAL LOG(FATAL)

const std::string LOG_PATH = "/var/log/valmond/";

inline void
LogInit(const char* argv)
{
    google::InitGoogleLogging((const char*)argv);
#ifdef _DEBUG
    google::SetStderrLogging(google::GLOG_INFO);
#else
    google::SetStderrLogging(google::GLOG_WARNING);
#endif
    FLAGS_colorlogtostderr = true;
    google::SetLogDestination(google::GLOG_INFO, LOG_PATH.c_str());
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 100;
    FLAGS_stop_logging_if_full_disk = true;
    google::SetLogFilenameExtension("log_");

    // for capturing core dump
    google::InstallFailureSignalHandler();

    // customize the destination by InstallFailureWriter()
    google::InstallFailureWriter([](const char* data, int size) {
        std::string str = std::string(data, size);
        LOG(ERROR) << str;
    });
}
