//
// Created by Harry on 8/26/2023.
//

#include "TestCase03.h"
#include <core/Containers/List.h>

namespace LXXTest
{

bool TestCase03::Run()
{
    {
        List< int > list = { 1, 2, 3 };
        list.PushBack( 4 );
        DO_TEST( list.Size() == 4 );

        for( const auto &i : list )
        {

        }
    }

    return true;
}

}