#ifndef QUEUE_S_HXX
#define QUEUE_S_HXX



#include <algorithm>
#include <cassert>
#include <exception>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>



template <class> class queue;


/* START CONSTRAINTS */

template <class T> concept is_class = std::is_class<T>::value;

/* END CONSTRAINTS */



//
// START NODE
//
template <class T> class node final
{

public:
    friend class queue<T>;

public: /** TYPE ALIAS **/
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;



public: /** CONSTRUCTORS **/
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    /** DEFAULT CTOR **/
    node() noexcept requires(std::default_initializable<T>)
        : m_data{T{}}
        , m_prev{this}
        , m_next{this}
    {}
//  --------------------------------------------------------------------------
    /** PARAM CTOR (copy) **/
    explicit node(T const& p_data) noexcept
    requires(std::copy_constructible<T>)
        : m_data{p_data}
        , m_prev{this}
        , m_next{this}
    {}
//  --------------------------------------------------------------------------
    /** PARAM CTOR (move) **/
    explicit node(T&& p_data) noexcept requires(std::move_constructible<T>)
        : m_data{std::move(p_data)}
        , m_prev{this}
        , m_next{this}
    {}
//  --------------------------------------------------------------------------
    /** **/
    template <class... ARGS>
    node(ARGS&&... p_args) noexcept
    requires(is_class<T> && std::constructible_from<T, ARGS...>)
        : m_data{T{std::forward<ARGS>(p_args)...}}
        , m_prev{this}
        , m_next{this}
    {}
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

public: /** GETTERS **/
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    auto data() const noexcept -> decltype(auto) { return m_data; }
//  --------------------------------------------------------------------------
    auto data()       noexcept -> decltype(auto) { return m_data; }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

private: /** HELPERS **/
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    void push_front(node* p_node) noexcept
    {
        p_node->m_next = m_next;
        p_node->m_prev = this;
        m_next->m_prev = p_node;
        m_next = p_node;
    }
//  --------------------------------------------------------------------------
    void push_back(node* p_node) noexcept
    {
        p_node->m_prev = m_prev;
        p_node->m_next = this;
        m_prev->m_next = p_node;
        m_prev = p_node;
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

private:
    value_type m_data;
    node*      m_prev;
    node*      m_next;
};

//  //

//* END NODE *//


// /*    */ //

// /** **/ //
template <class T> class queue final
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
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** DEFAULT CTOR **/
    queue() noexcept;
//  -----------------------------------------------------------------------
    /** COPY CTOR **/
    queue(queue const&) noexcept;
//  -----------------------------------------------------------------------
    /** MOVE CTOR **/
    queue(queue&&) noexcept;
//  -----------------------------------------------------------------------
    /**
    * RANGE CTOR
    * (Construct with the contents of the range [ begin, end ])
    * **/
    template <std::input_iterator I, std::sentinel_for<I> S>
    queue(I, S) noexcept
        requires(std::convertible_to<std::iter_value_t<I>, T>);
//  -----------------------------------------------------------------------
    /**
    * RANGE CTOR
    * (Construct with the contents of the range)
    * **/
    template <std::ranges::range R>
    queue(R&&) noexcept
        requires(std::convertible_to<std::ranges::range_value_t<R>, T>);
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------



//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /**
    * ASSIGNMENT OP
    * ( copy-swap idiom )
    * **/
    auto operator=(queue) noexcept -> queue &;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


public: /** **/
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class U>
    void push_back(U&&) noexcept requires(std::convertible_to<U, T>);
//  -----------------------------------------------------------------------
    template <class U>
    void push(U&&) noexcept requires(std::convertible_to<U, T>);
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    template <class... ARGS>
    void emplace_back(ARGS&&...) noexcept
        requires(is_class<T> && std::constructible_from<T, ARGS...>);
//  -----------------------------------------------------------------------
    template <class... ARGS>
    void emplace(ARGS&&...) noexcept
        requires(is_class<T> && std::constructible_from<T, ARGS...>);
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


public:
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    void pop_front() noexcept;
//  -----------------------------------------------------------------------
    void pop() noexcept;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


public:
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto peek() const noexcept -> std::optional<value_type>;
//  -----------------------------------------------------------------------
    [[nodiscard]] auto peek()       noexcept -> std::optional<value_type>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto front() const noexcept -> std::optional<value_type>;
//  -----------------------------------------------------------------------
    [[nodiscard]] auto front()       noexcept -> std::optional<value_type>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto back() const noexcept -> std::optional<value_type>;
//  -----------------------------------------------------------------------
    [[nodiscard]] auto back()       noexcept -> std::optional<value_type>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


public:
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto empty() const noexcept -> bool;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    [[nodiscard]] auto size() const noexcept -> size_type;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


public:
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    ~queue() noexcept;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


public:
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    friend void swap(queue& p_lhs, queue& p_rhs) noexcept
    {
        using std::swap;

        swap(p_lhs.m_head, p_rhs.m_head);
        swap(p_lhs.m_size, p_rhs.m_size);
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    void debug() noexcept
    {
        node_type* v_node = m_head->m_next;

        while (v_node != m_head)
        {
            std::cout << v_node->m_data << ' ';
            v_node = v_node->m_next;
        }
        std::cout << std::endl;
    }
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------

private:
    node_type* m_head;
    size_type  m_size;
};


/** **/
//  //
/** **/

template <class T>
template <std::input_iterator I, std::sentinel_for<I> S>
queue<T>::queue(I p_begin, S p_end) noexcept
    requires(std::convertible_to<std::iter_value_t<I>, T>)
    : queue()
{
    using std::placeholders::_1;

    auto lambda = &queue::template push_back<std::iter_reference_t<I>>;

    std::ranges::for_each(p_begin, p_end, std::bind(lambda, this, _1));
}


template <class T>
template <std::ranges::range R>
queue<T>::queue(R&& p_container) noexcept
    requires(std::convertible_to<std::ranges::range_value_t<R>, T>)
    : queue(std::ranges::begin(p_container), std::ranges::end(p_container))
{}


template <class T>
template <class U>
void queue<T>::push_back(U&& p_value) noexcept requires(std::convertible_to<U,T>)
{
    if (not m_head) return;

    if ( auto v_node = new(std::nothrow) node_type(std::forward<U>(p_value)) )
    {
        m_head->push_back(v_node);

        m_size = m_size + 1ul;
    }
}


template <class T>
template <class U>
void queue<T>::push(U&& p_value) noexcept requires(std::convertible_to<U, T>)
{
    push_back(std::forward<U>(p_value));
}


template <class T>
template <class... ARGS>
void queue<T>::emplace_back(ARGS&&... p_args) noexcept
    requires(is_class<T> && std::constructible_from<T, ARGS...>)
{
    if (not m_head) return;
    
    if ( auto v_node = new node_type(std::forward<ARGS>(p_args)...) )
    {
        m_head->push_back(v_node);

        m_size = m_size + 1ul;
    }
}


template <class T>
template <class... ARGS>
void queue<T>::emplace(ARGS&&... p_args) noexcept
    requires(is_class<T> && std::constructible_from<T, ARGS...>)
{
    emplace_front(std::forward<ARGS>(p_args)...);
}



    /** USER DEFINED TYPE DEDUCTION **/
//  -----------------------------------------------------------------------
    template <std::ranges::range R>
    queue( R&& ) -> queue<std::ranges::range_value_t<R>>;
//  -----------------------------------------------------------------------
    template <std::input_iterator I, std::sentinel_for<I> S>
    queue( I, S ) -> queue<std::iter_value_t<I>>;
//  -----------------------------------------------------------------------


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

#endif
