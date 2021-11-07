//how to generate random numbers
/*
the most short version:
    std::random_device rd1;
    std::uniform_int_distribution<int> ufd(0, 8); or std::uniform_real_distribution<float> urd(0, 8);
*/
#include <iostream>

#include <cstdlib> //for std::rand()
#include <ctime> // for seeding std::srand()

#include <random> // for c++1 random (engine, uniform dist.)

int main()
{
    //**********************c style random int******************************
    std::cout << std::rand() << '\n'; //int between 0 and RAND_MAX inclded (32767)
    std::cout << std::rand() << '\n';

    /*
    Issues:
    1. although both statements above generate different numbers, with each build
    they generate the SAME NUMBERS! why? when std::rand() is uesd before std::srand()
    is executed, std::rand() behaves as if it was seeded with std::srand(1) (same seed!)
    std::srand "seeds" std::rand with some initial value - the same seed will yield
    the same random numbers. Solution:
    */

    //**********************c style random int  - correct******************************
    std::srand(std::time(nullptr)); // current time as a seed
    std::cout << std::rand() << '\n'; // random number with each compilation
    std::cout << std::rand() << '\n'; // produces different number then the above

    //cppreference recommends not to use std::rand, but rather recommends
    //c++11 random number genarator

    //**************random int between 2 and 10 (including both)*******************
    //std::rand() % 9 is bounded between 0 and 8 including (99%10 =9  because 99/10 = 9(***9****/10))
    //and we add 2 to reach numbers between 2 and 10
    std::cout << std::rand() % 8 + 2 << '\n';
    
    //**************random float between [0,1]*******************
    //explicitly casting RAND_MAX (32767) to double will cause std::rand to be implicitly
    //promooted to a double too. std::rand produces an int [0,RAND_MAX], divided by RANDOM_MAX will produce a random float.
    //more precisely, the following floats can be produced:
    //(1/32767), (2/32767), (3/32767), ...., (32767/32767).
    //note: if we want a random float between [0,1) we can just add 1 to RAND_MAX:
    // std::rand() / (double(RAND_MAX) +1  this will produce at most the following value (32767/32768)
    std::cout << std::rand() / (double(RAND_MAX)) << '\n';

    //**************random float between [M,N), e.g: [4.0, 8.0]*******************
    //having a double value in the statement below will cause an implicit promotion
    //of all ints such as RAND_MAX to a double (see more at 69_casting)
    //explained: rand() / (RAND_MAX/ 8.0 - 4.0) ensures that we get values such as:
    // 28742 / 8191.75  = in other words, a number between 0-4
    //4.0 + simply adds for to ensure the value we get as minimum 4
    std::cout << 4.0 + (std::rand() / (RAND_MAX / 8.0 - 4.0)) << '\n';

    /**********************c++11 recommended random int******************************
    uses three parts:
    1. std::random_device: random number generator used to seed (2). we leave out (2)
    and use only (1) instead. The issue is that random_device is represents a call
    to microsoft API (we don't know which as opposed to (2)) and cpp reference
    warns against degrading performance after some calls and recommends to use 
    it only to seed a random engine
    https://en.cppreference.com/w/cpp/numeric/random/random_device
    https://stackoverflow.com/questions/39288595/why-not-just-use-random-device
    2. default_random_engine: random number generators (mt19937, ranlux48....)
    3. Random number distributions (uniform_int, real, bernoulli, poisson, exponentioal,...)

    */
    //random_device is equivalent to srand here, without it - same results in each compilation.
    //how to remember? distribution uses generator and generator uses rd
    std::random_device rd;
    std::default_random_engine generator(rd()); // 
    std::uniform_int_distribution<int> distribution(0, 9);

    std::cout << distribution(generator) << '\n';
    std::cout << distribution(generator) << '\n';

    //wihout random_device we will get the same random results in each compilation
    /*std::default_random_engine generator();
    std::uniform_int_distribution<int> distribution(0, 9);*/


    /**********************c++11 random short version (without engine)******************************/
    std::random_device rd1;
    std::uniform_int_distribution<int> ufd(0, 8);

    //both statements produce different numbers each time
    std::cout << ufd(rd1) << '\n'; 
    std::cout << ufd(rd1) << '\n';

    /**********************c++11 float random******************************/
    std::uniform_real_distribution<double> urd(0, 100);
    std::cout << urd(rd1) << '\n';
    
}