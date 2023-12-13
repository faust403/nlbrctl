# pragma once

# include <nlbrctl/connector.hpp>
# include <nlbrctl/interface.hpp>

# include <exception>
# include <filesystem>
# include <fstream>
# include <list>

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

        nlbrctl::connector _connector;

        bridge _bridge;
        std::string _name;
        bool _enabled = false;
        std::list<nlbrctl::interface> _interfaces;

        public:
            nl_bridge(void) = delete;

            nl_bridge(std::string name, const bridge& c_bridge) noexcept:
                _name(name), _enabled(true)
            {
                memcpy(&_bridge, &c_bridge, sizeof(bridge));

                update_interfaces();
            }

            inline std::string name(void) const noexcept {
                return _name;
            }

            inline const bridge& c_bridge(void) const noexcept {
                return _bridge;
            }

            inline bool enabled(void) const noexcept {
                return _enabled;
            }

            inline const std::list<nlbrctl::interface>& interfaces(void) const noexcept {
                return _interfaces;
            }

            operator bridge(void) const noexcept {
                return _bridge;
            }

            operator std::string(void) const noexcept {
                return _name;
            }

            operator const char*(void) const noexcept {
                return _name.c_str();
            }

            operator bool(void) const noexcept {
                return _enabled;
            }

            void update_interfaces(void) noexcept;

            void open(void) noexcept;
            void close(void) noexcept;

            void add_interface(std::string) noexcept;
            void del_interface(std::string) noexcept;

            using bridge_id_t = bridge_id;
            using bridge_t = bridge;
    };

    [[ nodiscard ]] extern std::optional<std::list<nlbrctl::nl_bridge>> get_bridges(void) noexcept;
}
