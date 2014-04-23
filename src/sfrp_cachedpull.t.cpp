#include <boost/optional.hpp>
#include <sfrp/cachedpull.hpp>

static int someTimeFunction(double time) { return 3; }

static int someTimeFunctionCached(double time) {
  // We keep our cache in a static variable.
  static boost::optional<sfrp::CachedPull<int>> previousPull;

  // If the cache matches use it.
  if (previousPull && previousPull->time() == time)
    return previousPull->value();
  else {
    // Call 'someTimeFunction', set the cache, and return the result.
    int result = someTimeFunction(time);
    previousPull = sfrp::CachedPull<int>(time, result);
    return result;
  }
}
