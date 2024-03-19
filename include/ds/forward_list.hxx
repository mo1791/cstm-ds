#ifndef SINGLY_LINKEDLIST_HXX
#define SINGLY_LINKEDLIST_HXX



#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <type_traits>
#include <utility>



template <typename> class forward_list;
template <typename> class Iterator;
template <typename> class Sentinel;


/** START CONCEPTS **/

template <class T, class U>
concept non_self = 
        not std::is_same<std::decay_t<T>, U>::value &&
        not std::is_base_of<U, std::decay_t<T>>::value;


/* END CONSTRAINTS */



//
template <class T> class node final
{

public:
    friend class forward_list<T>;
    friend class Iterator<T>;


public: /** TYPE ALIAS **/
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;



public: /** CONSTRUCTORS **/


    /** DEFAULT CTOR **/
    node() 
            noexcept(std::is_nothrow_default_constructible<T>::value)
            requires(std::is_default_constructible<T>::value)
        : m_data{T{}}
    {}

    /** PARAM CTOR **/
    explicit node(T const &p_data, node* p_next = nullptr)
            noexcept(std::is_nothrow_copy_constructible<T>::value)
            requires(std::is_copy_constructible<T>::value)
        : m_data{p_data}
        , m_next{p_next}
    {}

    /** PARAM CTOR (rvalue ref) **/
    explicit node(T &&p_data, node* p_next = nullptr)
            noexcept(std::is_nothrow_move_constructible<T>::value)
            requires(std::is_move_constructible<T>::value)
        : m_data{std::move(p_data)}
        , m_next{p_next}
    {}

    /** **/
    template <class... ARGS>
    node(std::in_place_t, ARGS &&...p_args)
            noexcept(std::is_nothrow_constructible<T, ARGS...>::value)
            requires(std::is_constructible<T, ARGS...>::value)
        : m_data{T{std::forward<ARGS>(p_args)...}}
    {}



public: /** GETTERS **/

    auto data()       noexcept(true) -> value_type { return m_data; }
    auto data() const noexcept(true) -> value_type { return m_data; }



private:
    value_type m_data;
    node*      m_next {};
};

//  //

//* END NODE *//

// //
template <class T> class Iterator
{

public:
    friend class Sentinel<T>;

public: /** TYPE ALIAS **/
    using node_type = node<T>;

public: /** TYPE ALIAS **/
    using value_type      = typename node_type::value_type;
    using reference       = typename node_type::reference;
    using const_reference = typename node_type::const_reference;
    using pointer         = typename node_type::pointer;
    using const_pointer   = typename node_type::const_pointer;
    using difference_type = std::ptrdiff_t;



public: /** CONSTRUCTORS **/


    /** DEFAULT CTOR **/
    Iterator() noexcept(true) = default;

    /** DEFAULT COPY CTOR **/
    Iterator(Iterator const& ) noexcept(true) = default;

    /** DEFAULT MOVE CTOR **/
    Iterator(Iterator&&) noexcept(true) = default;

    /** PARAM CTOR **/
    Iterator(node_type* p_node) noexcept(true) : m_node(p_node) {}


    /** DEFAULT COPY ASSIGN **/
    Iterator& operator=(Iterator const&) noexcept(true) = default;

    /** DEFAULT MOVE ASSIGN **/
    Iterator& operator=(Iterator&&) noexcept(true) = default;


public:
    /** DEREFERENCE OP **/
    auto operator*()       noexcept(true) -> reference { return m_node->m_data; }
    auto operator*() const noexcept(true) -> reference { return m_node->m_data; }


    /** ARROW OP **/
    auto operator->()       noexcept(true) ->       pointer { return std::addressof( m_node->m_data ); }
    auto operator->() const noexcept(true) -> const_pointer { return std::addressof( m_node->m_data ); }





    /** (POST & PRE ) INCREMENT OP **/
    auto operator++() noexcept(true) -> Iterator &
    {
        m_node = m_node->m_next;

        return *this;
    }

    auto operator++(this Iterator self, int) noexcept(true) -> Iterator
    {
        return ++self;
    }


    /** COMPAR OP **/
    [[nodiscard]] friend auto operator==(Iterator const &p_lhs,
                                         Iterator const &p_rhs) noexcept(true)
        -> bool
    {
        return (p_lhs.m_node == p_rhs.m_node);
    }

    /* As of C++20, operator!= is auto generated as !(operator==) */



protected:
    node_type* m_node{};

};

