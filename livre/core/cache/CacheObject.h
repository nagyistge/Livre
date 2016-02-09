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

#ifndef _CacheObject_h_
#define _CacheObject_h_

#include <livre/core/api.h>
#include <livre/core/types.h>
#include <livre/core/lunchboxTypes.h>
#include <livre/core/util/ThreadClock.h>

namespace livre
{

/**
 * The CacheObject class for memory objects that can be managed with \see Cache.
 * If it is generated by cache it is valid and one reference is hold by the cache, otherwise
 * it is an invalid cache object.
 */

class CacheObject
{
public:
    LIVRECORE_API virtual ~CacheObject() { }

    /**
     * @return True if the object is valid.
     */
    LIVRECORE_API bool isValid() const;

    /**
     * @return The unique cache id.
     */
    CacheId getId() const;

    /**
     * Loads the object to cache ( if the reference count is more than 1, it is not unloaded ).
     * Function is thread safe for loading.
     */
    LIVRECORE_API void load();

    /**
     * Unloads the object from the memory, if there are not any references. Function is threadsafe
     * for unloading.
     */
    LIVRECORE_API void unload();

    /**
     * @return The memory size of the object in bytes.
     */
    virtual size_t getCacheSize() const = 0;

    /**
     * @return The last time object is used.
     */
    LIVRECORE_API double getLastUsed() const;

    /**
     * @return The last time object is loaded.
     */
    LIVRECORE_API double getLoadTime() const;

    /**
     * @return The object is unloadable, where \see Cache cannot unload it.
     */
    LIVRECORE_API bool isUnloadable() const;

    /**
     * @return The object is loaded in cache. The function is thread safe.
     */
    LIVRECORE_API bool isLoaded() const;

    /**
     * setUnloadable Sets the objects unloadablity.
     * @param unloadable If true, object can not be unloaded.
     */
    LIVRECORE_API void setUnloadable( bool unloadable );

    /**
     * getReferenceCount_ Should not be called by user. The function is threadsafe.
     * @return The number of references to CacheObject.
     */
    LIVRECORE_API uint32_t getReferenceCount_() const;

    /**
     * Register an observer for operations done on the object.
     * @param observer The observer to add.
     */
    LIVRECORE_API void registerObserver( CacheObjectObserver* observer );

    /**
     * Unregisters an observer from the object
     * @param observer The observer to remove.
     */
    LIVRECORE_API void unregisterObserver( CacheObjectObserver* observer );

    /**
     * Should not be called by user.
     * updateLastUsedWithCurrentTime_ Updates the last used time with current time.
     */
    LIVRECORE_API void updateLastUsedWithCurrentTime_();

protected:

    friend class Cache;
    friend class CacheStatistics;

    LIVRECORE_API CacheObject( const CacheId& cacheId = INVALID_CACHE_ID );

    /**
     * Implemented by the derived class, for loading data to memory. Thread safety is satisfied for
     * loading and unloading.
     * @return True if data is loaded.
     */
    virtual bool _load() = 0;

    /**
     * Implemented by the derived class, for unloading data from memory. Thread safety is satisfied for
     * loading and unloading.
     */
    virtual void _unload() = 0;

    /**
     * @return The validity of derived class.
     */
    virtual bool _isValid() const;

    /**
     * @return True if object is loaded into memory.
     */
    virtual bool _isLoaded() const = 0;

    /**
     * @return The unconst object.
     */
    CacheObject* _getUnconst() const { return const_cast< CacheObject * >( this ); }

    /**
     * @return On default returns true if cache ids are same
     */
    virtual bool operator==( const CacheObject& cacheObject ) const;

private:
    LIVRECORE_API void _increaseReference();
    LIVRECORE_API void _decreaseReference();

    friend void intrusive_ptr_add_ref( CacheObject *object ) { object->_increaseReference(); }
    friend void intrusive_ptr_release( CacheObject *object ) { object->_decreaseReference(); }

    friend void intrusive_ptr_add_ref( const CacheObject *object ) { object->_getUnconst()->_increaseReference(); }
    friend void intrusive_ptr_release( const CacheObject *object ) { object->_getUnconst()->_decreaseReference(); }

    struct Status;
    mutable boost::shared_ptr< Status > _status;

    LB_TS_VAR( thread_ );
};

}

#endif // _CacheObject_h_
