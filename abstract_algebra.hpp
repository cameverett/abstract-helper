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
        unsigned int gcd( unsigned int a, unsigned int b)
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

        unsigned int lcm( unsigned int a, unsigned int b)
        {
            unsigned int temp = gcd( a, b);
            return temp ? (a / temp * b) : 0;
        }
    }
    
    typedef std::unordered_map<unsigned int, unsigned int> nPermutation;
    typedef std::list<nPermutation> composition;
    typedef std::pair<unsigned int, unsigned int> movesToPair;
    
    inline void printPermutation( nPermutation &permutation)
    {
        for( auto &it : permutation)
        {
            printf( "%u -> %u\n", it.first, it.second);
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

        std::set<unsigned int> remaining;
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

            printf( "( %u", begin);
            
            while(  destination != begin )
            {
                printf( " %u", destination);
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
        
        std::unordered_set<unsigned int> keys;
        
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

    unsigned int order( nPermutation& permutation)
    {
        std::set<unsigned int> remaining;
        for( auto &it : permutation)
        {
            remaining.insert( it.first);
        }
        
        std::forward_list<unsigned int> lengthsOfCycles;
        while( !remaining.empty())
        {
            auto begin = *(remaining.begin());
            remaining.erase( begin);
            
            auto destination = permutation.at( begin);
            if( destination == begin)
            {
                continue;
            }

            unsigned int length = 1;
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
        unsigned int totalPermutations = 0;

        fin >> totalPermutations;
        if( fin.good())
        {
            unsigned int first, second;
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
        std::set<unsigned int> remaining;
        for( auto&it : permutation)
        {
            remaining.insert( it.first);
        }

        while( !remaining.empty())
        {
            std::stack<unsigned int> inCycle;

            unsigned int begin = *remaining.begin();
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
                printf( "( %u %u ) ", begin, inCycle.top());
                inCycle.pop();
            }
        }
    }

    bool isEven( nPermutation& permutation)
    {
        std::set<unsigned int> remaining;
        for( auto&it : permutation)
        {
            remaining.insert( it.first);
        }

        unsigned int count = 0;
        while( !remaining.empty())
        {
           unsigned int begin = *remaining.begin();
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