//** END ITERATOR **//


//** START SENTINEL **//

template <class T> struct Sentinel
{

public:
    [[nodiscard]] friend auto operator==(Iterator<T> const& lhs,
                                         Sentinel<T> const&) noexcept(true)
        -> bool
    {
        return ( lhs.m_node == nullptr );
    }
};

//** END SENTINEL **//


//*****//

// FORWARD_LIST //
template <class T> class forward_list final
{
   
private: /** TYPE ALIAS **/
    using node_type = node<T>;


public:/** TYPE ALIAS **/
    using value_type      = typename node_type::value_type;
    using reference       = typename node_type::reference;
    using const_reference = typename node_type::const_reference;
    using pointer         = typename node_type::pointer;
    using const_pointer   = typename node_type::const_pointer;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    using iterator          = Iterator<T>;
    using const_iterator    = Iterator<typename std::add_const<T>::type>;
    using sentinel          = Sentinel<T>;
    using iterator_category = std::forward_iterator_tag;
   

public: /** CONSTRUCTORS **/


    /** DEFAULT CTOR **/
    forward_list() noexcept(true);

    /** COPY CTOR **/
    forward_list(forward_list const&) noexcept(std::is_nothrow_copy_constructible<T>::value);

    /** MOVE CTOR **/
    forward_list(forward_list&&) noexcept(true);

    /** 
    * INITIALIZER_LIST CTOR 
    * (Construct with the contents of the initializer forward_list)
    * **/
    forward_list(std::initializer_list<T>) noexcept(std::is_nothrow_copy_constructible<T>::value);

    /**
    * RANGE CTOR
    * (Construct with the contents of the range)
    * **/
    template <std::ranges::range R>
    forward_list(R&&) noexcept(std::is_nothrow_constructible<T, std::ranges::range_value_t<R>>::value)
        requires(non_self<R, forward_list> && 
                 std::is_constructible<T, std::ranges::range_value_t<R>>::value);

    /**
    * RANGE CTOR
    * (Construct with the contents of the range [ begin, end ])
    * **/
    template <std::input_iterator I, std::sentinel_for<I> S>
    forward_list(I, S) noexcept(std::is_nothrow_constructible<T, std::iter_value_t<I>>::value)
        requires(std::is_constructible<T, std::iter_value_t<I>>::value);



    /**
    * ASSIGNMENT OP
    * ( copy-swap idiom )
    * **/
    auto operator=(forward_list)
        noexcept(std::is_nothrow_copy_constructible<T>::value) -> forward_list &;



public:


    template <class U>
    void push_front(U&&) 
        noexcept(std::is_nothrow_constructible<T, U>::value)
        requires(std::is_constructible<T, U>);

    template <class U>
    void push_before(U&&, std::integral auto)
        noexcept(std::is_nothrow_constructible<T, U>::value)
        requires(std::is_constructible<T, U>);

    template <class U>
    void push_after(U&&, std::integral auto) 
        noexcept(std::is_nothrow_constructible<T, U>::value)
        requires(std::is_constructible<T, U>);

    template <class U>
    void push_at(U&&, std::integral auto)
        noexcept(std::is_nothrow_constructible<T, U>::value)
        requires(std::is_constructible<T, U>);





    template <class... ARGS>
    void emplace_front(ARGS&& ...)
        noexcept(std::is_nothrow_constructible<T, ARGS...>::value);

    template <class... ARGS>
    void emplace_before(std::integral auto, ARGS&& ...)
        noexcept(std::is_nothrow_constructible<T, ARGS...>::value);

    template <class... ARGS>
    void emplace_after(std::integral auto, ARGS&& ...)
        noexcept(std::is_nothrow_constructible<T, ARGS...>::value);

    template <class... ARGS>
    void emplace_at(std::integral auto, ARGS&& ...)
        noexcept(std::is_nothrow_constructible<T, ARGS...>::value);



public:
    void pop_front() noexcept(std::is_nothrow_destructible<T>::value);
    void pop_at(std::integral auto) noexcept(std::is_nothrow_destructible<T>::value);



public:
    [[nodiscard]] auto front() const noexcept(true) -> std::optional<value_type>;
    [[nodiscard]] auto front()       noexcept(true) -> std::optional<value_type>;



public:
    [[nodiscard]] auto empty() const noexcept(true) -> bool;

    [[nodiscard]] auto size() const noexcept(true) -> size_type;



public:
    auto begin() const noexcept(true) -> iterator;
    auto begin()       noexcept(true) -> iterator;

    auto end() const noexcept(true) -> sentinel;
    auto end()       noexcept(true) -> sentinel;


public:
    friend void swap(forward_list& p_lhs, forward_list& p_rhs) noexcept(true)
    {
        using std::swap;

        swap(p_lhs.m_head, p_rhs.m_head);
        swap(p_lhs.m_size, p_rhs.m_size);
    }


public:
    ~forward_list() noexcept(std::is_nothrow_destructible<T>::value);


private:
    auto reverse(node_type* p_head) noexcept(true) -> node_type*
    {
        node_type* p_prev = nullptr;
        node_type* p_curr = p_head;

        while( p_curr != nullptr)
        {
            node_type* p_next = p_curr->m_next;
            
            p_curr->m_next = p_prev;
            p_prev         = p_curr;
            p_curr         = p_next;
        }
        return p_prev;
    }


    auto find_at(std::integral auto p_pos) noexcept(true) -> node_type*
    {
        if ( p_pos < 0ul || p_pos >= m_size )
        {
            return nullptr;
        }

        node_type* v_curr = m_head;

        for( auto index = 0ul; v_curr != nullptr; )
        {
            if ( index++ == p_pos ) break;

            v_curr = v_curr->m_next;
        }

        return v_curr;
    }

private:
    node_type* m_head {};
    size_type  m_size {};

};

