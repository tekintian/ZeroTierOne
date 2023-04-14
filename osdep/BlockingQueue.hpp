/*
 * Copyright (c)2019 ZeroTier, Inc.
 *
 * Use of this software is governed by the Business Source License included
 * in the LICENSE.TXT file in the project's root directory.
 *
 * Change Date: 2025-01-01
 *
 * On the date above, in accordance with the Business Source License, use
 * of this software will be governed by version 2.0 of the Apache License.
 */
/****/

#ifndef ZT_BLOCKINGQUEUE_HPP
#define ZT_BLOCKINGQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <vector>

#include "../node/b_mutex.h"

namespace ZeroTier {

/**
 * Simple C++11 thread-safe queue
 *
 * Do not use in node/ since we have not gone C++11 there yet.
 */
template <class T>
class BlockingQueue
{
public:
	BlockingQueue(void) : r(true) {}

	inline void post(T t) REQUIRES(!m)
	{
		zt::lock_guard<zt::mutex> lock(m);
		q.push(t);
		c.notify_one();
	}

	inline void postLimit(T t,const unsigned long limit)
	{   
		zt::unique_lock<zt::mutex> lock(m);
		for(;;) {
			if (q.size() < limit) {
				q.push(t);
				c.notify_one();
				break;
			}
			if (!r)
				break;
			gc.wait(lock);
		}
	}

	inline void stop(void) REQUIRES(!m)
	{
		zt::lock_guard<zt::mutex> lock(m);
		r = false;
		c.notify_all();
		gc.notify_all();
	}

	inline bool get(T &value)
	{
		zt::unique_lock<zt::mutex> lock(m);
		if (!r)
			return false;
		while (q.empty()) {
			c.wait(lock);
			if (!r) {
				gc.notify_all();
				return false;
			}
		}
		value = q.front();
		q.pop();
		gc.notify_all();
		return true;
	}

	inline std::vector<T> drain()
	{
		std::vector<T> v;
		while (!q.empty()) {
			v.push_back(q.front());
			q.pop();
		}
		return v;
	}

	enum TimedWaitResult
	{
		OK,
		TIMED_OUT,
		STOP
	};

	inline TimedWaitResult get(T &value,const unsigned long ms) REQUIRES(!m) {
		const std::chrono::milliseconds ms2{ms};
		zt::unique_lock<zt::mutex> lock(m);
		if (!r)
			return STOP;
		while (q.empty()) {
//			if (c.wait_for(lock,ms2) == std::cv_status::timeout)
//				return ((r) ? TIMED_OUT : STOP);
//			else if (!r)
//				return STOP;
            return STOP;
		}
		value = q.front();
		q.pop();
		return OK;
	}

	inline size_t size() const REQUIRES(m) {
		return q.size();
	}

private:
	std::queue<T> q GUARDED_BY(m);
//	mutable zt::mutex m;
	mutable std::condition_variable c GUARDED_BY(m);
	mutable std::condition_variable gc GUARDED_BY(m);
	std::atomic_bool r GUARDED_BY(m);
public:
    mutable zt::mutex m;
};

} // namespace ZeroTier

#endif
