#include <gtest/gtest.h>
#include <string>
#include "vector.h"

using namespace bmstu;

TEST(Constructor, SameValueEverywhere){
    vector<int> test_vector(5, 5);
    for (auto element : test_vector){
        ASSERT_EQ(element, 5);
    }
    ASSERT_EQ(test_vector.size(), 5);

    try{
        vector<std::string> bad_vector(-100, "I am a bad vector");
    } catch (std::bad_alloc const& error) {
        ASSERT_EQ(error.what(), std::string("std::bad_alloc"));
    }
}

TEST(Constructor, Copy){
    vector<int> test_vector(5, 5);
    vector<int> another_test_vector(test_vector);
    for (auto element : another_test_vector){
        ASSERT_EQ(element, 5);
    }
    ASSERT_EQ(another_test_vector.size(), 5);
}

TEST(Constructor, InitializerList){
    vector<int> test_vector({5, 5, 5, 5, 5, 5, 5});
    vector<int> correct_vector(7, 5);
    for (int i=0; i<7; i++){
        ASSERT_EQ(test_vector[i], correct_vector[i]);
    }
    ASSERT_EQ(test_vector.size(), correct_vector.size());
}

TEST(Method, Clear){
    vector<std::string> test_vector({"Ne", "Chistite menya", "Neeeeet!!!"});
    test_vector.clear();
    ASSERT_EQ(test_vector.size(), 0);
}

TEST(Operator, Assignment){
    vector<int> test_vector({1, 2, 3, 4, 5, 6});
    vector<int> vector_using_operator = test_vector;
    for (int j=0; j<6; j++){
        ASSERT_EQ(test_vector[j], vector_using_operator[j]);
    }
}

TEST(Method, Swap1){
    vector<char> test_vector({'I', 'l', 'o', 'v', 'e', 'K', 'V', 'A', 'S', 'S'});
    vector<char> after_swap_vector({'S', 'a', 'l', 'a', 'm', 'b', 'r', 'a', 't'});

    vector<char> correct_vector({'I', 'l', 'o', 'v', 'e', 'K', 'V', 'A', 'S', 'S'});

    after_swap_vector.swap(test_vector);
    for (int k=0; k<correct_vector.size(); k++){
        ASSERT_EQ(after_swap_vector[k], correct_vector[k]);
    }
}

TEST(Method, Swap2){
    vector<int> test_vector({1, 2, 3, 4, 5});
    vector<int> another_test_vector({100});

    vector<int> correct_test_vector_after_swap({100});
    vector<int> correct_another_test_vector_after_swap({1, 2, 3, 4, 5});

    swap(test_vector, another_test_vector);

    ASSERT_EQ(test_vector[0], correct_test_vector_after_swap[0]);
    for (int g=0; g<5; g++){
        ASSERT_EQ(another_test_vector[g], correct_another_test_vector_after_swap[g]);
    }

    ASSERT_EQ(test_vector.size(), correct_test_vector_after_swap.size());
    ASSERT_EQ(another_test_vector.size(), correct_another_test_vector_after_swap.size());
}

TEST(Method, Resize){
    vector<int> test_vector({1, 2, 3, 4, 5});
    test_vector.reserve(10);
    test_vector.resize(10);
    ASSERT_EQ(test_vector.size(), 10);
}

TEST(Method, Reserve){
    vector<int> test_vector({1, 2, 3, 4, 5});
    test_vector.reserve(10);
    ASSERT_EQ(test_vector.size(), 5);
    ASSERT_EQ(test_vector.capacity(), 10);
}

TEST(Method, Insert){
    vector<int> test_vector({1, 2, 3, 4, 5});
    test_vector.insert(++test_vector.begin(), 200);
    vector<int> correct_vector({1, 200, 2, 3, 4, 5});
    ASSERT_EQ(test_vector.size(), correct_vector.size());
    for (int n=0; n<test_vector.size(); n++){
        ASSERT_EQ(test_vector[n], correct_vector[n]);
    }
}

TEST(Method, Pushback){
    vector<int> test_vector({1, 2, 3, 4, 5});
    test_vector.push_back(100);
    vector<int> correct_vector({1, 2, 3, 4, 5, 100});
    ASSERT_EQ(test_vector.size(), correct_vector.size());
    for (int n=0; n<test_vector.size(); n++){
        ASSERT_EQ(test_vector[n], correct_vector[n]);
    }
}

