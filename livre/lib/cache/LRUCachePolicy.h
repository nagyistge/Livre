/* Copyright (c) 2011-2014, EPFL/Blue Brain Project
 *                     Ahmet Bilgili <ahmet.bilgili@epfl.ch>
 *
 * This file is part of Livre <https://github.com/BlueBrain/Livre>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _LRUCachePolicy_h_
#define _LRUCachePolicy_h_

#include <livre/core/cache/CachePolicy.h>


namespace livre
{

/**
 * The LRUCachePolicy class provides \see Cache class, Least Reecently Used unloading policy.
 */
class LRUCachePolicy : public CachePolicy
{
public:

    LRUCachePolicy();

    /**
     * Checks whether the policy will be activated for current state of \see Cache.
     * @param cache The \see Cache to check for the policy.
     * @return True if the limitations are passed.
     */
    virtual bool willPolicyBeActivated( const Cache& cache ) const;

    /**
     * Checks whether the policy satisfied after each unload.
     * @param cache The \see Cache to check for the policy.
     * @return True if the limitations are satisfied.
     */
    virtual bool isPolicySatisfied( const Cache& cache ) const;

    /**
     * Sets the maximum memory.
     * @param maxMemBytes Maximum memory in bytes.
     */
    void setMaximumMemory( const size_t maxMemBytes );

    /**
     * Sets the clean up ratio.
     * @param cleanUpRatio Once the policy is activated, ( 1.0 - cleanUpRatio ) can be cleaned.
     */
    void setCleanupRatio( float cleanUpRatio );

private:

    virtual void _apply( const Cache& cache,
                         const std::vector< CacheObject * >& cacheObjects,
                         std::vector< CacheObject * >& modifiedObjects );

    size_t _maxMemBytes;
    float _cleanUpRatio;
};

}

#endif // _LRUCachePolicy_h_
