#include <smisc/lazytree.hpp>
#include <string>

//This source file is exclusively for compilation testing.

namespace smisc
{
    static void compilationTest()
    {
        using namespace smisc;

        LtNodes< std::string, int >::type
            r = lnsJoin
                    ( *(LtNodes
                          < std::string
                          , LtNode
                               < std::string
                               , int
                               >
                          >::type*
                       )(0)
                    );

        LtNodes< std::string, int >::type
            r2 = lnsMap
                    (  []( const int ){ return std::string("hello"); }
                    ,  []( const std::string ){ return 33; }
                    , *(LtNodes<int,std::string>::type*)(0)
                    );
    }
}
