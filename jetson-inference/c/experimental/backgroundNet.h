/*
 * Copyright (c) 2022, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
 
#ifndef __BACKGROUND_NET_H__
#define __BACKGROUND_NET_H__


#include "tensorNet.h"
#include "cudaFilterMode.h"


/**
 * Name of default input layer for backgroundNet model.
 * @ingroup backgroundNet
 */
#define BACKGROUNDNET_DEFAULT_INPUT   "input_0"

/**
 * Name of default output layer for backgroundNet model.
 * @ingroup backgroundNet
 */
#define BACKGROUNDNET_DEFAULT_OUTPUT  "output_0"


/**
 * Standard command-line options able to be passed to backgroundNet::Create()
 * @ingroup backgroundNet
 */
#define BACKGROUNDNET_USAGE_STRING  "backgroundNet arguments: \n" 							\
		  "  --network=NETWORK    pre-trained model to load, one of the following:\n" 	\
		  "                           * u2net (default)\n" 						\
		  "  --model=MODEL        path to custom model to load (caffemodel, uff, or onnx)\n" 			\
		  "  --input-blob=INPUT   name of the input layer (default is '" BACKGROUNDNET_DEFAULT_INPUT "')\n" 	\
		  "  --output-blob=OUTPUT name of the output layer (default is '" BACKGROUNDNET_DEFAULT_OUTPUT "')\n" 	\
		  "  --batch-size=BATCH   maximum batch size (default is 1)\n"								\
		  "  --profile            enable layer profiling in TensorRT\n\n"


/**
 * Background subtraction/removal, TensorRT.
 * @ingroup backgroundNet
 */
class backgroundNet : public tensorNet
{
public:
	/**
	 * Network choice enumeration.
	 */
	enum NetworkType
	{
		CUSTOM,        /**< Custom model provided by the user */
		U2NET,		/**< U2-Net (U-Square Net) for Salient Object Detection */
	};

	/**
	 * Parse a string to one of the built-in pretrained models.
	 * Valid names are "u2net", ect.
	 * @returns one of the backgroundNet::NetworkType enums, or backgroundNet::CUSTOM on invalid string.
	 */
	static NetworkType NetworkTypeFromStr( const char* model_name );

	/**
	 * Convert a NetworkType enum to a string.
	 */
	static const char* NetworkTypeToStr( NetworkType network );

	/**
	 * Load a new network instance
	 */
	static backgroundNet* Create( NetworkType networkType=U2NET, uint32_t maxBatchSize=DEFAULT_MAX_BATCH_SIZE, 
							precisionType precision=TYPE_FASTEST, deviceType device=DEVICE_GPU, bool allowGPUFallback=true );
	
	/**
	 * Load a new network instance
	 * @param prototxt_path File path to the deployable network prototxt
	 * @param model_path File path to the caffemodel
	 * @param mean_binary File path to the mean value binary proto (can be NULL)
	 * @param class_labels File path to list of class name labels
	 * @param input Name of the input layer blob.
	 * @param output Name of the output layer blob.
	 * @param maxBatchSize The maximum batch size that the network will support and be optimized for.
	 */
	static backgroundNet* Create( const char* model_path, 
							const char* input=BACKGROUNDNET_DEFAULT_INPUT, 
							const char* output=BACKGROUNDNET_DEFAULT_OUTPUT, 
							uint32_t maxBatchSize=DEFAULT_MAX_BATCH_SIZE, 
							precisionType precision=TYPE_FASTEST,
							deviceType device=DEVICE_GPU, bool allowGPUFallback=true );
	
	/**
	 * Load a new network instance by parsing the command line.
	 */
	static backgroundNet* Create( int argc, char** argv );

	/**
	 * Load a new network instance by parsing the command line.
	 */
	static backgroundNet* Create( const commandLine& cmdLine );

	/**
	 * Usage string for command line arguments to Create()
	 */
	static inline const char* Usage() 		{ return BACKGROUNDNET_USAGE_STRING; }

	/**
	 * Destroy
	 */
	virtual ~backgroundNet();
	
	/**
	 * Perform background subtraction/removal on the image (in-place).
	 * @param image input/output image in CUDA device memory.
	 * @param width width of the image in pixels.
	 * @param height height of the output image in pixels.
	 * @param filter the upsampling mode used to resize the DNN mask (FILTER_LINEAR or FILTER_POINT)
	 * @param maskAlpha if true (default), the mask will be applied to the alpha channel in addition to the color channels.
	 * @returns true on success and false if an error occurred.
	 */
	template<typename T> int Process( T* image, uint32_t width, uint32_t height,
							    cudaFilterMode filter=FILTER_LINEAR, bool maskAlpha=true )	{ return Process((void*)image, width, height, imageFormatFromType<T>(), filter, maskAlpha); }
	
	/**
	 * Perform background subtraction/removal on the image.
	 * @param input input image in CUDA device memory.
	 * @param output output image in CUDA device memory.
	 * @param width width of the image in pixels.
	 * @param height height of the output image in pixels.
	 * @param filter the upsampling mode used to resize the DNN mask (FILTER_LINEAR or FILTER_POINT)
	 * @param maskAlpha if true (default), the mask will be applied to the alpha channel in addition to the color channels.
	 * @returns true on success and false if an error occurred.
	 */
	template<typename T> int Process( T* input, T* output, uint32_t width, uint32_t height,
							    cudaFilterMode filter=FILTER_LINEAR, bool maskAlpha=true )	{ return Process((void*)input, (void*)output, width, height, imageFormatFromType<T>(), filter, maskAlpha); }
	
	/**
	 * Perform background subtraction/removal on the image (in-place).
	 * @param image input/output image in CUDA device memory.
	 * @param width width of the image in pixels.
	 * @param height height of the output image in pixels.
	 * @param filter the upsampling mode used to resize the DNN mask (FILTER_LINEAR or FILTER_POINT)
	 * @param maskAlpha if true (default), the mask will be applied to the alpha channel as well.
	 * @returns true on success and false if an error occurred.
	 */
	inline bool Process( void* image, uint32_t width, uint32_t height, imageFormat format,
					 cudaFilterMode filter=FILTER_LINEAR, bool maskAlpha=true )					{ return Process(image, image, width, height, format, filter, maskAlpha); }
			    
	/**
	 * Perform background subtraction/removal on the image.
	 * @param input input image in CUDA device memory.
	 * @param output output image in CUDA device memory.
	 * @param width width of the image in pixels.
	 * @param height height of the output image in pixels.
	 * @param filter the upsampling mode used to resize the DNN mask (FILTER_LINEAR or FILTER_POINT)
	 * @param maskAlpha if true (default), the mask will be applied to the alpha channel as well.
	 * @returns true on success and false if an error occurred.
	 */
	bool Process( void* input, void* output, uint32_t width, uint32_t height, imageFormat format,
			    cudaFilterMode filter=FILTER_LINEAR, bool maskAlpha=true );
	
	/**
	 * Retrieve the network type (alexnet or googlenet)
	 */
	inline NetworkType GetNetworkType() const					{ return mNetworkType; }

	/**
 	 * Retrieve a string describing the network name.
	 */
	inline const char* GetNetworkName() const					{ return NetworkTypeToStr(mNetworkType); }

protected:
	backgroundNet();
	
	bool init(const char* model_path, const char* input, const char* output, uint32_t maxBatchSize, precisionType precision, deviceType device, bool allowGPUFallback );

	NetworkType mNetworkType;
};


#endif
