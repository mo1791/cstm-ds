  
//  --------------------------------------------
//  --------------------------------------------
        #include <ds/binary_search_tree.hxx>
//  --------------------------------------------
//  --------------------------------------------


// start Tree

// start binary_search_tree



//
template <std::totally_ordered T>
binary_search_tree<T>::binary_search_tree() noexcept(true) = default;


//
template <std::totally_ordered T>
binary_search_tree<T>::binary_search_tree( std::initializer_list<T> p_list)
        noexcept(std::is_nothrow_copy_constructible<T>::value)
    : binary_search_tree(std::ranges::begin(p_list), std::ranges::end(p_list)) {
}


//
template <std::totally_ordered T>
binary_search_tree<T>::binary_search_tree(binary_search_tree const& p_outer)
        noexcept(std::is_nothrow_copy_constructible<T>::value)
    : binary_search_tree()
{
    m_root = clone(p_outer.m_root);
    m_size = p_outer.m_size;
}

//
template <std::totally_ordered T>
binary_search_tree<T>::binary_search_tree(
    binary_search_tree &&p_outer) noexcept(true)
    : binary_search_tree()
{
    swap(*this, p_outer);
}

//
template <std::totally_ordered T>
auto binary_search_tree<T>::operator=(binary_search_tree p_rhs) noexcept(
    std::is_nothrow_copy_constructible<T>::value) -> binary_search_tree &
{
    swap(*this, p_rhs);

    return *this;
}


//
template <std::totally_ordered T>
auto binary_search_tree<T>::search(T const &p_key) const noexcept(true)
    -> std::optional<typename binary_search_tree::value_type>
{
    if (auto v_current = m_root)
    {
        while (v_current != nullptr && (p_key != v_current->m_data))
        {
            if (p_key == v_current->m_data)
            {
                return v_current->m_data;
            }
            else if (p_key < v_current->m_data) {
                v_current = v_current->m_left;
            }
            else {
                v_current = v_current->m_right;
            }
        }

        return std::nullopt;
    }

    return std::nullopt;
}


//
template <std::totally_ordered T>
void binary_search_tree<T>::remove(T const &p_key) noexcept(std::is_nothrow_destructible<T>::value)
{
    if (auto v_current = m_root)
    {
        while (v_current != nullptr)
        {
            if (p_key < v_current->m_data)
            {
                v_current = v_current->m_left;
            } 
            else if (p_key > v_current->m_data) {
                v_current = v_current->m_right;
            }
            else break;
        }

        if (not v_current)
        {
            return;
        }

        node_type* v_parent = v_current->m_parent;

        if ((not v_current->m_left) && (not v_current->m_right))
        {
            if (v_parent->m_left == v_current)
            {
                v_parent->m_left = nullptr;
            }
            else {
                v_parent->m_right = nullptr;
            }

            v_current = (delete v_current, nullptr);

            m_size = m_size - 1ul;

            return;
        }

        if ((not v_current->m_left) || (not v_current->m_right))
        {
            if (v_current->m_left != nullptr)
            {
                v_parent->m_left            = v_current->m_left;
                v_current->m_left->m_parent = v_parent;
            }
            else {
                v_parent->m_right           = v_current->m_right;
                v_parent->m_right->m_parent = v_parent;
            }

            v_current = (delete v_current, nullptr);

            m_size = m_size - 1ul;

            return;
        }

        if ((v_current->m_left != nullptr) && (v_current->m_right != nullptr))
        {
            node_type* v_temp = v_current->m_right;

            v_parent = nullptr;

            while (v_temp->m_left != nullptr)
            {
                v_parent = v_temp;
                v_temp = v_temp->m_left;
            }

            if (v_parent != nullptr)
            {
                v_parent->m_left = v_temp->m_right;
            }
            else {
                v_current->m_right = v_temp->m_right;

                if (v_temp->m_right)
                {
                    v_temp->m_right->m_parent = v_current;
                }
                else {
                    v_temp->m_parent = v_current;
                }
            }

            v_current->m_data = v_temp->m_data;

            v_temp = (delete v_temp, nullptr);

            m_size = m_size - 1ul;

            return;
        }
    }
}


//
template <std::totally_ordered T>
void binary_search_tree<T>::clear() noexcept(std::is_nothrow_destructible<T>::value)
{
    node_type *v_current = m_root, *v_temp = nullptr;

    while (v_current != nullptr)
    {
        if (not v_current->m_left)
        {
            v_temp = v_current->m_right;
            
            v_current = (delete v_current, nullptr);
        }
        else {
            v_temp            = v_current->m_left;
            v_current->m_left = v_temp->m_right;
            v_temp->m_right   = v_current;
        }

        v_current = v_temp;
    }

    m_root = nullptr;
}


//
template <std::totally_ordered T>
void binary_search_tree<T>::print_inorder() const noexcept(true)
{
    if (not m_root) return;

    node_type *v_current = m_root, *v_temp = nullptr;

    // iterating tree nodes
    while (v_current != nullptr)
    {
        if (not v_current->m_left)
        {
            // Print node value
            std::cout << v_current->m_data << ' ';
            // When left child are empty then
            // visit to right child
            v_current = v_current->m_right;
        }
        else {
            v_temp = v_current->m_left;
            // Find rightmost node which is
            // equal to current node
            while (v_temp->m_right && (v_temp->m_right != v_current))
            {
                // Visit to right subtree
                v_temp = v_temp->m_right;
            }

            if (v_temp->m_right != nullptr)
            {
                // Print node value
                std::cout << v_current->m_data << ' ';
                // Unlink
                v_temp->m_right = nullptr;
                // Visit to right child
                v_current = v_current->m_right;
            }
            else {
                // Change link
                v_temp->m_right = v_current;
                // Visit to right child
                v_current = v_current->m_left;
            }
        }
    }

    std::cout << std::endl;
}


