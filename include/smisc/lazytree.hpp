#ifndef SMISC_LAZYTREE_HPP_
#define SMISC_LAZYTREE_HPP_

#include <boost/variant.hpp>
#include <smisc/lazylist.hpp>

/** \file smisc/lazytree.hpp
 * This file contains an implementation of a rose tree that has different
 * data at its leaves than it does at its junctions.
 *
 * The "lazy" part of it indicates that the implementation will compute
 * children nodes only on demand and only one set of children at a time. This
 * allows for us to represent trees of infinite depth with a finitely size
 * number of children at each step. Of course, we must be sure never to traverse
 * every part of such trees.
 */
namespace smisc
{
    template< typename LeafData >
    struct LtLeaf
    {
        LtLeaf
            ( const LeafData & ld_
            )
            : ld( ld_ )
        {
        }
        LeafData ld;
    };

    template<typename BranchData, typename LeafData>
    struct LtNode;

    /** LtNodes : (b l : Set) → Set
     *  LtNodes b l = LazyList (Node b l)
     *
     *  This list is implemented as a generator function
     */
    template<typename BranchData, typename LeafData>
    struct LtNodes
    {
        typedef LazyList< LtNode<BranchData,LeafData > > type;
    };

    template<typename BranchData, typename LeafData>
    struct LtBranch
    {
        LtBranch
            ( const BranchData & bd_
            , const typename LtNodes<BranchData,LeafData>::type & ns_
            )
            : bd( bd_ )
            , ns( ns_ )
        {
        }
        BranchData bd;
        typename LtNodes<BranchData,LeafData>::type ns;
    };
    template< typename BranchData, typename LeafData >
    LtBranch<BranchData,LeafData> lbFromNodes
        ( const BranchData & bd
        , const typename LtNodes<BranchData,LeafData>::type & nodes
        )
    {
        return LtBranch<BranchData,LeafData>( bd, nodes );
    }
    template<typename BranchData_, typename LeafData_>
    struct LtNode
    {
        typedef BranchData_ BranchData;
        typedef LeafData_ LeafData;
        typedef boost::variant
            < LtLeaf<LeafData_>
            , LtBranch<BranchData_,LeafData_>
            > Impl;
        LtNode( const Impl & impl_ )
            : impl( impl_ )
        {
        }
        Impl impl;
    };

    template< typename BranchData, typename LeafData >
    LtNode<BranchData,LeafData> lnLeaf
        ( const LeafData & ld
        )
    {
        return LtNode<BranchData,LeafData>
            ( LtLeaf<LeafData>
             ( ld
             )
            );
    }
    template< typename BranchData, typename LeafData, typename LtNodes0 >
    LtNode<BranchData,LeafData> lnBranch
        ( const BranchData & bd
        , LtNodes0 ns
        )
    {
        return LtNode<BranchData,LeafData>
            ( LtBranch<BranchData,LeafData>
             ( bd
             , ns
             )
            );
    }

    namespace detail
    {
        template
            < typename F
            , typename G
            , typename BranchData
            , typename LeafData
            , typename BranchDataNew
            , typename LeafDataNew
            >
        struct LnMapCase
        {
            typedef LtNode
                < BranchDataNew
                , LeafDataNew
                > result_type;
            LnMapCase
                ( F f_
                , G g_
                )
                : f( f_ )
                , g( g_ )
            {
            }
            result_type operator()( const LtLeaf<LeafData> & l ) const
            {
                return lnLeaf<BranchDataNew>( g( l.ld ) );
            }
            result_type operator()( const LtBranch<BranchData,LeafData> & b ) const
            {
                return lnBranch
                    < BranchDataNew
                    , LeafDataNew
                    >
                    ( f( b.bd )
                    , lnsMap( f, g, b.ns )
                    );
            }
            F f;
            G g;
        };
    }
    template
        < typename F
        , typename G
        , typename BranchData
        , typename LeafData
        >
    auto lnMap( F f, G g, const LtNode<BranchData,LeafData> & ln )
        -> LtNode
            < decltype( f(*(BranchData*)(0)) )
            , decltype( g(*(LeafData*)(0)) )
            >
    {
        // I'm passing these on ot LmMapCase instead of computing them in 
        // LnMapCase to work around an ICE on vs2010.
        typedef decltype( f(*(BranchData*)(0)) ) BranchDataNew;
        typedef decltype( g(*(LeafData*)(0)) ) LeafDataNew;
        return boost::apply_visitor
            ( detail::LnMapCase
                < F
                , G
                , BranchData
                , LeafData
                , BranchDataNew
                , LeafDataNew
                >( f, g )
            , ln.impl
            );
    }
    namespace detail
    {
        template
            < typename BranchData
            , typename LeafData
            >
        struct LnJoinCase
        {
            typedef LtNode< BranchData, LeafData > result_type;
            typedef LtNode< BranchData, LeafData > OldNode;
            result_type operator()( const LtLeaf<OldNode> & l ) const
            {
                return l.ld;
            }
            result_type operator()( const LtBranch<BranchData,OldNode> & b ) const
            {
                return lnBranch
                    < BranchData
                    , LeafData
                    >
                    ( b.bd
                    , lnsJoin( b.ns )
                    );
            }
        };
    }
    template
        < typename BranchData
        , typename LeafData
        >
    LtNode< BranchData, LeafData >
    lnJoin( const LtNode
               < BranchData
               , LtNode
                   < BranchData
                   , LeafData
                   >
               > & ln )
    {
        return boost::apply_visitor
            ( detail::LnJoinCase<BranchData,LeafData>()
            , ln.impl
            );
    }

    template< typename F, typename G, typename BranchData, typename LeafData >
    auto lnsMap( F f, G g, const LazyList< LtNode<BranchData, LeafData > > & lns )
        -> LtNodes
             < decltype( f( *(BranchData*)(0) ) )
             , decltype( g( *(LeafData*)(0) ) )
             >::type
    {
        return llMap
            ( [=]( const LtNode<BranchData, LeafData> & n )
              {
                return lnMap( f, g, n );
              }
            , lns
            );
    }

    template< typename BranchData, typename LeafData >
    typename LtNodes< BranchData, LeafData >::type
    lnsJoin
        ( const LazyList
                   < LtNode
                       < BranchData
                       , LtNode
                           < BranchData
                           , LeafData
                           >
                       >
                   > & lns )
    {
        return llMap
            ( [=]( const LtNode<BranchData, LtNode<BranchData,LeafData> > & n )
              {
                return lnJoin( n );
              }
            , lns
            );
    }
}
#endif
