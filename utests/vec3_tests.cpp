#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <../graphicsLib/vec3.h>

TEST_CASE( "Simple Vec3 Addition and Subtraction" )
{
  vec3 v1 = vec3(1,0,1);

  vec3 v2 = vec3(0,1,0);

  REQUIRE(vec3(1,1,1) == (v1 + v2));
  REQUIRE(vec3(1,-1,1) == (v1 - v2));
}

TEST_CASE( "Simple Vec3 Multiplication and Division" )
{
  vec3 v1 = vec3(1,0,1);
  vec3 v2 = vec3(0,1,0);

  REQUIRE(vec3(0,0,0) == (v1 * v2));
  REQUIRE(vec3(2,0,2) == (2 * v1));
  REQUIRE(vec3(2,0,2) == (v1 * 2));
  REQUIRE(vec3(0, 0.5, 0) == (v2/2));
}

TEST_CASE( "Simple Vec3 Functions" )
{
  vec3 v1 = vec3(1,0,1);

  vec3 v2 = vec3(0,1,0);

  REQUIRE(0 == (dot(v1, v2)));
  REQUIRE(vec3(-1,0,1) == cross(v1, v2));
}

TEST_CASE("Unit Vector")
{
  vec3 v1 = vec3(2,2,2);
  vec3 expected = vec3(1,1,1) / sqrt(3);

  REQUIRE(unit_vector(v1) == expected);
}
