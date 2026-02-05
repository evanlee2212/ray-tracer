#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../graphicsLib/Sphere.h"

TEST_CASE("Ray hits sphere head-on")
{
  Sphere s(point3(0,0,-5), 1.0f);
  ray r(point3(0,0,0), vec3(0,0,-1));

  REQUIRE(s.intersect(r) == true);
}

TEST_CASE("Ray misses sphere")
{
  Sphere s(point3(0,0,-5), 1.0f);
  ray r(point3(0,0,0), vec3(2,0,-1)); // angled away

  REQUIRE(s.intersect(r) == false);
}

TEST_CASE("Ray tangent to sphere")
{
  Sphere s(point3(0,1,-5), 1.0f);
  ray r(point3(0,0,0), vec3(0,0,-1));

  REQUIRE(s.intersect(r) == true);
}
