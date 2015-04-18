#include <string>
#include <queue>
#include <vector>
#include <cstdlib>
#include <boost/noncopyable.hpp>
#include "c_queue.cpp"

#pragma once

template<typename e>
class scatter : private boost::noncopyable 
{
    public: 
        typedef std::shared_ptr< concurrent_queue<e> > q_ptr;
        q_ptr getQueue()
        {
            q_ptr q(new concurrent_queue<e>());
            list.push_back(q);
            return q;
        }
        void push(e element) 
        {
            if(list.size() == 0)
                return;
            srand (time(NULL));
            int pos     =    rand() % list.size();
            auto q  = list.at(pos);
            q->push(element);
        }
    private:
        int number;
        std::vector<q_ptr> list;
};
