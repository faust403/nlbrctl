# include <nlbrctl/bridge.hpp>
# include <nlbrctl/connector.hpp>
# include <nlbrctl/nlbrctl.hpp>

# include <iostream>

// According to https://kernel.googlesource.com/pub/scm/network/bridge/bridge-utils/+/refs/tags/v1.7.1

int main(int argc, char** argv) {
    using T = std::optional<std::list<nlbrctl::nl_bridge>>;

    nlbrctl::connector connector;
    connector.add_bridge("connector_added");
    connector.add_bridge("connector_added");
}
