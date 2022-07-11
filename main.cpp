#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

enum class Gender {
    FEMALE,
    MALE
};

struct Person {
    int age;  // возраст
    Gender gender;  // пол
    bool is_employed;  // имеет ли работу
};


template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = begin(range_copy) + range_copy.size() / 2;
    nth_element(
            begin(range_copy), middle, end(range_copy),
            [](const Person& lhs, const Person& rhs) {
                return lhs.age < rhs.age;
            }
    );
    return middle->age;
}

void PrintStats(std::vector<Person> persons) {
    const int median_age = ComputeMedianAge(std::begin(persons), std::end(persons));

    auto female_begin = std::partition(std::begin(persons), std::end(persons), [](const Person& person) { return person.gender == Gender::MALE;});
    const int median_age_female = ComputeMedianAge(female_begin, std::end(persons));
    const int median_age_male = ComputeMedianAge(std::begin(persons), female_begin);

    auto female_unemployed_begin = std::partition(female_begin, std::end(persons), [](const Person& person) { return person.is_employed;});
    const int median_age_female_employed = ComputeMedianAge(female_begin, female_unemployed_begin);
    const int median_age_female_unemployed = ComputeMedianAge(female_unemployed_begin, std::end(persons));

    auto male_unemployed_begin = std::partition(std::begin(persons), female_begin, [](const Person& person) { return person.is_employed;});
    const int median_age_male_employed = ComputeMedianAge(std::begin(persons), male_unemployed_begin);
    const int median_age_male_unemployed = ComputeMedianAge(male_unemployed_begin, female_begin);

    cout << "Median age = " <<  median_age << "\n";
    cout << "Median age for females = " <<  median_age_female << "\n";
    cout << "Median age for males = " <<  median_age_male << "\n";
    cout << "Median age for employed females = " <<  median_age_female_employed << "\n";
    cout << "Median age for unemployed females = " <<  median_age_female_unemployed << "\n";
    cout << "Median age for employed males = " <<  median_age_male_employed << "\n";
    cout << "Median age for unemployed males = " <<  median_age_male_unemployed << "\n";

}

int main() {
    vector<Person> persons = {
            {31, Gender::MALE, false},
            {40, Gender::FEMALE, true},
            {24, Gender::MALE, true},
            {20, Gender::FEMALE, true},
            {80, Gender::FEMALE, false},
            {78, Gender::MALE, false},
            {10, Gender::FEMALE, false},
            {55, Gender::MALE, true},
    };
    PrintStats(persons);
    return 0;
}
