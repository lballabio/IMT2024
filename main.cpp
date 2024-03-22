
#include <ql/qldefines.hpp>
#ifdef BOOST_MSVC
#  include <ql/auto_link.hpp>
#endif
#include "constantblackscholesprocess.hpp"
#include "mceuropeanengine.hpp"
#include "mc_discr_arith_av_strike.hpp"
#include "mcbarrierengine.hpp"
#include "utils.hpp"
#include <ql/instruments/europeanoption.hpp>
#include <ql/instruments/asianoption.hpp>
#include <ql/instruments/barrieroption.hpp>
#include <ql/instruments/payoffs.hpp>
#include <ql/exercise.hpp>
#include <ql/pricingengines/vanilla/mceuropeanengine.hpp>
#include <ql/pricingengines/asian/mc_discr_arith_av_strike.hpp>
#include <ql/pricingengines/barrier/mcbarrierengine.hpp>
#include <ql/termstructures/yield/zerocurve.hpp>
#include <ql/termstructures/volatility/equityfx/blackvariancecurve.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/utilities/dataformatters.hpp>
#include <iostream>
#include <chrono>

using namespace QuantLib;

int main() {

    try {

        // modify the sample code below to suit your project

        Calendar calendar = TARGET();
        Date today = Date(24, February, 2022);
        Settings::instance().evaluationDate() = today;

        Real underlying = 36;

        Handle<Quote> underlyingH(ext::make_shared<SimpleQuote>(underlying));

        DayCounter dayCounter = Actual365Fixed();
        Handle<YieldTermStructure> riskFreeRate(
            ext::make_shared<ZeroCurve>(std::vector<Date>{today, today + 6*Months},
                                        std::vector<Rate>{0.01, 0.015},
                                        dayCounter));
        Handle<BlackVolTermStructure> volatility(
            ext::make_shared<BlackVarianceCurve>(today,
                                                 std::vector<Date>{today+3*Months, today+6*Months},
                                                 std::vector<Volatility>{0.20, 0.25},
                                                 dayCounter));

        auto bsmProcess = ext::make_shared<BlackScholesProcess>(underlyingH, riskFreeRate, volatility);

        // options

        Real strike = 40;
        Date maturity(24, May, 2022);

        Option::Type type(Option::Put);
        auto exercise = ext::make_shared<EuropeanExercise>(maturity);
        auto payoff = ext::make_shared<PlainVanillaPayoff>(type, strike);

        EuropeanOption europeanOption(payoff, exercise);

        DiscreteAveragingAsianOption asianOption(Average::Arithmetic,
                                                 {
                                                     Date(4, March, 2022), Date(14, March, 2022), Date(24, March, 2022),
                                                     Date(4, April, 2022), Date(14, April, 2022), Date(24, April, 2022),
                                                     Date(4, May, 2022), Date(14, May, 2022), Date(24, May, 2022)
                                                 },
                                                 payoff, exercise);

        BarrierOption barrierOption(Barrier::UpIn, 40, 0, payoff, exercise);

        // Monte Carlo pricing and timing

        // table heading

        Size width = 15;
        auto spacer = std::setw(width);
        std::cout << std::setw(40) << "old engine"
                  << std::setw(30) << "non constant"
                  << std::setw(30) << "constant"
                  << std::endl;
        std::cout << spacer << "kind"
                  << spacer << "NPV" << spacer << "time [s]"
                  << spacer << "NPV" << spacer << "time [s]"
                  << spacer << "NPV" << spacer << "time [s]"
                  << std::endl;

        Size timeSteps = 10;
        Size samples = 1000000;
        Size mcSeed = 42;

        // European, old engine

        europeanOption.setPricingEngine(
            MakeMCEuropeanEngine<PseudoRandom>(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(samples)
            .withSeed(mcSeed)
        );

        auto startTime = std::chrono::steady_clock::now();

        Real NPV = europeanOption.NPV();

        auto endTime = std::chrono::steady_clock::now();

        double us = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::cout << spacer << "European" << spacer << NPV << spacer << us / 1000000;

        // European, new engine, non constant

        europeanOption.setPricingEngine(
            MakeMCEuropeanEngine_2<PseudoRandom>(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(samples)
            .withSeed(mcSeed)
            .withConstantParameters(false)
        );

        startTime = std::chrono::steady_clock::now();

        NPV = europeanOption.NPV();

        endTime = std::chrono::steady_clock::now();

        us = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::cout << spacer << NPV << spacer << us / 1000000;

        // European, new engine, constant

        europeanOption.setPricingEngine(
            MakeMCEuropeanEngine_2<PseudoRandom>(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(samples)
            .withSeed(mcSeed)
            .withConstantParameters(true)
        );

        startTime = std::chrono::steady_clock::now();

        NPV = europeanOption.NPV();

        endTime = std::chrono::steady_clock::now();

        us = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::cout << spacer << NPV << spacer << us / 1000000 << std::endl;

        // Asian, old engine

        asianOption.setPricingEngine(
            MakeMCDiscreteArithmeticASEngine<PseudoRandom>(bsmProcess)
            .withSamples(samples)
            .withSeed(mcSeed)
        );

        startTime = std::chrono::steady_clock::now();

        NPV = asianOption.NPV();

        endTime = std::chrono::steady_clock::now();

        us = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::cout << spacer << "Asian" << spacer << NPV << spacer << us / 1000000;

        // Asian, non constant

        asianOption.setPricingEngine(
            MakeMCDiscreteArithmeticASEngine_2<PseudoRandom>(bsmProcess)
            .withSamples(samples)
            .withSeed(mcSeed)
            .withConstantParameters(false)
        );

        startTime = std::chrono::steady_clock::now();

        NPV = asianOption.NPV();

        endTime = std::chrono::steady_clock::now();

        us = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::cout << spacer << NPV << spacer << us / 1000000;

        // Asian, constant

        asianOption.setPricingEngine(
            MakeMCDiscreteArithmeticASEngine_2<PseudoRandom>(bsmProcess)
            .withSamples(samples)
            .withSeed(mcSeed)
            .withConstantParameters(true)
        );

        startTime = std::chrono::steady_clock::now();

        NPV = asianOption.NPV();

        endTime = std::chrono::steady_clock::now();

        us = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::cout << spacer << NPV << spacer << us / 1000000 << std::endl;

        // Barrier, old engine

        barrierOption.setPricingEngine(
            MakeMCBarrierEngine<PseudoRandom>(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(samples)
            .withSeed(mcSeed)
        );

        startTime = std::chrono::steady_clock::now();

        NPV = barrierOption.NPV();

        endTime = std::chrono::steady_clock::now();

        us = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::cout << spacer << "Barrier" << spacer << NPV << spacer << us / 1000000;

        // Barrier, non constant

        barrierOption.setPricingEngine(
            MakeMCBarrierEngine_2<PseudoRandom>(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(samples)
            .withSeed(mcSeed)
            .withConstantParameters(false)
        );

        startTime = std::chrono::steady_clock::now();

        NPV = barrierOption.NPV();

        endTime = std::chrono::steady_clock::now();

        us = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::cout << spacer << NPV << spacer << us / 1000000;

        // Barrier, constant

        barrierOption.setPricingEngine(
            MakeMCBarrierEngine_2<PseudoRandom>(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(samples)
            .withSeed(mcSeed)
            .withConstantParameters(true)
        );

        startTime = std::chrono::steady_clock::now();

        NPV = barrierOption.NPV();

        endTime = std::chrono::steady_clock::now();

        us = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::cout << spacer << NPV << spacer << us / 1000000 << std::endl;

        // All done

        return 0;

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 1;
    }


    // Modification of the main we used to obtain data on more samples and timeSteps for analysis 
    //   /!\ COMMENT OUT OPTION 1 OR 2 
    //  try {

    //     Calendar calendar = TARGET();
    //     Date today = Date(24, February, 2022);
    //     Settings::instance().evaluationDate() = today;

    //     Real underlying = 36;

    //     Handle<Quote> underlyingH(ext::make_shared<SimpleQuote>(underlying));

    //     DayCounter dayCounter = Actual365Fixed();
    //     Handle<YieldTermStructure> riskFreeRate(
    //         ext::make_shared<ZeroCurve>(std::vector<Date>{today, today + 6*Months},
    //                                     std::vector<Rate>{0.01, 0.015},
    //                                     dayCounter));
    //     Handle<BlackVolTermStructure> volatility(
    //         ext::make_shared<BlackVarianceCurve>(today,
    //                                              std::vector<Date>{today+3*Months, today+6*Months},
    //                                              std::vector<Volatility>{0.20, 0.25},
    //                                              dayCounter));

    //     auto bsmProcess = ext::make_shared<BlackScholesProcess>(underlyingH, riskFreeRate, volatility);

    //     // options

    //     Real strike = 40;
    //     Date maturity(24, May, 2022);

    //     Option::Type type(Option::Put);
    //     auto exercise = ext::make_shared<EuropeanExercise>(maturity);
    //     auto payoff = ext::make_shared<PlainVanillaPayoff>(type, strike);

    //     EuropeanOption europeanOption(payoff, exercise);

    //     DiscreteAveragingAsianOption asianOption(Average::Arithmetic,
    //                                              {
    //                                                  Date(4, March, 2022), Date(14, March, 2022), Date(24, March, 2022),
    //                                                  Date(4, April, 2022), Date(14, April, 2022), Date(24, April, 2022),
    //                                                  Date(4, May, 2022), Date(14, May, 2022), Date(24, May, 2022)
    //                                              },
    //                                              payoff, exercise);

    //     BarrierOption barrierOption(Barrier::UpIn, 40, 0, payoff, exercise);

    //     // Monte Carlo pricing and timing

    //     //OPTION 1 : VARIATION OF THE NUMBER OF SAMPLES 
    //     Size timeSteps = 10;
    //     std::vector<Size> samplesRange = {500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000};

    //     //OPTION 2 : VARIATION OF THE NUMBER OF TIMESTEPS
    //     // std::vector<Size> timeStepsRange = {5, 10, 50, 100, 500}; 
    //     // Size samples = 1000000;


        
    //     std::ofstream outputFile("npv_results.csv");
    //     // NB : rfor option 2, replace "Samples" by "TimeSteps" below
    //     outputFile << "Samples, European (Old) NPV, European (Old) Time (s), European (New - Non-constant) NPV, European (New - Non-constant) Time (s), European (New - Constant) NPV, European (New - Constant) Time (s),"
    //                << "Asian (Old) NPV, Asian (Old) Time (s), Asian (New - Non-constant) NPV, Asian (New - Non-constant) Time (s), Asian (New - Constant) NPV, Asian (New - Constant) Time (s),"
    //                << "Barrier (Old) NPV, Barrier (Old) Time (s), Barrier (New - Non-constant) NPV, Barrier (New - Non-constant) Time (s), Barrier (New - Constant) NPV, Barrier (New - Constant) Time (s)" << std::endl;


    //     // NB : for option 2, in the for, in should be "(Size timeSteps : timeStepsRange)""
    //     for (Size samples : samplesRange) {
            
    //         std::cout<<"Calculation for samples = "<< samples <<std::endl;
    //         // Pricing engines
    //         auto oldEuropeanEngine = MakeMCEuropeanEngine<PseudoRandom>(bsmProcess).withSteps(timeSteps).withSamples(samples).withSeed(42);
    //         auto newEuropeanEngineNonConstant = MakeMCEuropeanEngine_2<PseudoRandom>(bsmProcess).withSteps(timeSteps).withSamples(samples).withSeed(42).withConstantParameters(false);
    //         auto newEuropeanEngineConstant = MakeMCEuropeanEngine_2<PseudoRandom>(bsmProcess).withSteps(timeSteps).withSamples(samples).withSeed(42).withConstantParameters(true);

    //         auto oldAsianEngine = MakeMCDiscreteArithmeticASEngine<PseudoRandom>(bsmProcess).withSamples(samples).withSeed(42);
    //         auto newAsianEngineNonConstant = MakeMCDiscreteArithmeticASEngine_2<PseudoRandom>(bsmProcess).withSamples(samples).withSeed(42).withConstantParameters(false);
    //         auto newAsianEngineConstant = MakeMCDiscreteArithmeticASEngine_2<PseudoRandom>(bsmProcess).withSamples(samples).withSeed(42).withConstantParameters(true);

    //         auto oldBarrierEngine = MakeMCBarrierEngine<PseudoRandom>(bsmProcess).withSteps(timeSteps).withSamples(samples).withSeed(42);
    //         auto newBarrierEngineNonConstant = MakeMCBarrierEngine_2<PseudoRandom>(bsmProcess).withSteps(timeSteps).withSamples(samples).withSeed(42).withConstantParameters(false);
    //         auto newBarrierEngineConstant = MakeMCBarrierEngine_2<PseudoRandom>(bsmProcess).withSteps(timeSteps).withSamples(samples).withSeed(42).withConstantParameters(true);

    //         auto [npvOldEuropean, timeOldEuropean] = computeNPVAndTime(oldEuropeanEngine, boost::make_shared<EuropeanOption>(europeanOption));
    //         auto [npvNewNonConstantEuropean, timeNewNonConstantEuropean] = computeNPVAndTime(newEuropeanEngineNonConstant, boost::make_shared<EuropeanOption>(europeanOption));
    //         auto [npvNewConstantEuropean, timeNewConstantEuropean] = computeNPVAndTime(newEuropeanEngineConstant, boost::make_shared<EuropeanOption>(europeanOption));

    //         auto [npvOldAsian, timeOldAsian] = computeNPVAndTime(oldAsianEngine, boost::make_shared<DiscreteAveragingAsianOption>(asianOption));
    //         auto [npvNewNonConstantAsian, timeNewNonConstantAsian] = computeNPVAndTime(newAsianEngineNonConstant, boost::make_shared<DiscreteAveragingAsianOption>(asianOption));
    //         auto [npvNewConstantAsian, timeNewConstantAsian] = computeNPVAndTime(newAsianEngineConstant, boost::make_shared<DiscreteAveragingAsianOption>(asianOption));

    //         auto [npvOldBarrier, timeOldBarrier] = computeNPVAndTime(oldBarrierEngine, boost::make_shared<BarrierOption>(barrierOption));
    //         auto [npvNewNonConstantBarrier, timeNewNonConstantBarrier] = computeNPVAndTime(newBarrierEngineNonConstant, boost::make_shared<BarrierOption>(barrierOption));
    //         auto [npvNewConstantBarrier, timeNewConstantBarrier] = computeNPVAndTime(newBarrierEngineConstant, boost::make_shared<BarrierOption>(barrierOption));

           
    //         outputFile << samples << ", "  // NB : replace "samples" by "timeSteps" for option 2
    //                    << npvOldEuropean << ", " << timeOldEuropean << ", "
    //                    << npvNewNonConstantEuropean << ", " << timeNewNonConstantEuropean << ", "
    //                    << npvNewConstantEuropean << ", " << timeNewConstantEuropean << ", "
    //                    << npvOldAsian << ", " << timeOldAsian << ", "
    //                    << npvNewNonConstantAsian << ", " << timeNewNonConstantAsian << ", "
    //                    << npvNewConstantAsian << ", " << timeNewConstantAsian << ", "
    //                    << npvOldBarrier << ", " << timeOldBarrier << ", "
    //                    << npvNewNonConstantBarrier << ", " << timeNewNonConstantBarrier << ", "
    //                    << npvNewConstantBarrier << ", " << timeNewConstantBarrier << std::endl;
    //     }

    //     // Close the CSV file
    //     outputFile.close();

    //     std::cout << "Results have been saved to the CSV file." << std::endl;

        


    //     return 0;

    // } catch (std::exception& e) {
    //     std::cerr << e.what() << std::endl;
    //     return 1;
    // } catch (...) {
    //     std::cerr << "unknown error" << std::endl;
    //     return 1;
    // }

}

