# AcitveMQ C++ failover crash repro

During failover of the ActiveMQ server, if the client experiences high CPU load, it is possible that client will crash due to a race condition in Inactivity Monitor implementation of AMQC++ library. Issue reported under [AMQCPP-626](https://issues.apache.org/jira/browse/AMQCPP-626).

## Symptoms

An application crashes in `Inactivity Monitor` referencing destroyed object:

```windbg
0:031> k
  *** Stack trace for last set context - .thread/.cxr resets it
 # ChildEBP RetAddr  
00 1323f880 614b4f3b ucrtbase!abort+0x4b
{{01 1323f888 5a16e846 VCRUNTIME140!_purecall+0x1b [f:\dd\vctools\crt\vcruntime\src\misc\purevirt.cpp @ 29] }}
WARNING: Stack unwind information not available. Following frames may be wrong.
02 1323f8c8 5a0383d4 activemq_cpp!decaf::util::concurrent::Lock::~Lock+0x46
03 1323f8f0 5a067077 activemq_cpp!activemq::threads::CompositeTaskRunner::wakeup+0x74
04 1323f924 5a068188 activemq_cpp!activemq::transport::inactivity::InactivityMonitor::writeCheck+0x47
05 1323f938 5a15cd24 activemq_cpp!activemq::transport::inactivity::WriteChecker::run+0x48
06 1323fa20 5a0f25ac activemq_cpp!decaf::util::StringTokenizer::reset+0xa04
07 1323fa74 5a0f254b activemq_cpp!decaf::internal::util::concurrent::SynchronizableImpl::~SynchronizableImpl+0x4cc
08 1323faac 60c78824 activemq_cpp!decaf::internal::util::concurrent::SynchronizableImpl::~SynchronizableImpl+0x46b
09 1323fae8 76a27c04 ucrtbase!_crt_atexit+0x104
0a 1323fafc 777fad2f kernel32!BaseThreadInitThunk+0x24
0b 1323fb44 777facfa ntdll!__RtlUserThreadStart+0x2f
0c 1323fb54 00000000 ntdll!_RtlUserThreadStart+0x1b
```

## Workaround

None

## Patch

[PR#5](https://github.com/apache/activemq-cpp/pull/5). Waiting on upstream.

