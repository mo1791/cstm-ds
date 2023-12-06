
#include <ds/list.hxx>


//  start linked_list

template <class T>
list<T>::list() noexcept(std::is_nothrow_default_constructible<T>::value)
    : m_head(new(std::nothrow) node_type{})
    , m_size( 0ul )
{}

 
//  
template <class T>
list<T>::list(std::initializer_list<T> p_list)
        noexcept(std::is_nothrow_copy_constructible<T>::value)
    : list(std::ranges::begin(p_list), std::ranges::end(p_list))
{}


//  
template <class T>
list<T>::list(list const& p_outer) noexcept(std::is_nothrow_copy_constructible<T>::value)
    : list(std::ranges::begin(p_outer), std::ranges::end(p_outer))
{}

 
//  
template <class T>
list<T>::list(list&& p_outer) noexcept(true) : list{}
{
    swap(*this, p_outer);
}

 
//  
template <class T>
auto list<T>::operator=(list p_list)
    noexcept(std::is_nothrow_copy_constructible<T>::value) -> list &
{
    swap(*this, p_list);

    return *this;
}


//  
template <class T>
void list<T>::pop_front() noexcept(std::is_nothrow_destructible<T>::value)
{
    assert(not empty());

    node_type* v_curr = m_head->m_next;

    v_curr->m_next->m_prev = m_head;
    m_head->m_next         = v_curr->m_next;
    
    v_curr = (delete v_curr, nullptr);

    m_size = m_size - 1ul;
}

 
//  
template <class T>
void list<T>::pop_back() noexcept(std::is_notrow_destructible<T>::value)
{
    assert(not empty());

    node_type* v_curr = m_head->m_prev;

    v_curr->m_prev->m_next = m_head;
    m_head->m_prev         = v_curr->m_prev;
    
    v_curr = (delete v_curr, nullptr);

    m_size = m_size - 1ul;
}


//  
template <class T>
[[nodiscard]] auto list<T>::empty() const noexcept(true) -> bool
{
    return ( (m_head == m_head->m_next) && (m_head == m_head->m_prev));
}

 
//  
template <class T>
[[nodiscard]] auto list<T>::size() const noexcept(true) -> typename list::size_type
{
    return m_size;
}


//  
template <class T>
[[nodiscard]] auto list<T>::front() const noexcept(true)
    -> std::optional<typename list::value_type>
{
    return not empty()  ? std::optional{m_head->m_next->data()}
                        : std::nullopt;
}

 
//  
template <class T>
[[nodiscard]] auto list<T>::back() const noexcept(true)
    -> std::optional<typename list::value_type>
{
    return not empty()  ? std::optional{m_head->m_prev->data()}
                        : std::nullopt;
}

 
//  
template <class T>
auto list<T>::begin() noexcept(true) -> typename list::iterator
{
    return iterator{m_head->m_next};
}

 
//  
template <class T>
auto list<T>::begin() const noexcept(true) -> typename list::iterator
{
    return iterator{m_head->m_next};
}

 
//  
template <class T>
auto list<T>::end() noexcept(true) -> typename list::iterator
{
    return iterator{m_head};
}

 
//  
template <class T>
auto list<T>::end() const noexcept(true) -> typename list::iterator
{
    return iterator{m_head};
}

 
//  
template <class T>
auto list<T>::rbegin() noexcept(true) -> typename list::reverse_iterator
{
    return reverse_iterator{m_head->m_prev};
}

 
//  
template <class T>
auto list<T>::rbegin() const noexcept(true) -> typename list::reverse_iterator
{
    return reverse_iterator{m_head->m_prev};
}

  
//  
template <class T>
auto list<T>::rend() noexcept(true) -> typename list::reverse_iterator
{
    return reverse_iterator{m_head};
}

 
//  
template <class T>
auto list<T>::rend() const noexcept(true) -> typename list::reverse_iterator
{
    return reverse_iterator{m_head};
}

 
//  
template <class T>
list<T>::~list() noexcept(std::is_notrow_destructible<T>::value)
{
    while (not empty()) pop_front();

    m_head = (delete m_head, nullptr);
}
//  
//  

//  
//  
template class list<int>;
template class list<char>;
template class list<long>;
template class list<short>;
template class list<unsigned int>;
template class list<unsigned char>;
template class list<unsigned long>;
template class list<unsigned short>;
template class list<float>;
template class list<double>;
//  
//  