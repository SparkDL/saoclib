#ifndef SAOCLIB_CPP_CLCONTEXT_H
#define SAOCLIB_CPP_CLCONTEXT_H

#include "AOCLUtils/aocl_utils.h"

namespace saoclib {
    using namespace aocl_utils;

    // TODO CheckError with cleanup
    /**
     * @note this class must be thread safe
     */
    class CLContext {
    public:
        /**
         * Constructor
         * @return
         */
        CLContext();

        /**
         * Destructor
         */
        ~CLContext();

        /**
         * Init OpenCL resources,
         * including platform, device, context
         * @return
         */
        bool initOpenCL();

        /**
         * Get platformName
         * @return
         */
        const std::string &getPlatformName() const;

        /**
         * Get platformID
         * @return
         */
        const cl_platform_id getPlatformID() const;

        /**
         * Get the inner cl_context object
         * @return
         */
        cl_context getRawContext() const;

        /**
         * Get the number of managed devices in this context
         * @return
         */
        int getNumDevices() const;

        /**
         * Get the devices managed by this context
         * @return A vector of cl_device_id
         */
        const std::vector<cl_device_id> &getDevices() const;

        /**
         * Get the i'th device id
         * @param i
         * @return
         */
        cl_device_id getDeviceID(unsigned i) const;

        /**
         * Check if the given device is managed by this context
         * @param id
         * @return
         */
        bool isManagedDevice(cl_device_id id) const;

        /**
         * Clean managed resources(release the OpenCL context)
         */
        void cleanup();

    private:
        /// Platform name, for intel FPGA sdk, it should be "Intel"
        std::string platformName = "";

        /// Platform id
        cl_platform_id platformID = NULL;

        /// Opencl Context, 1 context could manage multiple devices
        cl_context context = NULL;

        /// Devices managed by this context, 1 device could only be managed by 1 cl_context
        std::vector<cl_device_id> devices;
    };
}
#endif //SAOCLIB_CPP_CLCONTEXT_H
