#include <gtest/gtest.h>
#include <iostream>
#include <vector>

/*
TEST(Pipeline, Chaining) {
  PROGENY_TYPE(std::tuple<int, double, char>);

  Crossover<1, Progeny> co;
  PassThrough<Progeny> pt;

  Progeny p1(1, 1.0, 'a');
  Progeny p2(2, 2.0, 'b');
  Progeny p3(3, 3.0, 'c');
  Progeny p4(4, 4.0, 'd');
  Progeny p5(5, 5.0, 'e');
  Progeny p6(6, 6.0, 'f');

  Population p { p1, p2, p3, p4, p5, p6 };

  auto pipeline = (pt >> co);
  pipeline(p);

  EXPECT_EQ(p.size(), 6);
}

TEST(Pipeline, Inlining) {
  PROGENY_TYPE(std::tuple<int, double, char>);

  Progeny p1(1, 1.0, 'a');
  Progeny p2(2, 2.0, 'b');
  Progeny p3(3, 3.0, 'c');
  Progeny p4(4, 4.0, 'd');
  Progeny p5(5, 5.0, 'e');
  Progeny p6(6, 6.0, 'f');

  Population p { p1, p2, p3, p4, p5, p6 };

  auto pipeline = Crossover<1, Progeny>() >> PassThrough<Progeny>();
  pipeline(p);

  EXPECT_EQ(p.size(), 6);
}
*/
