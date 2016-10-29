#ifndef ABSTRACT_ALGEBRA_HPP
#define ABSTRACT_ALGEBRA_HPP

#include <algorithm>
#include <forward_list>
#include <iostream>
#include <fstream>
#include <list>
#include <memory>
#include <numeric>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

// TODO: Rethink how to track remaining elements. Loading a set with all elements could be a space hog with for large permutations.
// TODO: Add usage and comment functions
namespace Permutation
{
    
    namespace utility
    {
        size_t gcd( size_t a, size_t b)
        {
            for (;;)
            {
                if ( a == 0)
                {
                    return b;
                }
                b %= a;
                if ( b == 0)
                {
                    return a;
                }
                a %= b;
            }
        }

        size_t lcm( size_t a, size_t b)
        {
            size_t temp = gcd( a, b);
            return temp ? (a / temp * b) : 0;
        }
    }
    
    typedef std::unordered_map<size_t, size_t> nPermutation;
    typedef std::list<nPermutation> composition;
    typedef std::pair<size_t, size_t> movesToPair;
    
    inline void printPermutation( nPermutation &permutation)
    {
        for( auto &it : permutation)
        {
            printf( "%zu -> %zu\n", it.first, it.second);
        }
    }
    
    inline bool isIdentity( nPermutation &permutation)
    {
        for( auto& p : permutation)
        {
            if( p.first != p.second)
            {
                return false;
            }
        }
        return true;
    }
    
    void printCycleNotation( nPermutation &permutation)
    {
        if( isIdentity(permutation))
        {
            printf( "( 1 ) ");
            return;
        }

        std::set<size_t> remaining;
        for( auto &it : permutation)
        {
            remaining.insert( it.first);
        }
        
        while( !remaining.empty())
        {
            auto begin = *(remaining.begin());
            remaining.erase( begin);
            
            auto destination = permutation.at( begin);
            if( destination == begin)
            {
                continue;
            }

            printf( "( %zu", begin);
            
            while(  destination != begin )
            {
                printf( " %zu", destination);
                remaining.erase( destination);
                destination = permutation.at( destination);
            }

            printf( " ) ");
            
        }            
    }
    
    void printCOF( composition& permutations)
    {
        composition::iterator it = permutations.begin();
        if( it == permutations.end())
        {
            return;
        }
        printCycleNotation( *(it++));            
        
        while( it != permutations.end())
        {
            if( (*it).empty())
            {
                continue;
            }

            printf(" ∘ ");
            printCycleNotation( *(it++));
        }
        
    }
    
    std::unique_ptr<nPermutation> compose( composition& permutations)
    {
        std::unique_ptr<nPermutation> result = std::make_unique<nPermutation>();
        composition::reverse_iterator rit = permutations.rbegin();
        
        std::unordered_set<size_t> keys;
        
        for( ; rit != permutations.rend(); ++rit)
        {
            for( auto& p : *rit)
            {
                keys.insert( p.first);
            }
        }
        
        rit = permutations.rbegin();
        
        for( auto& element : keys)
        {
            auto destination = element;
            while( rit != permutations.rend())
            {
                if( (*rit).count( destination))
                {
                    destination = (*rit).at( destination);
                }
                ++rit;
            }
            result->insert( movesToPair( element, destination));
            rit = permutations.rbegin();
        }
        
        return result;
    }
    
    std::unique_ptr<nPermutation> inverse( nPermutation& permutation)
    {
        std::unique_ptr<nPermutation> inv = std::make_unique<nPermutation>();
        for( auto& pair : permutation)
        {
            inv->insert( movesToPair( pair.second, pair.first));
        }
        return inv;
    }

    size_t order( nPermutation& permutation)
    {
        std::set<size_t> remaining;
        for( auto &it : permutation)
        {
            remaining.insert( it.first);
        }
        
        std::forward_list<size_t> lengthsOfCycles;
        while( !remaining.empty())
        {
            auto begin = *(remaining.begin());
            remaining.erase( begin);
            
            auto destination = permutation.at( begin);
            if( destination == begin)
            {
                continue;
            }

            size_t length = 1;
            while(  destination != begin )
            {
                remaining.erase( destination);
                destination = permutation.at( destination);
                ++length;
            }
            lengthsOfCycles.push_front( length);

        }            

        return std::accumulate( lengthsOfCycles.begin(), lengthsOfCycles.end(), 1, utility::lcm);
    }
    
    std::unique_ptr<std::unordered_map<std::string, nPermutation>> readPermutations( const char* filename)
    {
        std::ifstream fin{ filename};

        if( !fin.is_open())
        {
            std::cerr << '\'' << filename << '\'' << " was not opened properly. Please check if the filename is correct. The function will return 'nullptr'." << std::endl;
            return nullptr;
        }

        std::unique_ptr<std::unordered_map<std::string,nPermutation>> hash_bag = std::make_unique<std::unordered_map<std::string,nPermutation>>();

        std::string key;
        size_t totalPermutations = 0;

        fin >> totalPermutations;
        if( fin.good())
        {
            size_t first, second;
            std::unique_ptr<nPermutation> perm = std::make_unique<nPermutation>();
            for(; totalPermutations > 0; --totalPermutations)
            {
                fin >> key;
                while( fin >> first >> second)
                {
                    perm->insert( movesToPair( first, second));
                }
                hash_bag->insert( std::make_pair( key, *perm));
                perm.reset( new nPermutation);
                fin.clear();
            }
        }
        fin.close();
        return hash_bag;
    }

    void printProductOfTrans( nPermutation& permutation)
    {
        std::set<size_t> remaining;
        for( auto&it : permutation)
        {
            remaining.insert( it.first);
        }

        while( !remaining.empty())
        {
            std::stack<size_t> inCycle;

            size_t begin = *remaining.begin();
            remaining.erase( begin);
            
            auto destination = permutation.at( begin);
            while( destination != begin )
            {
                inCycle.push(destination);
                remaining.erase( destination);
                destination = permutation.at( destination);
            }

            while( !inCycle.empty())
            {
                printf( "( %zu %zu ) ", begin, inCycle.top());
                inCycle.pop();
            }
        }
    }

    bool isEven( nPermutation& permutation)
    {
        std::set<size_t> remaining;
        for( auto&it : permutation)
        {
            remaining.insert( it.first);
        }

        size_t count = 0;
        while( !remaining.empty())
        {
           size_t begin = *remaining.begin();
           remaining.erase( begin);

            auto destination = permutation.at( begin);
            while( destination != begin)
            {
                count = (count+1) % 2;
                remaining.erase( destination);
                destination = permutation.at( destination);
            }
        }
        return count ? 0 : 1;
    }

} // end of namespace Permutation

#endif // SYMMETRIC_HPP
