#ifndef LIST_H
#define LIST_H

#include <iostream> // cin/cout
#include <iterator> // std::distance
#include <initializer_list> // std::initializer_list
#include <cstdlib> // size_t
#include <algorithm>
#include <cassert>
#include <exception>

/* ALIAS */
using size_type = size_t;
using value_type = int;

namespace sc{ // sequence container.

    template< typename T >

        class list
        {
            private:
                struct Node{
                    T data; //> value
                    Node *next; //> pointer to next node.
                    Node *prev; //> pointer to prev node.

                    // default constructor
                    Node( const T& d = T(), Node * n = nullptr, Node * p = nullptr ) : data( d ), next(n), prev(p)
                { /* Empty */ }
                };

                Node *head; //> pointer to first/top node.
                Node *tail; //> pointer to last/bottom node.
                size_type m_size;


            public:
/*=====================================================================================*/
//              Iterator
/*=====================================================================================*/

                class iterator
                {
                    private:

                        Node * current;

                    public:

                        /* ALIAS */
                        typedef T& reference;
                        typedef T* pointer;
                        typedef T value_type;
                        typedef std::bidirectional_iterator_tag iterator_category;
                        typedef std::ptrdiff_t difference_type;

                        // default constructors.
                        iterator( Node * pt = nullptr ): current(pt)
                        {/*Empty*/ }

                        iterator(){}

                        iterator( const iterator & it ): current( it.current )
                        {/*Empty*/}

                        // destructor.
                        ~iterator() = default;

                        /* Operations */
                        // ++it
                        iterator operator++(void)
                        {
                            current = current->next;
                            return *this;
                        }

                        // it++
                        iterator operator++(value_type)
                        {
                            iterator temp(current);
                            current = current->next;
                            return temp;
                        }

                        // --it
                        iterator operator--(void)
                        {
                            current = current->prev;
                            return *this;
                        }

                        // it--
                        iterator operator--(value_type)
                        {
                            iterator temp(current);
                            current = current->prev;
                            return temp;
                        }

                        // *it
                        reference operator*(void) const
                        {
                            return current->data;
                        }

                        // it == it2
                        bool operator==( const iterator & rhs ) const
                        {
                            return this->current == rhs.current;
                        }

                        // it != it2
                        bool operator!=( const iterator & rhs ) const
                        {
                            return this->current != rhs.current;
                        }

                        iterator & operator=( const iterator rhs )
                        {
                            current = rhs.current;

                            return *this;
                        }

                        Node * getNode( void )
                        {
                            return current;
                        }



                };

/*=====================================================================================*/
//              Const Iterator
/*=====================================================================================*/

                class const_iterator
                {
                    private:

                        const Node * current;

                    public:

                        /* ALIAS */
                        typedef T& reference;
                        typedef T* pointer;
                        typedef T value_type;
                        typedef std::bidirectional_iterator_tag iterator_category;
                        typedef std::ptrdiff_t difference_type;


                        // default constructors.
                        const_iterator( const Node * pt = nullptr ): current(pt)
                        {/*Empty*/ }

                        const_iterator(){}

                        const_iterator( const iterator & it ): current( it.current )
                        {/*Empty*/}

                        // destructor
                        ~const_iterator() = default;

                        /* Operations */

                        // ++it
                        const_iterator operator++(void)
                        {
                            current = current->next;
                            return *this;
                        }

                        // it++
                        const_iterator operator++(value_type)
                        {
                            const_iterator temp(current);
                            current = current->next;
                            return temp;
                        }

                        // --it
                        const_iterator operator--(void)
                        {
                            current = current->prev;
                            return *this;
                        }

                        // it--
                        const_iterator operator--(value_type)
                        {
                            const_iterator temp(current);
                            current = current->prev;
                            return temp;
                        }

                        // *it
                        const T& operator*(void) const
                        {
                            return current->data;
                        }

                        // it == it2
                        bool operator==( const_iterator & rhs ) const
                        {
                            return this->current == rhs.current;
                        }

                        // it != it2
                        bool operator!=( const_iterator & rhs ) const
                        {
                            return this->current != rhs.current;
                        }

                        const_iterator operator=( const iterator & rhs )
                        {
                              this->current = rhs.current;
                        }



                };
/*=====================================================================================*/
//               Getting an iterator

                iterator begin()
                {
                    return iterator(head->next);
                }

                const_iterator cbegin() const
                {
                    return const_iterator(head->next);
                }

                iterator end()
                {
                    return iterator(tail);
                }

                const_iterator cend() const
                {
                    return const_iterator(tail);
                }
/*=====================================================================================*/


/*=====================================================================================*/
//        Operations that require iterators
/*=====================================================================================*/

