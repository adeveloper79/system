/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <linux/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>


#define LOG_TAG "ThroughputMonitor"
#define DBG 1

#include <cutils/log.h>

#include "ThroughputMonitor.h"
#include "PerfController.h"

ThroughputMonitor::ThroughputMonitor() {
    mThread = 0;
    mRunning = 0;
}

ThroughputMonitor::~ThroughputMonitor() {
    mThread = 0;
    mRunning = 0;
}

int ThroughputMonitor::start() {
    if(mThread == 0){
        pthread_create(&mThread, NULL, ThroughputMonitor::threadStart, this);
    }else{
        ALOGW("ThroughputMonitor is already running");
    }

    return 0;
}

void ThroughputMonitor::stop() {
  if(mThread == 0) {
        ALOGI("ThroughputMonitor has already stop");
        return;
    }

    ALOGI("ThroughputMonitor trys to stop!");
    if(PerfController::get_lowpower_perfhandle() != -1) {
        PerfController::exit_little_cpu();
        PerfController::restore_ack_reduction();
    }
    mRunning = 0;
    mThread = 0;
    return;
}

void* ThroughputMonitor::threadStart(void* obj) {
    ThroughputMonitor* monitor = reinterpret_cast<ThroughputMonitor*>(obj);

    monitor->run();
    //delete monitor;

    pthread_exit(NULL);
    ALOGI("ThroughputMonitor thread exit!");
    return NULL;
}

long ThroughputMonitor::readCount(char const* filename) {
    char buf[80];
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        if (errno != ENOENT) ALOGE("Can't open %s: %s", filename, strerror(errno));
        return -1;
    }

    int len = read(fd, buf, sizeof(buf) - 1);
    if (len < 0) {
        ALOGE("Can't read %s: %s", filename, strerror(errno));
        close(fd);
        return -1;
    }

    close(fd);
    buf[len] = '\0';
    return atoll(buf);
}


void ThroughputMonitor::run() {
    char filename[80];
    int index = 0;
    const char *ack_setting = "3"; //data:ack = (3+1):1
    unsigned long current = 0;
    unsigned long last = 0;
    unsigned long lowpower_threshold = 35<<18; // k*2^17 bytes <-> k Mbps
	const char* iface_list[] = {
		"ccmni0",
		"ccmni1",
		"ccmni2",
		0
	};

    mRunning = 1;
    ALOGI("ThroughputMonitor is running, thread id = %d!", gettid());
    while (mRunning){
        if(!PerfController::is_testsim()) {
            //ALOGI("testsim is not checked, thoughput monitor suspend");
            if(PerfController::get_lowpower_perfhandle() != -1) {
                PerfController::exit_little_cpu();
                PerfController::restore_ack_reduction();
            }
            sleep(2);
            continue;
        }
        while (iface_list[index] != 0) {
            snprintf(filename, sizeof(filename), "/sys/class/net/%s/statistics/rx_bytes", iface_list[index]);
            long number = readCount(filename);
            if (number >= 0) {
                current += number;
            }
        index++;
        }
        //ALOGI("current rx %lu, last rx %lu", current, last);
        if((current - last) > lowpower_threshold) {
            if((PerfController::get_lowpower_perfhandle() == -1) && (PerfController::get_tether_perfhandle() == -1)) {
                PerfController::enter_little_cpu();
                PerfController::set_ack_reduction(ack_setting);
                ALOGI("throughput reaches %lu bps and exceeds limitation", (current-last)*8L);
            }
        } else {
            if(PerfController::get_lowpower_perfhandle() != -1) {
                PerfController::exit_little_cpu();
                PerfController::restore_ack_reduction();
                ALOGI("throughput reaches %lu bps and belows limitation", (current-last)*8L);
            }
        }
        //ALOGI("ThroughputMonitor: %lu bps", (current-last)*8L);
        last = current;
        current = 0;
        index = 0;

        usleep(1000000);
    }
}


