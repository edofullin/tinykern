#pragma once

#define KLOG_INFO(fmt, ...)             \
    kprintf(fmt "\n", ##__VA_ARGS__)    \

