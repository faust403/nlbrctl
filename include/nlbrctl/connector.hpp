# pragma once

# include <cstdint>
# include <cassert>
# include <cstdint>
# include <expected>
# include <optional>
# include <functional>

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <dirent.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/stat.h>

namespace nlbrctl {
    class connector {
        int socket__;

        using cb_t = std::function<int(const int)>;
        using opt_cb_t = std::optional<cb_t>;

        public:

            enum errors : std::uint8_t {
                INVALID_CONNECTOR,
                SOCKET_OPENING_ERROR,
                SOCKET_CLOSING_ERROR
            };

            ~connector(void) noexcept {
                this->close(std::nullopt);
            }

            std::expected<int, errors> open(const int, const int, const int, opt_cb_t) noexcept;

            std::expected<int, errors> close(opt_cb_t) noexcept;

            [[ nodiscard ]] inline int descriptor(void) noexcept {
                return socket__;
            }
    };
}
