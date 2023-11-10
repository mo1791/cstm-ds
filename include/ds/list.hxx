#ifndef LINKEDLIST_SENTINEL_NODE
#define LINKEDLIST_SENTINEL_NODE



#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <type_traits>
#include <utility>




template <class> class list;
template <class> class Iterator;
template <class> class Forward;
template <class> class Backward;


/* START CONSTRAINTS */

template <class T, class U>
concept non_self =
        not std::is_same<std::decay_t<T>, U>::value &&
        not std::is_base_of<U, std::decay_t<T>>::value;

/* END CONSTRAINTS */

//
template <class T> class node final
{

public:
    friend class list<T>;
    friend class Iterator<T>;
    friend class Forward<T>;
    friend class Backward<T>;


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
    /** PARAM CTOR **/
    explicit node(T const &p_data) noexcept
    requires(std::copy_constructible<T>)
        : m_data{p_data}
        , m_prev{this}
        , m_next{this}
    {}
//  --------------------------------------------------------------------------
    /** PARAM CTOR (rvalue ref) **/
    explicit node(T &&p_data) noexcept
    requires(std::move_constructible<T>)
        : m_data{std::move(p_data)}
        , m_prev{this}
        , m_next{this}
    {}
//  --------------------------------------------------------------------------
    /** **/
    template <class... ARGS>
    node(std::in_place_t, ARGS &&...p_args) noexcept
    requires(std::constructible_from<T, ARGS...>)
        : m_data{T{std::forward<ARGS>(p_args)...}}
        , m_prev{this}
        , m_next{this}
    {}
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

public: /** GETTERS **/
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    auto data() const noexcept -> value_type { return m_data; }
//  --------------------------------------------------------------------------
    auto data()       noexcept -> value_type { return m_data; }
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

// //
template <class T> class Iterator
{

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
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    /** DEFAULT CTOR **/
    Iterator() noexcept = default;
//  --------------------------------------------------------------------------
    /** DEFAULT COPY CTOR **/
    Iterator(Iterator const& ) noexcept = default;
//  --------------------------------------------------------------------------
    /** DEFAULT MOVE CTOR **/
    Iterator(Iterator&&) noexcept = default;
//  --------------------------------------------------------------------------
    /** PARAM CTOR **/
    Iterator(node_type* p_node) noexcept : m_node(p_node) {}
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    /** DEFAULT COPY ASSIGN **/
    Iterator& operator=(Iterator const&) noexcept = default;
//  --------------------------------------------------------------------------
    /** DEFAULT MOVE ASSIGN **/
    Iterator& operator=(Iterator&&) noexcept = default;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


public:
//  --------------------------------------------------------------------------
    /** DEREFERENCE OP **/
//  --------------------------------------------------------------------------
    auto operator*() const noexcept -> reference { return m_node->m_data; }
//  --------------------------------------------------------------------------
    auto operator*()       noexcept -> reference { return m_node->m_data; }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


//  --------------------------------------------------------------------------
    /** ARROW OP (only if T is class type ) **/
//  --------------------------------------------------------------------------
    auto operator->() const noexcept -> const_pointer requires( is_class<T> )
    {
        return std::addressof( m_node->m_data );
    }
//  --------------------------------------------------------------------------
    auto operator->()       noexcept -> pointer requires( is_class<T> )
    {
        return std::addressof( m_node->m_data );
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


//  --------------------------------------------------------------------------
    /** COMPAR OP **/
//  --------------------------------------------------------------------------
    [[nodiscard]] friend auto operator==(Iterator const &p_lhs,
                                         Iterator const &p_rhs) noexcept
        -> bool
    {
        return (p_lhs.m_node == p_rhs.m_node);
    }
//  --------------------------------------------------------------------------
    /* As of C++20, operator!= is auto generated as !(operator==) */
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

protected:
    node_type* m_node;

};

//  //

//*****//

// //
template <class T>
class Forward final : public Iterator<T>
{

protected:

    using Iterator<T>::m_node;

public:
    
    using Iterator<T>::Iterator;


public:
//  --------------------------------------------------------------------------
    /** (POST & PRE ) INCREMENT OP **/
//  --------------------------------------------------------------------------
    auto operator++() noexcept -> Forward &
    {
        m_node = m_node->m_next;

        return *this;
    }
//  --------------------------------------------------------------------------
    auto operator++(int) noexcept -> Forward
    {
        auto v_tmp = *this;
        ++(*this);

        return (v_tmp);
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


//  --------------------------------------------------------------------------
    /** (POST & PRE ) DECREMENT OP **/
//  --------------------------------------------------------------------------
    auto operator--() noexcept -> Forward &
    {
        m_node = m_node->m_prev;
        
        return *this;
    }
//  --------------------------------------------------------------------------
    auto operator--(int) noexcept -> Forward
    {
        auto v_tmp = *this;
        --(*this);

        return (v_tmp);
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
};

//  //

//*****//

// //
template <class T>
class Backward final : public Iterator<T>
{

protected:

    using Iterator<T>::m_node;

public:
    
    using Iterator<T>::Iterator;


public:
//  --------------------------------------------------------------------------
    /** (POST & PRE) INCREMENT OP **/
//  --------------------------------------------------------------------------
    auto operator++() noexcept -> Backward &
    {
        m_node = m_node->m_prev;

        return *this;
    }
//  --------------------------------------------------------------------------
    auto operator++(int) noexcept -> Backward
    {
        auto v_tmp = *this;
        ++(*this);

        return (v_tmp);
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


//  --------------------------------------------------------------------------
    /** (POST & PRE) DECREMENT OP **/
//  --------------------------------------------------------------------------
    auto operator--() noexcept -> Backward &
    {
        m_node = m_node->m_next;

        return *this;
    }
//  --------------------------------------------------------------------------
    auto operator--(int) noexcept -> Backward
    {
        auto v_tmp = *this;
        --(*this);

        return (v_tmp);
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
};

//  //

//*****//

// //
template <class T> class list final
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

    using iterator               = Forward<T>;
    using const_iterator         = Forward<typename std::add_const<T>::type>;
    using reverse_iterator       = Backward<T>;
    using const_reverse_iterator = Backward<typename std::add_const<T>::type>;
    using iterator_category      = std::bidirectional_iterator_tag;
   

public: /** CONSTRUCTORS **/
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    /** DEFAULT CTOR **/
    list() noexcept;
//  --------------------------------------------------------------------------
    /** COPY CTOR **/
    list(list const&) noexcept;
//  --------------------------------------------------------------------------
    /** MOVE CTOR **/
    list(list&&) noexcept;
//  --------------------------------------------------------------------------
    /**
    * INITIALIZER_LIST CTOR 
    * (Construct with the contents of the initializer list)
    * **/
    list(std::initializer_list<T>) noexcept;
//  --------------------------------------------------------------------------
    /**
    * RANGE CTOR
    * (Construct with the contents of the range)
    * **/
    template <std::ranges::range R>
    list(R&&) noexcept
        requires(non_self<R, list> &&
                 std::convertible_to<std::ranges::range_value_t<R>, T>);
//  --------------------------------------------------------------------------
    /**
    * RANGE CTOR
    * (Construct with the contents of the range [ begin, end ])
    * **/
    template <std::input_iterator I, std::sentinel_for<I> S>
    list(I, S) noexcept
        requires(std::convertible_to<std::iter_value_t<I>, T>);
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    /**
    * ASSIGNMENT OP
    * ( copy-swap idiom )
    * **/
    auto operator=(list) noexcept -> list &;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <class U>
    void push_front(U&&) noexcept requires(std::convertible_to<U, T>);
//  --------------------------------------------------------------------------
    template <class U>
    void push_back(U&&) noexcept requires(std::convertible_to<U, T>);
//  --------------------------------------------------------------------------
    template <class U>
    void push_before(U&&, std::integral auto) noexcept
        requires(std::convertible_to<U, T>);
//  --------------------------------------------------------------------------
    template <class U>
    void push_after(U&&, std::integral auto) noexcept
        requires(std::convertible_to<U, T>);
//  --------------------------------------------------------------------------
    template <class U>
    void push_at(U&&, std::integral auto) noexcept
        requires(std::convertible_to<U,T>);
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    template <class... ARGS>
    void emplace_back(ARGS&& ...) noexcept;
//  --------------------------------------------------------------------------
    template <class... ARGS>
    void emplace_front(ARGS&& ...) noexcept;
//  --------------------------------------------------------------------------
    template <class... ARGS>
    void emplace_before(std::integral auto, ARGS&& ...) noexcept;
//  --------------------------------------------------------------------------
    template <class... ARGS>
    void emplace_after(std::integral auto, ARGS&& ...) noexcept;
//  --------------------------------------------------------------------------
    template <class... ARGS>
    void emplace_at(std::integral auto, ARGS&& ...) noexcept;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    void pop_front() noexcept;
//  --------------------------------------------------------------------------
    void pop_back() noexcept;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    [[nodiscard]] auto front() const noexcept -> std::optional<value_type>;
//  --------------------------------------------------------------------------
    [[nodiscard]] auto front()       noexcept -> std::optional<value_type>;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    [[nodiscard]] auto back() const noexcept -> std::optional<value_type>;
//  --------------------------------------------------------------------------
    [[nodiscard]] auto back()       noexcept -> std::optional<value_type>;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    [[nodiscard]] auto empty() const noexcept -> bool;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    [[nodiscard]] auto size() const noexcept -> size_type;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


public:    
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    auto begin() const noexcept -> iterator;
//  --------------------------------------------------------------------------
    auto begin()       noexcept -> iterator;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    auto end() const noexcept -> iterator;
//  --------------------------------------------------------------------------
    auto end()       noexcept -> iterator;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    auto rbegin() const noexcept -> reverse_iterator;
//  --------------------------------------------------------------------------
    auto rbegin()       noexcept -> reverse_iterator;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    auto rend() const noexcept -> reverse_iterator;
//  --------------------------------------------------------------------------
    auto rend()       noexcept -> reverse_iterator;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    ~list() noexcept;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


public:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    friend void swap(list &p_lhs, list &p_rhs) noexcept
    {
        using std::swap;

        swap(p_lhs.m_head, p_rhs.m_head);
        swap(p_lhs.m_size, p_rhs.m_size);
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------


private:
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
    auto find_at(std::integral auto p_pos) const noexcept -> node_type*
    {
        assert(p_pos < m_size);

        node_type* v_temp = nullptr;

        size_type  v_mid = (m_size / 2ul);

        if (v_mid < p_pos)
        {
            auto v_index = (m_size - 1ul);
            v_temp       = m_head->m_prev;

            while (v_index > p_pos)
            {
                v_temp  = v_temp->m_prev;
                v_index = v_index - 1ul;
            }
        }
        else {
            auto v_index{0ul};

            v_temp = m_head->m_next;

            while (v_index < p_pos)
            {
                v_temp  = v_temp->m_next;
                v_index = v_index + 1ul;
            }
        }

        return v_temp;
    }
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

private:
    node_type* m_head;
    size_type  m_size;

};

//  //

//*****//

template <class T>
template <std::input_iterator I, std::sentinel_for<I> S>
list<T>::list(I p_first, S p_last) noexcept
requires(std::convertible_to<std::iter_value_t<I>, T>) : list{}
{
    using std::placeholders::_1;

    auto lambda = &list::template push_back<std::iter_reference_t<I>>;

    std::ranges::for_each(p_first, p_last, std::bind(lambda, this, _1));
}


template <class T>
template <std::ranges::range R>
list<T>::list(R&& p_list) noexcept
    requires(non_self<R, list> &&
             std::convertible_to<std::ranges::range_value_t<R>, T>)
    : list(std::ranges::begin(p_list), std::ranges::end(p_list))
{}


template <class T>
template <class U>
void list<T>::push_front(U&& p_data) noexcept requires(std::convertible_to<U, T>)
{
    if( not m_head ) return;

    if ( auto v_node = new(std::nothrow) node_type(std::forward<U>(p_data)) )
    {
        m_head->push_front(v_node);
        
        m_size = m_size + 1ul;
    }
}


template <class T>
template <class U>
void list<T>::push_back(U&& p_data) noexcept requires(std::convertible_to<U, T>)
{
    if ( not m_head ) return;

    if ( auto v_node = new(std::nothrow) node_type(std::forward<U>(p_data)) )
    {
        m_head->push_back(v_node);
        
        m_size = m_size + 1ul;
    }
}


template <class T>
template <class U>
void list<T>::push_before(U&& p_data, std::integral auto p_pos) noexcept
    requires(std::convertible_to<U, T>)
{
    if ( not m_head ) return;

    if ( auto v_node = new(std::nothrow) node_type(std::forward<U>(p_data)) )
    {
        find_at(p_pos)->push_back(v_node);
        
        m_size = m_size + 1ul;
    }
}


template <class T>
template <class U>
void list<T>::push_after(U&& p_data, std::integral auto p_pos) noexcept
    requires(std::convertible_to<U, T>)
{
    if ( not m_head ) return;

    if ( auto v_node = new(std::nothrow) node_type(std::forward<U>(p_data)) )
    {
        find_at(p_pos)->push_front(v_node);
        
        m_size = m_size + 1ul;
    }
}


template <class T>
template <class U>
void list<T>::push_at(U&& p_data, std::integral auto p_pos) noexcept
    requires(std::convertible_to<U,T>)
{
    if ( not m_head ) return;
}



template <class T>
template <class... ARGS>
void list<T>::emplace_front(ARGS&& ...p_args) noexcept
{
    if ( not m_head ) return;

    if ( auto v_node = new node_type(std::in_place,std::forward<ARGS>(p_args)...) )
    {
        m_head->push_front(v_node);
        
        m_size = m_size + 1ul;
    }
}

template <class T>
template <class... ARGS>
void list<T>::emplace_back(ARGS&& ...p_args) noexcept
{
    if ( not m_head ) return;

    if ( auto v_node = new node_type(std::in_place, std::forward<ARGS>(p_args)...) )
    {
        m_head->push_back(v_node);
        
        m_size = m_size + 1ul;
    }
}

template <class T>
template <class... ARGS>
void list<T>::emplace_before(std::integral auto p_pos, ARGS&& ...p_args) noexcept
{
    if ( not m_head ) return;

    if ( auto v_node = new node_type(std::in_place, std::forward<ARGS>(p_args)...) )
    {
        find_at(p_pos)->push_back(v_node);
        
        m_size = m_size + 1ul;
    }
}

template <class T>
template <class... ARGS>
void list<T>::emplace_after(std::integral auto p_pos, ARGS&& ...p_args) noexcept
{
    if ( not m_head ) return;

    if ( auto v_node = new node_type(std::in_place, std::forward<ARGS>(p_args)...) )
    {
        find_at(p_pos)->push_front(v_node);
        
        m_size = m_size + 1ul;
    }
}


template <class T>
template <class... ARGS>
void list<T>::emplace_at(std::integral auto p_pos, ARGS&& ...p_args) noexcept
{
    if ( not m_head ) return;
}




    /** USER DEFINED TYPE DEDUCTION **/
//  -----------------------------------------------------------------------
    template <std::ranges::range R>
    list( R&& ) -> list<std::ranges::range_value_t<R>>;
//  -----------------------------------------------------------------------
    template <std::input_iterator I, std::sentinel_for<I> S>
    list( I, S ) -> list<std::iter_value_t<I>>;
//  -----------------------------------------------------------------------



//  --------------------------------------------------------------------------
    /**
    * This specialization of std::ranges::enable_borrowed_range 
    * makes list satisfy borrowed_range 
    * **/
//  --------------------------------------------------------------------------
    template <class T>
    inline constexpr bool std::ranges::enable_borrowed_range<list<T>> = true;
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------

extern template class list<int>;
extern template class list<char>;
extern template class list<long>;
extern template class list<short>;
extern template class list<unsigned int>;
extern template class list<unsigned char>;
extern template class list<unsigned long>;
extern template class list<unsigned short>;
extern template class list<float>;
extern template class list<double>;
extern template class list<std::string>;


#endif
// // end linked_list
