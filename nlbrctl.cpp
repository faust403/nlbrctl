# include <nlbrctl/nlbrctl.hpp>

# include <iostream>
# include <netlink/route/route.h>

// According to https://kernel.googlesource.com/pub/scm/network/bridge/bridge-utils/+/refs/tags/v1.7.1

int main(int argc, char** argv) {
    using T = std::optional<std::list<nlbrctl::nl_bridge>>;

    nlbrctl::connector con;

    con.add_bridge("example_bridge1");
    con.add_bridge("example_bridge2");
    con.add_bridge("example_bridge3");

    con.del_bridge("example_bridge2");

    nlbrctl::get_bridges()
    .and_then([](auto&& bridges) noexcept -> T {
        for(auto& bridge : bridges) {
            std::cout << bridge.name() << std::endl;

            for(auto& interface: bridge.interfaces()) {
                std::cout << "   " << interface.name() << std::endl;
            }
        }
        return bridges;
    });
}
