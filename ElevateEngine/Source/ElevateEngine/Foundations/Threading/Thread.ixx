export module Elevate.Foundations.Threading;

export namespace Elevate
{
    enum class ThreadState : int
    {
        Idle = 0,
        Running = 1,
        Finished = 2,
        Suspended = 3,
        Terminated = 4
    };
}