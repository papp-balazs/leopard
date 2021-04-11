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

TEST(UriTests, ParseFromStringURN_DefaultPathDelimeter)
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

TEST(UriTests, ParseFromStringHasAPortNumber)
{
	Uri::Uri uri;

	ASSERT_TRUE(uri.ParseFromString("http://www.example.com:8080/foo/bar"));
	ASSERT_EQ("www.example.com", uri.GetHost());
	ASSERT_TRUE(uri.HasPort());
	ASSERT_EQ(8080, uri.GetPort());
}

TEST(UriTests, ParseFromStringDoesNotHaveAPortNumber)
{
	Uri::Uri uri;

	ASSERT_TRUE(uri.ParseFromString("http://www.example.com/foo/bar"));
	ASSERT_EQ("www.example.com", uri.GetHost());
	ASSERT_FALSE(uri.HasPort());
}

TEST(UriTests, ParseFromStringTwiceFirstWithPortNumberThenWithout)
{
	Uri::Uri uri;

	ASSERT_TRUE(uri.ParseFromString("http://www.example.com:8080/foo/bar"));
	ASSERT_TRUE(uri.ParseFromString("http://www.example.com/foo/bar"));
	ASSERT_FALSE(uri.HasPort());
}

TEST(UriTests, ParseFromStringBadPortNumberPurelyAlphabetic)
{
	Uri::Uri uri;

	ASSERT_FALSE(uri.ParseFromString("http://www.example.com:spam/foo/bar"));
}

TEST(UriTests, ParseFromStringBadPortNumberStartsNumericEndAlphabetic)
{
	Uri::Uri uri;

	ASSERT_FALSE(uri.ParseFromString("http://www.example.com:8080spa/foo/bar"));
}

TEST(UriTests, ParseFromStringLargestValidPortNumber)
{
	Uri::Uri uri;

	ASSERT_TRUE(uri.ParseFromString("http://www.example.com:65535/foo/bar"));
	ASSERT_EQ(65535, uri.GetPort());
}

TEST(UriTests, ParseFromStringBadPortNumberTooLong)
{
	Uri::Uri uri;

	ASSERT_FALSE(uri.ParseFromString("http://www.example.com:65536/foo/bar"));
}

TEST(UriTests, ParseFromStringBadPortNumberNegative)
{
	Uri::Uri uri;

	ASSERT_FALSE(uri.ParseFromString("http://www.example.com:-1111/foo/bar"));
}
