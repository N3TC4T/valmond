#pragma once

#include "glog/logging.h"
#include <string.h>

const std::string LOG_PATH = "/var/log/testloooog";

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
    FLAGS_max_log_size = 512;
    FLAGS_stop_logging_if_full_disk = true;
}
