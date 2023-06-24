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

/* Python MUST be included before all else */
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "mocr.h"

#include <stdlib.h>

/* The thread state for the main thread */
PyThreadState *g_mainThreadState;

/**
 * @brief The definition of the mangaocr context object
 */
struct mocr_ctx
{
    /* The instance of the mangaocr object */
    PyObject *obj_mangaocr;

    /* The result of "from PIL.Image import frombytes" */
    PyObject *func_pil_image_frombytes;
};

/**
 * @brief Take the ceiling of a division
 *
 * @param numer The numerator of the division
 * @param denom The denominator of the division
 * @return The ceiling of the division
 */
#define CEILING(numer, denom) (((numer) + (denom) - 1) / (denom))

/**
 * @brief Converts a mocr_mode to PIL image mode string
 *
 * @param mode The mode to convert
 * @return The PIL mode string, NULL if it doesn't exist
 */
static const char *mode_to_pil_mode(mocr_mode mode)
{
    switch (mode)
    {
        case mocr_mode_1:
            return "1";
        case mocr_mode_L:
            return "L";
        case mocr_mode_P:
            return "P";
        case mocr_mode_RGB:
            return "RGB";
        case mocr_mode_RGBA:
            return "RGBA";
        case mocr_mode_CMYK:
            return "CMYK";
        case mocr_mode_YCbCr:
            return "YCbCr";
        case mocr_mode_LAB:
            return "LAB";
        case mocr_mode_HSV:
            return "HSV";
        case mocr_mode_I:
            return "I";
        case mocr_mode_F:
            return "F";
    }
    return NULL;
}

/**
 * @brief Convert an mocr_mode to its value size in bits
 *
 * @param mode The mocr_mode to convert
 * @return The size in bits of the smallest data worked with. 0 if the mode is
 * invalid.
 */
static size_t mode_to_size(mocr_mode mode)
{
    switch (mode)
    {
        case mocr_mode_1:
            return 1;

        case mocr_mode_L:
        case mocr_mode_P:
            return 8;

        case mocr_mode_RGB:
        case mocr_mode_YCbCr:
        case mocr_mode_LAB:
        case mocr_mode_HSV:
            return 24;

        case mocr_mode_RGBA:
        case mocr_mode_CMYK:
        case mocr_mode_I:
        case mocr_mode_F:
            return 32;
    }
    return 0;
}

/**
 * @brief Calls the mocr object's read method with args and returns a UTF8
 * string containing the text in args
 *
 * @param ctx The mangaocr context
 * @param args The arguments to call the mangaocr read method with
 * @return A UTF8 representation of the returned string, NULL on error. Must be
 * freed with free().
 */
static char *call_read(mocr_ctx *ctx, PyObject *args)
{
    char *text = NULL;
    PyObject *result = PyObject_CallObject(ctx->obj_mangaocr, args);
    if (result == NULL)
    {
        PyErr_Print();
        goto cleanup;
    }
    const char *str = PyUnicode_AsUTF8(result);
    if (str == NULL)
    {
        PyErr_Print();
        goto cleanup;
    }
    text = strdup(str);

cleanup:
    Py_XDECREF(result);
    result = NULL;

    return text;
}

