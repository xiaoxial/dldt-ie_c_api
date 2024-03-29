// Copyright (C) 2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#ifndef INFERENCE_ENGINE_IE_BRIDGES_C_INCLUDE_IE_C_API_H_
#define INFERENCE_ENGINE_IE_BRIDGES_C_INCLUDE_IE_C_API_H_

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
    #define EXTERN extern "C"
#else
    #define EXTERN
#endif

#if defined(__GNUC__) && (__GNUC__ < 4)
    #define INFERENCE_ENGINE_C_API(...) EXTERN __VA_ARGS__
#else
    #if defined(_WIN32)
        #ifdef inference_engine_c_api_EXPORTS
            #define INFERENCE_ENGINE_C_API(...) EXTERN   __declspec(dllexport) __VA_ARGS__ __cdecl
        #else
            #define INFERENCE_ENGINE_C_API(...) EXTERN  __declspec(dllimport) __VA_ARGS__ __cdecl
        #endif
    #else
        #define INFERENCE_ENGINE_C_API(...) EXTERN __attribute__((visibility("default"))) __VA_ARGS__
    #endif
#endif

typedef struct ie_core ie_core_t;
typedef struct ie_network ie_network_t;
typedef struct ie_executable ie_executable_network_t;
typedef struct ie_infer_request ie_infer_request_t;
typedef struct ie_blob ie_blob_t;

/**
 * @struct ie_core_version
 * @brief  Represents version information that describes devices and the inference engine runtime library
 */
typedef struct ie_core_version {
    size_t major;
    size_t minor;
    const char *device_name;
    const char *build_number;
    const char *description;
}ie_core_version_t;

/**
 * @struct ie_core_versions
 * @brief Represents all versions information that describes all devices and the inference engine runtime library
 */
typedef struct ie_core_versions {
    ie_core_version_t *versions;
    size_t num_vers;
}ie_core_versions_t;

/**
 * @struct ie_config
 * @brief Represents configuration information that describes devices
 */
typedef struct ie_config {
    const char *name;
    const char *value;
    struct ie_config *next;
}ie_config_t;

/**
 * @struct ie_param
 * @brief metric and config parameters.
 */
typedef struct ie_param {
    union {
    char *params;
    unsigned int number;
    unsigned int range_for_async_infer_request[3];
    unsigned int range_for_streams[2];
    };
}ie_param_t;

/**
 * @struct ie_param_config
 * @brief Represents configuration parameter information
 */
typedef struct ie_param_config {
    char *name;
    ie_param_t *param;
}ie_param_config_t;

/**
 * @struct desc
 * @brief Represents detailed information for an error
 */
typedef struct desc {
    char msg[256];
}desc_t;

/**
 * @struct dimensions
 * @brief Represents dimensions for input or output data
 */
typedef struct dimensions {
    size_t ranks;
    size_t dims[8];
}dimensions_t;

/**
 * @enum layout_e
 * @brief Layouts that the inference engine supports
 */
typedef enum {
    ANY = 0,    // "any" layout

    // I/O data layouts
    NCHW = 1,
    NHWC = 2,
    NCDHW = 3,
    NDHWC = 4,

    // weight layouts
    OIHW = 64,

    // Scalar
    SCALAR = 95,

    // bias layouts
    C = 96,

    // Single image layout (for mean image)
    CHW = 128,

    // 2D
    HW = 192,
    NC = 193,
    CN = 194,

    BLOCKED = 200,
}layout_e;

/**
 * @enum precision_e
 * @brief Precisions that the inference engine supports
 */
typedef enum {
    UNSPECIFIED = 255, /**< Unspecified value. Used by default */
    MIXED = 0,  /**< Mixed value. Can be received from network. No applicable for tensors */
    FP32 = 10,  /**< 32bit floating point value */
    FP16 = 11,  /**< 16bit floating point value */
    Q78 = 20,   /**< 16bit specific signed fixed point precision */
    I16 = 30,   /**< 16bit signed integer value */
    U8 = 40,    /**< 8bit unsigned integer value */
    I8 = 50,    /**< 8bit signed integer value */
    U16 = 60,   /**< 16bit unsigned integer value */
    I32 = 70,   /**< 32bit signed integer value */
    I64 = 72,   /**< 64bit signed integer value */
    BIN = 71,   /**< 1bit integer value */
    CUSTOM = 80 /**< custom precision has it's own name and size of elements */
}precision_e;

