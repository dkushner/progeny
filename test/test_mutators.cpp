#include <gtest/gtest.h>
#include <iostream>
#include <vector>

/*
TEST(Mutators, Crossover) {
  PROGENY_TYPE(std::tuple<int, double, char>);

  Crossover<1, Progeny> xo;

  Progeny p1(1, 0.1, 'a');
  Progeny p2(2, 0.2, 'b');
  Progeny p3(3, 0.3, 'c');
  Progeny p4(4, 0.4, 'd');
  Progeny p5(5, 0.5, 'e');
  Population p { p1, p2, p3, p4, p5 };

  std::cout << "BEFORE CROSSOVER" << std::endl;
  std::cout << "================" << std::endl;
  for (auto m : p) {
    std::cout << m << std::endl;
  }

  xo(p);

  std::cout << "AFTER CROSSOVER" << std::endl;
  std::cout << "================" << std::endl;
  for (auto m : p) {
    std::cout << m << std::endl;
  }

  EXPECT_EQ(p.size(), 5);
}

TEST(Mutators, CrossoverString) {
  PROGENY_TYPE(std::string);

  Crossover<2, Progeny> xo;

  Progeny p1("00000000");
  Progeny p2("11111111");

  Population p { p1, p2 };

  std::cout << "BEFORE CROSSOVER" << std::endl;
  std::cout << "================" << std::endl;
  for (auto m : p) {
    std::cout << m << std::endl;
  }
  
  xo(p);

  std::cout << "AFTER CROSSOVER" << std::endl;
  std::cout << "================" << std::endl;
  for (auto m : p) {
    std::cout << m << std::endl;
  }

  EXPECT_EQ(p.size(), 2);
}

TEST(Mutators, Split) {
  PROGENY_TYPE(std::string);
}

TEST(Mutators, PassThrough) {
  PROGENY_TYPE(std::string);

  PassThrough<Progeny> pt;

  Progeny p1("test");
  Progeny p2("test");
  Progeny p3("test");
  Progeny p4("test");
  Progeny p5("test");

  Population p { p1, p2, p3, p4, p5 };
  pt(p);

  EXPECT_EQ(p.size(), 5);
  for (auto m : p) {
    EXPECT_STREQ("test", m.c_str());
  }
}
*/
