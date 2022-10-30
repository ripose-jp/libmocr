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

#include <gtest/gtest.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "mocr++.h"

#define KHA_WHITE_MODEL ("kha-white/manga-ocr-base")

TEST(init_test, basic)
{
    mocr::model model;
    ASSERT_TRUE(model.valid());
    ASSERT_FALSE(!model);
}

TEST(init_test, cpu)
{
    mocr::model model(KHA_WHITE_MODEL, true);
    ASSERT_TRUE(model.valid());
    ASSERT_FALSE(!model);
}

TEST(init_test, model_not_found)
{
    mocr::model model("/model/does/not/exist");
    ASSERT_FALSE(model.valid());
    ASSERT_TRUE(!model);
}

TEST(init_test, basic_std_string)
{
    mocr::model model(std::string(KHA_WHITE_MODEL));
    ASSERT_TRUE(model.valid());
    ASSERT_FALSE(!model);
}

TEST(init_test, basic_std_string_cpu)
{
    mocr::model model(std::string(KHA_WHITE_MODEL), true);
    ASSERT_TRUE(model.valid());
    ASSERT_FALSE(!model);
}

TEST(init_test, model_std_string_not_found)
{
    mocr::model model(std::string("/model/does/not/exist"));
    ASSERT_FALSE(model.valid());
    ASSERT_TRUE(!model);
}

class read_file_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ASSERT_TRUE(ctx.valid());
    }

    void test_file(const char *path, const char *expected_text)
    {
        ASSERT_STREQ(ctx.read(path).c_str(), expected_text);
    }

    mocr::model ctx;
};

TEST_F(read_file_test, basic0)
{
    test_file("data/00.jpg", "素直にあやまるしか");
}

TEST_F(read_file_test, basic1)
{
    test_file("data/01.jpg", "立川で見た〝穴〟の下の巨大な眼は：");
}

TEST_F(read_file_test, basic2)
{
    test_file("data/02.jpg", "実戦剣術も一流です");
}

TEST_F(read_file_test, basic3)
{
    test_file("data/03.jpg", "第３０話重苦しい闇の奥で静かに呼吸づきながら");
}

TEST_F(read_file_test, basic4)
{
    test_file("data/04.jpg", "よかったじゃないわよ！何逃げてるのよ！！早くあいつを退治してよ！");
}

TEST_F(read_file_test, basic5)
{
    test_file("data/05.jpg", "ぎゃっ");
}

TEST_F(read_file_test, basic6)
{
    test_file("data/06.jpg", "ピンポーーン");
}

TEST_F(read_file_test, basic7)
{
    test_file("data/07.jpg", "ＬＩＮＫ！私達７人の力でガノンの塔の結界をやぶります");
}

TEST_F(read_file_test, basic8)
{
    test_file("data/08.jpg", "ファイアパンチ");
}

TEST_F(read_file_test, basic9)
{
    test_file("data/09.jpg", "少し黙っている");
}

TEST_F(read_file_test, basic10)
{
    test_file("data/10.jpg", "わかるかな〜？");
}

TEST_F(read_file_test, basic11)
{
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}

TEST_F(read_file_test, basic_multi)
{
    test_file("data/00.jpg", "素直にあやまるしか");
    test_file("data/01.jpg", "立川で見た〝穴〟の下の巨大な眼は：");
    test_file("data/02.jpg", "実戦剣術も一流です");
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}

TEST_F(read_file_test, missing_file)
{
    std::string text = ctx.read("/file/does/not/exist.jpg");
    ASSERT_TRUE(text.empty());
}

TEST_F(read_file_test, basic0_std_string)
{
    ASSERT_STREQ(ctx.read(std::string("data/00.jpg")).c_str(), "素直にあやまるしか");
}

TEST_F(read_file_test, missing_file_std_string)
{
    ASSERT_TRUE(ctx.read(std::string("/file/does/not/exist")).empty());
}

class read_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ASSERT_TRUE(ctx.valid());
    }

    void test_file(const char *path, const char *expected_text)
    {
        int width, height, channels;
        stbi_uc *data = stbi_load(path, &width, &height, &channels, 3);

        std::string text = ctx.read(data, width, height, mocr::mode::RGB);
        ASSERT_STREQ(text.c_str(), expected_text);

        stbi_image_free(data);
    }

    mocr::model ctx;
};

TEST_F(read_test, basic0)
{
    test_file("data/00.jpg", "素直にあやまるしか");
}

TEST_F(read_test, basic1)
{
    test_file("data/01.jpg", "立川で見た、穴への下の巨大な眼は．．．");
}

TEST_F(read_test, basic2)
{
    test_file("data/02.jpg", "実戦剣術も一流です");
}

TEST_F(read_test, basic3)
{
    test_file("data/03.jpg", "第３０話重苦しい闇の奥で静かに呼吸づきながら");
}

TEST_F(read_test, basic4)
{
    test_file("data/04.jpg", "よかったじゃないわよ！何逃げてるのよ！！早くあいつを退治してよ！");
}

TEST_F(read_test, basic5)
{
    test_file("data/05.jpg", "ぎゃっ");
}

TEST_F(read_test, basic6)
{
    test_file("data/06.jpg", "ピンポーーン");
}

TEST_F(read_test, basic7)
{
    test_file("data/07.jpg", "ＬＩＮＫ！私達７人の力でガノンの塔の結界をやぶります");
}

TEST_F(read_test, basic8)
{
    test_file("data/08.jpg", "ファイアパンチ");
}

TEST_F(read_test, basic9)
{
    test_file("data/09.jpg", "少し黙っている");
}

TEST_F(read_test, basic10)
{
    test_file("data/10.jpg", "わかるかな〜？");
}

TEST_F(read_test, basic11)
{
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}

TEST_F(read_test, basic_multi)
{
    test_file("data/00.jpg", "素直にあやまるしか");
    test_file("data/01.jpg", "立川で見た、穴への下の巨大な眼は．．．");
    test_file("data/02.jpg", "実戦剣術も一流です");
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}
