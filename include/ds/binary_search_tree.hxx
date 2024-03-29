#ifndef BINARY_SEARCH_TREE_NODE
#define BINARY_SEARCH_TREE_NODE

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>

/** FROWARD DECL **/
template <std::totally_ordered> class binary_search_tree;

/** START NODE **/
template <class T>  class node final
{

public:
    friend struct binary_search_tree<T>;

public: /** TYPE ALIAS **/
    using value_type = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using pointer         = typename std::add_pointer<T>::type;
    using const_pointer   = typename std::add_pointer<typename std::add_const<T>::type>::type;


public: /** CONSTRUCTORS **/
    
    /** DEFAULT CTOR **/
    node()  noexcept(std::is_nothrow_default_constructible<T>::value)
            requires(std::is_default_constructible<T>::value)
        : m_data(T{})
    {
    }
    
    /** PARAM CTOR **/
    explicit node(T const& data)
            noexcept( std::is_nothrow_copy_constructible<T>::value)
            requires(std::is_copy_constructible<T>::value)
        : m_data(data)
    {
    }
    
    /** PARAM CTOR ( rvalue ref ) **/
    explicit node(T&& data)
            noexcept( std::is_nothrow_move_constructible<T>::value)
            requires(std::is_move_constructible<T>::value)
        : m_data(std::move(data))
    {
    }
    
    /** **/
    template <class... ARGS>
    node(std::in_place_t, ARGS&&... p_args)
            noexcept( std::is_nothrow_constructible<T, ARGS...>::value)
            requires(std::is_constructible<T, ARGS...>::value)
        : m_data(std::forward<ARGS>(p_args)...)
    {
    }
    
    

private:
    value_type m_data;
    node* m_left   {};
    node* m_right  {};
    node* m_parent {};
};

/** **/

/** **/

template <std::totally_ordered T> class binary_search_tree
{

public: /** TYPE ALIAS **/
    using node_type = node<T>;

public: /** TYPE ALIAS **/
    using value_type = typename node_type::value_type;
    using reference  = typename node_type::reference;
    using const_reference = typename node_type::const_reference;
    using pointer         = typename node_type::pointer;
    using const_pointer   = typename node_type::const_pointer;
    using size_type       = std::size_t;


public: /** CONSTRUCTORS **/
        
    /** DEFAULT CTOR **/
    binary_search_tree() noexcept(true);
    
    /**
     * INITIALIZER_LIST CTOR
     * (Construct with the contents of the initializer list)
     * **/
    binary_search_tree(std::initializer_list<T> /* list */) noexcept(
        std::is_nothrow_copy_constructible<T>::value);
    
    /**
     * RANGE TYPE CTOR
     * (Construct with the contents of the range)
     * **/
    template <std::ranges::range R>
    binary_search_tree(R&& /* range */)
        noexcept(std::is_nothrow_constructible<T, std::ranges::range_value_t<R>>::value)
        requires(std::is_constructible<T, std::ranges::range_value_t<R>>::value);
    
    /**
     * RANGE CTOR
     * (Construct with the contents of the range [ begin, end ])
     * **/
    template <std::input_iterator I, std::sentinel_for<I> S>
    binary_search_tree(I /* begin */, S /* end */)
        noexcept(std::is_nothrow_constructible<T, std::iter_value_t<I>>::value)
        requires(std::is_constructible<T, std::iter_value_t<I>>::value);
    
    /** COPY CTOR  **/
    binary_search_tree(binary_search_tree const& /* outer */) noexcept(
        std::is_nothrow_copy_constructible<T>::value);
    
    /** MOVE CTOR **/
    binary_search_tree(binary_search_tree&& /* outer */) noexcept(true);
    
    /** ASSIGNMENT **/
    binary_search_tree& operator=(binary_search_tree /* rhs */) noexcept(
        std::is_copy_constructible<T>::value);
    
    

public: /** MEMBER FUNCTION **/
    
    /** INSERTING A NODE IN TREE **/
    template <class U>
    void insert(U&& /* data */)
        noexcept(std::is_nothrow_constructible<T, U>::value)
        requires(std::is_constructible<T, U>::value);
    
    /** INSERTING A NODE IN TREE ( construct in place ) **/
    template <class... Args>
    void emplace(Args&&... /* args */) noexcept(
        std::is_nothrow_constructible<T, Args...>::value);
    
    
public:
    /** SEARCH FOR A NODE IN TREE **/
    auto search(T const& /* key */) const
        noexcept(true) -> std::optional<value_type>;
    

public:
    
    /** DELETE A NODE FROM TREE **/
    void remove(T const& /* key */) noexcept(
        std::is_nothrow_destructible<T>::value);
    
    /** MAKE TREE EMPTY **/
    void clear() noexcept(std::is_nothrow_destructible<T>::value);
    
    

public:
    
    /** PRINT TREE NODES IN INORDER **/
    void print_inorder() const noexcept(true);
    
    /** PRINT TREE NODES IN PRE_ORDER **/
    void print_preorder() const noexcept(true);
    
    /** PRINT TREE NODES IN POST_ORDER **/
    void print_postorder() const noexcept(true);
    
    

public:
    
    [[nodiscard]] constexpr auto empty() const noexcept(true) -> bool;
    
    [[nodiscard]] constexpr auto size() const noexcept(true) -> size_type;
    
    [[nodiscard]] auto max() const noexcept(true) -> std::optional<value_type>;
    
    [[nodiscard]] auto min() const noexcept(true) -> std::optional<value_type>;
    
    

public:

    ~binary_search_tree() noexcept(std::is_nothrow_destructible<T>::value);
    
    
public:

    void swap(binary_search_tree& p_lhs,
              binary_search_tree& p_rhs) noexcept(true) {
        using std::swap;

        swap(p_lhs.m_root, p_rhs.m_root);
        swap(p_lhs.m_size, p_rhs.m_size);
    }
    

private:
    
    [[nodiscard]] auto clone(node_type* p_root) const
        noexcept(std::is_nothrow_copy_constructible<T>::value) -> node_type*
    {
        if (p_root != nullptr)
        {
            auto v_current = new (std::nothrow) node_type(p_root->m_data);

            if (not v_current) return nullptr;

            auto v_copy = v_current;

            while (p_root != nullptr)
            {
                if ((p_root->m_left != nullptr) && (not v_copy->m_left))
                {
                    auto v_node =
                        new (std::nothrow) node_type(p_root->m_left->m_data);

                    if (not v_node) break;

                    v_copy->m_left           = v_node;
                    v_copy->m_left->m_parent = v_copy;

                    p_root = p_root->m_left;
                    v_copy = v_copy->m_left;
                }
                else if ((p_root->m_right != nullptr) &&
                           (not v_copy->m_right))
                {
                    auto v_node =
                        new (std::nothrow) node_type(p_root->m_right->m_data);

                    if (not v_node) break;

                    v_copy->m_right           = v_node;
                    v_copy->m_right->m_parent = v_copy;

                    p_root = p_root->m_right;
                    v_copy = v_copy->m_right;
                }
                else {
                    p_root = p_root->m_parent;
                    v_copy = v_copy->m_parent;
                }
            }
            return v_current;
        }
        return p_root;
    }
    
    

private:
    node_type* m_root {};
    size_type  m_size {};
};

