#include <sfrp/behaviorutil.hpp>

namespace sfrp {
Behavior<double> BehaviorUtil::time() {
  return Behavior<double>::fromValuePullFunc([](double time) {
    return boost::make_optional(time);
  });
}
}
