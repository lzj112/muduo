// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/net/Poller.h>
#include <muduo/net/poller/PollPoller.h>
#include <muduo/net/poller/EPollPoller.h>

#include <stdlib.h>

using namespace muduo::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
  //getenv函数获取环境变量, 如果环境变量中没有设置MUDUO_USE_POLL,默认使用epoll,
  if (::getenv("MUDUO_USE_POLL")) //没有该环境变量就返回NULL
  {
    return new PollPoller(loop);
  }
  else
  {
    return new EPollPoller(loop);
  }
}
