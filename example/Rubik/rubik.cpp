#include "../../symmetric.hpp"

using namespace Permutation;

int main( int argc, char* argv[])
{
    std::unique_ptr<composition> fns = std::make_unique<composition>();

    auto bag = *readPermutations( "rubik.in");
    std::cout << "Finished reading " << bag.size() << " permutations." << std::endl;
    for( auto& p : bag)
    {
        std::cout << p.first << ": ";
        printCycleNotation( p.second);
        std::cout << std::endl;
    }

    std::cout << "\nCreate a series of moves (composition of functions): " << std::endl;
    fns->push_back( bag.at( "U"));
    fns->push_back( bag.at( "L"));
    printCOF( *fns);

    // Return resulting permutation from composition of functions
    auto result = *compose( *fns);

    std::cout << "\nSee the result" << std::endl;
    printCycleNotation( result);
    std::cout << std::endl;

    std::cout << "\nResult represented as a product of transpositions:" << std::endl;
    printProductOfTrans( result);
    std::cout << std::endl;
    
    std::cout << "\nThe inverse of the result is: " << std::endl;
    printCycleNotation( *inverse( result));
    std::cout << std::endl;

    
    std::cout << "\nThe order of the result is " << order( result) << '.' << std::endl;

    std::cout.flush();
    return 0;
}
