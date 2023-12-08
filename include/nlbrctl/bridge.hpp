# pragma once

# include <nlbrctl/connector.hpp>
# include <nlbrctl/interface.hpp>

# include <string>
# include <cstdlib>
# include <iostream>
# include <cstdint>
# include <expected>
# include <functional>
# include <list>
# include <string_view>

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>

# include <net/if.h>
extern unsigned int if_nametoindex(const char* __ifname);
extern char* if_indextoname(unsigned int __ifindex, char* __ifname);

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <linux/if.h>
# include <linux/sockios.h>
# include <sys/time.h>
# include <sys/ioctl.h>
# include <linux/if_bridge.h>

# define MAX_BRIDGES 1024
# define SYSFS_PATH_MAX 256
# define SYSFS_CLASS_NET "/sys/class/net/"

namespace nlbrctl {
    class nl_bridge {
        struct bridge_id {
            unsigned char prio[2];
            unsigned char addr[6];
        };

        struct bridge {
            struct bridge_id designated_root;
            struct bridge_id bridge_id;

            unsigned root_path_cost;

            struct timeval max_age;
            struct timeval hello_time;
            struct timeval forward_delay;
            struct timeval bridge_max_age;
            struct timeval bridge_hello_time;
            struct timeval bridge_forward_delay;

            uint16_t root_port;

            unsigned char stp_enabled;
            unsigned char topology_change;
            unsigned char topology_change_detected;
            struct timeval ageing_time;

            struct timeval hello_timer_value;
            struct timeval tcn_timer_value;
            struct timeval topology_change_timer_value;
            struct timeval gc_timer_value;
        };

        bridge bridge__;
        std::string name__;
        std::list<nlbrctl::interface> interfaces__;

        public:
            nl_bridge(void) = delete;

            nl_bridge(std::string name, const bridge& c_bridge) noexcept:
                name__(name)
            {
                memcpy(&bridge__, &c_bridge, sizeof(bridge));

                update_interfaces();
            }

            inline std::string name(void) const noexcept {
                return name__;
            }

            inline bridge c_bridge(void) const noexcept {
                return bridge__;
            }

            inline const std::list<nlbrctl::interface>& interfaces(void) const noexcept {
                return interfaces__;
            }

            operator bridge(void) const noexcept {
                return bridge__;
            }

            operator std::string(void) const noexcept {
                return name__;
            }

            operator const char*(void) const noexcept {
                return name__.c_str();
            }

            void update_interfaces(void) noexcept;

            void add_interface(std::string) noexcept;
            void del_interface(std::string) noexcept;

            void open(void) noexcept;
            void close(void) noexcept;

            using bridge_id_t = bridge_id;
            using bridge_t = bridge;
    };

    enum SCAN_BRIDGES_ERRORS : std::uint8_t {
        ERROR_SCAN_BRIDGES
    };

    [[ nodiscard ]] extern std::optional<std::list<nlbrctl::nl_bridge>> get_bridges(void) noexcept;
}
