#include <Uri/Uri.hpp>
#include <inttypes.h>

namespace Uri {

	/**
	 * This contains the private properties of a Uri instance.
	 */
	struct Uri::Impl {
		/**
		 * This is the "scheme" element of the URI.
		 */
		std::string scheme;

		/**
		 * This is the "host" element of the URI.
		 */
		std::string host;

		/**
		 * This flag indicates whether or not the URI has a port number.
		 */
		bool hasPort = false;

		/**
		 * This is the "port" element of the URI.
		 */
		uint16_t port = 0;

		/**
		 * This is the "path" element of the URI as a sequence of segments.
		 */
		std::vector< std::string > path;

	};

	Uri::~Uri() = default;

	Uri::Uri()
		: impl_(new Impl)
	{}

	bool Uri::ParseFromString(const std::string& uriString)
	{
		// First parse the scheme.
		const auto schemeEnd = uriString.find(':');
		impl_->scheme = uriString.substr(0, schemeEnd);
		auto rest = uriString.substr(schemeEnd + 1);

		// Next, parse the host.
		impl_->hasPort = false;

		if (rest.substr(0, 2) == "//")
		{
			const auto authorityEnd = rest.find('/', 2);
			const auto portDelimiter = rest.find(':');

			if (portDelimiter == std::string::npos)
			{
				impl_->host = rest.substr(2, authorityEnd - 2);
			}
			else
			{
				impl_->host = rest.substr(2, portDelimiter - 2);
				uint32_t newPort = 0;

				for (auto c: rest.substr(portDelimiter + 1, authorityEnd - portDelimiter - 1))
				{
					if ((c < '0') || (c > '9'))
					{
						return false;
					}

					newPort *= 10;
					newPort += (uint16_t)(c - '0');

					if ((newPort & ~((1 << 16) - 1)) != 0)
					{
						return false;
					}
				}

				impl_->hasPort = true;
				impl_->port = (uint16_t)newPort;
			}

			rest = rest.substr(authorityEnd);
		}
		else
		{
			impl_->host.clear();
		}

		// Finally, parse the path.
		impl_->path.clear();

		if (rest == "/")
		{
			// Special case of a path that is empty but needs a single
			// empty-string element to indiciate that is is absolute.
			impl_->path.push_back("");
		}
		else if (!rest.empty())
		{
			for (;;)
			{
				auto pathDelimeter = rest.find('/');

				if (pathDelimeter == std::string::npos)
				{
					impl_->path.push_back(rest);
					break;
				}
				else
				{
					impl_->path.emplace_back(
						rest.begin(),
						rest.begin() + pathDelimeter
					);

					rest = rest.substr(pathDelimeter + 1);
				}
			}
		}

		return true;
	}

	std::string Uri::GetScheme() const
	{
		return impl_->scheme;
	}

	std::string Uri::GetHost() const
	{
		return impl_->host;
	}

	bool Uri::HasPort() const
	{
		return impl_->hasPort;
	}

	uint16_t Uri::GetPort() const
	{
		return impl_->port;
	}

	std::vector< std::string > Uri::GetPath() const
	{
		return impl_->path;
	}

}
