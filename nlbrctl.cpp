# include <nlbrctl/bridge.hpp>
# include <nlbrctl/connector.hpp>
# include <nlbrctl/nlbrctl.hpp>

# include <iostream>

// According to https://kernel.googlesource.com/pub/scm/network/bridge/bridge-utils/+/refs/tags/v1.7.1

int main(int argc, char** argv) {
    using T = std::optional<std::list<nlbrctl::nl_bridge>>;

    nlbrctl::get_bridges()
    .and_then([](auto&& list) noexcept -> T {
        for(auto& item : list) {
            std::cout << item.name() << std::endl;
        }
        return list;
    })
    .and_then([](auto&& list) noexcept -> T {
        for(auto& item : list) {
            for(auto& iface : item.interfaces()) {
                std::cout << iface.name() << std::endl;
            }
        }
        return list;
    });

}
