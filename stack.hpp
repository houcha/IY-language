#ifndef STACK_HPP
#define STACK_HPP

#define DUMP_LEVEL DUMP_ALL

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <utility>
#include <string>
#include <string_view>
#include "canary.h"
#include "poison.h"
#include "verifier.h"
#include "dumptools.h"
#include "protected_data.hpp"


//==============================================================================
template <typename T, class Container>
class BaseStack {

    ENABLE_VERIFICATION(std::cout);

  protected:

    Canary l_canary_;
    Container* data_;
    Canary r_canary_;

  public:

    typedef typename Container::size_type size_type;
    typedef typename Container::hash_type hash_type;

     /// Capture container pointer.
     BaseStack(Container* container);
     BaseStack(BaseStack&& other);

     BaseStack& operator=(const BaseStack& other);
     BaseStack& operator=(BaseStack&&      other);
     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ WARNING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     /// Base copy constructor realization depends on Container type.
     /// (Because container creation depends on Container type)
     /// So it is deleted in order to avoid unexpected behaviour.
     BaseStack(const BaseStack& other) = delete;
     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Do not delete captured data pointer.
    ~BaseStack() = default;

    // Capacity methods.
    virtual bool      empty()    const { return size() == 0; }
    virtual size_type size()     const = 0;
    virtual size_type capacity() const = 0;

    // Elements access.
          T& top();
    const T& top() const;

    // Modifiers.
    void push(const T& value);
    void emplace(T&& value);
    void pop();

    // Dump.
    void SetDumpStream();

  protected:

    virtual void IncrementSize() const = 0;
    virtual void DecrementSize() const = 0;

    /* Getters. */

    hash_type&    GetHash()        const { return data_->hash_;        }
    const Canary& GetDataLCanary() const { return data_->l_canary_;    }
    const Canary& GetDataRCanary() const { return data_->GetRCanary(); }

    /* Change state. */

    void Update();
    hash_type CalculateHash() const;

    /* Validity. */

    bool CheckPoison()                       const;
    /// Check stack state. `message` is error/warning message.
    bool OK(std::string& message)            const;
    void Dump(std::ostream& out, bool is_ok) const;

    /* Virtual methods. */

    virtual void     Reallocate()           = 0;
    virtual       T& Get(size_type i)       = 0;
    virtual const T& Get(size_type i) const = 0;
};



//==============================================================================
template <typename T>
class DynamicStack : public BaseStack<T, DynamicProtectedData<T>> {

    ENABLE_VERIFICATION(std::cout);

    typedef BaseStack<T, DynamicProtectedData<T>> Base;

  public:

    typedef typename Base::size_type size_type;
    typedef typename Base::hash_type hash_type;

     /// If no capacity is specified, create Stack with capacity = 1.
     DynamicStack(size_t capacity = 1);
     DynamicStack(const DynamicStack& other);
     DynamicStack(DynamicStack&&      other);

     DynamicStack& operator=(const DynamicStack& other);
     DynamicStack& operator=(DynamicStack&&      other);

    // Delete allocated data.
    ~DynamicStack() { delete Base::data_; }

    size_type size()     const { return Base::data_->size_;     }
    size_type capacity() const { return Base::data_->capacity_; }

    // Returns container pointer.
    DynamicProtectedData<T>* data() const { return Base::data_; }

  protected:

    void IncrementSize() const { ++Base::data_->size_; }
    void DecrementSize() const { --Base::data_->size_; }

    // Used in StaticStack::Get(i) implementation.
    template <typename U, size_t N>
    friend class StaticStack;

    void DoubleCapacity() const { this->data_->capacity_ *= 2; }

    /* Overridden methods. */

    void     Reallocate()           override;
          T& Get(size_type i)       override;
    const T& Get(size_type i) const override;
};



//==============================================================================
template <typename T, size_t N>
class StaticStack : protected StaticProtectedData<T, N>,
                    public    BaseStack<T, StaticProtectedData<T, N>> {

    ENABLE_VERIFICATION(std::cout);

    typedef BaseStack<T, StaticProtectedData<T, N>> Base;

  public:

    typedef typename Base::size_type size_type;

     StaticStack();
     StaticStack(const StaticStack& other);
     StaticStack(StaticStack&&      other);

     StaticStack& operator=(const StaticStack& other);
     StaticStack& operator=(StaticStack&&      other);

    ~StaticStack() = default;

    size_type size()     const;
    size_type capacity() const;

  protected:

    /* Overridden methods. */

    void IncrementSize() const;
    void DecrementSize() const;

    bool OK(std::string& message) const;

    void     Reallocate()           override;
          T& Get(size_type i)       override;
    const T& Get(size_type i) const override;
};


#include "stack.inl"

#endif // STACK_HPP

