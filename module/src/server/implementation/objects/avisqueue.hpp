/*
 *  Created by Avi Saranga
 */

#include <boost/thread/thread.hpp>
#include <queue>

template<typename VideoFrameElement>
class avis_blocking_queue
{
private:
    std::queue<VideoFrameElement> _queue;
    mutable boost::mutex _mutex;
    boost::condition_variable _condvar;
public:

    void push(VideoFrameElement const& vf)
    {
        boost::mutex::scoped_lock lock(_mutex);
        _queue.push(vf);
        lock.unlock();
        _condvar.notify_one();
    }

    bool empty() const
    {
        boost::mutex::scoped_lock lock(_mutex);
        return _queue.empty();
    }

    void pop(VideoFrameElement& val)
    {
        boost::mutex::scoped_lock lock(_mutex);
        while(_queue.empty())
        {
            _condvar.wait(lock);
        }
        val=_queue.front();
        _queue.pop();
    }



};