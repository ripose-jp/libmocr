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

#include "mocr++.h"

#include <future>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define KHA_WHITE_MODEL ("kha-white/manga-ocr-base")

TEST(MocrxxInitTest, Basic)
{
    mocr::model model;
    EXPECT_TRUE(model.valid());
    EXPECT_FALSE(!model);
}

TEST(MocrxxInitTest, Cpu)
{
    mocr::model model(KHA_WHITE_MODEL, true);
    EXPECT_TRUE(model.valid());
    EXPECT_FALSE(!model);
}

TEST(MocrxxInitTest, ModelNotFound)
{
    mocr::model model("/model/does/not/exist");
    EXPECT_FALSE(model.valid());
    EXPECT_TRUE(!model);
}

TEST(MocrxxInitTest, BasicStdString)
{
    mocr::model model(std::string(KHA_WHITE_MODEL));
    EXPECT_TRUE(model.valid());
    EXPECT_FALSE(!model);
}

TEST(MocrxxInitTest, BasicStdStringCpu)
{
    mocr::model model(std::string(KHA_WHITE_MODEL), true);
    EXPECT_TRUE(model.valid());
    EXPECT_FALSE(!model);
}

TEST(MocrxxInitTest, ModelStdStringNotFound)
{
    mocr::model model(std::string("/model/does/not/exist"));
    EXPECT_FALSE(model.valid());
    EXPECT_TRUE(!model);
}

TEST(MocrxxInitTest, Multi)
{
    mocr::model model1;
    mocr::model model2;
    EXPECT_TRUE(model1.valid());
    EXPECT_FALSE(!model1);
    EXPECT_TRUE(model2.valid());
    EXPECT_FALSE(!model2);
}

/* This test only fails on macOS, I suspect due to a bug in OpenBLAS */
#if !defined(__APPLE__)
TEST(MocrxxInitTest, MultiThreadInit)
{
    mocr::model *model = std::async(
        std::launch::async,
        [] () -> mocr::model * { return new mocr::model; }
    ).get();
    ASSERT_NE(model, nullptr);
    EXPECT_TRUE(model->valid());
    EXPECT_FALSE(!*model);
    delete model;
}
#endif

class MocrxxReadFileTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ASSERT_TRUE(ctx.valid());
    }

    void test_file(const char *path, const char *expected_text)
    {
        EXPECT_STREQ(ctx.read(path).c_str(), expected_text);
    }

    std::future<void> test_file_async(const char *path, const char *expected_text)
    {
        return std::async(
            std::launch::async,
            &MocrxxReadFileTest::test_file, this, path, expected_text
        );
    }

    mocr::model ctx;
};

TEST_F(MocrxxReadFileTest, Basic0)
{
    test_file("data/00.jpg", "素直にあやまるしか");
}

TEST_F(MocrxxReadFileTest, Basic1)
{
    test_file("data/01.jpg", "立川で見た〝穴〟の下の巨大な眼は：");
}

TEST_F(MocrxxReadFileTest, Basic2)
{
    test_file("data/02.jpg", "実戦剣術も一流です");
}

TEST_F(MocrxxReadFileTest, Basic3)
{
    test_file("data/03.jpg", "第３０話重苦しい闇の奥で静かに呼吸づきながら");
}

TEST_F(MocrxxReadFileTest, Basic4)
{
    test_file("data/04.jpg", "よかったじゃないわよ！何逃げてるのよ！！早くあいつを退治してよ！");
}

TEST_F(MocrxxReadFileTest, Basic5)
{
    test_file("data/05.jpg", "ぎゃっ");
}

TEST_F(MocrxxReadFileTest, Basic6)
{
    test_file("data/06.jpg", "ピンポーーン");
}

TEST_F(MocrxxReadFileTest, Basic7)
{
    test_file("data/07.jpg", "ＬＩＮＫ！私達７人の力でガノンの塔の結界をやぶります");
}

TEST_F(MocrxxReadFileTest, Basic8)
{
    test_file("data/08.jpg", "ファイアパンチ");
}

TEST_F(MocrxxReadFileTest, Basic9)
{
    test_file("data/09.jpg", "少し黙っている");
}

TEST_F(MocrxxReadFileTest, Basic10)
{
    test_file("data/10.jpg", "わかるかな〜？");
}

TEST_F(MocrxxReadFileTest, Basic11)
{
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}

TEST_F(MocrxxReadFileTest, BasicMulti)
{
    test_file("data/00.jpg", "素直にあやまるしか");
    test_file("data/01.jpg", "立川で見た〝穴〟の下の巨大な眼は：");
    test_file("data/02.jpg", "実戦剣術も一流です");
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}

