#pragma once

#define KLOG_INFO(fmt, args...) \
    KLOG_INFO(fmt "\n", ##args)