// Copyright (c) 2021 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

//! [include]
#include "topic_data.hpp"

#include "iceoryx_dust/posix_wrapper/signal_watcher.hpp"
#include "iceoryx_posh/popo/subscriber.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"
//! [include]

#include <iostream>

int main(int argc, char **argv)
{
    char APP_NAME[32];
    if (argc > 1) {
	    memcpy(APP_NAME, argv[1], strlen(argv[1]));
    } else {
	    char *na = "iox-cpp-subscriber-helloworld1";
	    memcpy(APP_NAME, na, strlen(na));
	    printf("%s %s\n", na, APP_NAME);
    }

	std::clock_t start, end;
	float total = 0, cnt = 0;


    iox::runtime::PoshRuntime::initRuntime(APP_NAME);
    char dummy_gpu_buf[RADAROBJECT_MEM_SIZE];

//    iox::popo::SubscriberOptions subscriberOptions;
//    subscriberOptions.queueCapacity = 1U;
//    subscriberOptions.historyRequest = 5U;
//    subscriberOptions.requiresPublisherHistorySupport = false;
//    subscriberOptions.queueFullPolicy = iox::popo::QueueFullPolicy::BLOCK_PRODUCER;

    iox::popo::Subscriber<RadarObject> subscriber({"Radar", "FrontLeft", "Object"});

    while (!iox::posix::hasTerminationRequested())
    {

            start = std::clock();

        auto takeResult = subscriber.take();
        if (takeResult.has_value())
        {

        	std::cout << APP_NAME << " got value: " << takeResult.value()->x << std::endl;
            memcpy(dummy_gpu_buf, takeResult.value()->mem, RADAROBJECT_MEM_SIZE);

            cnt++;
        }
        else
        {
            if (takeResult.error() == iox::popo::ChunkReceiveResult::NO_CHUNK_AVAILABLE)
            {
                std::cout << "No chunk available." << std::endl;
            }
            else
            {
                std::cout << "Error receiving chunk." << std::endl;
            }
        }

        end = std::clock();
        total += (float(end - start)) / CLOCKS_PER_SEC;


        std::cout << "total time " << total << " cnt " << cnt << " Bandwidth " << float(cnt * 128)/(total*1024) << " GBps" <<  std::endl;


//	subscriber
//	    .take()
//	    .and_then([](auto& sample) {
//		std::cout << " got value: " << sample->x << std::endl;
//		memcpy(dummy_gpu_buf, sample->mem, RADAROBJECT_MEM_SIZE);
//	    })
//	    .or_else([](auto& result) {
//		if (result != iox::popo::ChunkReceiveResult::NO_CHUNK_AVAILABLE)
//		{
//		    std::cout << "Error receiving chunk." << std::endl;
//		}
//	    });

//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return (EXIT_SUCCESS);
}
