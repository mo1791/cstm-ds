#ifndef STACK_N_HXX
#define STACK_N_HXX

#include <algorithm>
#include <cassert>
#include <exception>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>


template <class> class stack;


// START NODE
//
template <class T> class node final
{

public:
    friend class stack<T>;

public: /** TYPE ALIAS **/
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;



public: /** CONSTRUCTORS **/
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    /** DEFAULT CTOR **/
    node() noexcept requires(std::default_initializable<T>)
        : m_data{T{}}
        , m_next{nullptr}
    {}
//  -------------------------------------------------------------------------
    /** PARAM CTOR **/
    explicit node(T const &p_value, node* p_next) noexcept
        requires(std::copy_constructible<T>)
        : m_data{p_value}
        , m_next{p_next}
    {}
//  -------------------------------------------------------------------------
    /** PARAM CTOR **/
    explicit node(T &&p_value, node* p_next) noexcept
        requires(std::move_constructible<T>)
        : m_data{std::move(p_value)}
        , m_next{p_next}
    {}
//  -------------------------------------------------------------------------
    /** **/
    template <class... ARGS>
    node(std::in_place_t,  &&...p_args) noexcept
        requires(std::constructible_from<T, ARGS...>)
        : m_data{std::forward<ARGS>(p_args)...}
        , m_next{nullptr}
    {}
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

public: /** GETTERS **/
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    auto data() const noexcept -> decltype(auto) { return m_data; }
//  --------------------------------------------------------------------------
    auto data()       noexcept -> decltype(auto) { return m_data; }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    auto next() const noexcept -> decltype(auto) { return m_next; }
//  --------------------------------------------------------------------------
    auto next()       noexcept -> decltype(auto) { return m_next; }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

private:
    value_type m_data;
    node*      m_next;

};

//  //

//* END NODE *//

template <class T> class stack final
{

public: /** TYPE ALIAS **/
    using node_type = node<T>;

public: /** TYPE ALIAS **/
    using value_type      = typename node_type::value_type;
    using reference       = typename node_type::reference;
    using const_reference = typename node_type::const_reference;
    using pointer         = typename node_type::pointer;
    using const_pointer   = typename node_type::const_pointer;
    using size_type       = std::size_t;


public: /** CONSTRUCTORS **/
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    /** DEFAULT CTOR **/
    stack() noexcept;
//  -------------------------------------------------------------------------
    /** COPY CTOR **/
    stack(stack const &) noexcept;
//  -------------------------------------------------------------------------
    /** MOVE CTOR **/
    stack(stack &&) noexcept;
//  -------------------------------------------------------------------------
    /**
    * RANGE CTOR
    * (Construct with the contents of the range [ begin, end ])
    * **/
    template <std::input_iterator I, std::sentinel_for<I> S>
    stack(I, S) noexcept
        requires(std::convertible_to<std::iter_value_t<I>, T>);
//  -------------------------------------------------------------------------
    /**
    * RANGE CTOR
    * (Construct with the contents of the range)
    * **/
    template <std::ranges::range R>
    stack(R &&) noexcept
        requires(std::convertible_to<std::ranges::range_value_t<R>, T>);
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------


//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------    
    /**
    * ASSIGNMENT OP
    * ( copy-swap idiom )
    * **/
    auto operator=(stack) noexcept -> stack &;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------


public:
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class U>
    void push_front(U &&) noexcept requires(std::convertible_to<U, T>);
//  -------------------------------------------------------------------------
    template <class U>
    void push(U &&) noexcept requires(std::convertible_to<U, T>);
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class... ARGS>
    void emplace_front(ARGS &&...) noexcept;
//  -------------------------------------------------------------------------
    template <class... ARGS>
    void emplace(ARGS &&...) noexcept;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------


public:
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    void pop_front() noexcept;
//  -------------------------------------------------------------------------
    void pop() noexcept;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------


public:
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    [[nodiscard]] auto peep() const noexcept -> std::optional<value_type>;
//  -------------------------------------------------------------------------
    [[nodiscard]] auto peep()       noexcept -> std::optional<value_type>;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------


public:
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    [[nodiscard]] auto empty() const noexcept -> bool;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    [[nodiscard]] auto size() const noexcept -> size_type;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

public:
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    ~stack() noexcept;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------


public:
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    friend void swap(stack &p_lhs, stack &p_rhs) noexcept
    {
        using std::swap;

        swap(p_lhs.m_head, p_rhs.m_head);
        swap(p_lhs.m_size, p_rhs.m_size);
    }
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    void debug() const noexcept
    {
        node_type* v_node = m_head;

        while (v_node)
        {
            std::cout << v_node->m_data << ' ';
            v_node = v_node->m_next;
        }

        std::cout << std::endl;
    }
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
private:
    node_type* m_head;
    size_type  m_size;
};



/****/
//  //
/****/


template <class T>
template <std::input_iterator I, std::sentinel_for<I> S>
stack<T>::stack(I p_begin, S p_end) noexcept
requires(std::convertible_to<std::iter_value_t<I>, T>)
    : stack()
{
    using std::placeholders::_1;

    auto lambda = &stack::template push_front<std::iter_reference_t<I>>;

    std::ranges::for_each(p_begin, p_end, std::bind(lambda, this, _1));
}


template <class T>
template <std::ranges::range R>
stack<T>::stack(R &&p_container) noexcept
requires(std::convertible_to<std::ranges::range_value_t<R>, T>)
    : stack(std::ranges::begin(p_container), std::ranges::end(p_container))
{}


template <class T>
template <class U>
void stack<T>::push_front(U &&p_value) noexcept requires(std::convertible_to<U, T>)
{
    if ( auto v_node = new(std::nothrow) node_type(std::forward<U>(p_value), m_head) )
    {
        m_head = v_node;
        
        m_size = m_size + 1ul;
    }
}


template <class T>
template <class U>
void stack<T>::push(U &&p_value) noexcept requires(std::convertible_to<U, T>)
{
    push_front(std::forward<U>(p_value));
}


template <class T>
template <class... ARGS>
void stack<T>::emplace_front(ARGS &&...p_args) noexcept
{
    if ( auto v_node = new node_type(std::in_place, std::forward<ARGS>(p_args)...) )
    {
        v_node->m_next = m_head;
        m_head         = v_node;
    
        m_size = m_size + 1ul;
    }
}


template <class T>
template <class... ARGS>
void stack<T>::emplace(ARGS &&...p_args) noexcept
{
    emplace_front(std::forward<ARGS>(p_args)...);
}



    /** USER DEFINED TYPE DEDUCTION **/
//  -----------------------------------------------------------------------
    template <std::ranges::range R>
    stack( R&& ) -> stack<std::ranges::range_value_t<R>>;
//  -----------------------------------------------------------------------
    template <std::input_iterator I, std::sentinel_for<I> S>
    stack( I, S ) -> stack<std::iter_value_t<I>>;
//  -----------------------------------------------------------------------


extern template class stack<int>;
extern template class stack<char>;
extern template class stack<long>;
extern template class stack<short>;
extern template class stack<unsigned int>;
extern template class stack<unsigned char>;
extern template class stack<unsigned long>;
extern template class stack<unsigned short>;
extern template class stack<float>;
extern template class stack<double>;
extern template class stack<std::string>;

#endif
