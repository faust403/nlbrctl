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
    header__ = {
        .nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
		.nlmsg_flags = NLM_F_REQUEST
    };

    if(int error = rtnl_link_bridge_add(this->socket__, name.c_str()); error < 0) {
        nl_perror(error, "error when adding bridge: ");
    }
}

void nlbrctl::connector::del_bridge(std::string name) noexcept {

}
