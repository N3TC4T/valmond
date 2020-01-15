#include "disk.hpp"

#include <blkid/blkid.h>
#include <cmath>
#include <ctime>
#include <ext2fs/ext2fs.h>
#include <iomanip>
#include <iostream>
#include <sys/statvfs.h>
#include <sys/vfs.h>
#include <unistd.h>

#include "common/utils.hpp"

namespace collectors {
namespace disk {

uint64_t
getDiskFree(const char* path)
{
    struct statvfs stfs;

    if (statvfs(path, &stfs) == -1)
    {
        return 0.0;
    }
    uint64_t t = (uint64_t)stfs.f_bsize * (uint64_t)stfs.f_bfree;
    return t / (1024 * 1024);
}

uint64_t
getDiskTotal(const char* path)
{
    struct statvfs stfs;
    if (statvfs(path, &stfs) == -1)
    {
        return 0;
    }
    uint64_t t = (uint64_t)stfs.f_blocks * (uint64_t)stfs.f_bsize;
    return t / (1024 * 1024);
}

DevInfo
getDevInfo(blkid_dev dev)
{
    blkid_tag_iterate iter;
    const char *type, *value, *devname;
    int mount_flags;
    char mtpt[1024];
    const char* fs_type = "";
    errcode_t retval;

    unsigned long long freeMb;
    unsigned long long totalMb;
    unsigned long long usedMb;
    double usedPercent;
    std::string name;

    devname = blkid_dev_devname(dev);
    if (access(devname, F_OK))
        return {};

    /* Get the uuid, label, type */
    iter = blkid_tag_iterate_begin(dev);
    while (blkid_tag_next(iter, &type, &value) == 0)
    {
        if (!strcmp(type, "TYPE"))
            fs_type = value;
    }
    blkid_tag_iterate_end(iter);

    /* Get the mount point */
    mtpt[0] = 0;
    retval = ext2fs_check_mount_point(devname, &mount_flags, mtpt, sizeof(mtpt));
    if (retval == 0)
    {
        if (mount_flags & EXT2_MF_MOUNTED)
        {
            if (!mtpt[0])
                return {};
        }
        else if (mount_flags & EXT2_MF_BUSY)
            return {};
        else
            return {};
    }

    totalMb = getDiskTotal(mtpt);
    freeMb = getDiskFree(mtpt);
    usedMb = totalMb - freeMb;

    // replace /dev/
    name = devname;
    name.replace(name.find("/dev/"), sizeof("/dev/") - 1, "");

    if (totalMb > 0 && usedMb > 0)
    {
        usedPercent = (double(usedMb) / double(totalMb) * 100.0);
        return {name, mtpt, fs_type, totalMb, freeMb, usedMb, usedPercent};
    }

    return {};
}

Json::Value
getDiskInfo()
{
    Json::Value result;

    blkid_dev_iterate iter;
    blkid_dev dev;
    blkid_cache cache = NULL;
    char *search_type = NULL, *search_value = NULL;
    char* read = NULL;

    blkid_get_cache(&cache, read);

    blkid_probe_all(cache);

    std::stringstream ss;
    ss.precision(100);  // weird, but seems to work

    ss << "<devices timestamp=\"";
    std::time_t t = std::time(0);
    ss << t << "\">";
    iter = blkid_dev_iterate_begin(cache);
    blkid_dev_set_search(iter, search_type, search_value);
    while (blkid_dev_next(iter, &dev) == 0)
    {
        dev = blkid_verify(cache, dev);
        if (!dev)
        {
            continue;
        }

        auto devinfo = getDevInfo(dev);

        if (devinfo.name != "")
        {
            Json::Value obj;
            obj["name"] = devinfo.name;
            obj["path"] = devinfo.path;
            obj["fstype"] = devinfo.fstype;
            obj["total"] = devinfo.total;
            obj["free"] = devinfo.free;
            obj["used"] = devinfo.used;
            obj["percent"] = floatDecimalPoint(devinfo.percent);
            result.append(obj);
        }
    }
    blkid_dev_iterate_end(iter);
    free(cache);

    return result;
};

}  // namespace disk
}  // namespace collectors

