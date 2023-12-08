#include "include/nlbrctl/bridge.hpp"
# include <nlbrctl/bridge.hpp>
# include <nlbrctl/connector.hpp>
# include <nlbrctl/nlbrctl.hpp>

# include <iostream>

// According to https://kernel.googlesource.com/pub/scm/network/bridge/bridge-utils/+/refs/tags/v1.7.1

int main(int argc, char** argv) {
    nlbrctl::get_bridges().and_then([](auto const list) noexcept -> std::optional<decltype(list)> {
        for(auto item : list) {
            std::cout << item.name() << std::endl;
        }
        return list;
    });
}
