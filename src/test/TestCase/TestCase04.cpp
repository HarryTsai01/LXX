//
// Created by Harry on 8/27/2023.
//

#include "TestCase04.h"
#include <core/Containers/UnorderedMap.h>

namespace LXXTest
{

bool TestCase04::Run()
{
    {
        UnorderedMap<int, int> map;
        map[1] = 1;
        map[2] = 2;

        DO_TEST( map.Size() == 2 );
        DO_TEST( map[1] == 1 );
        DO_TEST( map[2] == 2 );
    }

    {
        UnorderedMap<int, int> map =
        {
            { 1, 1 },
            { 2, 2 },
            { 3, 3 },
        };
        DO_TEST( map.Size() == 3 );
        DO_TEST( map[1] == 1 );
        DO_TEST( map[2] == 2 );
        DO_TEST( map[3] == 3 );

        for( auto it = map.Begin(); it != map.End(); ++it )
        {
            DO_TEST( it->First == it->Second );
        }

        for( const auto& KeyValue : map )
        {
            DO_TEST( KeyValue.First == KeyValue.Second );
        }

        for( auto& [key, value] : map )
        {
            DO_TEST( key == value );
        }

        map.Remove( 1 );
        DO_TEST( map.Size() == 2 );
        map.Clear();
        DO_TEST( map.Size() == 0 );
    }

    return true;
}

} // LXXTest