//
template <std::totally_ordered T>
void binary_search_tree<T>::print_preorder() const noexcept(true)
{
    if (not m_root) return;

    node_type *v_current = m_root, *v_temp = nullptr;

    // iterating tree nodes
    while (v_current != nullptr)
    {
        if (not v_current->m_left)
        {
            // Print node value
            std::cout << v_current->m_data << ' ';
            // Visit to right childs
            v_current = v_current->m_right;
        }
        else {
            v_temp = v_current->m_left;
            // Find rightmost node which is not
            // equal to current node
            while (v_temp->m_right && (v_temp->m_right != v_current))
            {
                // Visit to right subtree
                v_temp = v_temp->m_right;
            }

            if (v_temp->m_right != v_current)
            {
                // Print node value
                std::cout << v_current->m_data << ' ';
                // Connect rightmost right node to current node
                v_temp->m_right = v_current;
                // Visit to left childs
                v_current = v_current->m_left;
            }
            else {
                // unlink
                v_temp->m_right = nullptr;
                // Visit to right child
                v_current = v_current->m_right;
            }
        }
    }

    std::cout << std::endl;
}


//
template <std::totally_ordered T>
void binary_search_tree<T>::print_postorder() const noexcept(true)
{
    if (not m_root) return;

    // Create a dummy node
    node_type *v_dummy = new(std::nothrow) node_type();

    if ( v_dummy == nullptr ) return;

    v_dummy->m_left = m_root;
    
    node_type *v_current = v_dummy;

    // Define some useful variables
    node_type *v_parent = nullptr;
    node_type *v_middle = nullptr;
    node_type *v_temp   = nullptr;
    node_type *v_back   = nullptr;

    // iterating tree nodes
    while (v_current != nullptr)
    {
        if (v_current->m_left == nullptr)
        {
            // When left child are empty then
            // Visit to right child
            v_current = v_current->m_right;
        }
        else {
            // Get to left child
            v_temp = v_current->m_left;

            while (v_temp->m_right != nullptr && v_temp->m_right != v_current)
            {
                v_temp = v_temp->m_right;
            }

            if (v_temp->m_right != v_current)
            {
                v_temp->m_right = v_current;
                v_current       = v_current->m_left;
            }
            else {
                v_parent = v_current;
                v_middle = v_current->m_left;

                // Update new path
                while (v_middle != v_current)
                {
                    v_back            = v_middle->m_right;
                    v_middle->m_right = v_parent;
                    v_parent          = v_middle;
                    v_middle          = v_back;
                }

                v_parent = v_current;
                v_middle = v_temp;

                // Print the resultant nodes.
                // And correct node link in current path
                while (v_middle != v_current)
                {
                    std::cout << v_middle->m_data << ' ';

                    v_back            = v_middle->m_right;
                    v_middle->m_right = v_parent;
                    v_parent          = v_middle;
                    v_middle          = v_back;
                }

                // Unlink previous bind element
                v_temp->m_right = nullptr;
                // Visit to right child
                v_current = v_current->m_right;
            }
        }
    }

    std::cout << std::endl;
}


//
template <std::totally_ordered T>
[[nodiscard]] constexpr auto binary_search_tree<T>::size() const noexcept(true)
    -> typename binary_search_tree::size_type
{
    return m_size;
}


//
template <std::totally_ordered T>
[[nodiscard]] constexpr auto binary_search_tree<T>::empty() const noexcept(true) -> bool
{
    return (not m_root);
}


//
template <std::totally_ordered T>
[[nodiscard]] auto binary_search_tree<T>::max() const noexcept(true)
    -> std::optional<typename binary_search_tree::value_type>
{
    if (auto v_current = m_root)
    {
        while (v_current->m_right != nullptr)
            v_current = v_current->m_right;

        return v_current->m_data;
    }

    return std::nullopt;
}


//
template <std::totally_ordered T>
[[nodiscard]] auto binary_search_tree<T>::min() const noexcept(true)
    -> std::optional<typename binary_search_tree::value_type>
{
    if (auto v_current = m_root)
    {
        while (v_current->m_left != nullptr)
            v_current = v_current->m_left;

        return v_current->m_data;
    }

    return std::nullopt;
}


//
template <std::totally_ordered T>
binary_search_tree<T>::~binary_search_tree() noexcept(std::is_nothrow_destructible<T>::value)
{
    clear();
}



//
template class binary_search_tree<int>;
template class binary_search_tree<char>;
template class binary_search_tree<long>;
template class binary_search_tree<short>;
template class binary_search_tree<unsigned int>;
template class binary_search_tree<unsigned char>;
template class binary_search_tree<unsigned long>;
template class binary_search_tree<unsigned short>;
template class binary_search_tree<float>;
template class binary_search_tree<double>;
  