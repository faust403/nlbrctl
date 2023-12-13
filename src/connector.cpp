# include <nlbrctl/connector.hpp>

nlbrctl::connector::connector() noexcept:
    _socket(nl_socket_alloc(), _nl_sock_d_t{})
{
    if (int error = nl_connect(this->_socket.get(), NETLINK_ROUTE); error < 0) {
		nl_perror(error, "Unable to connect socket");
        return;
	}
}

void nlbrctl::connector::add_bridge(std::string name) noexcept {
    if(int error = rtnl_link_bridge_add(this->_socket.get(), name.c_str()); error < 0) {
        nl_perror(error, "error when adding bridge: ");
    }
}

void nlbrctl::connector::del_bridge(std::string name) noexcept {
    struct rtnl_link* link = rtnl_link_bridge_alloc();

    rtnl_link_set_name(link, name.c_str());

    if(int error = rtnl_link_delete(this->_socket.get(), link); error < 0) {
        nl_perror(error, "error when deleting bridge: ");
    }

    rtnl_link_put(link);
}

void nlbrctl::connector::add_interface(std::string bridge, std::string interface) noexcept {
    std::cout << if_nametoindex(interface.c_str()) << std::endl; // TODO
}

void nlbrctl::connector::del_interface(std::string bridge, std::string interface) noexcept {
    // TODO
}