                // Adds value into the list before the position given by the iterator pos.
                // The method returns an iterator to the position of the inserted item.
                iterator insert( iterator pos, const T & value )
                {

                    Node * runner = head->next;

                    while( runner->data != *pos )
                    {
                        runner = runner->next;
                    }

                    // create node
                    Node * temp = new Node( value, runner );

                    // link with prev element.
                    temp->prev = runner->prev;

                    temp->prev->next = temp;

                    // link with next element.
                    temp->next->prev = temp;

                    m_size++;

                    return temp;

                }


                // Insert elements from the range [first, last] before pos
                template< typename InItr >
                  iterator insert( iterator pos, InItr first, InItr last )
                  {
                      InItr runner = first;

                      iterator temp = pos;

                      while( runner != last )
                      {
                          temp = insert( pos, *runner++ );
                      }

                      return temp;
                  }


                // Insert elements from the initializer list `ilist` before pos.
                // Initializer list supports the user of insert as in:
                // myList.insert( pos, {1 ,2, 3, 4} ).
                // Which would insert the elements 1, 2, 3 and 4 in the list before pos.
                iterator insert( iterator pos, std::initializer_list<T> ilist )
                {

                    iterator it = pos;

                    for( auto i : ilist )
                    {
                        it = insert( pos, i );
                    }

                    return it;
                }


                // Removes the object at position pos. 
                // The method returns an iterator to the element that follows pos before
                // the call. This operation invalidates pos, since the item pointed to
                // was removed from the list.
                iterator erase( iterator pos )
                {
                    // save node adress;
                    Node * temp = pos.getNode();

                    // get iterator
                    iterator it = temp->next;

                    // link around nodes;
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;

                    delete[] temp; // remove

                    m_size--;

                    return it;

                }


                // Removes elements in the range [first, last]
                // The entire list may be erased by calling a.erase( a.begin(), a.end() )
                iterator erase( iterator first, iterator last )
                {
                    if( std::distance( first, last ) == m_size )
                    {
                        clear();
                        return end();
                    }

                    iterator runner = first;
                    iterator it = first;

                    while( runner != last )
                    {
                        it = erase( runner++ );
                    }

                    return it;
                }


                // Replaces the contents with count copies of value
                void assign( size_type count, const T & value )
                {
                    size_type aux = 0;

                    Node * runner = head->next;

                    while( aux < count )
                    {
                        runner->data = value;
                        runner = runner->next;
                    }
                }


                // Replaces the contents of the list with the elements
                // copies of the element in the range [first, left]
                template< typename InItr > 
                  void assign( InItr first, InItr last )
                  {
                      Node * runner = head->next;

                      while( first != last )
                      {
                          runner->data = *first;
                          runner = runner->next;
                          first++;
                      }
                  }


                void assign( std::initializer_list<T> ilist )
                {
                    Node * runner = head->next;

                    for( auto & i : ilist )
                    {
                        runner->data = i;
                        runner = runner->next;
                    }
                }
/*----------------------------- End of iterators --------------------------------------*/



/*-------------------------- Constructors & Destructor --------------------------------*/
                // Default constructor.
                list()
                {
                    /* empty list */
                    head = new Node;
                    tail = new Node;
                    head->next = tail;
                    tail->prev = head;

                    m_size = 0;
                }

                // Constructor with `count` times T().
                explicit list( size_type count )
                {
                    size_type aux = 1; // count.

                    m_size = 0;

                    // linking list
                    head = new Node;
                    tail = new Node;
                    head->next = tail;
                    tail->prev = head;

                    while( aux++ <= count )
                    {
                        push_back( T() );
                    }

                }

                // Constructor with the contents in range( first, last ).
                template< typename InputIt >
                  list( InputIt first, InputIt last )
                  {
                      m_size = 0;

                      // linking list
                      head = new Node;
                      tail = new Node;
                      head->next = tail;
                      tail->prev = head;

                      InputIt firstpos = first;

                      while( first != last )
                      {
                          push_back(*first);
                          first++;
                      }

                      m_size = std::distance( firstpos, last );
                  }

                // Constructs the list with the deep copy of contents of other.
                list( const list& other )
                {
                      m_size = 0;

                      // linking list
                      head = new Node;
                      tail = new Node;
                      head->next = tail;
                      tail->prev = head;

                      Node * runner = other.head->next;

                      while( runner->next )
                      {
                          push_back( runner->data );
                          runner = runner->next;
                      }

                      m_size = other.size();
                }

