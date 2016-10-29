#include "../../../abstract_algebra.hpp"

using namespace Permutation;

int main()
{
    auto cube_moves = readPermutations( "../rubik.in");
    if( !cube_moves)
    {
        return 1;
    }

    // Upper case => clockwise U
    // Lower case => counter-clockwise i.e. U'
    const std::string F2R("URurufUF");

    composition seriesOfMoves;
    for( auto &c : F2R)
    {
        switch(c)
        {
            case('U'):
                seriesOfMoves.push_front( cube_moves->at("U"));
                break;
            case('u'):
                seriesOfMoves.push_front( *inverse( cube_moves->at("U")));
                break;
            case('D'):
                seriesOfMoves.push_front( cube_moves->at("D"));
                break;
            case('d'):
                seriesOfMoves.push_front( *inverse( cube_moves->at("D")));
                break;
            case('L'):
                seriesOfMoves.push_front( cube_moves->at("L"));
                break;
            case('l'):
                seriesOfMoves.push_front( *inverse( cube_moves->at("L")));
                break;
            case('R'):
                seriesOfMoves.push_front( cube_moves->at("R"));
                break;
            case('r'):
                seriesOfMoves.push_front( *inverse( cube_moves->at("R")));
                break;
            case('F'):
                seriesOfMoves.push_front( cube_moves->at("F"));
                break;
            case('f'):
                seriesOfMoves.push_front( *inverse( cube_moves->at("F")));
                break;
            case('B'):
                seriesOfMoves.push_front( cube_moves->at("B"));
                break;
            case('b'):
                seriesOfMoves.push_front( *inverse( cube_moves->at("B")));
                break;
            default:
                std::cout << "Ignoring invalid move " << c << std::endl;
                break;
        }
    }


    std::cout << '\n' << F2R << " Done as a composition of functions" << std::endl;
    printCOF( seriesOfMoves);

    std::cout << "\nResult: " << std::endl;
    auto done = *compose( seriesOfMoves);
    printCycleNotation( done);
    std::cout << "\nOrder of the result is " << order( done) << std::endl;

    seriesOfMoves.clear();
    for( int i = 0; i < order(done); i++)
    {
        seriesOfMoves.push_front( done);
    }

    printCycleNotation( done);
    std::cout << " repeated 15 times is ";
    printCycleNotation( *compose( seriesOfMoves));

    std::cout << std::endl;
    return 0;
}