mocr_ctx *mocr_init(const char *model, int force_cpu)
{
    PyGILState_STATE gstate;
    mocr_ctx *ctx = NULL;
    PyObject *args = NULL;
    PyObject *module_manga_ocr = NULL;
    PyObject *module_pil = NULL;
    PyObject *module_pil_image = NULL;

    /* Deal with state */
    Py_Initialize();
    if (PyGILState_Check())
    {
        g_mainThreadState = PyEval_SaveThread();
    }
    gstate = PyGILState_Ensure();

    ctx = calloc(1, sizeof(mocr_ctx));
    if (ctx == NULL)
    {
        goto error;
    }

    /* from manga_ocr import MangaOcr */
    args = Py_BuildValue("s", "MangaOcr");
    if (args == NULL)
    {
        PyErr_Print();
        goto error;
    }
    module_manga_ocr = PyImport_ImportModuleEx("manga_ocr", NULL, NULL, args);
    if (module_manga_ocr == NULL)
    {
        PyErr_Print();
        goto error;
    }
    Py_DECREF(args);
    args = NULL;

    /* MangaOcr(model, force_cpu) */
    ctx->obj_mangaocr = PyObject_CallMethod(
        module_manga_ocr, "MangaOcr", "sO",
        model,
        force_cpu ? Py_True : Py_False
    );
    if (ctx->obj_mangaocr == NULL)
    {
        PyErr_Print();
        goto error;
    }

    /* from PIL import Image */
    args = Py_BuildValue("s", "Image");
    if (args == NULL)
    {
        PyErr_Print();
        goto error;
    }
    module_pil = PyImport_ImportModuleEx("PIL", NULL, NULL, args);
    if (module_pil == NULL)
    {
        PyErr_Print();
        goto error;
    }
    Py_DECREF(args);
    args = NULL;

    /* Get PIL.Image module */
    module_pil_image = PyObject_GetAttrString(module_pil, "Image");
    if (module_pil_image == NULL)
    {
        PyErr_Print();
        goto error;
    }

    /* Get PIL.Image.frombytes */
    ctx->func_pil_image_frombytes =
        PyObject_GetAttrString(module_pil_image, "frombytes");
    if (ctx->func_pil_image_frombytes == NULL)
    {
        PyErr_Print();
        goto error;
    }

    PyGILState_Release(gstate);

    return ctx;

error:
    Py_XDECREF(module_manga_ocr);
    Py_XDECREF(module_pil_image);
    Py_XDECREF(module_pil);
    Py_XDECREF(args);

    PyGILState_Release(gstate);

    mocr_destroy(ctx);

    return NULL;
}

int mocr_destroy(mocr_ctx *ctx)
{
    if (ctx)
    {
        PyGILState_STATE gstate = PyGILState_Ensure();

        Py_XDECREF(ctx->obj_mangaocr);
        Py_XDECREF(ctx->func_pil_image_frombytes);
        free(ctx);

        PyGC_Collect();

        PyGILState_Release(gstate);
    }
    return 0;
}

#define BITS_IN_BYTE    8

char *mocr_read(
    mocr_ctx *ctx, void *data, size_t width, size_t height, mocr_mode mode)
{
    PyGILState_STATE gstate;
    PyObject *image = NULL;
    PyObject *args = NULL;
    char *text = NULL;

    gstate = PyGILState_Ensure();

    /* image = PIL.Image.frombytes(
     *     mode, (width, height), data, 'raw', mode, 0, 1
     * )
     */
    {
        size_t data_bytes = mode_to_size(mode) * width * height;
        data_bytes = CEILING(data_bytes, BITS_IN_BYTE);

        const char *mode_str = mode_to_pil_mode(mode);

        image = PyObject_CallFunction(
            ctx->func_pil_image_frombytes, "s(nn)y#ssii",
            mode_str,
            width, height,
            data, data_bytes,
            "raw",
            mode_str,
            0,
            1
        );
    }
    if (image == NULL)
    {
        PyErr_Print();
        goto cleanup;
    }

    /* return mocr(image) */
    args = Py_BuildValue("(O)", image);
    if (args == NULL)
    {
        PyErr_Print();
        goto cleanup;
    }
    text = call_read(ctx, args);

cleanup:
    Py_XDECREF(args);
    Py_XDECREF(image);

    PyGILState_Release(gstate);

    return text;
}

#undef BITS_IN_BYTE

char *mocr_read_file(mocr_ctx *ctx, const char *path)
{
    char *text = NULL;

    PyGILState_STATE gstate = PyGILState_Ensure();

    PyObject *args = Py_BuildValue("(s)", path);
    if (args == NULL)
    {
        PyErr_Print();
        return NULL;
    }
    text = call_read(ctx, args);
    Py_DECREF(args);
    args = NULL;

    PyGILState_Release(gstate);

    return text;
}

int mocr_free(void *ptr)
{
    free(ptr);
    return 0;
}

int mocr_finalize(void)
{
    if (g_mainThreadState == NULL)
    {
        return 0;
    }
    PyEval_RestoreThread(g_mainThreadState);
    g_mainThreadState = NULL;
    return Py_FinalizeEx();
}
