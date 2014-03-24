/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2014                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#ifndef __CLCOMMANDS_H__
#define __CLCOMMANDS_H__

#include <ghoul/opencl/ghoul_cl.h>

namespace ghoul {
namespace opencl {

class CLKernel;
class CLWorkSize;

class CLCommandQueue {

public:
	CLCommandQueue(cl_context context, cl_device_id device);
    ~CLCommandQueue();
    
    // Blocking calls
    void enqueueKernelBlocking(const CLKernel& kernel, const CLWorkSize& ws);
    void enqueueReadBufferBlocking(cl_mem buffer, size_t size, void* data);
    
    // non blocking calls
    cl_event enqueueKernelNonBlocking(const CLKernel& kernel, const CLWorkSize& ws);
    cl_event enqueueReadBufferNonBlocking(cl_mem buffer, size_t size, void* data);
    
    // wait for cl calls to finish
    void finish();
    
    
private:
    cl_command_queue _commands;

};

} // namespace opencl
} // namespace ghoul

#endif