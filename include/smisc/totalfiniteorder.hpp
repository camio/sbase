#ifndef TOTAL_FINITE_ORDER_HPP_
#define TOTAL_FINITE_ORDER_HPP_

#include <vector>
#include <boost/optional.hpp>
#include <iosfwd>

/** \file smisc/totalfiniteorder.hpp
 * Utilities for dealing with total finite orders.
 */
namespace smisc
{
    /*** A TotalFiniteOrder is a strict total order for finite sets. In other
     * words, TotalFiniteOrders of size n represent all transitive total
     * relations on sets of size n.
     *
     * let < ∈ TotalFiniteOrder
     * For any finite set S, and any a, b, c ∈ S
     *
     * if a < b and b < c then a < c (transitivity)
     * either a < b or b < a         (total)
     */
    struct TotalFiniteOrder
    {
        typedef std::vector<size_t> Impl;
        explicit TotalFiniteOrder( const Impl & impl_ )
            : impl( impl_ )
        {
        }
        /** \invariant if impl.size() = n, then for any i ∈ size_t, impl[i] < n.
         *  Also ∀ i,j ∈ [0,n], i ≠ j → impl[i] ≠ impl[j]
         */
        Impl impl;
    };

    /** Creates a total finite order. v must have unique elements that range
     * from 0 to v.size() (non inclusive of the latter)
     */
    boost::optional<TotalFiniteOrder> tfoFromVector
        ( const std::vector<size_t> & v
        );

    bool tfoIsTrivial( const TotalFiniteOrder & tfo );

    bool tfoHasRelation
        ( const TotalFiniteOrder & tfo
        , const size_t i
        , const size_t j
        );
    /** given a set element \a i, returns the "count" of its element.
     *  a count is 0 for the least element, 1 for the second to least element,
     *  and so on.
     */
    size_t tfoCount( const TotalFiniteOrder & tfo, const size_t i );

    /** Given an index into the new ordering, what is the set item in that
     * place?
     */
    size_t tfoAtCount( const TotalFiniteOrder & tfo, const size_t i );

    /** Converts a TotalFiniteOrder to one that is greater in size by one
     *  where the new element, 0, is the smallest element. All the elements
     *  from \a tfo, are increased by 1.
     */
    TotalFiniteOrder tfoAddSmallest( const TotalFiniteOrder & tfo );

    inline size_t tfoSize( const TotalFiniteOrder & tfo )
    {
        return tfo.impl.size();
    }

    /** If \a v is the collection of unique elements in a set where v[0]
     *  is the element labeled 0, v[1] is the element labeled 1 and so on,
     *  tfoSorted will return all the elements again, but sorted according
     *  to \a tfo.
     */
    template< typename A >
    std::vector< A > tfoSorted
        ( const TotalFiniteOrder & tfo
        , const std::vector<A> & v
        )
    {
        assert( tfoSize( tfo ) == v.size() );
        std::vector< A > r;
        r.reserve( v.size() );
        for( size_t i = 0; i < v.size(); ++i )
            r.push_back( v[ tfoAtCount( tfo, i ) ] );
        return r;
    }

    std::ostream & operator<<( std::ostream & oss, const TotalFiniteOrder & tfo );
}

#endif
