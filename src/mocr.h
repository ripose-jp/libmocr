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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBMOCR_H
#define LIBMOCR_H

#include <stddef.h>
#include <stdint.h>

/* The default model used by mangaocr */
#define DEFAULT_MODEL   "kha-white/manga-ocr-base"

/* The libmocr state object */
typedef struct mocr_ctx mocr_ctx;

/* Defines the various modes for reading in image data */
typedef enum mocr_mode
{
    /* 1-bit pixels, black and white, stored with one pixel per byte */
    mocr_mode_1,

    /* 8-bit pixels, black and white */
    mocr_mode_L,

    /* 8-bit pixels, mapped to any other mode using a color palette */
    mocr_mode_P,

    /* 3x8-bit pixels, true color */
    mocr_mode_RGB,

    /* 4x8-bit pixels, true color with transparency mask */
    mocr_mode_RGBA,

    /* 4x8-bit pixels, color separation */
    mocr_mode_CMYK,

    /* 3x8-bit pixels, color video format */
    mocr_mode_YCbCr,

    /* 3x8-bit pixels, the L*a*b color space */
    mocr_mode_LAB,

    /* 3x8-bit pixels, Hue, Saturation, Value color space */
    mocr_mode_HSV,

    /* 32-bit signed integer pixels */
    mocr_mode_I,

    /* 32-bit floating point pixels */
    mocr_mode_F,
}
mocr_mode;

/**
 * @brief Initializes manga-ocr's state with a model
 *
 * @param model A HuggingFace repo, URL, or path to a local model
 * @param force_cpu 0 to use CUDA is available, nonzero to force CPU usage
 * @return A new mangaocr context object, NULL on error. This must be freed with
 * mocr_destroy().
 */
mocr_ctx *mocr_init(const char *model, int force_cpu);

/**
 * @brief Destroys a context object and frees its resources
 *
 * @param ctx The context object to destroy
 * @return 0 on success, nonzero on error
 */
int mocr_destroy(mocr_ctx *ctx);

/**
 * @brief Extracts text from an image buffer
 *
 * @param ctx The context containing the model
 * @param data The image data
 * @param width The width of the image in pixels
 * @param height The height of the image in pixels
 * @param mode The format of the image data
 * @return The text extracted from the image. This must be freed with
 * mocr_free().
 */
char *mocr_read(
    mocr_ctx *ctx, void *data, size_t width, size_t height, mocr_mode mode);

/**
 * @brief Extracts text from an image file
 *
 * @param ctx The context containing the model
 * @param path The path to the image
 * @return The text extracted from the image. This must be freed with
 * mocr_free().
 */
char *mocr_read_file(mocr_ctx *ctx, const char *path);

/**
 * @brief Frees memory allocated by libmocr
 *
 * @param ptr The memory to free
 * @return 0 on success, nonzero on failure
 */
int mocr_free(void *ptr);

/**
 * @brief Finalizes the Python state. All contexts should be destroyed before
 * calling this method. This method is not thread safe.
 *
 * This method should only be called if you know what you are doing. Many
 * libraries do not play nice with reinitializing Python after a call to
 * finalize. It is for this reason that you should ONLY call this method if you
 * are certain another context will not be initialized. Calling this method may
 * also interfere with other code if Python is embedded elsewhere. You have been
 * warned.
 *
 * @return 0 on success, nonzero on failure.
 */
int mocr_finalize();

#endif // LIBMOCR_H

#ifdef __cplusplus
}
#endif
