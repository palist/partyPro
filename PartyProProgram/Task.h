//
// Copyright (c) 2010-2012 Ping (vikeylee@qq.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TASK_H_
#define TASK_H_
#include <boost/algorithm/string.hpp>
#include <boost/pool/singleton_pool.hpp> 
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp> 
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
///业务
class CTask 
	: private boost::noncopyable
{
public:
	typedef CTask this_type;

protected:
	CTask()		
	{
		/*work_threads_.create_thread(
			boost::bind(&this_type::run_io_service,
			this, boost::ref(io_service_)));*/
	}

	virtual ~CTask()
	{		
		
		//work_threads_.join_all();
	}
protected:
	void run_io_service(boost::asio::io_service& io_service)
	{
		io_service.run();
	}
	void CloseService()
	{
        io_service_.stop();
	}

protected:
	boost::asio::io_service         io_service_; //I/O操作对象

private:
//	boost::asio::io_service::work   work_;///I/O流 工作对象
	//boost::thread_group             work_threads_;
};

#endif // TASK_H_
