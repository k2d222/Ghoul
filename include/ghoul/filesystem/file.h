/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2022                                                               *
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

#ifndef __GHOUL___FILE___H__
#define __GHOUL___FILE___H__

#include <filesystem>
#include <functional>
#include <string>

namespace ghoul::filesystem {

class FileSystem;

/**
 * This class is a handle for a generic file in the file system. The main functionality is
 * to be able to extract parts of the path like the #baseName, the #directoryName, or the
 * #fileExtension. The second functionality of this class is a platform-independent way of
 * being notified of changes of the file. The constructor or the #setCallback methods
 * expect an <code>std::function</code> object (possibly initialized using a lambda-
 * expression) that will be called whenever the file changes on the hard disk. The
 * callback function has this object passed as a parameter. If many changes of the file
 * happen in quick succession, each change will trigger a separate call of the callback.
 * The file system is not polled, but the changes are pushed to the application, so the
 * changes are registered efficiently and are solely impacted by the overhead of
 * <code>std::function</code>.
 */
class File {
public:
    /// The type of the std::function that is used as the prototype for the callback
    using FileChangedCallback = std::function<void()>;

    /**
     * This method constructs a new File object using a given \p filename. \p isRawPath
     * controls if the path is used without changes, or if tokens should be converted
     * first. The token conversion is done using the FileSystem. \p fileChangedCallback
     * will be called whenever the pointed file changes on the hard disk.
     *
     * \param filename The path to the file this File object should point to
     * \param isRawPath If this value is <code>true</code>, the value of \p filename is
     *        used as-is. If it is <code>false</code>, the path is converted into an
     *        absolute path and any tokens, if present, are resolved
     * \param fileChangedCallback The callback function that is called once per change of
     *        the file on the filesystem
     *
     * \pre \p filename must not be empty
     *
     * \see FileSystem The system to register and use tokens
     */
    File(std::filesystem::path filename);

    /**
     * Copy constructor.
     */
    File(const File& file);

    File(File&& file) = default;

    /**
     * The destructor will automatically stop the notification of future changes in the
     * file system.
     */
    ~File();

    File& operator=(const File& rhs) = default;
    File& operator=(File&& rhs) = default;


    /**
     * Sets a new callback function that will be used for this File object. If there
     * has not been a callback before, there are no race conditions. If there has been a
     * registered callback before and the callback is changed from another thread, a race
     * condition might appear if a file is changed in the file system at the same time.
     *
     * \param callback The new callback function that will be used in this File object
     */
    void setCallback(FileChangedCallback callback);

    /**
     * Returns the full path to the file as an <code>std::string</code>.
     *
     * \return The full path to the file as an <code>std::string</code>
     */
    const std::filesystem::path& path() const;

private:
    /**
     * Registers and starts the platform-dependent listener to file changes on disk. Will
     * remove and unregister the old listener in the process
     */
    void installFileChangeListener();

    /**
     * Removes the platform-dependent listener. If there is no listener present, this
     * operation is a no-op.
     */
    void removeFileChangeListener();

    /// The filename of this File
    std::filesystem::path _filename;

    /**
     * The callback that is called when the file changes on disk. Has no performance
     * impact when it is not used
     */
    FileChangedCallback _fileChangedCallback;

    int _indx = -1;
};

} // namespace ghoul::filesystem

#endif // __GHOUL___FILE___H__
