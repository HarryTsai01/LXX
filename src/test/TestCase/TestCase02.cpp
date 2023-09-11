//
// Created by Harry on 8/24/2023.
//

#include "TestCase02.h"
#include <Core/Containers/Array.h>

namespace LXXTest
{


bool TestCase02::Run()
{
    {
        Array<int> array;
        array.Add(1);

        DO_TEST( array.Size() == 1 );
        DO_TEST( array[0] == 1 );


        Array<int> array2(array);
        DO_TEST( array2.Size() == 1 );
        DO_TEST( array2[0] == 1 );

        Array<int> array3;
        array3 = array;
        DO_TEST( array3.Size() == 1 );
        DO_TEST( array3[0] == 1 );

        Array<int> array4;
        array4 = std::move( array );
        DO_TEST( array4.Size() == 1 );
        DO_TEST( array4[0] == 1 );
        DO_TEST( array.Size() == 0 );

        Array<int> array5 = { 2, 3 , 4 , 5 };
        DO_TEST( array5.Size() == 4 );
        DO_TEST( array5[0] == 2 );
        DO_TEST( array5[1] == 3 );
        DO_TEST( array5[2] == 4 );
        DO_TEST( array5[3] == 5 );

        Array<f32> array6 = array5;
        DO_TEST( array6.Size() == 4 );
        DO_TEST( array6[0] == 2.0f );
        DO_TEST( array6[1] == 3.0f );
        DO_TEST( array6[2] == 4.0f );
        DO_TEST( array6[3] == 5.0f );

        array6.AddRange( array5.GetData(), array5.Size() );
        DO_TEST( array6.Size() == 8 );
        DO_TEST( array6[4] == 2.0f );
        DO_TEST( array6[5] == 3.0f );
        DO_TEST( array6[6] == 4.0f );
        DO_TEST( array6[7] == 5.0f );
    }

    {
        Array<int> array = { 1, 2, 3, 4, 5 };
        Array<int> array2 = { 6 ,  7 , 8 , 9 , 10 };

        array.Append( array2 );

        int startValue = 1;
        for( const auto& element : array )
        {
            DO_TEST( element == startValue );
            ++ startValue;
        }

        startValue = 1;
        for( auto it = array.Begin(); it != array.End(); ++ it )
        {
            DO_TEST( *it == startValue );
            ++ startValue;
        }

        startValue = 1;
        for( auto it = array.ConstBegin(); it != array.ConstEnd(); ++ it )
        {
            DO_TEST( (*it) == startValue );
            ++ startValue;
        }


        startValue = array.Size() ;
        for( auto it = array.ReverseBegin(); it != array.ReverseEnd(); ++ it )
        {
            DO_TEST( *it == startValue );
            -- startValue;
        }
    }

    return true;
}


} // LXXTest