#ifndef VRC_RESOURCE_H
#define VRC_RESOURCE_H

#include <cstddef>
#include <string>

namespace VRC {
    class VResource;
}

class VRC::VResource {
public:
    VResource(const char *data, const size_t length) : resourceData(data), resourceLength(length) {}

    const char *const &data() const {
        return resourceData;
    }

    const size_t &size() const {
        return resourceLength;
    }

    const char *begin() const {
        return resourceData;
    }

    const char *end() const {
        return resourceData + resourceLength;
    }

    std::string toString() const {
        return std::string(data(), size());
    }

private:
    const char *resourceData;
    const size_t resourceLength;

};

#define VRC_LOAD_RESOURCE(RESOURCE) ([]() {                                           \
    extern const char _vrc_resource_##RESOURCE[];                                   \
    extern const size_t _vrc_resource_##RESOURCE##_len;                               \
    return VRC::VResource(_vrc_resource_##RESOURCE, _vrc_resource_##RESOURCE##_len);  \
})()

#endif //VRC_RESOURCE_H
