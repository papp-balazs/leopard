#include <Uri/Uri.hpp>

namespace Uri {
	/**
	 * This contains the private of a Uri instance.
	 */
	struct Uri::Impl {
	};

	Uri::~Uri() = default;

	Uri::Uri()
		: impl_(new Impl)
	{}
}
