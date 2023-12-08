# include <cstdlib>
# include <string>
# include <exception>
# include <filesystem>
# include <fstream>
# include <nlbrctl/bridge.hpp>

// can't be boolean because of strict policy for scandir, $ man scandir
static int isbridge(std::filesystem::path path) {
	return std::filesystem::exists(path/"bridge"); // it's necessary for /sys/devices/virtual/net/example_bridge to have 'bridge' directory
}

static int new_foreach_bridge(std::function<int(std::string)>&& iterator) {
	int count = 0;

	for(const std::filesystem::directory_entry& dir_entry : std::filesystem::recursive_directory_iterator(SYSFS_CLASS_NET)) {
		if(isbridge(dir_entry.path())) {
			count += 1;

			if(iterator(dir_entry.path().stem())) {
				break;
			}
		}
	}
	return count;
}

static int fetch_int(std::filesystem::path const& path) noexcept {
	std::ifstream file(path, std::ios::in);

	if(not file.is_open()) {
		return 0;
	}

	int value = 0;
	file >> value;

	return value;
}

static inline void __jiffies_to_tv(struct timeval *tv, unsigned long jiffies) {
	unsigned long long tvusec;

	tvusec = 10000ULL * jiffies;
	tv->tv_sec = tvusec / 1000000;
	tv->tv_usec = tvusec - 1000000 * tv->tv_sec;
}

static void fetch_tv(std::filesystem::path const& path, struct timeval* tv) {
	__jiffies_to_tv(tv, fetch_int(path));
}

static nlbrctl::nl_bridge::bridge_id_t fetch_id(std::filesystem::path const& path) {
	nlbrctl::nl_bridge::bridge_id_t id;
	std::ifstream file(path, std::ios::in);

	if (not file.is_open()) {
		fprintf(stderr, "%s: %s\n", path.string().c_str(), strerror(errno));
		std::terminate();
	}

	std::string bridge_id;
	file >> bridge_id;

	std::sscanf(bridge_id.c_str(), "%2hhx%2hhx.%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
				&id.prio[0], &id.prio[1], &id.addr[0],
				&id.addr[1], &id.addr[2], &id.addr[3],
				&id.addr[4], &id.addr[5]);
	return id;
}

static std::optional<nlbrctl::nl_bridge::bridge_t> get_bridge_info(std::string const& name) noexcept {
	nlbrctl::nl_bridge::bridge_t info;
	std::filesystem::path path(SYSFS_CLASS_NET);

	path /= name + "/bridge/";

	if(not std::filesystem::exists(path)) {
		return std::nullopt;
	}

	memset(&info, 0, sizeof(info));
	info.designated_root = fetch_id(path/"root_id");
	info.bridge_id = fetch_id(path/"bridge_id");
	info.root_path_cost = fetch_int(path/"root_path_cost");
	fetch_tv(path/"max_age", &info.max_age);
	fetch_tv(path/"hello_time", &info.hello_time);
	fetch_tv(path/"forward_delay", &info.forward_delay);
	fetch_tv(path/"max_age", &info.bridge_max_age);
	fetch_tv(path/"hello_time", &info.bridge_hello_time);
	fetch_tv(path/"forward_delay", &info.bridge_forward_delay);
	fetch_tv(path/"ageing_time", &info.ageing_time);
	fetch_tv(path/"hello_timer", &info.hello_timer_value);
	fetch_tv(path/"tcn_timer", &info.tcn_timer_value);
	fetch_tv(path/"topology_change_timer", &info.topology_change_timer_value);;
	fetch_tv(path/"gc_timer", &info.gc_timer_value);

	info.root_port = fetch_int(path/"root_port");
	info.stp_enabled = fetch_int(path/"stp_state");
	info.topology_change = fetch_int(path/"topology_change");
	info.topology_change_detected = fetch_int(path/"topology_change_detected");

	return info;
}

std::optional<std::list<nlbrctl::nl_bridge>> nlbrctl::get_bridges(void) noexcept {
    std::list<nlbrctl::nl_bridge> result;

	new_foreach_bridge([&result](std::string name) -> int {
		get_bridge_info(name).and_then([&]<typename T>(T&& bridge_ins) noexcept -> std::optional<nlbrctl::nl_bridge::bridge_t> {
			result.emplace_front(name, std::forward<T>(bridge_ins));
			return bridge_ins;
		});

		return 0;
	});
    return result;
}

static int foreach_bridge_interfaces(std::string const& brname, std::function<int(std::string)>&& callback) noexcept {
	int count = 0;
	std::filesystem::path path = SYSFS_CLASS_NET;

	path /= brname + "/brif/";

	if(not isbridge(path)) {
		return -1;
	}

	for(const std::filesystem::directory_entry& dir_entry : std::filesystem::recursive_directory_iterator(SYSFS_CLASS_NET)) {
		count += 1;

		if(callback(dir_entry.path().stem())) {
			break;
		}
	}

	return count;
}

void nlbrctl::nl_bridge::update_interfaces(void) noexcept {
	if(not enabled__) {
		return;
	}

	interfaces__.clear();

	foreach_bridge_interfaces(this->name__, [&](std::string port) -> int {
		interfaces__.emplace_front(port);
		return 0;
	});
}

void nlbrctl::nl_bridge::open(void) noexcept {
	if(enabled__) {
		return;
	}

	connector__.add_bridge(name__);
}

void nlbrctl::nl_bridge::close(void) noexcept {
	if(not enabled__) {
		return;
	}

	interfaces__.clear();
	connector__.del_bridge(name__);
}
