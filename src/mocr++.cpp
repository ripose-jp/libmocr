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

#include "mocr++.h"

#include "mocr.h"

using namespace mocr;

model::model(const char *path, bool force_cpu)
    : m_ctx(mocr_init(path, force_cpu))
{

}

model::model(const std::string &path, bool force_cpu)
    : model(path.c_str(), force_cpu)
{

}

model::~model()
{
    mocr_destroy(m_ctx);
}

bool model::valid() const
{
    return m_ctx != nullptr;
}

bool model::operator!() const
{
    return !valid();
}

std::string model::read(
    void *data, size_t width, size_t height, mocr::mode mode)
{
    char *str = mocr_read(
        m_ctx, data, width, height, static_cast<mocr_mode>(mode)
    );
    if (str == NULL)
    {
        return "";
    }
    std::string text(str);
    mocr_free(str);
    str = nullptr;
    return text;
}

std::string model::read(const char *path)
{
    char *str = mocr_read_file(m_ctx, path);
    if (str == NULL)
    {
        return "";
    }
    std::string text(str);
    mocr_free(str);
    str = nullptr;
    return text;
}

std::string model::read(const std::string &path)
{
    return read(path.c_str());
}

bool mocr::finalize()
{
    return mocr_finalize() == 0;
}