//  //

//*****//


template <class T>
template <std::input_iterator I, std::sentinel_for<I> S>
forward_list<T>::forward_list(I p_first, S p_last)
        noexcept(std::is_nothrow_constructible<T, std::iter_value_t<I>>::value)
        requires(std::is_constructible<T, std::iter_value_t<I>>::value)
    : forward_list{}
{
    using std::placeholders::_1;

    auto lambda = &forward_list::template push_front<std::iter_reference_t<I>>;

    std::ranges::for_each(p_first, p_last, std::bind(lambda, this, _1) );

}


template <class T>
template <std::ranges::range R>
forward_list<T>::forward_list(R&& p_list)
        noexcept(std::is_nothrow_constructible<T, std::ranges::range_value_t<R>>::value)
        requires(non_self<R, forward_list> &&
         std::is_constructible<T, std::ranges::range_value_t<R>>::value)
    : forward_list(std::ranges::rbegin(p_list), std::ranges::rend(p_list))
{
}


template <class T>
template <class U>
void forward_list<T>::push_front(U&& p_data)
    noexcept(std::is_nothrow_constructible<T, U>::value)
    requires(std::is_constructible<T, U>)
{

    if ( auto v_node = new(std::nothrow) node_type(std::forward<U>(p_data), m_head) )
    {
        m_head = v_node;
        m_size = m_size + 1ul;
    }
}


template <class T>
template <class U>
void forward_list<T>::push_before(U&& p_data, std::integral auto p_pos)
    noexcept(std::is_nothrow_constructible<T, U>::value)
    requires(std::is_constructible<T, U>)
{
    
    node_type* v_targ = find_at(p_pos - 1ul);

    if ( not v_targ ) return;


    if ( auto v_node = new(std::nothrow) node_type(std::forward<U>(p_data), v_targ->m_next) )
    {
        v_targ->m_next = v_node;
        m_size = m_size + 1ul;
    }
}


template <class T>
template <class U>
void forward_list<T>::push_after(U&& p_data, std::integral auto p_pos)
    noexcept(std::is_nothrow_constructible<T, U>::value)
    requires(std::is_constructible<T, U>)
{
    node_type* v_targ = find_at(p_pos);

    if ( not v_targ ) return;

    if ( auto v_node = new(std::nothrow) node_type(std::forward<U>(p_data), v_targ->m_next) )
    {
        v_targ->m_next = v_node;
        m_size = m_size + 1ul;
    }
}