                // Constructor with initializer list
                list( std::initializer_list<T> ilist )
                {
                      m_size = 0;

                      // linking list
                      head = new Node;
                      tail = new Node;
                      head->next = tail;
                      tail->prev = head;

                      size_type it = 0;

                      while( it < ilist.size() )
                      {
                          push_back( *( ilist.begin() + it ) );
                          it++;
                      }

                      m_size = ilist.size();
                }

                // Destructor;
                ~list()
                {
                    clear();
                }

                // Operator = overloading ===============================================
                // Replace the contents with a copy of the contents of other.
                list& operator=( const list& other )
                {
                      m_size = 0;

                      // linking list
                      head = new Node;
                      tail = new Node;
                      head->next = tail;
                      tail->prev = head;

                      Node * runner = other.head->next;

                      while( runner->next )
                      {
                          push_back( runner->data );
                          runner = runner->next;
                      }

                      m_size = other.size();

                      return *this;
                }

                // Repalce the contents with those identified by a initializer list.
                list& operator=( std::initializer_list<T> ilist )
                {
                      m_size = 0;

                      // linking list
                      head = new Node;
                      tail = new Node;
                      head->next = tail;
                      tail->prev = head;

                      size_type it = 0;

                      while( it < ilist.size() )
                      {
                          push_back( *( ilist.begin() + it ) );
                          it++;
                      }

                      m_size = ilist.size();

                      return *this;
                }
/*-------------------------------------------------------------------------------------*/


/*------------------------ Commom operations to all list ------------------------------*/
                // Return the number of elements in the container.
                size_type size()const{ return m_size; }

                // Remove(either logically or physically) all elements from the container.
                void clear()
                {
                    while( !empty() )
                    {
                        pop_front();
                    }

                    m_size = 0;
                }

                // Return true if the container contains no elements, and false otherwise.
                bool empty(){ return head->next == tail; }

                // Adds value to the front of the list.
                void push_front( const T & value )
                {
                    // Create node.
                    Node * temp = new Node(value, head->next, head); // linked to head.

                    // link head.
                    head->next = temp;

                    // link old front element with new one.
                    temp->next->prev = temp;

                    m_size++;
                }

                // Adds value to the end of the list.
                void push_back( const T & value )
                {
                    // Create node.
                    Node * temp = new Node(value, tail);
                    // link last element with new last one.
                    tail->prev->next = temp;
                    // link old last one
                    temp->prev = tail->prev;
                    // link new node to the tail.
                    tail->prev = temp;

                    m_size++;
                }

                // Removes the object at the end of the list.
                void pop_back( void )
                {
                    if( empty() ){ return; } //empty condition.

                    Node * temp = tail;

                    tail = tail->prev;

                    delete[] temp;

                    m_size--;
                }

                // Removes the object at the front of the list.
                void pop_front( void )
                {
                    if( empty() ){ return; } //empty condition.

                    Node * temp = head;

                    head = head->next;

                    delete[] temp;

                    m_size--;
                }

                // print the list
                void print()
                {
                    std::cout << "[ ";
                    while( head != nullptr )
                    {
                        std::cout << head->data << " ";
                        head = head->next;
                    }
                    std::cout << "]"; 
                }

                // Returns the object at the end of the list.
                const T & back( void )
                {
                    return tail->prev->data;
                }

                // Returns the object at the beginning of the list.
                const T & front( void )
                {
                    return head->next->data;
                }

                // Replaces the content of the list with copies of value.
                void assign( const T & value )
                {
                    Node * runner = head;

                    while( runner->next )
                    {
                        runner->data = value;
                        runner = runner->next;
                    }
                }


        };
/*-------------------------------------------------------------------------------------*/

} // END namespace sc

/*---------------------------- Non-member functions -----------------------------------*/

template< typename T >

bool operator==( const sc::list<T> & lhs, const sc::list<T> & rhs )
{
    if( lhs.size() == rhs.size() )
    {
        auto l = lhs.cbegin();
        auto lLast = lhs.cend();
        auto r = rhs.cbegin();
        while( l != lLast )
        {
            if( *l++ != *r++ )
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }

    return false;
}

template< typename T >

bool operator!=( const sc::list<T> & lhs, const sc::list<T> & rhs )
{
    if( lhs.size() == rhs.size() )
    {
        auto l = lhs.cbegin();
        auto lLast = lhs.cend();
        auto r = rhs.cbegin();
        while( l != lLast )
        {
            if( *l++ != *r++ )
            {
                return true;
            }
        }

        return false;
    }
    else
        return true;
}

template< typename T >

std::ostream& operator<<( std::ostream& stream, const sc::list<T>& list)
{

        stream << "[ ";

        auto l = list.cbegin();

        for (auto i = 0; i < list.size(); ++i)
        {

            std::cout << *l << " ";
            ++l;
        }

        stream << "]";

        return stream;
}

#endif
