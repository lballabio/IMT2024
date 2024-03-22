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
#include <chrono>
#include <fstream>
#include <vector>

using namespace QuantLib;

template <class OptionArguments>
// Function to create a ConstantBlackScholesProcess object with constant parameters
inline ext::shared_ptr<ConstantBlackScholesProcess> createConstantBlackScholesProcess(
    const ext::shared_ptr<GeneralizedBlackScholesProcess>& process,
    const OptionArguments& arguments) {

    // maturity date is retrieved through the option instance that we want to price
    Date maturityDate = arguments.exercise->lastDate();

    // Get the strike from the payoff
    ext::shared_ptr<StrikedTypePayoff> payoff =
        ext::dynamic_pointer_cast<StrikedTypePayoff>(arguments.payoff);
    Real strike = payoff->strike();

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


// Additional function returning the NPV and computation time to calculate this NPV using 
// an engine for a given option
template<typename OptionType>
std::tuple<Real, double> computeNPVAndTime(const boost::shared_ptr<PricingEngine>& engine, 
                            const boost::shared_ptr<OptionType>& option) {
    Real npv;
    double computationTime;

    option->setPricingEngine(engine);

    auto startTime = std::chrono::steady_clock::now();
    npv = option->NPV();
    auto endTime = std::chrono::steady_clock::now();
    computationTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1000000.0;
    return std::make_tuple(npv, computationTime);
}


#endif // UTILS_HPP



            