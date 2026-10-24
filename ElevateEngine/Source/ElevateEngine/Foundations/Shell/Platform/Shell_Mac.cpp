module;

#ifdef EE_PLATFORM_MACOS
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

module Elevate.Foundations.Shell;
import :Native;

#ifdef EE_PLATFORM_MACOS
namespace Elevate::Shell::Native
{
    bool Open(const std::string &target)
    {
        pid_t pid = fork();
        if (pid < 0)
            return false;

        if (pid == 0)
        {
            if (fork() == 0)
            {
                execlp("open", "open", target.c_str(), nullptr);
                _exit(127);
            }
            _exit(0);
        }

        int status = 0;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}
#endif
