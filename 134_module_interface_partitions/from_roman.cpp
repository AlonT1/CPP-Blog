// Implementation of the from_roman() function
module roman;
import :internals;

unsigned int from_roman(std::string_view roman1)
{
    unsigned int result{};
    for (size_t i{}, n{ roman1.length() }; i < n; ++i)
    {
        const auto j{ from_roman(roman1[i]) };   // Integer value of the i'th roman digit
        // Look at the next digit (if there is one) to know whether to add or subtract j
        if (i + 1 == n || j >= from_roman(roman1[i + 1])) result += j; else result -= j;
    }
    return result;
}