TEST(Method, Popback){
    vector<int> test_vector({1, 2, 3, 4, 5});
    test_vector.pop_back();
    ASSERT_EQ(test_vector.size(), 4);
}

TEST(Method, Emplaceback){
    vector<std::pair<int, int>> test_vector({std::make_pair(1, 2), std::make_pair(2, 3)});
    test_vector.emplace_back(3, 4); // определяет тип данных пары без std::make_pair суперхорош
    vector<std::pair<int, int>> correct_vector({std::make_pair(1, 2), std::make_pair(2, 3), std::make_pair(3, 4)});
    for (int p=0; p<test_vector.size(); p++){
        ASSERT_EQ(test_vector[p].first, correct_vector[p].first);
        ASSERT_EQ(test_vector[p].second, correct_vector[p].second);
    }
    ASSERT_EQ(test_vector.size(), 3);
}

TEST(Method, Emplace){
    vector<std::pair<int, int>> test_vector({std::make_pair(1, 2), std::make_pair(2, 3)});
    test_vector.emplace(++test_vector.begin(), 3, 4); // определяет тип данных пары без std::make_pair суперхорош
    vector<std::pair<int, int>> correct_vector({std::make_pair(1, 2), std::make_pair(3, 4), std::make_pair(2, 3)});
    for (int p=0; p<test_vector.size(); p++){
        ASSERT_EQ(test_vector[p].first, correct_vector[p].first);
        ASSERT_EQ(test_vector[p].second, correct_vector[p].second);
    }
    ASSERT_EQ(test_vector.size(), 3);

    try {
        test_vector.emplace(++++++++++++++++++++++++++++++++++++++++test_vector.begin(), 100, 200);
    } catch (std::out_of_range const &error) {
        ASSERT_EQ(error.what(), std::string("Position is out of range"));
    }
}

TEST(Method, Erase){
    vector<std::string> test_vector({"The next element is destined to die", "I am destined to die", "I am not destined to die, yay"});
    test_vector.erase(++test_vector.begin());
    ASSERT_EQ(test_vector[0], "The next element is destined to die");
    ASSERT_EQ(test_vector[1], "I am not destined to die, yay");
    ASSERT_EQ(test_vector.size(), 2);
}

TEST(Operator, Equal){
    vector<int> test_vector({1, 2, 3, 4, 5});
    vector<int> another_test_vector({1, 2, 3, 4, 5});
    ASSERT_TRUE(test_vector == another_test_vector);
}

TEST(Operator, NotEqual){
    vector<int> test_vector({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    vector<int> another_test_vector({1, 2, 3, 4, 5});
    ASSERT_TRUE(test_vector != another_test_vector);
}

TEST(Operator, Lessthan){
    vector<int> test_vector({1, 2, 3, 4, 5});
    vector<int> another_test_vector({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    ASSERT_TRUE(test_vector < another_test_vector);
}

TEST(Operator, Morethan){
    vector<int> test_vector({1, 2, 3, 4, 5});
    vector<int> another_test_vector({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    ASSERT_TRUE(another_test_vector > test_vector);
}

TEST(Operator, LessOrEqual){
    vector<int> test_vector({1, 2, 3, 4, 5});
    vector<int> another_test_vector({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    ASSERT_TRUE(test_vector < another_test_vector);

    vector<int> equal_vector({1, 2, 3, 4, 5});
    ASSERT_TRUE(test_vector <= equal_vector);
}

TEST(Operator, MoreOrEqual){
    vector<int> test_vector({1, 2, 3, 4, 5});
    vector<int> another_test_vector({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    ASSERT_TRUE(another_test_vector > test_vector);

    vector<int> equal_vector({1, 2, 3, 4, 5});
    ASSERT_TRUE(test_vector >= equal_vector);
}

TEST(Operator, Ostream){
    vector<int> test_vector({1, 2, 3, 4, 5});
    std::stringstream ss;
    ss << test_vector;
    ASSERT_STREQ(ss.str().c_str(), "1 2 3 4 5 ");
}

TEST(Operator, GetElementByIndex){
    vector<int> test_vector({1, 2, 3, 4, 5});
    ASSERT_EQ(test_vector[2], 3);
    try {
        int a = test_vector[100];
    } catch (std::out_of_range const &error) {
        ASSERT_EQ(error.what(), std::string("Index is out of range"));
    }
}