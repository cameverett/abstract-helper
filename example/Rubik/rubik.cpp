#include "../../symmetric.hpp"

using namespace Permutation;

int main( int argc, char* argv[])
{
    /**
     * Populate an unordered_map<string, nPermutation>
     * with the n named permutations in the input file. U,D,L,R,B,F
     * correspond to counterclockwise rotations of the 6 faces of the cube.
     */
    auto cube_moves = *readPermutations( "rubik.in");
    std::cout << "Finished reading " << cube_moves.size() << " permutations." << std::endl;
    for( auto& p : cube_moves)
    {
        std::cout << p.first << ": ";
        printCycleNotation( p.second);
        std::cout << std::endl;
    }

    std::cout << "\nCreate a series of moves (composition of functions): " << std::endl;
    composition fns;

    // Clockwise turn of left face followed by clockwise turn on top face.
    fns.push_back( cube_moves.at( "U"));
    fns.push_back( cube_moves.at( "L"));

    // Printed as composition of functions U ∘ L
    printCOF( fns);
    auto result = *compose( fns);

    std::cout << "\nSee the result" << std::endl;
    printCycleNotation( result);
    std::cout << std::endl;

    std::cout << "\nResult represented as a product of transpositions:" << std::endl;
    printProductOfTrans( result);
    std::cout << std::endl;
    
    std::cout << "\nThe inverse of the result is: " << std::endl;
    printCycleNotation( *inverse( result));
    std::cout << std::endl;

    std::cout << "Is even: " << isEven( result) << std::endl;

    
    std::cout << "\nThe order of the result is " << order( result) << '.' << std::endl;

    std::cout.flush();
    return 0;
}
