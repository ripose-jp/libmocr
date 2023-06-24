////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2022 Ripose
//
// This file is part of libmocr.
//
// libmocr is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, version 3 of the License.
//
// libmocr is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with libmocr.  If not, see <https://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MOCRXX_H
#define MOCRXX_H

#include <string>

/* Forward Declaration of the C mangaocr context struct */
struct mocr_ctx;

namespace mocr
{

/**
 * @brief Defines the various modes for reading in image data
 */
enum class mode
{
    /* 1-bit pixels, black and white, stored with one pixel per byte */
    One,

    /* 8-bit pixels, black and white */
    L,

    /* 8-bit pixels, mapped to any other mode using a color palette */
    P,

    /* 3x8-bit pixels, true color */
    RGB,

    /* 4x8-bit pixels, true color with transparency mask */
    RGBA,

    /* 4x8-bit pixels, color separation */
    CMYK,

    /* 3x8-bit pixels, color video format */
    YCbCr,

    /* 3x8-bit pixels, the L*a*b color space */
    LAB,

    /* 3x8-bit pixels, Hue, Saturation, Value color space */
    HSV,

    /* 32-bit signed integer pixels */
    I,

    /* 32-bit floating point pixels */
    F,
};

/**
 * @brief A mangaocr model object used for reading text from images
 */
class model
{
public:
    /**
     * @brief Construct a new model object
     *
     * @param model A HuggingFace repo, URL, or path to a local model
     * @param force_cpu false if GPU acceleration is desired, true if only
     *                  the CPU should be used
     */
    model(
        const char *path = "kha-white/manga-ocr-base", bool force_cpu = false);

    /**
     * @brief Construct a new model object
     *
     * @param model A HuggingFace repo, URL, or path to a local model
     * @param force_cpu false if GPU acceleration is desired, true if only
     *                  the CPU should be used
     */
    model(const std::string &path, bool force_cpu = false);

    /* Delete the copy constructor */
    model(const model &) = delete;

    /**
     * @brief Destroy the model object
     */
    virtual ~model();

    /**
     * @brief Whether or not this instance was successfully initialized
     *
     * @return true if the instance is valid,
     * @return false if invalid
     */
    bool valid() const;

    /**
     * @brief Whether or not this instance is valid
     *
     * @return true if this instance is invalid,
     * @return false if valid
     */
    bool operator!() const;

    /**
     * @brief Reads text from raw image data
     *
     * @param data The image data
     * @param width The width of the image
     * @param height The height of the image
     * @param mode The mode the image data should be read in
     * @return The text contained in the image data, empty string on error
     */
    std::string read(void *data, size_t width, size_t height, mocr::mode mode);

    /**
     * @brief Reads text from an image file
     *
     * @param path Path to the image file
     * @return The text contained in the image data, empty string on error
     */
    std::string read(const char *path);

    /**
     * @brief Reads text from an image file
     *
     * @param path Path to the image file
     * @return The text contained in the image data, empty string on error
     */
    std::string read(const std::string &path);

private:
    /* The C mocr context */
    mocr_ctx *m_ctx;
};

/**
 * @brief Finalizes the Python state. All models should be destroyed before
 * calling this method. This method is not thread safe.
 *
 * This method should only be called if you know what you are doing. Many
 * libraries do not play nice with reinitializing Python after a call to
 * finalize. It is for this reason that you should ONLY call this method if you
 * are certain another context will not be initialized. Calling this method may
 * also interfere with other code if Python is embedded elsewhere. You have been
 * warned.
 *
 * @return true if the the state for finalized without error
 * @return false otherwise
 */
bool finalize(void);

}

#endif // MOCRXX_H
