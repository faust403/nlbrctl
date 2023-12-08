# include <nlbrctl/connector.hpp>

// on success return 0
// on error return some of nlbrctl::connector::* errors
// based on callback return value return error or 0
std::expected<int, nlbrctl::connector::errors> nlbrctl::connector::open(const int domain, const int type, const int protocol, nlbrctl::connector::opt_cb_t callback) noexcept {
    if(this->socket__ = ::socket(domain, type, protocol); this->socket__ < 0) { // try to open connection
        return nlbrctl::connector::SOCKET_OPENING_ERROR;
    }

    if(not callback.has_value()) {
        return 0; // ok
    }

    return callback.and_then([&](auto&& callback) noexcept -> std::optional<int> { // if success call callback function
        if(callback(this->socket__) < 0) {
            this->close(std::nullopt);

            return nlbrctl::connector::SOCKET_OPENING_ERROR;
        }

        return 0; // ok
    }).value();
}

// on success return 0
// on error return some of nlbrctl::connector::* errors
// based on callback return value return error or 0
std::expected<int, nlbrctl::connector::errors> nlbrctl::connector::close(nlbrctl::connector::opt_cb_t callback) noexcept {
    if(not callback.has_value()) {
        if(::close(this->socket__) < 0) {
            return nlbrctl::connector::SOCKET_CLOSING_ERROR;
        }

        this->socket__ = -1;
        return 0;
    }

    return callback.and_then([&](auto&& callback) noexcept -> std::optional<int> { // if success call callback function
        const int ret = callback(this->socket__);

        if(ret < 0 and ::close(this->socket__) < 0) {
            return nlbrctl::connector::SOCKET_CLOSING_ERROR;
        }
        return 0; // ok
    }).value();
}
