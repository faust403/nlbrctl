# include <nlbrctl/connector.hpp>

nlbrctl::connector::connector(void) noexcept:
    socket__(nl_socket_alloc())
{
    if (int error = nl_connect(this->socket__, NETLINK_ROUTE); error < 0) {
		nl_perror(error, "Unable to connect socket");
        return;
	}
}

nlbrctl::connector::~connector(void) noexcept {
    nl_socket_free(socket__);
}

void nlbrctl::connector::add_bridge(std::string name) noexcept {
    if(int error = rtnl_link_bridge_add(this->socket__, name.c_str()); error < 0) {
        nl_perror(error, "error when adding bridge: ");
    }
}

void nlbrctl::connector::del_bridge(std::string name) noexcept {
    struct rtnl_link* link = rtnl_link_bridge_alloc();

    rtnl_link_set_name(link, name.c_str());

    if(int error = rtnl_link_delete(this->socket__, link); error < 0) {
        nl_perror(error, "error when deleting bridge: ");
    }
    rtnl_link_put(link);
}
