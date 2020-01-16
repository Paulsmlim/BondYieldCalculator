//
//  BondYieldCalculator.cpp
//  
//
//  Created by Paul Lim on 1/14/20.
//

#include <iostream>
#include <string>
#include <math.h>
#include <chrono>

#define accuracyDelta pow(10, -7)

// All inputs are assumed to be non-negative values
bool parseBondArgs(double& coupon, int& years, double& face, double& rateOrPrice, bool getRate) {
    std::string arg;
    try {
        std::cout << " Coupon %: ";
        std::cin >> arg;
        coupon = std::stod(arg);
        
        std::cout << " Year(s): ";
        std::cin >> arg;
        years = std::stoi(arg);
        
        std::cout << " Face Value: ";
        std::cin >> arg;
        face = std::stod(arg);
        
        if (getRate) {
            std::cout << " Rate: ";
        } else {
            std::cout << " Price: ";
        }
        std::cin >> arg;
        rateOrPrice = std::stod(arg);
    } catch (...) {
        // an exception is caught; an input string could not be converted to an int/double
        return false;
    }
    return true;
}

// -------------------------------------- BOND YIELD CALCULATOR METHODS --------------------------------------
double CalcPrice(double coupon, int years, double face, double rate) {
    double price = 0.0;
    double couponPayment = coupon * face;
    // Add discounted coupon payments using the Annuity formula
    price = couponPayment/rate * (1 - 1/pow(1.0+rate, years));
    // Add discounted face value
    price += face / pow(1.0+rate, years);
    return price;
}

// Recursive method using same algorithm as binary search to find the yield of the stock
double CalcYieldHelper(double coupon, int years, double face, double price, double lowYield, double highYield) {
    // midYield is the yield that will be used as the next guess
    double midYield = (lowYield + highYield) / 2.0;
    double currYieldPrice = CalcPrice(coupon, years, face, midYield);
    if (std::abs(currYieldPrice - price) <= accuracyDelta) {
        // A stock with midYield yield is valued at the price equal to the user's 'price' input
        //  (off by at most accuracy limit 10^-7)
        return midYield;
    } else if (currYieldPrice > price) {
        // midYield is too low / price did not get discounted enough, which led to a higher than actual price
        return CalcYieldHelper(coupon, years, face, price, midYield, highYield);
    } else {
        // currYieldPrice < price; therefore, midYield is too high / price got discounted too much
        return CalcYieldHelper(coupon, years, face, price, lowYield, midYield);
    }
}

double CalcYield(double coupon, int years, double face, double price) {
    return CalcYieldHelper(coupon, years, face, price, 0.0, 1.0);
}
// -----------------------------------------------------------------------------------------------------------

void CalcPriceOption() {
    double coupon, face, rate, price;
    int years;
    
    if (!parseBondArgs(coupon, years, face, rate, true)) {
        std::cout << "Input parse error. Please enter values in correct format\n" << std::endl;
        return;
    }
    // track the run time of bond price calculation
    auto start = std::chrono::high_resolution_clock::now();
    price = CalcPrice(coupon, years, face, rate);
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout.precision(7);
    std::cout << "The price of the bond is " << std::fixed << price << std::endl;
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "Runtime of calculating the price of this stock: " << duration.count() << " ns\n" << std::endl;
}

void CalcYieldOption() {
    double coupon, face, price, yield;
    int years;
    
    if (!parseBondArgs(coupon, years, face, price, false)) {
        std::cout << "Input parse error. Please enter values in correct format\n" << std::endl;
        return;
    }
    // track the run time of bond yield calculation
    auto start = std::chrono::high_resolution_clock::now();
    yield = CalcYield(coupon, years, face, price);
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout.precision(7);
    std::cout << "The yield of the bond is " << std::fixed << yield << std::endl;
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "Runtime of calculating the yield of this stock: " << duration.count() << " ns\n" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string command = "";
    while (true) {
        // Print prompt and read input
        std::cout << "Enter an option # (enter 'q' to exit):" << std::endl;
        std::cout << " 1) Calculate the Price of a Bond" << std::endl;
        std::cout << " 2) Calculate the Yield of a Bond" << std::endl;
        std::cout << "Option: ";
        std::cin >> command;
        // Execute command
        if (command == "1") {
            CalcPriceOption();
        } else if (command == "2") {
            CalcYieldOption();
        } else if (command == "q") {
            break;
        } else {
            std::cout << "Option not recognized\n" << std::endl;
        }
    }
    return 0;
}
