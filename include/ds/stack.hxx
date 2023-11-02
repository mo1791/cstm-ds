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

namespace rng = std::ranges;

/* START CONSTRAINTS */

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    concept is_class = std::is_class<typename std::decay<T>::type>::value;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

/* END CONSTRAINTS */



//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class> class stack;
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

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

public: /** TYPE ALIAS **/
    using node_ptr_t = typename std::add_pointer<node<T>>::type;



public: /** CONSTRUCTORS **/
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        /** DEFAULT CTOR **/
        node() noexcept
            requires(std::default_initializable<T>)
            : m_data{T{}}
            , m_next{nullptr}
        {}
    //  -------------------------------------------------------------------------
        /** PARAM CTOR **/
        explicit node(T const &p_value, node_ptr_t p_next) noexcept
            requires(std::copy_constructible<T>)
            : m_data{p_value}
            , m_next{p_next}
        {}
    //  -------------------------------------------------------------------------
        /** PARAM CTOR **/
        explicit node(T &&p_value, node_ptr_t p_next) noexcept
            requires(std::move_constructible<T>)
            : m_data{std::move(p_value)}
            , m_next{p_next}
        {}
    //  -------------------------------------------------------------------------
        /** **/
        template <class... ARGS>
        node(ARGS &&...p_args) noexcept
            requires(is_class<T> && std::constructible_from<T, ARGS...>)
            : m_data{std::forward<ARGS>(p_args)...}
            , m_next{nullptr}
        {}
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

public: /** GETTERS **/
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto data() noexcept -> decltype(auto) { return m_data; }
    //  --------------------------------------------------------------------------
        auto data() const noexcept -> decltype(auto) { return m_data; }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------
        auto next() noexcept -> decltype(auto) { return m_next; }
    //  --------------------------------------------------------------------------
        auto next() const noexcept -> decltype(auto) { return m_next; }
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------

private:
    value_type m_data;
    node_ptr_t m_next;

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
    //  --------------------------------------------------------------------------
    //  --------------------------------------------------------------------------


public: /** CONSTRUCTORS **/
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        /** DEFAULT CTOR **/
        stack() noexcept;
    //  -------------------------------------------------------------------------
        /** COPY CTOR **/
        stack(stack const &);
    //  -------------------------------------------------------------------------
        /** MOVE CTOR **/
        stack(stack &&) noexcept;
    //  -------------------------------------------------------------------------
        /** RANGE CTOR (Construct with the contents of the range [ begin, end ]) **/
        template <std::input_iterator I, std::sentinel_for<I> S>
        stack(I, S)
            requires(std::convertible_to<std::iter_value_t<I>, T>);
    //  -------------------------------------------------------------------------
        /** RANGE CTOR  (Construct with the contents of the range) **/
        template <rng::input_range R>
        stack(R &&)
            requires(std::convertible_to<rng::range_value_t<R>, T>);
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------


    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------    
        /** ASSIGNMENT OP ( copy-swap idiom ) **/
        stack &operator=(stack);
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------


public:
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        template <class U>
        void push_front(U &&) requires(std::convertible_to<U, T>);
    //  -------------------------------------------------------------------------
        template <class U>
        void push(U &&) requires(std::convertible_to<U, T>);
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        template <class... ARGS>
        void emplace_front(ARGS &&...)
            requires(is_class<T> && std::constructible_from<T, ARGS...>);
    //  -------------------------------------------------------------------------
        template <class... ARGS>
        void emplace(ARGS &&...)
            requires(is_class<T> && std::constructible_from<T, ARGS...>);
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        void pop_front();
    //  -------------------------------------------------------------------------
        void pop();
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        [[nodiscard]] auto peep() const noexcept -> std::optional<value_type>;
    //  -------------------------------------------------------------------------
        [[nodiscard]] auto peep()       noexcept -> std::optional<value_type>;
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

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

    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------
        ~stack();
    //  -------------------------------------------------------------------------
    //  -------------------------------------------------------------------------

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

public:
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

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    template <std::input_iterator I, std::sentinel_for<I> S>
    stack<T>::stack(I p_begin, S p_end)
    requires(std::convertible_to<std::iter_value_t<I>, T>)
        : stack()
    {
        using std::placeholders::_1;
        using std::ranges::for_each;

        for_each(p_begin, p_end,
                std::bind(&stack::template push_front<std::iter_reference_t<I>>,
                            this, _1));
    }
//  -------------------------------------------------------------------------
    template <class T>
    template <std::ranges::input_range R>
    stack<T>::stack(R &&p_container)
    requires(std::convertible_to<rng::range_value_t<R>, T>)
        : stack(rng::begin(p_container), rng::end(p_container))
    {}
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    template <class U>
    void stack<T>::push_front(U &&p_value) requires(std::convertible_to<U, T>)
    {
        node_type* v_node = nullptr;

        try {
            v_node = new node_type(std::forward<U>(p_value), m_head);
        }
        catch (...) {
            v_node = (delete v_node, nullptr);
            throw;
        }

        m_head = v_node;

        m_size = m_size + 1ul;
    }
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    template <class U>
    void stack<T>::push(U &&p_value) requires(std::convertible_to<U, T>)
    {
        push_front(std::forward<U>(p_value));
    }
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------

//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    template <class... ARGS>
    void stack<T>::emplace_front(ARGS &&...p_args)
        requires(is_class<T> && std::constructible_from<T, ARGS...>)
    {
        node_type* v_node = nullptr;

        try {
            v_node = new node_type(std::forward<ARGS>(p_args)...);
        }
        catch (...) {
            v_node = (delete v_node, nullptr);
            throw;
        }

        v_node->m_next = m_head;
        m_head         = v_node;

        m_size = m_size + 1ul;
    }
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
    template <class T>
    template <class... ARGS>
    void stack<T>::emplace(ARGS &&...p_args)
        requires(is_class<T> && std::constructible_from<T, ARGS...>)
    {
        emplace_front(std::forward<ARGS>(p_args)...);
    }
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------


//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------
    /** USER DEFINED TYPE DEDUCTION **/
//  -----------------------------------------------------------------------
    template <rng::range R>
    stack( R&& ) -> stack<rng::range_value_t<R>>;
//  -----------------------------------------------------------------------
    template <std::input_iterator I, std::sentinel_for<I> S>
    stack( I, S ) -> stack<std::iter_value_t<I>>;
//  -----------------------------------------------------------------------
//  -----------------------------------------------------------------------


//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
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
//  --------------------------------------------------------------------------
//  --------------------------------------------------------------------------
#endif
