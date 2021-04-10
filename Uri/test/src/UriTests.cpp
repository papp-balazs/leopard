#include <gtest/gtest.h>
#include <Uri/Uri.hpp>

TEST(UriTests, ParseFromStringURL)
{
	Uri::Uri uri;

	ASSERT_TRUE(uri.ParseFromString("http://www.example.com/foo/bar"));
	ASSERT_EQ("http", uri.GetScheme());
	ASSERT_EQ("www.example.com", uri.GetHost());
	ASSERT_EQ(
		(std::vector< std::string >{
			"",
			"foo",
			"bar",
		}),
		uri.GetPath()
	);
}

TEST(UriTests, ParseFromStringURNDeafultPathDelimiter)
{
	Uri::Uri uri;

	ASSERT_TRUE(uri.ParseFromString("urn:book:fantasy:Hobbit"));
	ASSERT_EQ("urn", uri.GetScheme());
	ASSERT_EQ("", uri.GetHost());
	ASSERT_EQ(
		(std::vector< std::string >{
			"book:fantasy:Hobbit",
		}),
		uri.GetPath()
	);
}

TEST(UriTests, ParseFromStringURNSingleCharacterPathDelimiter)
{
	Uri::Uri uri;

	uri.SetPathDelimiter(":");
	ASSERT_TRUE(uri.ParseFromString("urn:book:fantasy:Hobbit"));
	ASSERT_EQ("urn", uri.GetScheme());
	ASSERT_EQ("", uri.GetHost());
	ASSERT_EQ(
		(std::vector< std::string >{
			"book",
			"fantasy",
			"Hobbit",
		}),
		uri.GetPath()
	);
}

TEST(UriTests, ParseFromStringURNMultiCharacterPathDelimiter)
{
	Uri::Uri uri;

	uri.SetPathDelimiter("/-");
	ASSERT_TRUE(uri.ParseFromString("urn:bo-/ok/-fan/tasy/-Hob-bit"));
	ASSERT_EQ("urn", uri.GetScheme());
	ASSERT_EQ("", uri.GetHost());
	ASSERT_EQ(
		(std::vector< std::string >{
			"bo-/ok",
			"fan/tasy",
			"Hob-bit",
		}),
		uri.GetPath()
	);
}

TEST(UriTests, ParseFromStringPathCornerCases)
{
	struct TestVector {
		std::string pathIn;
		std::vector< std::string > pathOut;
	};

	const std::vector< TestVector > testVectors{
		{"", {}},
		{"/", {""}},
		{"/foo", {"", "foo"}},
		{"foo/", {"foo", ""}},
	};

	for (const auto& testVector : testVectors)
	{
		Uri::Uri uri;

		ASSERT_TRUE(uri.ParseFromString(testVector.pathIn));
		ASSERT_EQ(testVector.pathOut, uri.GetPath());
	}
}
