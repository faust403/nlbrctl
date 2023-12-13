# pragma once

# include <memory>
# include <optional>
# include <iostream>
# include <functional>

# include <net/if.h>

# include <linux/netlink.h>
# include <linux/rtnetlink.h>
# include <netlink/socket.h>
# include <netlink/route/link/bridge.h>
# include <netlink/route/addr.h>
# include <netlink/route/rtnl.h>
# include <netlink/route/route.h>

namespace nlbrctl {
    class connector {
        struct _nl_sock_d_t {
            void operator()(struct nl_sock* p) const
            {
                nl_socket_free(p);
            };
        };

        std::unique_ptr<struct nl_sock, _nl_sock_d_t> _socket;
        struct nlmsghdr _header;

        public:
            connector(void) noexcept;

            void add_bridge(std::string) noexcept;
            void del_bridge(std::string) noexcept;
            void add_interface(std::string, std::string) noexcept;
            void del_interface(std::string, std::string) noexcept;
    };
}
