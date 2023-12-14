
#include <ds/stack.hxx>

// star stack
//  
//  
template <class T>
stack<T>::stack() noexcept(true) = default;


//  
template <class T>
stack<T>::stack(stack const &p_outer)
    noexcept(std::is_nothrow_copy_constructible<T>::value) : stack()
{
    node_type* v_curr   = p_outer.m_head;

    if ( auto v_node = new(std::nothrow) node_type{v_curr->m_data, nullptr})
    {
        node_type* v_tracer = nullptr;

        m_head   = v_node;
        v_curr   = v_curr->m_next;
        v_tracer = m_head;

        while (v_curr)
        {
            v_node = new(std::nothrow) node_type{v_curr->m_data, nullptr};

            if (v_node == nullptr) break;

            v_tracer->m_next = v_node;
            v_tracer         = v_tracer->m_next;
            v_curr           = v_curr->m_next;
        }

        m_size = p_outer.m_size;

    }
}


//  
template <class T>
stack<T>::stack(stack &&p_outer) noexcept(true) : stack()
{
    swap(*this, p_outer);
}


//  
template <class T>
stack<T> &stack<T>::operator=(stack p_rhs)
    noexcept(std::is_nothrow_copy_constructible<T>::value)
{
    swap(*this, p_rhs);

    return *this;
}


//  
template <class T>
void stack<T>::pop_front() noexcept(std::is_nothrow_destructible<T>::value)
{
    assert(not empty());

    node_type* v_curr = m_head;

    m_head = m_head->m_next;

    v_curr = (delete v_curr, nullptr);

    m_size = m_size - 1ul;
}


//  
template <class T>
void stack<T>::pop() noexcept(std::is_nothrow_destructible<T>::value)
{
    pop_front();
}


//  
template <class T>
[[nodiscard]] auto stack<T>::empty() const noexcept(true) -> bool
{
    return (m_head == nullptr);
}


//  
template <class T>
[[nodiscard]] auto stack<T>::size() const noexcept(true)
    -> typename stack::size_type
{
    return m_size;
}
//  
//  

//  
//  
template <class T>
[[nodiscard]] auto stack<T>::peep() const noexcept(true)
    -> std::optional<typename stack::value_type>
{
    return (not empty() ? std::optional{m_head->data()}
                        : std::nullopt);
}


//  
template <class T>
[[nodiscard]] auto stack<T>::peep() noexcept(true)
    -> std::optional<typename stack::value_type>
{
    return (not empty() ? std::optional{m_head->data()}
                        : std::nullopt);
}


//  
template <class T>
stack<T>::~stack() noexcept(std::is_nothrow_destructible<T>::value)
{
    while (not empty())
    {
        pop_front();
    }
}
//  
//  
// end stack



//  
//  
template class stack<int>;
template class stack<char>;
template class stack<long>;
template class stack<short>;
template class stack<unsigned int>;
template class stack<unsigned char>;
template class stack<unsigned long>;
template class stack<unsigned short>;
template class stack<float>;
template class stack<double>;
//  
//  