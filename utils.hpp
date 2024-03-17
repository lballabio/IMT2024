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

// Function to extract constant parameters from process
inline std::tuple<Rate, Rate, Volatility> extractConstantParameters(
    const ext::shared_ptr<GeneralizedBlackScholesProcess>& process,
    const Date& maturityDate) {

    /*
        Constant parameters are extracted from the original process parameters (term structures)
        They are based on maturity date of the product priced, using zero rates
        Based on the class BinomialConvertibleEngine, in which the same operation of extracting constant parameters is done
    */
    
    DayCounter rfdc = process->riskFreeRate()->dayCounter();
    DayCounter divdc = process->dividendYield()->dayCounter();

    Volatility v = process->blackVolatility()->blackVol(maturityDate, process->x0());
    Rate riskFreeRate = process->riskFreeRate()->zeroRate(maturityDate, rfdc, Continuous, NoFrequency);
    Rate q = process->dividendYield()->zeroRate(maturityDate, divdc, Continuous, NoFrequency);

    return std::make_tuple(riskFreeRate, q, v);
}

// Function to convert constant parameters to Handle<Quote>
inline std::tuple<Handle<Quote>, Handle<Quote>, Handle<Quote>, Handle<Quote>>
convertToQuoteHandles(double x0, Rate riskFreeRate, Rate dividendYield, Volatility volatility) {
    Handle<Quote> underlying(ext::make_shared<SimpleQuote>(x0));
    Handle<Quote> flatRiskFree(ext::make_shared<SimpleQuote>(riskFreeRate));
    Handle<Quote> flatDividend(ext::make_shared<SimpleQuote>(dividendYield));
    Handle<Quote> flatVol(ext::make_shared<SimpleQuote>(volatility));
    
    return std::make_tuple(underlying, flatRiskFree, flatDividend, flatVol);
}

#endif // UTILS_HPP



            