/**
 * @struct tensor_desc
 * @brief Represents detailed information for a tensor
 */
typedef struct tensor_desc {
    layout_e layout;
    dimensions_t dims;
    precision_e precision;
}tensor_desc_t;

/**
 * @enum colorformat_e
 * @brief Extra information about input color format for preprocessing
 */
typedef enum {
    RAW = 0u,    ///< Plain blob (default), no extra color processing required
    RGB,         ///< RGB color format
    BGR,         ///< BGR color format, default in DLDT
    RGBX,        ///< RGBX color format with X ignored during inference
    BGRX,        ///< BGRX color format with X ignored during inference
    NV12,        ///< NV12 color format represented as compound Y+UV blob
}colorformat_e;

/**
 * @enum resize_alg_e
 * @brief Represents the list of supported resize algorithms.
 */
typedef enum {
    NO_RESIZE = 0,
    RESIZE_BILINEAR,
    RESIZE_AREA
}resize_alg_e;

/**
 * @enum IEStatusCode
 * @brief This enum contains codes for all possible return values of the interface functions
 */
typedef enum {
    OK = 0,
    GENERAL_ERROR = -1,
    NOT_IMPLEMENTED = -2,
    NETWORK_NOT_LOADED = -3,
    PARAMETER_MISMATCH = -4,
    NOT_FOUND = -5,
    OUT_OF_BOUNDS = -6,
    /*
     * @brief exception not of std::exception derived type was thrown
     */
    UNEXPECTED = -7,
    REQUEST_BUSY = -8,
    RESULT_NOT_READY = -9,
    NOT_ALLOCATED = -10,
    INFER_NOT_STARTED = -11,
    NETWORK_NOT_READ = -12
}IEStatusCode;

/**
 * @struct roi_t
 * @brief This structure describes roi data.
 */
typedef struct roi {
    size_t id;     // ID of a roi
    size_t posX;   // W upper left coordinate of roi
    size_t posY;   // H upper left coordinate of roi
    size_t sizeX;  // W size of roi
    size_t sizeY;  // H size of roi
}roi_t;

/**
 * @struct input_shape
 * @brief Represents shape for input data
 */
typedef struct input_shape {
    char *name;
    dimensions_t shape;
}input_shape_t;

/**
 * @struct input_shapes
 * @brief Represents shapes for all input data
 */
typedef struct input_shapes {
    input_shape_t *shapes;
    size_t shape_num;
}input_shapes_t;

/**
 * @struct ie_blob_buffer
 * @brief Represents copied data from the given blob.
 */
typedef struct ie_blob_buffer {
    union {
    void *buffer;  // buffer can be written
    const void *cbuffer;  // cbuffer is read-only
    };
}ie_blob_buffer_t;

/**
 * @struct ie_complete_call_back
 * @brief Completion callback definition about the function and args
 */
typedef struct ie_complete_call_back {
    void (*completeCallBackFunc)(void *args);
    void *args;
}ie_complete_call_back_t;

/**
 * @brief Returns number of version that is exported.
 * @return Version number of the API.
 */
INFERENCE_ENGINE_C_API(const char *) ie_c_api_version(void);


