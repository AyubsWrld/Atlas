#pragma once

#include <cstdlib>
#include <type_traits>
#include <concepts>
#include <utility>
namespace Atlas
{

    /*
        scope_exit is a general-purpose scope guard that calls its exit function when a scope is exited. The
        class templates scope_fail and scope_success share the scope_exit interface, only the situation
        when the exit function is called differs.
    */
    
    template<typename EF>
    class scope_exit;

    template<typename EF>
    class scope_fail;

    template<typename EF>
    class scope_success;

    template<typename R, typename D>
    class unique_resource;

    template<typename R, typename D, typename S=std::decay<R>>
    unique_resource<std::decay_t<R>, std::decay_t<D>> 
    make_unique_resource_checked(R&& r, const S& invalid, D&& d) noexcept;


    /* NOTE:    If the exit function object of a scope_success or scope_exit object refers to a local variable
                of the function where it is defined, e.g., as a lambda capturing the variable by reference, and that
                variable is used as a return operand in that function, that variable might have already been returned
                when the scope_guard’s destructor executes, calling the exit function. This can lead to surprising
                behavior.
     */
    
    template<typename EF> 
    class scope_exit 
    {
    private:
        EF      exit_function;                       // exposition only 
        bool    execute_on_destruction{true};      // exposition only
        
        // int     uncaught_on_creation{uncaught_exceptions()};
    public:

        template<typename EFP>
        requires std::is_nothrow_constructible_v<EF, EFP> || std::is_nothrow_constructible_v<EF, EFP&> 
        explicit scope_exit(EFP&& Callable) noexcept;

        explicit scope_exit(scope_exit&& rhs) noexcept;

        ~scope_exit() noexcept;

        void release() noexcept;

        scope_exit(const scope_exit& other)                 =   delete;
        scope_exit& operator=(const scope_exit& other)      =   delete;
        scope_exit& operator=(scope_exit&& other)           =   delete;

       
    };
}

namespace Atlas 
{
    template<typename EF>
    template<typename EFP>
    requires std::is_nothrow_constructible_v<EF, EFP> || 
            std::is_nothrow_constructible_v<EF, EFP&>
    scope_exit<EF>::scope_exit(EFP&& Callable) noexcept
        : exit_function(std::forward<EFP>(Callable))
    {
    }

    template<typename EF>
    scope_exit<EF>::scope_exit(scope_exit&& rhs) noexcept
        :
            exit_function(rhs.exit_function)
    {
        rhs.execute_on_destruction = false;
    }


    template<typename EF>
    scope_exit<EF>::~scope_exit() noexcept
    {
        if (execute_on_destruction)
            exit_function();
    }

    template<typename EF>
    void scope_exit<EF>::release() noexcept
    {
        execute_on_destruction = false;
    }
}
