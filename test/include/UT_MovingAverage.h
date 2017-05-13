#include "MovingAverageCalculator.h"

TEST(TestCaseMovingAverage, TestPublicFunctions){
    MovingAverageCalculator calc;
    const string test_str = "schedule -t mv_avg -d 20 -n 5";
    cout << "test input string: " << test_str << endl;
    string result = calc.Calculate(test_str);
    cout << "result is: " << result << endl;
    ASSERT_FALSE(result.empty());
}