// Core
/**
 * @brief Constructs Inference Engine Core instance using XML configuration file with devices description.
 * See RegisterPlugins for more details. Use the ie_core_free() method to free memory.
 * @param xml_config_file A path to .xml file with devices to load from. If XML configuration file is not specified,
 * then default Inference Engine devices are loaded from the default plugin.xml file.
 * @param core A pointer to the newly created ie_core_t.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_create(const char *xml_config_file, ie_core_t **core);

/**
 * @brief Releases memory occupied by core.
 * @param core A pointer to the core to free memory.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_free(ie_core_t **core);

/**
 * @brief Gets version information of the device specified. Use the ie_core_versions_free() method to free memory.
 * @param core A pointer to ie_core_t instance.
 * @param device_name Name to indentify device.
 * @param versions A pointer to versions corresponding to device_name.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_get_versions(const ie_core_t *core, const char *device_name, ie_core_versions_t *versions);

/**
 * @brief Releases memory occupied by ie_core_versions.
 * @param vers A pointer to the ie_core_versions to free memory.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_versions_free(ie_core_versions_t *vers);

/**
 * @brief Creates an executable network from a network object. Users can create as many networks as they need and use
 * them simultaneously (up to the limitation of the hardware resources). Use the ie_exec_network_free() method to free memory.
 * @param core A pointer to ie_core_t instance.
 * @param network A pointer to ie_network instance.
 * @param device_name Name of device to load network to.
 * @param config Device configuration.
 * @param exe_network A pointer to the newly created executable network.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_load_network(ie_core_t *core, const ie_network_t *network, const char *device_name, \
        const ie_config_t *config, ie_executable_network_t **exe_network);

/**
 * @brief Sets configuration for device.
 * @param core A pointer to ie_core_t instance.
 * @param ie_core_config Device configuration.
 * @param device_name An optinal name of a device. If device name is not specified,
 * the config is set for all the registered devices.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_set_config(ie_core_t *core, const ie_config_t *ie_core_config, const char *device_name);

/**
 * @brief Registers a new device and a plugin which implement this device inside Inference Engine.
 * @param core A pointer to ie_core_t instance.
 * @param plugin_name A name of a plugin. Depending on a platform, plugin_name is wrapped with
 * a shared library suffix and a prefix to identify a full name of the library.
 * @param device_name A device name to register plugin for. If not specified, the method registers
 * a plugin with the default name.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_register_plugin(ie_core_t *core, const char *plugin_name, const char *device_name);

/**
 * @brief Registers plugins specified in an ".xml" configuration file.
 * @param core A pointer to ie_core_t instance.
 * @param xml_config_file A full path to ".xml" file containing plugins configuration.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_register_plugins(ie_core_t *core, const char *xml_config_file);

/**
 * @brief Unregisters a plugin with a specified device name.
 * @param core A pointer to ie_core_t instance.
 * @param device_name A device name of the device to unregister.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_unregister_plugin(ie_core_t *core, const char *device_name);

/**
 * @brief Loads extension library to the device with a specified device name.
 * @param core A pointer to ie_core_t instance.
 * @param extension_path Path to the extensions library file to load to a device.
 * @param device_name A device name of a device to load the extensions to.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_add_extension(ie_core_t *core, const char *extension_path, const char *device_name);

/**
 * @brief Gets general runtime metric for dedicated hardware. The method is needed to request common device properties
 * which are executable network agnostic. It can be device name, temperature, other devices-specific values.
 * @param core A pointer to ie_core_t instance.
 * @param device_name A name of a device to get a metric value.
 * @param metric_name A metric name to request.
 * @param param_result A metric value corresponding to the metric_name.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_get_metric(const ie_core_t *core, const char *device_name, const char *metric_name, ie_param_t *param_result);

/**
 * @brief Gets configuration dedicated to device behaviour. The method is targeted to extract information
 * which can be set via SetConfig method.
 * @param core A pointer to ie_core_t instance.
 * @param device_name A name of a device to get a configuration value.
 * @param config_name Name of a configuration.
 * @param param_result A configuration value corresponding to the config_name.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_core_get_config(const ie_core_t *core, const char *device_name, const char *config_name, ie_param_t *param_result);


// ExecutableNetwork
/**
 * @brief Releases memory occupied by ExecutableNetwork.
 * @param ie_exec_network A pointer to the ExecutableNetwork to free memory.
 * return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_exec_network_free(ie_executable_network_t **ie_exec_network);

/**
 * @brief Creates an inference request instance used to infer the network. The created request has allocated input
 * and output blobs (that can be changed later). Use the ie_infer_request_free() method to free memory.
 * @param ie_exec_network A pointer to ie_executable_network_t instance.
 * @param request A pointer to the newly created ie_infer_request_t instance
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_exec_network_create_infer_request(ie_executable_network_t *ie_exec_network, ie_infer_request_t **request);

/**
 * @brief Gets general runtime metric for an executable network. It can be network name, actual device ID on which executable network is running
 * or all other properties which cannot be changed dynamically.
 * @param ie_exec_network A pointer to ie_executable_network_t instance.
 * @param metric_name A metric name to request.
 * @param param_result A metric value corresponding to the metric_name.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_exec_network_get_metric(const ie_executable_network_t *ie_exec_network, \
        const char *metric_name, ie_param_t *param_result);

/**
 * @brief Sets configuration for current executable network. Currently, the method can be used
 * when the network run on the Multi device and the configuration paramter is only can be "MULTI_DEVICE_PRIORITIES"
 * @param ie_exec_network A pointer to ie_executable_network_t instance.
 * @param param_config A pointer to device configuration..
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_exec_network_set_config(ie_executable_network_t *ie_exec_network, const ie_config_t *param_config);

/**
 * @brief Gets configuration for current executable network. The method is responsible to
 * extract information which affects executable network execution.
 * @param ie_exec_network A pointer to ie_executable_network_t instance.
 * @param metric_config A configuration parameter name to request.
 * @param param_result A configuration value corresponding to a configuration paramter name.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_exec_network_get_config(const ie_executable_network_t *ie_exec_network, \
        const char *metric_config, ie_param_t *param_result);


// InferRequest
/**
 * @brief Releases memory occupied by ie_infer_request_t instance.
 * @param infer_request A pointer to the ie_infer_request_t to free memory.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_infer_request_free(ie_infer_request_t **infer_request);

/**
 * @brief Gets input/output data for inference
 * @param infer_request A pointer to ie_infer_request_t instance.
 * @param name Name of input or output blob.
 * @param blob A pointer to input or output blob. The type of Blob must match the network input precision and size.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_infer_request_get_blob(ie_infer_request_t *infer_request, const char *name, ie_blob_t **blob);

/**
 * @brief Sets input/output data to inference.
 * @param infer_request A pointer to ie_infer_request_t instance.
 * @param name Name of input or output blob.
 * @param blob Reference to input or output blob. The type of a blob must match the network input precision and size.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_infer_request_set_blob(ie_infer_request_t *infer_request, const char *name, const ie_blob_t *blob);

/**
 * @brief Starts synchronous inference of the infer request and fill outputs.
 * @param infer_request A pointer to ie_infer_request_t instance.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_infer_request_infer(ie_infer_request_t *infer_request);

/**
 * @brief Starts asynchronous inference of the infer request and fill outputs.
 * @param infer_request A pointer to ie_infer_request_t instance.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_infer_request_infer_async(ie_infer_request_t *infer_request);

/**
 * @brief Sets a callback function that will be called on success or failure of asynchronous request
 * @param infer_request A pointer to ie_infer_request_t instance.
 * @param callback  A function to be called.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_infer_set_completion_callback(ie_infer_request_t *infer_request, ie_complete_call_back_t *callback);

/**
 * @brief Waits for the result to become available. Blocks until specified timeout elapses or the result becomes available, whichever comes first.
 * @param infer_request A pointer to ie_infer_request_t instance.
 * @param timeout Maximum duration in milliseconds to block for
 * @note There are special cases when timeout is equal some value of the WaitMode enum:
 * * 0 - Immediately returns the inference status. It does not block or interrupt execution.
 * * -1 - waits until inference result becomes available
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_infer_request_wait(ie_infer_request_t *infer_request, const int64_t timeout);

/**
 * @brief  Sets new batch size for certain infer request when dynamic batching is enabled in executable network that created this request.
 * @param infer_request A pointer to ie_infer_request_t instance.
 * @param size New batch size to be used by all the following inference calls for this request.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_infer_request_set_batch(ie_infer_request_t *infer_request, const size_t size);


// Network
/**
 * @brief Reads the model from the .xml and .bin files of the IR. Use the ie_network_free() method to free memory.
 * @param xml .xml file's path of the IR.
 * @param weights_file .bin file's path of the IR.
 * @param network A pointer to the newly created network.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_read(const char *xml, const char *weights_file, ie_network_t **network);

/**
 * @brief Gets network's name.
 * @param network A pointer to the instance of the ie_network_t.
 * @param name  network's name.
 * @return status Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_name(ie_network_t *network, char**name);

/**
 * @brief When netowrk is loaded into the Infernece Engine, it is not required anymore and should be released
 * @param network The pointer to the instance of the ie_network_t to free.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_free(ie_network_t **network);

/**
 * @brief Gets number of inputs for the network.
 * @param network A pointer to the instance of the ie_network_t to get number of input information.
 * @param size_result A number of the instance's input information.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_inputs_number(const ie_network_t *network, size_t *size_result);

/**
 * @brief Gets name corresponding to the "number". Use the ie_network_name_free() method to free memory.
 * @param network A pointer to theinstance of the ie_network_t to get input information.
 * @param number An id of input information .
 * @param name Input name corresponding to the number.
 * @return status Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_input_name(const ie_network_t *network, size_t number, char **name);

/**
 * @brief Gets a precision of the input data provided by user.
 * @param network A pointer to ie_network_t instance.
 * @param input_name Name of input data.
 * @param prec_result A pointer to the precision used for input blob creation.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_input_precision(const ie_network_t *network, const char *input_name, precision_e *prec_result);

/**
 * @brief Changes the precision of the input data provided by the user.
 * This function should be called before loading the network to the device.
 * @param network A pointer to ie_network_t instance.
 * @param input_name Name of input data.
 * @param p A new precision of the input data to set (eg. precision_e.FP16).
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_set_input_precision(ie_network_t *network, const char *input_name, const precision_e p);

/**
 * @brief Gets a layout of the input data.
 * @param network A pointer to ie_network_t instance.
 * @param input_name Name of input data.
 * @param layout_result A pointer to the layout used for input blob creation.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_input_layout(const ie_network_t *network, const char *input_name, layout_e *layout_result);

/**
 * @brief Changes the layout of the input data named "input_name".
 * This function should be called before loading the network to the device.
 * @param network A pointer to ie_network_t instance.
 * @param input_name Name of input data.
 * @param l A new layout of the input data to set.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_set_input_layout(ie_network_t *network, const char *input_name, const layout_e l);

/**
 * @Gets dimensions/shape of the input data with reversed order.
 * @param network A pointer to ie_network_t instance.
 * @param input_name Name of input data.
 * @param dims_result A pointer to the dimensions used for input blob creation.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_input_dims(const ie_network_t *network, const char *input_name, dimensions_t *dims_result);

/**
 * @brief Gets pre-configured resize algorithm.
 * @param network A pointer to ie_network_t instance.
 * @param input_name Name of input data.
 * @parm resize_alg_result The pointer to the resize algorithm used for input blob creation.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_input_resize_algorithm(const ie_network_t *network, const char *input_name, \
        resize_alg_e *resize_alg_result);

/**
 * @brief Sets resize algorithm to be used during pre-processing
 * @param network A pointer to ie_network_t instance.
 * @param input_name Name of input data.
 * @param resize_algo Resize algorithm.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_set_input_resize_algorithm(ie_network_t *network, const char *input_name, const resize_alg_e resize_algo);

/**
 * @brief Gets color format of the input data.
 * @param network A pointer to ie_network_t instance.
 * @param input_name Name of input data.
 * @param colformat_result The pointer to the color format used for input blob creation.
 * @reutrn Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_color_format(const ie_network_t *network, const char *input_name, colorformat_e *colformat_result);

/**
 * @brief Changes the color format of the input data.
 * @param network A pointer to ie_network_t instance.
 * @param input_name Name of input data.
 * @param color_format Color format of the input data.
 * @reutrn Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_set_color_format(ie_network_t *network, const char *input_name, const colorformat_e color_format);

/**
 * @brief Helper method collect all input shapes with input names of corresponding input data.
 * Use the ie_network_input_shapes_free() method to free memory.
 * @param network A pointer to the instance of the ie_network_t to get input shapes.
 * @param shapes A pointer to the input_shapes.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_input_shapes(ie_network_t *network, input_shapes_t *shapes);

/**
 * @brief Run shape inference with new input shapes for the network.
 * @param network A pointer to the instance of the ie_network_t to reshape.
 * @param shapes A new input shapes to set for the network.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_reshape(ie_network_t *network, const input_shapes_t shapes);

/**
 * @brief Gets number of output for the network.
 * @param network A pointer to the instance of the ie_network_t to get number of ouput information.
 * @param size_result A number of the network's output information.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_outputs_number(const ie_network_t *network, size_t *size_result);

/**
 * @brief Gets name corresponding to the "number". Use the ie_network_name_free() method to free memory.
 * @param network A pointer to theinstance of the ie_network_t to get output information.
 * @param number An id of output information .
 * @param name Output name corresponding to the number.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_output_name(const ie_network_t *network, const size_t number, char **name);

/**
 * @brief Gets a precision of the output data named "output_name".
 * @param network A pointer to ie_network_t instance.
 * @param output_name Name of output data.
 * @param prec_result A pointer to the precision used for output blob creation.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_output_precision(const ie_network_t *network, const char *output_name, precision_e *prec_result);

/**
 * @brief Changes the precision of the output data named "output_name".
 * @param network A pointer to ie_network_t instance.
 * @param output_name Name of output data.
 * @param p A new precision of the output data to set (eg. precision_e.FP16).
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_set_output_precision(ie_network_t *network, const char *output_name, const precision_e p);

/**
 * @brief Gets a layout of the output data.
 * @param network A pointer to ie_network_t instance.
 * @param output_name Name of output data.
 * @param layout_result A pointer to the layout used for output blob creation.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_output_layout(const ie_network_t *network, const char *output_name, layout_e *layout_result);

/**
 * @brief Changes the layout of the output data named "output_name".
 * @param network A pointer to ie_network_t instance.
 * @param output_name Name of output data.
 * @param l A new layout of the output data to set.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_set_output_layout(ie_network_t *network, const char *output_name, const layout_e l);

/**
 * @Gets dimensions/shape of the output data with reversed order.
 * @param network A pointer to ie_network_t instance.
 * @param output_name Name of output data.
 * @param dims_result A pointer to the dimensions used for output blob creation.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_get_output_dims(const ie_network_t *network, const char *output_name, dimensions_t *dims_result);

/**
 * @brief Releases memory occupied by input_shapes.
 * @param inputShapes A pointer to the input_shapes to free memory.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_input_shapes_free(input_shapes_t *inputShapes);

/**
 * @brief Releases momory occupied by input_name or output_name.
 * @param name A pointer to the input_name or output_name to free memory.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_network_name_free(char **name);

// Blob
/**
 * @brief Creates a blob with the specified dimensions, layout and to allocate memory.
 * @param tensorDesc Tensor descriptor for Blob creation.
 * @param blob A pointer to the newly created blob.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_blob_make_memory(const tensor_desc_t *tensorDesc, ie_blob_t **blob);

/**
 * @brief Creates a blob with the given tensor descriptor from the pointer to the pre-allocated memory.
 * @param tensorDesc Tensor descriptor for Blob creation.
 * @param ptr Pointer to the pre-allocated memory.
 * @param size Length of the pre-allocated array.
 * @param blob A pointer to the newly created blob.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_blob_make_memory_from_preallocated(const tensor_desc_t *tensorDesc, void *ptr, size_t size, ie_blob_t **blob);

/**
 * @brief Creates a blob describing given roi_t instance based on the given blob with pre-allocated memory.
 * @param inputBlob original blob with pre-allocated memory.
 * @param roi A roi_tinstance inside of the original blob.
 * @param blob A pointer to the newly created blob.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_blob_make_memory_with_roi(const ie_blob_t *inputBlob, const roi_t *roi, ie_blob_t **blob);

/**
 * @brief Gets the total number of elements, which is a product of all the dimensions.
 * @param blob A pointer to the blob.
 * @param size_result The total number of elements.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_blob_size(ie_blob_t *blob, int *size_result);

/**
 * @brief Gets the size of the current Blob in bytes.
 * @param blob A pointer to the blob.
 * @param bsize_result The size of the current blob in bytes.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_blob_byte_size(ie_blob_t *blob, int *bsize_result);

/**
 * @brief Releases previously allocated data
 * @param blob A pointer to the blob to free memory.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_blob_deallocate(ie_blob_t **blob);

/**
 * @brief Gets access to the allocated memory .
 * @param blob A pointer to the blob.
 * @param blob_buffer A pointer to the copied data from the given blob.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_blob_get_buffer(const ie_blob_t *blob, ie_blob_buffer_t *blob_buffer);

/**
 * @brief Gets read-only access to the allocated memory.
 * @param blob A pointer to the blob.
 * @param blob_cbuffer A pointer to the coped data from the given pointer to the blob and the data is read-only.
 * @return Status code of the operation: OK(0) for success
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_blob_get_cbuffer(const ie_blob_t *blob, ie_blob_buffer_t *blob_cbuffer);

/**
 * @brief Gets dimensions of blob's tensor.
 * @param blob A pointer to the blob.
 * @parma dims_result A pointer to the dimensions of blob's tensor.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_blob_get_dims(const ie_blob_t *blob, dimensions_t *dims_result);

/**
 * @brief Gets layout of blob's tensor.
 * @parma blob A pointer to the blob.
 * @param layout_result A pointer to the layout of blob's tensor.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_blob_get_layout(const ie_blob_t *blob, layout_e *layout_result);

/**
 * @brief Gets precision of blob's tensor.
 * @parma blob A pointer to the blob.
 * @param prec_result A pointer to the precision of blob's tensor.
 * @return Status code of the operation: OK(0) for success.
 */
INFERENCE_ENGINE_C_API(IEStatusCode) ie_blob_get_precision(const ie_blob_t *blob, precision_e *prec_result);

#endif  // INFERENCE_ENGINE_IE_BRIDGES_C_INCLUDE_IE_C_API_H_