/** END TREE **/

// //

template <std::totally_ordered T>
template <std::ranges::range R>
binary_search_tree<T>::binary_search_tree(R&& p_range)
        noexcept(std::is_nothrow_constructible<T, std::ranges::range_value_t<R>>::value)
        requires(std::is_constructible<T, std::ranges::range_value_t<R>>::value)
    : binary_search_tree(std::ranges::begin(p_range),
                         std::ranges::end(p_range))
{
}

template <std::totally_ordered T>
template <std::input_iterator I, std::sentinel_for<I> S>
binary_search_tree<T>::binary_search_tree(I p_begin, S p_end)
        noexcept(std::is_nothrow_constructible<T, std::iter_value_t<I>>::value)
        requires(std::is_constructible<T, std::iter_value_t<I>>::value)
    : binary_search_tree()
{
    using std::placeholders::_1;

    auto lambda =
        &binary_search_tree::template insert<std::iter_reference_t<I>>;

    std::ranges::for_each(p_begin, p_end, std::bind(lambda, this, _1));
}


template <std::totally_ordered T>
template <class U>
void binary_search_tree<T>::insert(U&& p_data)
    noexcept(std::is_nothrow_constructible<T, U>::value)
    requires(std::is_constructible<T, U>::value)
{
    if (auto v_node = new (std::nothrow) node_type(std::forward<U>(p_data)))
    {
        if (not m_root)
        {
            m_root = v_node;
            m_size = m_size + 1ul;
            return;
        }

        node_type *v_current = m_root, *v_parent = nullptr;

        while (v_current != nullptr)
        {
            v_parent = v_current;

            if (p_data <= v_current->m_data)
            {
                v_current = v_current->m_left;
            }
            else {
                v_current = v_current->m_right;
            }
        }

        v_node->m_parent = v_parent;

        if (not v_parent)
        {
            v_parent = v_node;
            m_size = m_size + 1ul;
            return;
        }

        if (p_data <= v_parent->m_data)
        {
            v_parent->m_left = v_node;
        }
        else {
            v_parent->m_right = v_node;
        }

        m_size = m_size + 1ul;
    }
}

template <std::totally_ordered T>
template <class... Args>
void binary_search_tree<T>::emplace(Args&&... p_args)
    noexcept( std::is_nothrow_constructible<T, Args...>::value)
{
    if (auto v_node = new (std::nothrow)
            node_type(std::in_place, std::forward<Args>(p_args)...))
    {
        if (not m_root)
        {
            m_root = v_node;
            m_size = m_size + 1ul;
            return;
        }

        node_type *v_current = m_root, *v_parent = nullptr;

        while (v_current != nullptr)
        {
            v_parent = v_current;

            if (v_node->m_data <= v_current->m_data)
            {
                v_current = v_current->m_left;
            }
            else {
                v_current = v_current->m_right;
            }
        }

        v_node->m_parent = v_parent;

        if (not v_parent)
        {
            v_parent = v_node;
            m_size = m_size + 1ul;
            return;
        }

        if (v_node->m_data <= v_parent->m_data)
        {
            v_parent->m_left = v_node;
        }
        else {
            v_parent->m_right = v_node;
        }

        m_size = m_size + 1ul;
    }
}

/** USER DEFINED TYPE DEDUCTION **/
template <std::ranges::range R>
binary_search_tree(R) -> binary_search_tree<std::ranges::range_value_t<R>>;

template <std::input_iterator I, std::sentinel_for<I> S>
binary_search_tree(I, S) -> binary_search_tree<std::iter_value_t<I>>;


extern template class binary_search_tree<int>;
extern template class binary_search_tree<char>;
extern template class binary_search_tree<long>;
extern template class binary_search_tree<short>;
extern template class binary_search_tree<unsigned int>;
extern template class binary_search_tree<unsigned char>;
extern template class binary_search_tree<unsigned long>;
extern template class binary_search_tree<unsigned short>;
extern template class binary_search_tree<float>;
extern template class binary_search_tree<double>;
extern template class binary_search_tree<std::string>;

#endif