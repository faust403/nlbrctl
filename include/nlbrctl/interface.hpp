# pragma once

# include <string>

namespace nlbrctl {
    class interface {
        std::string _name;

        public:
            interface(std::string const& name) noexcept:
                _name(name)
            { }

            operator std::string(void) const noexcept {
                return _name;
            }

            operator const char*(void) const noexcept {
                return _name.c_str();
            }

            const std::string& name(void) const noexcept {
                return _name;
            }
    };
}
