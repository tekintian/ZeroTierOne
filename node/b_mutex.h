#ifndef THREAD_SAFETY_ANALYSIS_MUTEX_H
#define THREAD_SAFETY_ANALYSIS_MUTEX_H

// Enable thread safety attributes only with clang.
// The attributes can be safely erased when compiling with other compilers.
#if defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   // no-op
#endif

#define CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(capability(x))

#define SCOPED_CAPABILITY \
  THREAD_ANNOTATION_ATTRIBUTE__(scoped_lockable)

#define GUARDED_BY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))

#define REQUIRES(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

#define ACQUIRE(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

#define RELEASE(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(release_capability(__VA_ARGS__))

#define NO_THREAD_SAFETY_ANALYSIS \
  THREAD_ANNOTATION_ATTRIBUTE__(no_thread_safety_analysis)








namespace zt {

template <class _Mutex>
class SCOPED_CAPABILITY shared_lock
{
public:
    typedef _Mutex mutex_type;

private:
    mutex_type* __m_;
    bool __owns_;

public:

    shared_lock(mutex_type& __m) ACQUIRE(__m) {}
    
    ~shared_lock() RELEASE() {}
    
};


template <class _Mutex>
class SCOPED_CAPABILITY unique_lock
{
public:
    typedef _Mutex mutex_type;

private:
    mutex_type* __m_;

public:
    unique_lock(mutex_type& __m) ACQUIRE(__m) {}
    
    ~unique_lock() RELEASE() {}
};




class CAPABILITY("mutex") shared_mutex
{
 public:
//     shared_mutex() {}
//     ~shared_mutex() = default;

//     shared_mutex(const shared_mutex&) = delete;
//     shared_mutex& operator=(const shared_mutex&) = delete;

//     void lock_shared() {}
    
    
    // For negative capabilities.
    const shared_mutex& operator!() const { return *this; }
};



class CAPABILITY("mutex") mutex
{
//    __libcpp_mutex_t __m_ = _LIBCPP_MUTEX_INITIALIZER;

public:
//    _LIBCPP_INLINE_VISIBILITY
//    _LIBCPP_CONSTEXPR mutex() = default;

//    mutex(const mutex&) = delete;
//    mutex& operator=(const mutex&) = delete;

//#if defined(_LIBCPP_HAS_TRIVIAL_MUTEX_DESTRUCTION)
//    ~mutex() = default;
//#else
//    ~mutex() _NOEXCEPT;
//#endif
//
    void lock() ACQUIRE() {}
//    bool try_lock() _NOEXCEPT _LIBCPP_THREAD_SAFETY_ANNOTATION(try_acquire_capability(true));
    void unlock() RELEASE() {}
//
//    typedef __libcpp_mutex_t* native_handle_type;
//    _LIBCPP_INLINE_VISIBILITY native_handle_type native_handle() {return &__m_;}
    
    
    // For negative capabilities.
    const mutex& operator!() const { return *this; }
};




template <class _Mutex>
class SCOPED_CAPABILITY lock_guard
{
 public:
     typedef _Mutex mutex_type;

// private:
//     mutex_type& __m_;
  
 public:

//     _LIBCPP_NODISCARD_EXT _LIBCPP_INLINE_VISIBILITY
    lock_guard(mutex_type& __m) ACQUIRE(__m) {}

//     _LIBCPP_NODISCARD_EXT _LIBCPP_INLINE_VISIBILITY
//     lock_guard(mutex_type& __m, adopt_lock_t) _LIBCPP_THREAD_SAFETY_ANNOTATION(requires_capability(__m))
//         : __m_(__m) {}
//     _LIBCPP_INLINE_VISIBILITY
    ~lock_guard() RELEASE() {}

// private:
//     lock_guard(lock_guard const&) = delete;
//     lock_guard& operator=(lock_guard const&) = delete;
};




}















#endif  // THREAD_SAFETY_ANALYSIS_MUTEX_H
