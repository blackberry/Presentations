/*
 * Copyright (c) 2013 Research In Motion.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <sys/time.h>
#include <stdlib.h>

#include "Now.hpp"

Now::Now() {
}

Now::~Now() {
}

long long Now::now() {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	long long now = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
	return now;
}

