//  --------------------------------------
//  --------------------------------------
        #include <ds/forward_list.hxx>
//  --------------------------------------
//  --------------------------------------



//  //
/*** start forward_list ***/
// //
//  
//  
template <class T>
forward_list<T>::forward_list() noexcept
    : m_head{ nullptr }
    , m_size{ 0ul }
{}


//  
template <class T>
forward_list<T>::forward_list(std::initializer_list<T> p_list) noexcept
    : forward_list(std::ranges::rbegin(p_list), std::ranges::rend(p_list))
{}


//  
template <class T>
forward_list<T>::forward_list(forward_list const& p_outer) noexcept
    : forward_list( std::ranges::begin(p_outer), std::ranges::end(p_outer) )
{
    m_head = reverse(m_head);
}


//  
template <class T>
forward_list<T>::forward_list(forward_list&& p_outer) noexcept
    : forward_list{}
{
    swap(*this, p_outer);
}


//  
template <class T>
auto forward_list<T>::operator=(forward_list p_list) noexcept -> forward_list &
{
    swap(*this, p_list);

    return *this;
}


//  
template <class T>
void forward_list<T>::pop_front() noexcept
{
    assert(not empty());

    node_type* v_targ = m_head;

    m_head = m_head->m_next;
    v_targ = ( delete v_targ, nullptr );

    m_size = m_size - 1ul;
}


//  
template <class T>
[[nodiscard]] auto forward_list<T>::empty() const noexcept -> bool
{
    return ( m_head == nullptr );
}


//  
template <class T>
[[nodiscard]] auto forward_list<T>::size() const noexcept
    -> typename forward_list::size_type
{
    return m_size;
}


//  
template <class T>
[[nodiscard]] auto forward_list<T>::front() const noexcept
    -> std::optional<typename forward_list::value_type>
{
    return not empty()  ? std::optional{ m_head->data() }
                        : std::nullopt;
}


//  
template <class T>
[[nodiscard]] auto forward_list<T>::front() noexcept
    -> std::optional<typename forward_list::value_type>
{
    return not empty()  ? std::optional{ m_head->data() }
                        : std::nullopt;
}


//  
template <class T>
auto forward_list<T>::begin() noexcept -> typename forward_list::iterator
{
    return iterator{m_head};
}


//  
template <class T>
auto forward_list<T>::begin() const noexcept -> typename forward_list::iterator
{
    return iterator{m_head};
}


//  
template <class T>
auto forward_list<T>::end() noexcept -> typename forward_list::sentinel
{
    return sentinel{};
}


//  
template <class T>
auto forward_list<T>::end() const noexcept -> typename forward_list::sentinel
{
    return sentinel{};
}


//  
template <class T>
forward_list<T>::~forward_list() noexcept
{
    while (not empty()) pop_front();

    m_head = (delete m_head, nullptr);
}
//  
//  




//  
//  
template class forward_list<int>;
template class forward_list<char>;
template class forward_list<long>;
template class forward_list<short>;
template class forward_list<unsigned int>;
template class forward_list<unsigned char>;
template class forward_list<unsigned long>;
template class forward_list<unsigned short>;
template class forward_list<float>;
template class forward_list<double>;
//  
//  