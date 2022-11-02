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

#include "mocr.h"

TEST(MocrInitTest, Basic)
{
    mocr_ctx *ctx = mocr_init(DEFAULT_MODEL, 0);
    ASSERT_NE(ctx, nullptr);
    EXPECT_EQ(mocr_destroy(ctx), 0);
}

TEST(MocrInitTest, Cpu)
{
    mocr_ctx *ctx = mocr_init(DEFAULT_MODEL, 1);
    ASSERT_NE(ctx, nullptr);
    EXPECT_EQ(mocr_destroy(ctx), 0);
}

TEST(MocrInitTest, ModelNotFound)
{
    mocr_ctx *ctx = mocr_init("/path/to/a/model", 0);
    ASSERT_EQ(ctx, nullptr);
}

TEST(MocrDestroyTest, Vaild)
{
    mocr_ctx *ctx = mocr_init(DEFAULT_MODEL, 0);
    ASSERT_NE(ctx, nullptr);
    EXPECT_EQ(mocr_destroy(ctx), 0);
}

TEST(MocrDestroyTest, Null)
{
    mocr_ctx *ctx = mocr_init(DEFAULT_MODEL, 0);
    ASSERT_NE(ctx, nullptr);
    EXPECT_EQ(mocr_destroy(ctx), 0);
}

class MocrReadFileTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ctx = mocr_init(DEFAULT_MODEL, 0);
        ASSERT_NE(ctx, nullptr);
    }

    void TearDown() override
    {
        EXPECT_EQ(mocr_destroy(ctx), 0);
    }

    void test_file(const char *path, const char *expected_text)
    {
        char *text = mocr_read_file(ctx, path);
        ASSERT_NE(text, nullptr);
        EXPECT_STREQ(text, expected_text);
        EXPECT_EQ(mocr_free(text), 0);
    }

    mocr_ctx *ctx;
};

TEST_F(MocrReadFileTest, Basic0)
{
    test_file("data/00.jpg", "素直にあやまるしか");
}

TEST_F(MocrReadFileTest, Basic1)
{
    test_file("data/01.jpg", "立川で見た〝穴〟の下の巨大な眼は：");
}

TEST_F(MocrReadFileTest, Basic2)
{
    test_file("data/02.jpg", "実戦剣術も一流です");
}

TEST_F(MocrReadFileTest, Basic3)
{
    test_file("data/03.jpg", "第３０話重苦しい闇の奥で静かに呼吸づきながら");
}

TEST_F(MocrReadFileTest, Basic4)
{
    test_file("data/04.jpg", "よかったじゃないわよ！何逃げてるのよ！！早くあいつを退治してよ！");
}

TEST_F(MocrReadFileTest, Basic5)
{
    test_file("data/05.jpg", "ぎゃっ");
}

TEST_F(MocrReadFileTest, Basic6)
{
    test_file("data/06.jpg", "ピンポーーン");
}

TEST_F(MocrReadFileTest, Basic7)
{
    test_file("data/07.jpg", "ＬＩＮＫ！私達７人の力でガノンの塔の結界をやぶります");
}

TEST_F(MocrReadFileTest, Basic8)
{
    test_file("data/08.jpg", "ファイアパンチ");
}

TEST_F(MocrReadFileTest, Basic9)
{
    test_file("data/09.jpg", "少し黙っている");
}

TEST_F(MocrReadFileTest, Basic10)
{
    test_file("data/10.jpg", "わかるかな〜？");
}

TEST_F(MocrReadFileTest, Basic11)
{
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}

TEST_F(MocrReadFileTest, BasicMulti)
{
    test_file("data/00.jpg", "素直にあやまるしか");
    test_file("data/01.jpg", "立川で見た〝穴〟の下の巨大な眼は：");
    test_file("data/02.jpg", "実戦剣術も一流です");
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}

TEST_F(MocrReadFileTest, MissingFile)
{
    char *text = mocr_read_file(ctx, "/file/does/not/exist.jpg");
    EXPECT_EQ(text, nullptr);
}

class MocrReadTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ctx = mocr_init(DEFAULT_MODEL, 0);
        ASSERT_NE(ctx, nullptr);
    }

    void TearDown() override
    {
        EXPECT_EQ(mocr_destroy(ctx), 0);
    }

    void test_file(const char *path, const char *expected_text)
    {
        int width, height, channels;
        stbi_uc *data = stbi_load(path, &width, &height, &channels, 3);
        ASSERT_NE(data, nullptr);

        char *text = mocr_read(ctx, data, width, height, mocr_mode_RGB);
        ASSERT_NE(text, nullptr);
        EXPECT_STREQ(text, expected_text);
        EXPECT_EQ(mocr_free(text), 0);

        stbi_image_free(data);
    }

    mocr_ctx *ctx;
};

TEST_F(MocrReadTest, Basic0)
{
    test_file("data/00.jpg", "素直にあやまるしか");
}

TEST_F(MocrReadTest, Basic1)
{
    test_file("data/01.jpg", "立川で見た、穴への下の巨大な眼は．．．");
}

TEST_F(MocrReadTest, Basic2)
{
    test_file("data/02.jpg", "実戦剣術も一流です");
}

TEST_F(MocrReadTest, Basic3)
{
    test_file("data/03.jpg", "第３０話重苦しい闇の奥で静かに呼吸づきながら");
}

TEST_F(MocrReadTest, Basic4)
{
    test_file("data/04.jpg", "よかったじゃないわよ！何逃げてるのよ！！早くあいつを退治してよ！");
}

TEST_F(MocrReadTest, Basic5)
{
    test_file("data/05.jpg", "ぎゃっ");
}

TEST_F(MocrReadTest, Basic6)
{
    test_file("data/06.jpg", "ピンポーーン");
}

TEST_F(MocrReadTest, Basic7)
{
    test_file("data/07.jpg", "ＬＩＮＫ！私達７人の力でガノンの塔の結界をやぶります");
}

TEST_F(MocrReadTest, Basic8)
{
    test_file("data/08.jpg", "ファイアパンチ");
}

TEST_F(MocrReadTest, Basic9)
{
    test_file("data/09.jpg", "少し黙っている");
}

TEST_F(MocrReadTest, Basic10)
{
    test_file("data/10.jpg", "わかるかな〜？");
}

TEST_F(MocrReadTest, Basic11)
{
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}

TEST_F(MocrReadTest, BasicMulti)
{
    test_file("data/00.jpg", "素直にあやまるしか");
    test_file("data/01.jpg", "立川で見た、穴への下の巨大な眼は．．．");
    test_file("data/02.jpg", "実戦剣術も一流です");
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}

TEST(MocrFreeTest, Null)
{
    EXPECT_EQ(mocr_free(nullptr), 0);
}