TEST_F(MocrxxReadFileTest, Basic0Async)
{
    test_file_async("data/00.jpg", "素直にあやまるしか").wait();
}

TEST_F(MocrxxReadFileTest, BasicMultiAsync)
{
    std::future<void> basic0 =
        test_file_async("data/00.jpg", "素直にあやまるしか");
    std::future<void> basic1 =
        test_file_async("data/01.jpg", "立川で見た〝穴〟の下の巨大な眼は：");
    std::future<void> basic2 =
        test_file_async("data/02.jpg", "実戦剣術も一流です");
    std::future<void> basic3 =
        test_file_async("data/11.jpg", "警察にも先生にも町中の人達に！！");

    basic0.wait();
    basic1.wait();
    basic2.wait();
    basic3.wait();
}

TEST_F(MocrxxReadFileTest, MissingFile)
{
    std::string text = ctx.read("/file/does/not/exist.jpg");
    EXPECT_TRUE(text.empty());
}

TEST_F(MocrxxReadFileTest, Basic0StdString)
{
    EXPECT_STREQ(ctx.read(std::string("data/00.jpg")).c_str(), "素直にあやまるしか");
}

TEST_F(MocrxxReadFileTest, MissingFileStdString)
{
    EXPECT_TRUE(ctx.read(std::string("/file/does/not/exist")).empty());
}

class MocrxxReadTest : public ::testing::Test
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
        ASSERT_NE(data, nullptr);

        std::string text = ctx.read(data, width, height, mocr::mode::RGB);
        EXPECT_STREQ(text.c_str(), expected_text);

        stbi_image_free(data);
    }

    std::future<void> test_file_async(const char *path, const char *expected)
    {
        return std::async(
            std::launch::async,
            &MocrxxReadTest::test_file, this, path, expected
        );
    }

    mocr::model ctx;
};

TEST_F(MocrxxReadTest, Basic0)
{
    test_file("data/00.jpg", "素直にあやまるしか");
}

TEST_F(MocrxxReadTest, Basic1)
{
    test_file("data/01.jpg", "立川で見た、穴への下の巨大な眼は．．．");
}

TEST_F(MocrxxReadTest, Basic2)
{
    test_file("data/02.jpg", "実戦剣術も一流です");
}

TEST_F(MocrxxReadTest, Basic3)
{
    test_file("data/03.jpg", "第３０話重苦しい闇の奥で静かに呼吸づきながら");
}

TEST_F(MocrxxReadTest, Basic4)
{
    test_file("data/04.jpg", "よかったじゃないわよ！何逃げてるのよ！！早くあいつを退治してよ！");
}

TEST_F(MocrxxReadTest, Basic5)
{
    test_file("data/05.jpg", "ぎゃっ");
}

TEST_F(MocrxxReadTest, Basic6)
{
    test_file("data/06.jpg", "ピンポーーン");
}

TEST_F(MocrxxReadTest, Basic7)
{
    test_file("data/07.jpg", "ＬＩＮＫ！私達７人の力でガノンの塔の結界をやぶります");
}

TEST_F(MocrxxReadTest, Basic8)
{
    test_file("data/08.jpg", "ファイアパンチ");
}

TEST_F(MocrxxReadTest, Basic9)
{
    test_file("data/09.jpg", "少し黙っている");
}

TEST_F(MocrxxReadTest, Basic10)
{
    test_file("data/10.jpg", "わかるかな〜？");
}

TEST_F(MocrxxReadTest, Basic11)
{
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}

TEST_F(MocrxxReadTest, BasicMulti)
{
    test_file("data/00.jpg", "素直にあやまるしか");
    test_file("data/01.jpg", "立川で見た、穴への下の巨大な眼は．．．");
    test_file("data/02.jpg", "実戦剣術も一流です");
    test_file("data/11.jpg", "警察にも先生にも町中の人達に！！");
}

TEST_F(MocrxxReadTest, Basic0Async)
{
    test_file_async("data/00.jpg", "素直にあやまるしか").wait();
}

TEST_F(MocrxxReadTest, BasicMultiAsync)
{
    std::future<void> basic0 =
        test_file_async("data/00.jpg", "素直にあやまるしか");
    std::future<void> basic1 =
        test_file_async("data/01.jpg", "立川で見た、穴への下の巨大な眼は．．．");
    std::future<void> basic2 =
        test_file_async("data/02.jpg", "実戦剣術も一流です");
    std::future<void> basic3 =
        test_file_async("data/11.jpg", "警察にも先生にも町中の人達に！！");

    basic0.wait();
    basic1.wait();
    basic2.wait();
    basic3.wait();
}
