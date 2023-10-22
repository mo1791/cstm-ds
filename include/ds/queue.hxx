#ifndef QUEUE_S_HXX
#define QUEUE_S_HXX



#include <algorithm>
#include <cassert>
#include <exception>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <ranges>

namespace rng = std::ranges;

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class> class queue;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T> concept is_class = std::is_class<T>::value;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


//
// START NODE
//
template <class T> class node final
{

public:
    friend class queue<T>;

public:
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;


public:
    using node_ptr_t = std::add_pointer_t<node<T>>;

public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    node() noexcept requires( std::default_initializable<T> )
        : m_data{ T{} }
        , m_prev{ this }
        , m_next{ this }
   {}
//  --------------------------------------------------------------------------
    node(T const & p_data) noexcept requires( std::copy_constructible<T> )
        : m_data{ p_data }
        , m_prev{ this }
        , m_next{ this }
    {}
//  --------------------------------------------------------------------------
    node(T && p_data) noexcept requires( std::move_constructible<T> )
        : m_data{ std::move(p_data) }
        , m_prev{ this }
        , m_next{ this }
    {}
//  --------------------------------------------------------------------------
    template <class ...ARGS>
    node( ARGS&& ...p_args ) noexcept requires( is_class<T> && std::constructible_from<T, ARGS...> )
        : m_data{ T{ std::forward<ARGS>(p_args)... } }
        , m_prev{ this }
        , m_next{ this }
    {}
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    auto data()       noexcept -> decltype(auto) { return m_data; }
//  --------------------------------------------------------------------------
    auto data() const noexcept -> decltype(auto) { return m_data; }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    void push_front( node_ptr_t p_node ) noexcept
    {
        p_node->m_next  = m_next;
        p_node->m_prev  = this;
        m_next->m_prev  = p_node;
        m_next          = p_node;
    }
//  --------------------------------------------------------------------------
    void push_back( node_ptr_t p_node ) noexcept
    {
        p_node->m_prev  = m_prev;
        p_node->m_next  = this;
        m_prev->m_next  = p_node;
        m_prev          = p_node;
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


private:
    value_type m_data;
    node_ptr_t m_prev;
    node_ptr_t m_next;
};

//  //

//* END NODE *//



// /*    */ //

// /** **/ //
template <class T> class queue final
{
public:
    using node_type = node<T>;
    using node_t    = typename node_type::node_ptr_t;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

public:
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;
    using size_type       = std::size_t;

public:
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    queue() requires( std::default_initializable<T> );
//  -----------------------------------------------------------------------
    template <std::input_iterator I, std::sentinel_for<I> S>
    queue( I , S ) requires( std::convertible_to<std::iter_value_t<I>, T> );
//  -----------------------------------------------------------------------
    template <std::ranges::input_range R>
    queue( R && ) requires( std::convertible_to<std::ranges::range_value_t<R>, T>);
//  -----------------------------------------------------------------------
    queue( queue const & );
//  -----------------------------------------------------------------------
    queue( queue && ) noexcept;
//  -----------------------------------------------------------------------
    auto operator=(queue) -> decltype(auto);
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class U> void push_back( U && ) requires( std::convertible_to<U,T> );
//  -----------------------------------------------------------------------
    template <class U> void push( U && ) requires( std::convertible_to<U,T> );
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class ...ARGS>
    void emplace_back(ARGS&&...) requires( is_class<T> && std::constructible_from<T, ARGS...> );
//  -----------------------------------------------------------------------
    template <class ...ARGS>
    void emplace(ARGS&&...) requires( is_class<T> && std::constructible_from<T, ARGS...> );
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    void pop_front();
//  -----------------------------------------------------------------------
    void pop();
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto empty() const noexcept -> bool;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto peek() const noexcept -> std::optional<node_type>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto size() const noexcept -> size_type;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto front()       noexcept -> std::optional<node_type>;
//  -----------------------------------------------------------------------
    [[nodiscard]] auto front() const noexcept -> std::optional<node_type>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto back()       noexcept -> std::optional<node_type>;
//  -----------------------------------------------------------------------
    [[nodiscard]] auto back() const noexcept -> std::optional<node_type>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    friend void swap(queue & p_lhs, queue & p_rhs)
    {
        using std::swap;

        swap(p_lhs.m_head, p_rhs.m_head);
        swap(p_lhs.m_size, p_rhs.m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------




//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    ~queue();
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


public:
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    void debug() noexcept
    {
        node_t v_node = m_head->m_next;

        while ( v_node != m_head )
        {
            std::cout << v_node->m_data << ' ';
            v_node = v_node->m_next;
        }
        std::cout << std::endl;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


private:
    node_t    m_head;
    size_type m_size;
};


//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <std::input_iterator I, std::sentinel_for<I> S>
    queue<T>::queue( I p_begin, S p_end ) requires( std::convertible_to<std::iter_value_t<I>, T> )
        : queue()
    {
        while( p_begin != p_end )
        {
            push_back( std::move(*p_begin) );

            ++p_begin;
        }
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <std::ranges::input_range R>
    queue<T>::queue( R && p_container ) requires( std::convertible_to<std::ranges::range_value_t<R>, T>)
        : queue(rng::begin(p_container), rng::end(p_container))
    {
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------





//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <class U>
    void queue<T>::push_back( U && p_value ) requires( std::convertible_to<U,T> )
    {
        node_t v_node = nullptr;

        try
        {
            v_node = new node_type( std::forward<U>(p_value) );
        }
        catch(...)
        {
            v_node = ( delete v_node, nullptr );

            throw;
        }

        m_head->push_back( v_node );

        m_size = -(~m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <class U>
    void queue<T>::push( U && p_value ) requires( std::convertible_to<U,T> )
    {
        push_back( std::forward<U>(p_value) );
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <class...ARGS>
    void queue<T>::emplace_back( ARGS && ... p_args ) requires( is_class<T> && std::constructible_from<T, ARGS...> )
    {

        node_t v_node = nullptr;

        try
        {
            v_node = new node_type( std::forward<ARGS>(p_args)... );
        }
        catch(...)
        {
            v_node = ( delete v_node, nullptr );

            throw;
        }


        m_head->push_back( v_node );

        m_size = -(~m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <class ...ARGS>
    void queue<T>::emplace( ARGS && ... p_args ) requires( is_class<T> && std::constructible_from<T, ARGS...> )
    {
        emplace_front( std::forward<ARGS>(p_args)... );
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------#include <algorithm>
#include <cassert>
#include <exception>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <ranges>

namespace rng = std::ranges;

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class> class queue;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T> concept is_class = std::is_class<T>::value;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


//
// START NODE
//
template <class T> class node final
{

public:
    friend class queue<T>;

public:
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;


public:
    using node_ptr_t = std::add_pointer_t<node<T>>;

public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    node() noexcept requires( std::default_initializable<T> )
        : m_data{ T{} }
        , m_prev{ this }
        , m_next{ this }
   {}
//  --------------------------------------------------------------------------
    node(T const & p_data) noexcept requires( std::copy_constructible<T> )
        : m_data{ p_data }
        , m_prev{ this }
        , m_next{ this }
    {}
//  --------------------------------------------------------------------------
    node(T && p_data) noexcept requires( std::move_constructible<T> )
        : m_data{ std::move(p_data) }
        , m_prev{ this }
        , m_next{ this }
    {}
//  --------------------------------------------------------------------------
    template <class ...ARGS>
    node( ARGS&& ...p_args ) noexcept requires( is_class<T> && std::constructible_from<T, ARGS...> )
        : m_data{ T{ std::forward<ARGS>(p_args)... } }
        , m_prev{ this }
        , m_next{ this }
    {}
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    auto data()       noexcept -> decltype(auto) { return m_data; }
//  --------------------------------------------------------------------------
    auto data() const noexcept -> decltype(auto) { return m_data; }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------



public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    void push_front( node_ptr_t p_node ) noexcept
    {
        p_node->m_next  = m_next;
        p_node->m_prev  = this;
        m_next->m_prev  = p_node;
        m_next          = p_node;
    }
//  --------------------------------------------------------------------------
    void push_back( node_ptr_t p_node ) noexcept
    {
        p_node->m_prev  = m_prev;
        p_node->m_next  = this;
        m_prev->m_next  = p_node;
        m_prev          = p_node;
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


private:
    value_type m_data;
    node_ptr_t m_prev;
    node_ptr_t m_next;
};

//  //

//* END NODE *//



// /*    */ //

// /** **/ //
template <class T> class queue final
{
public:
    using node_type = node<T>;
    using node_t    = typename node_type::node_ptr_t;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

public:
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;
    using size_type       = std::size_t;

public:
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    queue() requires( std::default_initializable<T> );
//  -----------------------------------------------------------------------
    template <std::input_iterator I, std::sentinel_for<I> S>
    queue( I , S ) requires( std::convertible_to<std::iter_value_t<I>, T> );
//  -----------------------------------------------------------------------
    template <std::ranges::input_range R>
    queue( R && ) requires( std::convertible_to<std::ranges::range_value_t<R>, T>);
//  -----------------------------------------------------------------------
    queue( queue const & );
//  -----------------------------------------------------------------------
    queue( queue && ) noexcept;
//  -----------------------------------------------------------------------
    auto operator=(queue) -> decltype(auto);
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class U> void push_back( U && ) requires( std::convertible_to<U,T> );
//  -----------------------------------------------------------------------
    template <class U> void push( U && ) requires( std::convertible_to<U,T> );
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class ...ARGS>
    void emplace_back(ARGS&&...) requires( is_class<T> && std::constructible_from<T, ARGS...> );
//  -----------------------------------------------------------------------
    template <class ...ARGS>
    void emplace(ARGS&&...) requires( is_class<T> && std::constructible_from<T, ARGS...> );
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    void pop_front();
//  -----------------------------------------------------------------------
    void pop();
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto empty() const noexcept -> bool;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto peek() const noexcept -> std::optional<node_type>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto size() const noexcept -> size_type;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto front()       noexcept -> std::optional<node_type>;
//  -----------------------------------------------------------------------
    [[nodiscard]] auto front() const noexcept -> std::optional<node_type>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto back()       noexcept -> std::optional<node_type>;
//  -----------------------------------------------------------------------
    [[nodiscard]] auto back() const noexcept -> std::optional<node_type>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    friend void swap(queue & p_lhs, queue & p_rhs)
    {
        using std::swap;

        swap(p_lhs.m_head, p_rhs.m_head);
        swap(p_lhs.m_size, p_rhs.m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------




//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    ~queue();
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


public:
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    void debug() noexcept
    {
        node_t v_node = m_head->m_next;

        while ( v_node != m_head )
        {
            std::cout << v_node->m_data << ' ';
            v_node = v_node->m_next;
        }
        std::cout << std::endl;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


private:
    node_t    m_head;
    size_type m_size;
};


//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <std::input_iterator I, std::sentinel_for<I> S>
    queue<T>::queue( I p_begin, S p_end ) requires( std::convertible_to<std::iter_value_t<I>, T> )
        : queue()
    {
        while( p_begin != p_end )
        {
            push_back( std::move(*p_begin) );

            ++p_begin;
        }
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <std::ranges::input_range R>
    queue<T>::queue( R && p_container ) requires( std::convertible_to<std::ranges::range_value_t<R>, T>)
        : queue(rng::begin(p_container), rng::end(p_container))
    {
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------





//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <class U>
    void queue<T>::push_back( U && p_value ) requires( std::convertible_to<U,T> )
    {
        node_t v_node = nullptr;

        try
        {
            v_node = new node_type( std::forward<U>(p_value) );
        }
        catch(...)
        {
            v_node = ( delete v_node, nullptr );

            throw;
        }

        m_head->push_back( v_node );

        m_size = -(~m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <class U>
    void queue<T>::push( U && p_value ) requires( std::convertible_to<U,T> )
    {
        push_back( std::forward<U>(p_value) );
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <class...ARGS>
    void queue<T>::emplace_back( ARGS && ... p_args ) requires( is_class<T> && std::constructible_from<T, ARGS...> )
    {

        node_t v_node = nullptr;

        try
        {
            v_node = new node_type( std::forward<ARGS>(p_args)... );
        }
        catch(...)
        {
            v_node = ( delete v_node, nullptr );

            throw;
        }


        m_head->push_back( v_node );

        m_size = -(~m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class T>
    template <class ...ARGS>
    void queue<T>::emplace( ARGS && ... p_args ) requires( is_class<T> && std::constructible_from<T, ARGS...> )
    {
        emplace_front( std::forward<ARGS>(p_args)... );
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------




//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    extern template class queue<int>;
    extern template class queue<char>;
    extern template class queue<long>;
    extern template class queue<short>;
    extern template class queue<unsigned int>;
    extern template class queue<unsigned char>;
    extern template class queue<unsigned long>;
    extern template class queue<unsigned short>;
    extern template class queue<float>;
    extern template class queue<double>;
    extern template class queue<std::string>;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

#endif
