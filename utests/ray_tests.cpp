#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <../graphicsLib/ray.h>

TEST_CASE("Construction")
{
  ray r;

  REQUIRE(r.origin() == point3(0,0,0));
  REQUIRE(r.direction() == vec3(0,0,0));
}

TEST_CASE("Evaluation of Parametric Line")
{
  ray r;
  auto at = r.at(2);

  REQUIRE(point3(0,0,0) == at);
}

TEST_CASE("Immutability")
{
  ray r(point3(1,1,1), vec3(1,1,1));
  point3 line = r.at(2);

  REQUIRE(point3(3,3,3) == line);
}

TEST_CASE("Numerical Robustness")
{
  ray r(point3(0.1, 0.2, 0.3), vec3(0.4, 0.5, 0.6));
  double t = 2.0;

  point3 p = r.at(t);

  REQUIRE_THAT(p.x(), Catch::Matchers::WithinAbs(0.9, 1e-12));
  REQUIRE_THAT(p.y(), Catch::Matchers::WithinAbs(1.2, 1e-12));
  REQUIRE_THAT(p.z(), Catch::Matchers::WithinAbs(1.5, 1e-12));
}