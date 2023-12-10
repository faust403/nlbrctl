# pragma once

#include <linux/netlink.h>
# include <string>
# include <cstdint>
# include <cassert>
# include <cstdint>
# include <optional>
# include <functional>

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <dirent.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/stat.h>

# include <linux/rtnetlink.h>
# include <netlink/socket.h>
# include <netlink/route/link/bridge.h>

namespace nlbrctl {
    class connector {
        struct nl_sock* socket__;
        struct nlmsghdr header__;

        public:
            connector(void) noexcept;

            ~connector(void) noexcept;

            void add_bridge(std::string) noexcept;
            void del_bridge(std::string) noexcept;
            void add_interface(std::string, std::string) noexcept;
            void del_interface(std::string, std::string) noexcept;
    };
}
