// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_TCPCLIENT_H
#define MUDUO_NET_TCPCLIENT_H

#include <muduo/base/Mutex.h>
#include <muduo/net/TcpConnection.h>

namespace muduo
{
namespace net
{

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient : noncopyable
{
 public:
  // TcpClient(EventLoop* loop);
  // TcpClient(EventLoop* loop, const string& host, uint16_t port);
  TcpClient(EventLoop* loop,
            const InetAddress& serverAddr,
            const string& nameArg);
  ~TcpClient();  // force out-line dtor, for std::unique_ptr members.

  void connect();
  void disconnect();
  void stop();

  TcpConnectionPtr connection() const
  {
    MutexLockGuard lock(mutex_);
    return connection_;
  }

  EventLoop* getLoop() const { return loop_; }
  bool retry() const { return retry_; }
  void enableRetry() { retry_ = true; }

  const string& name() const
  { return name_; }

  /// Set connection callback.
  /// Not thread safe.
  void setConnectionCallback(ConnectionCallback cb)
  { connectionCallback_ = std::move(cb); }

  /// Set message callback.
  /// Not thread safe.
  void setMessageCallback(MessageCallback cb)
  { messageCallback_ = std::move(cb); }

  /// Set write complete callback.
  /// Not thread safe.
  void setWriteCompleteCallback(WriteCompleteCallback cb)
  { writeCompleteCallback_ = std::move(cb); }

 private:
  /// Not thread safe, but in loop
  void newConnection(int sockfd);
  /// Not thread safe, but in loop
  void removeConnection(const TcpConnectionPtr& conn);

  //用户创建传入的EventLoop
  EventLoop* loop_;
  //指向Connector的shared_ptr
  ConnectorPtr connector_; // avoid revealing Connector
  //用户指定的名字
  const string name_;
  //这些回调函数都需要显示设置
  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  WriteCompleteCallback writeCompleteCallback_;
  //连接断开后是否重连
  bool retry_;   // atomic
  bool connect_; // atomic
  // always in loop thread
  int nextConnId_;
  mutable MutexLock mutex_;
  TcpConnectionPtr connection_ GUARDED_BY(mutex_);
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_TCPCLIENT_H