template <class T>
template <class U>
void forward_list<T>::push_at(U&& p_data, std::integral auto p_pos)
    noexcept(std::is_nothrow_constructible<T, U>::value)
    requires(std::is_constructible<T, U>)
{
    node_type* v_targ = find_at(p_pos);

    if ( not v_targ ) return;

    ::new (static_cast<void*>(v_targ)) node_type(std::forward<U>(p_data), v_targ->m_next);
}


template <class T>
template <class... ARGS>
void forward_list<T>::emplace_front(ARGS&& ...p_args)
    noexcept(std::is_nothrow_constructible<T, ARGS...>::value)
{
    if ( auto v_node = new(std::nothrow) node_type(std::in_place, std::forward<ARGS>(p_args)...) )
    {
        v_node->m_next = m_head;
        m_head         = v_node;
    
        m_size = m_size + 1ul;
    }

}


template <class T>
template <class... ARGS>
void forward_list<T>::emplace_before(std::integral auto p_pos, ARGS&& ...p_args)
    noexcept(std::is_nothrow_constructible<T, ARGS...>::value)
{
    node_type* v_targ = find_at(p_pos - 1ul);

    if ( not v_targ ) return;

    if ( auto v_node = new(std::nothrow) node_type(std::in_place, std::forward<ARGS>(p_args)...) )
    {
        v_node->m_next = v_targ->m_next;
        v_targ->m_next = v_node;

        m_size = m_size + 1ul;
    }
}


template <class T>
template <class... ARGS>
void forward_list<T>::emplace_after(std::integral auto p_pos, ARGS&& ...p_args)
    noexcept(std::is_nothrow_constructible<T, ARGS...>::value)
{
    node_type* v_targ = find_at(p_pos);

    if ( not v_targ) return;

    if ( auto v_node = new(std::nothrow) node_type(std::in_place, std::forward<ARGS>(p_args)...) )
    {
        v_node->m_next = v_targ->m_next;
        v_targ->m_next = v_node;

        m_size = m_size + 1ul;
    }
}

template <class T>
template <class... ARGS>
void forward_list<T>::emplace_at(std::integral auto p_pos, ARGS&& ...p_args)
    noexcept(std::is_nothrow_constructible<T, ARGS...>::value)
{
    node_type* v_targ = find_at(p_pos);

    if ( not v_targ) return;

    auto v_next = v_targ->m_next;

    ::new (static_cast<void*>(v_targ)) node_type(std::in_place, std::forward<ARGS>(p_args)...);

    v_targ->m_next = v_next;
}



template <class T>
void forward_list<T>::pop_at(std::integral auto p_pos)
    noexcept(std::is_nothrow_destructible<T>::value)
{
    assert( not empty() );

    node_type* v_tmp = find_at(p_pos - 1ul);

    if ( not v_tmp || not v_tmp->m_next )
        return;

    node_type* v_targ = v_tmp->m_next;

    v_tmp->m_next = v_targ->m_next;

    v_targ = ( delete v_targ, nullptr );

    m_size = m_size - 1ul;
}




/** USER DEFINED TYPE DEDUCTION **/
template <std::ranges::range R>
forward_list( R ) -> forward_list<std::ranges::range_value_t<R>>;

template <std::input_iterator I, std::sentinel_for<I> S>
forward_list( I, S ) -> forward_list<std::iter_value_t<I>>;



/**
* This specialization of std::ranges::enable_borrowed_range 
* makes list satisfy borrowed_range 
* **/
template <class T>
inline constexpr bool std::ranges::enable_borrowed_range<forward_list<T>> = true;



extern template class forward_list<int>;
extern template class forward_list<char>;
extern template class forward_list<long>;
extern template class forward_list<short>;
extern template class forward_list<unsigned int>;
extern template class forward_list<unsigned char>;
extern template class forward_list<unsigned long>;
extern template class forward_list<unsigned short>;
extern template class forward_list<float>;
extern template class forward_list<double>;
extern template class forward_list<std::string>;


#endif
// // end forward_list
