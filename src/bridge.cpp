# include <cstdlib>
# include <exception>
# include <nlbrctl/bridge.hpp>

// can't be boolean because of strict policy for scandir, $ man scandir
static int isbridge(const struct dirent* entry) {
	char path[PATH_MAX];
	struct stat st;
	int ret, saved_errno;

	if (entry->d_name[0] == '.' and (entry->d_name[1] == '\0' or (entry->d_name[1] == '.' and entry->d_name[2] == '\0'))) {
		return false;
	}

	snprintf(path, PATH_MAX, SYSFS_CLASS_NET "%s/bridge", entry->d_name); // it's necessary for /sys/devices/virtual/net/example_bridge to have 'bridge' directory

	saved_errno = errno;
	ret = (stat(path, &st) == 0 and S_ISDIR(st.st_mode)); // is /sys/devices/virtual/net/example_bridge/bridge exists ?
	errno = saved_errno;
	return static_cast<bool>(ret);
}

static int new_foreach_bridge(std::function<int(std::string)>&& iterator) {
	struct dirent **namelist;
	int i, count = 0;

	count = scandir(SYSFS_CLASS_NET, &namelist, isbridge, alphasort);
	if (count < 0) {
		return -1;
	}

	for (i = 0; i < count; i++) {
		if (iterator(namelist[i]->d_name)) {
			break;
		}
	}

	for (i = 0; i < count; i++) {
		free(namelist[i]);
	}
	free(namelist);

	return count;
}

static inline void __jiffies_to_tv(struct timeval *tv, unsigned long jiffies) {
	unsigned long long tvusec;

	tvusec = 10000ULL*jiffies;
	tv->tv_sec = tvusec/1000000;
	tv->tv_usec = tvusec - 1000000 * tv->tv_sec;
}

static FILE* fpopen(std::string const& path) noexcept {
	return fopen(path.c_str(), "r");
}

static int fetch_int(std::string const& path) noexcept {
	FILE* f = fpopen(path);
	int value = -1;

	if (not f) {
		return 0;
	}

	fscanf(f, "%i", &value);
	fclose(f);
	return value;
}

static nlbrctl::nl_bridge::bridge_id_t fetch_id(std::string const& path) {
	nlbrctl::nl_bridge::bridge_id_t id;
	FILE* f = fpopen(path);

	if (not f) {
		fprintf(stderr, "%s: %s\n", path.c_str(), strerror(errno));
		std::terminate();
	} else {
		fscanf(f, "%2hhx%2hhx.%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
		       &id.prio[0], &id.prio[1],
		       &id.addr[0], &id.addr[1], &id.addr[2],
		       &id.addr[3], &id.addr[4], &id.addr[5]);
		fclose(f);
	}
	return id;
}

static void fetch_tv(std::string const& path, struct timeval* tv) {
	__jiffies_to_tv(tv, fetch_int(path));
}

static std::optional<nlbrctl::nl_bridge::bridge_t> get_bridge_info(std::string_view name) noexcept {
	nlbrctl::nl_bridge::bridge_t info;
	DIR* dir;

	std::string path = SYSFS_CLASS_NET;
	path += name.data();
	path += "/bridge/";

	if(dir = opendir(path.c_str()); dir == NULL) {
		return std::nullopt;
	}

	memset(&info, 0, sizeof(info));
	info.designated_root = fetch_id(path + "root_id");
	info.bridge_id = fetch_id(path + "bridge_id");
	info.root_path_cost = fetch_int(path + "root_path_cost");
	fetch_tv(path + "max_age", &info.max_age);
	fetch_tv(path + "hello_time", &info.hello_time);
	fetch_tv(path + "forward_delay", &info.forward_delay);
	fetch_tv(path + "max_age", &info.bridge_max_age);
	fetch_tv(path + "hello_time", &info.bridge_hello_time);
	fetch_tv(path + "forward_delay", &info.bridge_forward_delay);
	fetch_tv(path + "ageing_time", &info.ageing_time);
	fetch_tv(path + "hello_timer", &info.hello_timer_value);
	fetch_tv(path + "tcn_timer", &info.tcn_timer_value);
	fetch_tv(path + "topology_change_timer", &info.topology_change_timer_value);;
	fetch_tv(path + "gc_timer", &info.gc_timer_value);

	info.root_port = fetch_int(path + "root_port");
	info.stp_enabled = fetch_int(path + "stp_state");
	info.topology_change = fetch_int(path + "topology_change");
	info.topology_change_detected = fetch_int(path + "topology_change_detected");

	closedir(dir);

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
