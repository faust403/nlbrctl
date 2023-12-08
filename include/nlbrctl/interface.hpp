# pragma once

# include <string>

namespace nlbrctl {
    class interface {
        std::string name__;

        public:
            interface(std::string const& name) noexcept:
                name__(name)
            { }

            operator std::string(void) const noexcept {
                return name__;
            }

            operator const char*(void) const noexcept {
                return name__.c_str();
            }

            const std::string& name(void) const noexcept {
                return name__;
            }
    };
}
