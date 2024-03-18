#ifndef UTILS_HPP
#define UTILS_HPP

// #include <ql/quote/simplequote.hpp>
#include <ql/handle.hpp>
#include <ql/processes/blackscholesprocess.hpp>
#include <ql/time/date.hpp>
#include <ql/time/daycounter.hpp>
#include <ql/math/interpolations/linearinterpolation.hpp>
#include <ql/pricingengines/vanilla/binomialengine.hpp>
#include <ql/math/distributions/normaldistribution.hpp>
#include <ql/methods/lattices/binomialtree.hpp>
#include <tuple>

using namespace QuantLib;

// Function to create a ConstantBlackScholesProcess object with constant parameters
inline ext::shared_ptr<ConstantBlackScholesProcess> createConstantBlackScholesProcess(
    const ext::shared_ptr<GeneralizedBlackScholesProcess>& process,
    const Date& maturityDate, Real strike) {

    // Extract constant parameters
    DayCounter rfdc = process->riskFreeRate()->dayCounter();
    DayCounter divdc = process->dividendYield()->dayCounter();
    Volatility v = process->blackVolatility()->blackVol(maturityDate, strike);
    Rate riskFreeRate = process->riskFreeRate()->zeroRate(maturityDate, rfdc, Continuous, NoFrequency);
    Rate q = process->dividendYield()->zeroRate(maturityDate, divdc, Continuous, NoFrequency);

    // Create ConstantBlackScholesProcess using extracted parameters
    return ext::make_shared<ConstantBlackScholesProcess>(
        process->x0(), q, riskFreeRate, v);
}



#endif // UTILS_HPP



            